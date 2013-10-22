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
   * {@inheritdoc}
   */
  public function entityType() {
    return 'relation';
  }

  /**
   * {@inheritdoc}
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Relations');
    return $info;
  }

  /**
   * {@inheritdoc}
   */
  protected function newEntity(FeedsSource $source) {
    $account = user_load($this->config['author']);
    $relation = relation_create($this->config['bundle'], array(), $account);
    return $relation;
  }

  /**
   * {@inheritdoc}
   */
  protected function entityLoad(FeedsSource $source, $rid) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $relation = relation_load($rid, NULL, TRUE);
    }
    else {
      // We're replacing the existing relation. Only save the absolutely necessary.
      $relation = db_query("SELECT created, rid, vid, relation_type FROM {relation} WHERE rid = :rid", array(':rid' => $rid))->fetchObject();
      $relation->uid = $this->config['author'];
    }
    return $relation;
  }

  /**
   * {@inheritdoc}
   */
  protected function map(FeedsSource $source, FeedsParserResult $result, $target_item = NULL) {
    parent::map($source, $result, $target_item);
    $target_item->endpoints[LANGUAGE_NONE] = $this->mapEndpoints($source, $result);
  }

  /**
   * Map endpoints to entity keys array
   *
   * @param FeedsSource $source
   *   Source information about this import.
   * @param FeedsParserResult $result
   *   The result of the parsing stage.
   *
   * @return array
   *   An array of mapped endpoint items
   */
  private function mapEndpoints(FeedsSource $source, FeedsParserResult $result) {
    $field = array();
    static $sources;
    if (!isset($sources)) {
      foreach ($this->config['mappings'] as $map) {
        $sources[$map['target']] = $map['source'];
      }
    }
    $parser = feeds_importer($this->id)->parser;

    $type = $this->getTypeInfo();
    $r_index = 0;
    foreach (array('source_bundles', 'target_bundles') as $endpoint_type) {
      foreach ($type->{$endpoint_type} as $endpoint) {
        $endpoint = explode(':', $endpoint);
        // We're using GUID
        if (isset($sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':guid'])) {
          $entity_feed = db_select('feeds_item', 'f')
               ->condition('guid', $parser->getSourceElement($source, $result, $sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':guid']))
               ->condition('entity_type', $endpoint[0])
               ->fields('f', array('entity_id', 'entity_type'))
               ->execute()
               ->fetchAssoc();
          if (!empty($entity_feed['entity_type']) && !empty($entity_feed['entity_id'])) {
            $field[] = array('entity_type' => $entity_feed['entity_type'],
                            'entity_id' => $entity_feed['entity_id'],
                            'r_index' => $r_index);
          }
        }
        // We're using entity ID
        if (isset($sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':entity_id'])) {
          $entity_id = $parser->getSourceElement($source, $result, $sources[$endpoint_type . ':' . $endpoint[0] . ':' . $endpoint[1] . ':entity_id']);
          if (!empty($entity_id) && !empty($endpoint[0])) {
            $field[] = array('entity_type' => $endpoint[0],
                              'entity_id' => $entity_id,
                              'r_index' => $r_index);
          }
        }
        $r_index++;
      }
    }
    return $field;
  }

  /**
   * Get configured relation type
   */
  protected function getTypeInfo() {
    $types = relation_get_types();
    return $types[$this->config['bundle']];
  }

  /**
   * {@inheritdoc}
   */
  public function entitySave($entity) {
    relation_save($entity);
  }

  /**
   * {@inheritdoc}
   */
  protected function entityDeleteMultiple($rids) {
    relation_delete_multiple($rids);
  }

  /**
   * {@inheritdoc}
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
   * {@inheritdoc}
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * {@inheritdoc}
   */
  public function configDefaults() {
    return array(
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
      'unique_enpoints' => 0
    ) + parent::configDefaults();
  }

  /**
   * {@inheritdoc}
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['unique_enpoints'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unique endpoints'),
      '#description' => t('Use mapped endpoints as unique identifier for imported relations.'),
      '#default_value' => $this->config['unique_enpoints'],
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
    return $form;
  }

  /**
   * {@inheritdoc}
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
   * {@inheritdoc}
   */
  public function configFormSubmit(&$values) {
    if ($this->config['expire'] != $values['expire']) {
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * {@inheritdoc}
   */
  public function setTargetElement(FeedsSource $source, $target_relation, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_relation->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'feeds_source':
        // Get the class of the feed relation importer's fetcher and set the source
        // property. See feeds_relation_update() how $relation->feeds gets stored.
        if ($id = feeds_get_importer_id($this->config['bundle'])) {
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
   * {@inheritdoc}
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

    // If the target content type is a Feed relation, expose its source field.
    if ($id = feeds_get_importer_id($this->config['bundle'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The content type created by this processor is a Feed relation, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'relation', $this->config['bundle']);

    return $targets;
  }

  /**
   * {@inheritdoc}
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
          if ($id = feeds_get_importer_id($this->config['bundle'])) {
            $rid = db_query("SELECT fs.feed_rid FROM {relation} r JOIN {feeds_source} fs ON r.rid = fs.feed_rid WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($rid) {
        // Return with the first rid found.
        return $rid;
      }
    }
    // If other unique targets didn't match this relation check if endpoints
    // are unique.
    if ($this->config['unique_enpoints']) {
      $endpoints = $this->mapEndpoints($source, $result);
      $relations = relation_relation_exists($endpoints, $this->config['bundle']);
      if ($relations) {
        return key($relations);
      }
    }
    return 0;
  }
}
