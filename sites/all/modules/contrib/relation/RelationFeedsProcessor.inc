<?php
/**
 * @file
 * Class definition of RelationFeedsProcessor.
 */

/**
 * Creates relations from feed items.
 */
class RelationFeedsProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'relation';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Relations');
    return $info;
  }

  /**
   * Process the result of the parsing stage.
   *
   * @param FeedsSource $source
   *   Source information about this import.
   * @param FeedsParserResult $parser_result
   *   The result of the parsing stage.
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
    $state = $source->state(FEEDS_PROCESS);
    $field_mapping = $this->config['mappings'];
    while ($item = $parser_result->shiftItem()) {
      if (!($entity_id = $this->existingEntityId($source, $parser_result)) ||
           ($this->config['update_existing'] != FEEDS_SKIP_EXISTING)) {

        // Only proceed if item has actually changed.
        $hash = $this->hash($item);
        if (!empty($entity_id) && $hash == $this->getHash($entity_id)) {
          continue;
        }
        // Skip relations if duplicate already exists.
        if ($this->config['skip_duplicates'] && $this->getHashExists($hash)) {
          continue;
        }

        try {
          // Assemble relation, map item to it, save.
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
          $this->addEndpoints($entity, $field_mapping, $item);
          $this->entityValidate($entity);
          // Allow modules to alter the entity before saving.
          module_invoke_all('feeds_presave', $source, $entity);
          if (module_exists('rules')) {
            rules_invoke_event('feeds_import_' . $source->importer()->id, $entity);
          }

          // Enable modules to skip saving at all.
          if (empty($entity->feeds_item->skip)) {
            $this->entitySave($entity);
            // Track progress.
            if (empty($entity_id)) {
              $state->created++;
            }
            else {
              $state->updated++;
            }
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
   *
   */
  private function addEndpoints(&$entity, $field_mapping, $item) {
    $endpoint = array();
    static $sources;
    if (!isset($sources)) {
      foreach ($field_mapping as $map) {
        $sources[$map['target']] = $map['source'];
      }
    }
    $type = $this->getTypeInfo();
    $r_index = 0;
    foreach (array('source_bundles', 'target_bundles') as $endpoint_type) {
      foreach ($type->{$endpoint_type} as $endpoint) {
        $endpoint = explode(':', $endpoint);
        //We're using GUID
        if (isset($sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':guid'])) {
          $entity_feed = db_select('feeds_item', 'f')
               ->condition('guid', $item[$sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':guid']])
               ->condition('entity_type', $endpoint[0])
               ->fields('f', array('entity_id', 'entity_type'))
               ->execute()
               ->fetchAssoc();
          $field[] = array('entity_type' => $entity_feed['entity_type'],
                            'entity_id' => $entity_feed['entity_id'],
                            'r_index' => $r_index);
        }
        //We're using entity ID
        if (isset($sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':entity_id'])) {
          $field[] = array('entity_type' => $endpoint[0],
                            'entity_id' => $item[$sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':entity_id']],
                            'r_index' => $r_index);
        }
        $r_index++;
      }
    }
    $entity->endpoints = array(LANGUAGE_NONE => $field);
  }

  protected function getTypeInfo() {
    $types = relation_get_types();
    return $types[$this->config['relation_type']];
  }

  /**
   * Creates a new relation in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $relation = new stdClass();
    $relation->relation_type = $this->config['relation_type'];
    $relation->changed = REQUEST_TIME;
    $relation->created = REQUEST_TIME;
    $relation->language = LANGUAGE_NONE;
    $relation->uid = $this->config['author'];
    // @todo - not sure what to do with object_reppare yet
    // relation_object_prepare($relation);
    // Populate properties that are set by relation_object_prepare().
    return $relation;
  }

  /**
   * Loads an existing relatior.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the relation
   * table will be loaded, foregoing the relation_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $rid) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $relation = relation_load($rid, NULL, TRUE);
    }
    else {
      // We're replacing the existing relatior. Only save the absolutely necessary.
      $relation = db_query("SELECT created, rid, vid, relation_type FROM {relation} WHERE rid = :rid", array(':rid' => $rid))->fetchObject();
      $relation->uid = $this->config['author'];
    }
    // @todo - check the relation equivalent of this
    //relation_object_prepare($relation);
    return $relation;
  }

  /**
   * Save a relatior.
   */
  public function entitySave($entity) {
    // If rid is set and a relation with that id doesn't exist, flag as new.
    if (!empty($entity->rid) && !relation_load($entity->rid)) {
      $entity->is_new = TRUE;
    }
    relation_save($entity);
  }

  /**
   * Delete a series of relations.
   */
  protected function entityDeleteMultiple($rids) {
    relation_delete_multiple($rids);
  }

  /**
   * Implement expire().
   *
   * @todo: move to processor stage?
   */
  public function expire($time = NULL) {
    if ($time === NULL) {
      $time = $this->expiryTime();
    }
    if ($time == FEEDS_EXPIRE_NEVER) {
      return;
    }
    $count = $this->getLimit();
    $relations = db_query_range("SELECT r.rid FROM {relation} r JOIN {feeds_item} fi ON fi.entity_type = 'relation' AND r.rid = fi.entity_id WHERE fi.id = :id AND r.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $rids = array();
    foreach ($relations as $relation) {
      $rids[$relation->rid] = $relation->rid;
    }
    $this->entityDeleteMultiple($rids);
    if (db_query_range("SELECT 1 FROM {relation} r JOIN {feeds_item} fi ON fi.entity_type = 'relation' AND r.rid = fi.entity_id WHERE fi.id = :id AND r.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
      return FEEDS_BATCH_ACTIVE;
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Return expiry time.
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = relation_get_types();
    $type = key($types);
    return array(
      'relation_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
      'skip_duplicates' => 0
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = array();
    foreach (relation_get_types() as $type) {
      $types[$type->relation_type] = $type->label;
    }
    array_walk($types, 'check_plain');
    $form = parent::configForm($form_state);
    $form['relation_type'] = array(
      '#type' => 'select',
      '#title' => t('Relation type'),
      '#description' => t('Select the type of relations to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['relation_type'],
    );
    $form['skip_duplicates'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip duplicates'),
      '#description' => t('Skip duplicate relations.'),
      '#default_value' => $this->config['skip_duplicates'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the relations to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600, 43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire relations'),
      '#options' => $period,
      '#description' => t('Select after how much time relations should be deleted. The relation\'s creation date will be used for determining the relation\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing relations',
      FEEDS_REPLACE_EXISTING => 'Replace existing relations',
      FEEDS_UPDATE_EXISTING => 'Update existing relations (slower than replacing them)',
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($this->config['expire'] != $values['expire']) {
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * Override setTargetElement to operate on a target item that is a relatior.
   */
  public function setTargetElement(FeedsSource $source, $target_relation, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_relation->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'feeds_source':
        // Get the class of the feed relation importer's fetcher and set the source
        // property. See feeds_relation_update() how $relation->feeds gets stored.
        if ($id = feeds_get_importer_id($this->config['relation_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_relation->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_relation_insert().
          $target_relation->feeds['suppress_import'] = TRUE;
        }
        break;
      default:
        parent::setTargetElement($source, $target_relation, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $type = $this->getTypeInfo();
    $targets = parent::getMappingTargets();

    $targets['rid'] = array(
      'name' => t('Relation ID'),
      'description' => t('The rid of the relatior. NOTE: use this feature with care, relation ids are usually assigned by Drupal.'),
      'optional_unique' => TRUE,
    );
    $targets['uid'] = array(
      'name' => t('User ID'),
      'description' => t('The Drupal user ID of the relation author.'),
    );
    $targets['created'] = array(
      'name' => t('Published date'),
      'description' => t('The UNIX time when a relation has been published.'),
    );
    foreach (array('source_bundles' => t('Source'), 'target_bundles' => t('Target')) as $endpoint_type => $endpoint_label) {
      foreach ($type->{$endpoint_type} as $endpoint) {
        $endpoint = explode(':', $endpoint);
        $entity = entity_get_info($endpoint[0]);
        $bundle_label = ($endpoint[1] == '*')
                          ? t('any type')
                          : $entity['bundles'][$endpoint[1]]['label'];
        $targets[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':guid'] = array(
          'name' => t('@type @entity of @bundle: Feeds GUID', array('@type' => $endpoint_label, '@entity' => $entity['label'], '@bundle' => $bundle_label)),
          'description' => t('The GUID of the entity if it was already imported via feeds.'),
        );
        $targets[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':entity_id'] = array(
          'name' => t('@type @entity of @bundle: Entity ID', array('@type' => $endpoint_label, '@entity' => $entity['label'], '@bundle' => $bundle_label)),
          'description' => t('The ID of the entity as it exists in Drupal.'),
        );
      }
    }
    // Include language field if Locale module is enabled.
    // @todo - not sure about language handling yet.
    //if (module_exists('locale')) {
    //  $targets['language'] = array(
    //    'name' => t('Language'),
    //    'description' => t('The two-character language code of the relatior.'),
    //  );
    //}

    // If the target content type is a Feed relation, expose its source field.
    if ($id = feeds_get_importer_id($this->config['relation_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The content type created by this processor is a Feed relation, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'relation', $this->config['relation_type']);

    return $targets;
  }

  /**
   * Get rid of an existing feed item relation if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($rid = parent::existingEntityId($source, $result)) {
      return $rid;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'rid':
          $rid = db_query("SELECT rid FROM {relation} WHERE rid = :rid", array(':rid' => $value))->fetchField();
          break;
        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['relation_type'])) {
            $rid = db_query("SELECT fs.feed_rid FROM {relation} r JOIN {feeds_source} fs ON r.rid = fs.feed_rid WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($rid) {
        // Return with the first rid found.
        return $rid;
      }
    }
    return 0;
  }

  /**
   * Check if hash exists in database.
   *
   * @return boolean
   *   FALSE no item is found
   */
  protected function getHashExists($hash) {

    if ($hash = db_query("SELECT hash FROM {feeds_item} WHERE entity_type = :type AND hash = :hash", array(':type' => $this->entityType(), ':hash' => $hash))->fetchField()) {
      // Return with the hash.
      return TRUE;
    }
    return FALSE;
  }
}