<?php

/**
 * @file
 * Support for entity types implementing the Entity API.
 *
 * @todo Remove this when the class lands in migrate_extras (http://drupal.org/node/1133914)
 */

/**
 * Destination class implementing migration into entity types.
 */
class MigrateDestinationEntityAPI extends MigrateDestinationEntity {
  /**
   * Info about the current entity type.
   *
   * @var array
   */
  protected $info;

  /**
   * Name of the entity id key (for example, nid for nodes).
   *
   * @var string
   */
  protected $id;

  /**
   * Get the schema for the base key.
   *
   * @param string $entity_type
   */
  static public function getKeySchema($entity_type = NULL) {
    // Done this way so that the Migrate UI doesn't blow
    // up trying to call getKeySchema without params.
    if (!$entity_type) {
      return array();
    }

    $info = entity_get_info($entity_type);
    $schema = drupal_get_schema($info['base table']);
    $key = isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'];
    $key_schema = $schema['fields'][$key];

    // We can't have any form of serial fields here, since the mapping table
    // already has it's own.
    $key_schema['auto_increment'] = FALSE;
    if ($key_schema['type'] == 'serial') {
      $key_schema['type'] = 'int';
    }

    return array($key => $key_schema);
  }

  /**
   * Return an options array (language, text_format), used for creating fields.
   *
   * @param string $language
   * @param string $text_format
   */
  static public function options($language, $text_format) {
    return compact('language', 'text_format');
  }

  /**
   * Basic initialization
   *
   * @param string $entity_type
   * @param string $bundle
   * @param array $options
   *  Options (language, text_format) used for creating fields.
   */
  public function __construct($entity_type, $bundle, array $options = array()) {
    parent::__construct($entity_type, $bundle, $options);

    $this->info = entity_get_info($entity_type);
    $this->id = isset($this->info['entity keys']['name']) ? $this->info['entity keys']['name'] : $this->info['entity keys']['id'];
  }

  /**
   * Returns a list of fields available to be mapped for entities attached to
   * a particular bundle.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $properties = entity_get_property_info($this->entityType);
    $fields = array();

    foreach ($properties['properties'] as $name => $property_info) {
      if (isset($property_info['setter callback'])) {
        $fields[$name] = $property_info['description'];
      }
    }

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Delete an entity.
   * @todo Support bulkRollback() once http://drupal.org/node/1124558 gets in.
   *
   * @param $ids
   *  Array of fields representing the key.
   */
  public function rollback(array $ids) {
    $id = reset($ids);

    migrate_instrument_start('entity_delete');
    $this->prepareRollback($id);
    $result = entity_delete($this->entityType, $id);
    $this->completeRollback($id);
    migrate_instrument_stop('entity_delete');
    return $result;
  }

  /**
   * Import a single entity.
   *
   * @param $entity
   *  Generic entity object, refilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (entity id only in this case) of the entity that was
   *  saved if successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      if (isset($entity->{$this->id})) {
        if ($entity->{$this->id} != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming id !id and map destination id !destid1 don't match",
            array('!id' => $entity->{$this->id}, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $entity->{$this->id} = $row->migrate_map_destid1;
      }
      if (!empty($this->info['entity keys']['revision'])) {
        // We need to load the revision to allow updates. There might be a more
        // efficent way to do this.
        $old_entity = entity_load_single($this->entityType, $entity->{$this->id});
        $revision_key = $this->info['entity keys']['revision'];
        $entity->$revision_key = $old_entity->$revision_key;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (empty($entity->{$this->id})) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination id provided'));
      }
      // Load the entity that's being updated, update its values, then
      // substitute the (fake) passed in entity with that one.
      $old_entity = entity_load_single($this->entityType, $entity->{$this->id});
      if (empty($old_entity)) {
        throw new MigrateException(t("Failed to load entity of type %type and id %id", array('%type' => $this->entityType, '%id' => $entity->{$this->id})));
      }
      foreach ($entity as $field => $value) {
        $old_entity->$field = $entity->$field;
      }
      $entity = $old_entity;
      $this->prepare($entity, $row);
    }
    else {
      // Create a real entity object, update its values with the ones we have
      // and pass it along.
      $new_entity = array();
      if (!empty($this->info['entity keys']['bundle'])) {
        $new_entity[$this->info['entity keys']['bundle']] = $this->bundle;
      }
      $new_entity = entity_create($this->entityType, $new_entity);
      foreach ($entity as $field => $value) {
        $new_entity->$field = $entity->$field;
      }

      // If a destination id exists, the entity is obviously not new.
      if (!empty($new_entity->{$this->id})) {
        unset($new_entity->is_new);
      }
      $entity = $new_entity;
      $this->prepare($entity, $row);
    }

    $updating = (!empty($entity->{$this->id}) && empty($entity->is_new));

    migrate_instrument_start('entity_save');
    entity_save($this->entityType, $entity);
    migrate_instrument_stop('entity_save');

    $this->complete($entity, $row);

    if (isset($entity->{$this->id}) && $entity->{$this->id} > 0) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      return array($entity->{$this->id});
    }
    return FALSE;
  }

  /**
   * After the import do cleanup actions like clearing the field cache.
   */
  public function postImport() {
    field_cache_clear();
  }
  public function postRollback() {
    field_cache_clear();
  }

}
