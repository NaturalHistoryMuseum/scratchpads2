<?php

/**
 * @file
 * Support for entity types implementing the Entity API.
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
   * Name of the entity revision key (for example, vid for nodes).
   *
   * @var string
   */
  protected $revision;

  /**
   * Gets the schema for the base key(s) of an entity type.
   *
   * @param string $entity_type
   *   A Drupal entity type.
   */
  static public function getKeySchema($entity_type = NULL) {
    // Migrate UI invokes $destination->getKeySchema() without any parameters.
    if (!$entity_type) {
      if (isset($this)) {
        if ($this instanceof MigrateDestination) {
          $entity_type = $this->entityType;
        }
        elseif ($this instanceof Migration) {
          $entity_type = $this->destination->entityType;
        }
      }
      else {
        return array();
      }
    }

    $info = entity_get_info($entity_type);
    $schema = drupal_get_schema($info['base table']);

    $key = isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'];
    $key_schema = $schema['fields'][$key];

    $revision_key = isset($info['entity keys']['revision']) ? $info['entity keys']['revision'] : NULL;
    $revision_schema = empty($revision_key) ? NULL : $schema['fields'][$revision_key];

    // We can't have any form of serial fields here, since the mapping table
    // already has it's own.
    $key_schema['auto_increment'] = FALSE;
    if ($key_schema['type'] == 'serial') {
      $key_schema['type'] = 'int';
    }

    $return = array($key => $key_schema);
    if (!empty($revision_key)) {
      $return[$revision_key] = $revision_schema;
    }

    return $return;
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
    $this->revision = isset($this->info['entity keys']['revision']) ? $this->info['entity keys']['revision'] : NULL;
  }

  /**
   * Returns a list of fields available to be mapped for entities attached to
   * a particular bundle.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
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
   * Deletes multiple entities.
   *
   * @param array $ids
   *   An array of entity ids of the entities to delete.
   */
  public function bulkRollback(array $ids) {
    migrate_instrument_start('entity_delete_multiple');
    $this->prepareRollback($ids);
    $result = entity_delete_multiple($this->entityType, $ids);
    $this->completeRollback($ids);
    migrate_instrument_stop('entity_delete_multiple');

    return $result;
  }

  /**
   * Imports a single entity.
   *
   * @param stdClass $entity
   *   Generic entity object, refilled with any fields mapped in the Migration.
   * @param stdClass $row
   *   Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *   An array of key fields (entity id, and revision id if applicable) of the
   *   entity that was saved if successful. FALSE on failure.
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
    }
    else {
      unset($entity->{$this->id});
    }

    if (isset($row->migrate_map_destid2)) {
      if (isset($entity->{$this->revision})) {
        if ($entity->{$this->revision} != $row->migrate_map_destid2) {
          throw new MigrateException(t("Incoming revision !id and map destination revision !destid2 don't match",
            array('!id' => $entity->{$this->revision}, '!destid2' => $row->migrate_map_destid2)));
        }
      }
      else {
        $entity->{$this->revision} = $row->migrate_map_destid2;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($entity->{$this->id})) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination id provided'));
      }
      // Load the entity that's being updated, update its values, then
      // substitute the (fake) passed in entity with that one.
      $old_entity = entity_load_single($this->entityType, $entity->{$this->id});
      if (empty($old_entity)) {
        throw new MigrateException(t("Failed to load entity of type %type and id %id", array('%type' => $this->entityType, '%id' => $entity->{$this->id})));
      }

      // Prepare the entity to get the right array structure.
      $this->prepare($entity, $row);

      foreach ($entity as $field => $value) {
        $old_entity->$field = $entity->$field;
      }
      $entity = $old_entity;
    }
    else {
      // Create a real entity object, update its values with the ones we have
      // and pass it along.
      $new_entity = array();
      if (!empty($this->bundle) && !empty($this->info['entity keys']['bundle'])) {
        $new_entity[$this->info['entity keys']['bundle']] = $this->bundle;
      }
      $new_entity = entity_create($this->entityType, $new_entity);
      foreach ($entity as $field => $value) {
        $new_entity->$field = $entity->$field;
      }

      // If a destination id exists, the entity is obviously not new.
      if (!empty($new_entity->{$this->id}) && isset($new_entity->is_new)) {
        unset($new_entity->is_new);
      }
      $entity = $new_entity;
      $this->prepare($entity, $row);
    }

    $updating = (!empty($entity->{$this->id}) && empty($entity->is_new));

    migrate_instrument_start('entity_save');
    entity_save($this->entityType, $entity);
    // It's probably not worth keeping the static cache around.
    entity_get_controller($this->entityType)->resetCache();
    migrate_instrument_stop('entity_save');

    $this->complete($entity, $row);

    if (isset($entity->{$this->id}) && $entity->{$this->id} > 0) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($entity->{$this->id});

      if (isset($entity->{$this->revision}) && $entity->{$this->revision} > 0) {
        $return[] = array($entity->{$this->revision});
      }

      return $return;
    }
    return FALSE;
  }

  /**
   * Clear the field cache after an import or rollback.
   */
  public function postImport() {
    field_cache_clear();
  }
  public function postRollback() {
    field_cache_clear();
  }
}
