<?php

/**
 * @file
 * FeedsTermProcessor class.
 */

/**
 * Feeds processor plugin. Create taxonomy terms from feed items.
 */
class FeedsTermProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'taxonomy_term';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Terms');
    return $info;
  }

  /**
   * Creates a new term in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $vocabulary = $this->vocabulary();
    $term = new stdClass();
    $term->vid = $vocabulary->vid;
    $term->vocabulary_machine_name = $vocabulary->machine_name;
    return $term;
  }

  /**
   * Loads an existing term.
   */
  protected function entityLoad(FeedsSource $source, $tid) {
    return taxonomy_term_load($tid);
  }

  /**
   * Validates a term.
   */
  protected function entityValidate($term) {
    if (empty($term->name)) {
      throw new FeedsValidationException(t('Term name missing.'));
    }
  }

  /**
   * Saves a term.
   */
  protected function entitySave($term) {
    taxonomy_term_save($term);
  }

  /**
   * Deletes a series of terms.
   */
  protected function entityDeleteMultiple($tids) {
    foreach ($tids as $tid) {
      taxonomy_term_delete($tid);
    }
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'vocabulary' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $options = array(0 => t('Select a vocabulary'));
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $options[$vocab->machine_name] = $vocab->name;
    }
    $form = parent::configForm($form_state);
    $form['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Import to vocabulary'),
      '#description' => t('Choose the vocabulary to import into. <strong>CAUTION:</strong> when deleting terms through the "Delete items" tab, Feeds will delete <em>all</em> terms from this vocabulary.'),
      '#options' => $options,
      '#default_value' => $this->config['vocabulary'],
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if (empty($values['vocabulary'])) {
      form_set_error('vocabulary', t('Choose a vocabulary'));
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'name' => array(
        'name' => t('Term name'),
        'description' => t('Name of the taxonomy term.'),
        'optional_unique' => TRUE,
       ),
      'description' => array(
        'name' => t('Term description'),
        'description' => t('Description of the taxonomy term.'),
       ),
      'parent' => array(
        'name' => t('Parent Term Name'),
        'description' => t('Name of the Parent Term.'),
        'optional_unique' => TRUE,
      ),
      'parentguid' => array(
        'name' => t('Parent GUID'),
        'description' => t('The globally unique identifier of the item\'s parent term.'),
        'optional_unique' => TRUE,
      ),
      'weight' => array(
        'name' => t('Term Weight'),
        'description' => t('Weight of the Term.'),
        'optional_unique' => TRUE,
      ),
    );
    // Let implementers of hook_feeds_term_processor_targets() add their targets.
    try {
      self::loadMappers();
      feeds_alter('feeds_processor_targets', $targets, 'taxonomy_term', $this->vocabulary()->machine_name);
    }
    catch (Exception $e) {
      // Do nothing.
    }
    return $targets;
  }
  
  /**
   * Override the process function to ensure that we update a term, even if it
   * doesn't look like it needs updating.
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
    $state = $source->state(FEEDS_PROCESS);

    while ($item = $parser_result->shiftItem()) {
      if (!($entity_id = $this->existingEntityId($source, $parser_result)) ||
           ($this->config['update_existing'] != FEEDS_SKIP_EXISTING)) {
        try {
          // Assemble node, map item to it, save.
          $hash = $this->hash($item);
          if (empty($entity_id)) {
            $entity = $this->newEntity($source);
            $this->newItemInfo($entity, $source->feed_nid, $hash);
          }
          else {
            $entity = $this->entityLoad($source, $entity_id);
            // The feeds_item table is always updated with the info for the most recently processed entity.
            // The only carryover is the entity_id.
            $this->newItemInfo($entity, $source->feed_nid, $hash);
            $entity->feeds_item->entity_id = $entity_id;
          }
          $this->map($source, $parser_result, $entity);
          $this->entityValidate($entity);
          $this->entitySave($entity);

          // Track progress.
          if (empty($entity_id)) {
            $state->created++;
          }
          else {
            $state->updated++;
          }
        }
        catch (Exception $e) {
          $state->failed++;
          drupal_set_message($e->getMessage(), 'warning');
          $message = $e->getMessage();
          $message .= '<h3>Original item</h3>';
          $message .= '<pre>' . var_export($item, TRUE) . '</pre>';
          $message .= '<h3>Entity</h3>';
          $message .= '<pre>' . var_export($entity, TRUE) . '</pre>';
          $source->log('import', $message, array(), WATCHDOG_ERROR);
        }
      }
    }

    // Set messages if we're done.
    if ($source->progressImporting() != FEEDS_BATCH_COMPLETE) {
      return;
    }
    $info = $this->entityInfo();
    $tokens = array(
      '@entity' => strtolower($info['label']),
      '@entities' => strtolower($info['label plural']),
    );
    $messages = array();
    if ($state->created) {
      $messages[] = array(
       'message' => format_plural(
          $state->created,
          'Created @number @entity.',
          'Created @number @entities.',
          array('@number' => $state->created) + $tokens
        ),
      );
    }
    if ($state->updated) {
      $messages[] = array(
       'message' => format_plural(
          $state->updated,
          'Updated @number @entity.',
          'Updated @number @entities.',
          array('@number' => $state->updated) + $tokens
        ),
      );
    }
    if ($state->failed) {
      $messages[] = array(
       'message' => format_plural(
          $state->failed,
          'Failed importing @number @entity.',
          'Failed importing @number @entities.',
          array('@number' => $state->failed) + $tokens
        ),
        'level' => WATCHDOG_ERROR,
      );
    }
    if (empty($messages)) {
      $messages[] = array(
        'message' => t('There are no new @entities.', array('@entities' => strtolower($info['label plural']))),
      );
    }
    foreach ($messages as $message) {
      drupal_set_message($message['message']);
      $source->log('import', $message['message'], array(), isset($message['level']) ? $message['level'] : WATCHDOG_INFO);
    }
  }
  /**
  * Override setTargetElement to operate on a target item that is a term.
  */
  public function setTargetElement(FeedsSource $source, $target_node, $target_element, $value) {
    switch ($target_element) {
      case 'parent':      
        if (!empty($value)) {
          $terms = taxonomy_get_term_by_name($value);
          $parent_tid = '';
          foreach ($terms as $term) {
            if ($term->vid == $target_node->vid) {
              $parent_tid = $term->tid;
            }
          }

          if (!empty($parent_tid)) {
            $target_node->parent = $parent_tid;
          }
          else {
            $target_node->parent = 0;
          }
        }
        else {
          $target_node->parent = 0;
        }
        break;
      case 'parentguid':
        if($value){
          $query = db_select('feeds_item')
            ->fields('feeds_item', array('entity_id'))
            ->condition('entity_type', $this->entityType());
          $parent_tid = $query->condition('guid', $value)->execute()->fetchField();
  
          if (!empty($parent_tid)) {
            $target_node->parent = $parent_tid;
          }
          else {
            $target_node->parent = 0;
          }
        }
        break;
      case 'weight':
        if (!empty($value)) {
          $weight = intval($value);
        }
        else {
          $weight = 0;
        }
        $target_node->weight = $weight;
        break;
      default:
        parent::setTargetElement($source, $target_node, $target_element, $value);
        break;
    }
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($tid = parent::existingEntityId($source, $result)) {
      return $tid;
    }

    // The only possible unique target is name.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      if ($target == 'name') {
        $vocabulary = $this->vocabulary();
        if ($tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name AND vid = :vid", array(':name' => $value, ':vid' => $vocabulary->vid))->fetchField()) {
          return $tid;
        }
      }
    }
    return 0;
  }

  /**
   * Return vocabulary to map to.
   */
  public function vocabulary() {
    if (isset($this->config['vocabulary'])) {
      if ($vocabulary = taxonomy_vocabulary_machine_name_load($this->config['vocabulary'])) {
        return $vocabulary;
      }
    }
    throw new Exception(t('No vocabulary defined for Taxonomy Term processor.'));
  }
}
