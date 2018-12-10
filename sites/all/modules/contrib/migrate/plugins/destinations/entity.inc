<?php

/**
 * @file
 * Defines base for migration destinations implemented as Drupal entities.
 */

/**
 * Abstract base class for entity-based destination handling. Holds common
 * Field API-related functions.
 */
abstract class MigrateDestinationEntity extends MigrateDestination {

  /**
   * The entity type (node, user, taxonomy_term, etc.) of the destination.
   *
   * @var string
   */
  protected $entityType;

  public function getEntityType() {
    return $this->entityType;
  }

  /**
   * The bundle (node type, vocabulary, etc.) of the destination.
   *
   * @var string
   */
  protected $bundle;

  public function getBundle() {
    return $this->bundle;
  }

  /**
   * Default language for text fields in this destination.
   *
   * @var string
   */
  protected $language;

  public function getLanguage() {
    return $this->language;
  }

  /**
   * Default input format for text fields in this destination.
   *
   * @var int
   */
  protected $textFormat;

  public function getTextFormat() {
    return $this->textFormat;
  }

  /**
   * Simply save the key schema.
   *
   * @param array $key_schema
   */
  public function __construct($entity_type, $bundle, array $options = array()) {
    parent::__construct();
    $this->entityType = $entity_type;
    $this->bundle = $bundle;
    $this->language = isset($options['language']) ? $options['language'] : LANGUAGE_NONE;
    $this->textFormat = isset($options['text_format']) ? $options['text_format'] : filter_fallback_format();
  }

  public function __toString() {
    // TODO: Link to configuration page
    if ($this->entityType == $this->bundle) {
      $output = t('%type', array('%type' => $this->entityType));
    }
    else {
      $output = t('%type (%bundle)',
        array('%type' => $this->entityType, '%bundle' => $this->bundle));
    }
    // TODO: Non-default language, input format
    return $output;
  }

  /**
   * Give handlers a shot at cleaning up before an entity has been rolled back.
   *
   * @param $entity_id
   *  ID of the entity about to be deleted..
   */
  public function prepareRollback($entity_id) {
    $migration = Migration::currentMigration();

    // Call any general entity handlers (in particular, the builtin field handler)
    migrate_handler_invoke_all('Entity', 'prepareRollback', $entity_id);
    // Then call any entity-specific handlers
    migrate_handler_invoke_all($this->entityType, 'prepareRollback', $entity_id);
    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepareRollback')) {
      $migration->prepareRollback($entity_id);
    }
  }

  /**
   * Give handlers a shot at cleaning up after an entity has been rolled back.
   *
   * @param $entity_id
   *  ID of the entity which has been deleted.
   */
  public function completeRollback($entity_id) {
    $migration = Migration::currentMigration();
    // Call any general entity handlers (in particular, the builtin field handler)
    migrate_handler_invoke_all('Entity', 'completeRollback', $entity_id);
    // Then call any entity-specific handlers
    migrate_handler_invoke_all($this->entityType, 'completeRollback', $entity_id);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'completeRollback')) {
      $migration->completeRollback($entity_id);
    }
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *  Entity object to build. Prefilled with any fields mapped in the Migration.
   * @param $source_row
   *  Raw source data object - passed through to prepare handlers.
   */
  public function prepare($entity, stdClass $source_row) {
    // Add source keys for debugging and identification of migrated data by hooks.
    /* TODO: Restore
    foreach ($migration->sourceKeyMap() as $field_name => $key_name) {
      $keys[$key_name] = $source_row->{$field_name};
    }
    */
    $migration = Migration::currentMigration();
    $entity->migrate = array(
      //      'source_keys' => $keys,
      'machineName' => $migration->getMachineName(),
    );

    // Call any general entity handlers (in particular, the builtin field handler)
    migrate_handler_invoke_all('Entity', 'prepare', $entity, $source_row);
    // Then call any entity-specific handlers
    migrate_handler_invoke_all($this->entityType, 'prepare', $entity, $source_row);

    // Apply defaults, removing empty fields from the entity object.
    $form = $form_state = array();
    _field_invoke_default('submit', $this->entityType, $entity, $form, $form_state);

    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $source_row);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $object
   *  Entity object to build. This is the complete object after saving.
   * @param $source_row
   *  Raw source data object - passed through to complete handlers.
   */
  public function complete($entity, stdClass $source_row) {
    // Call any general entity handlers (in particular, the builtin field handler)
    migrate_handler_invoke_all('Entity', 'complete', $entity, $source_row);
    // Then call any entity-specific handlers
    migrate_handler_invoke_all($this->entityType, 'complete', $entity, $source_row);
    // Then call any complete handler for this specific Migration.
    $migration = Migration::currentMigration();
    if (method_exists($migration, 'complete')) {
      try {
        $migration->complete($entity, $source_row);
      } catch (Exception $e) {
        // If we catch any errors here, save the messages without letting
        // the exception prevent the saving of the entity being recorded.
        $migration->saveMessage($e->getMessage());
      }
    }
  }

  /**
   * Perform field validation against the field data in an entity. Wraps
   * field_attach_validate to handle exceptions cleanly and provide maximum
   * information for identifying the cause of validation errors.
   *
   * @param $entity_type
   *   The type of $entity; e.g. 'node' or 'user'.
   * @param $entity
   *   The entity with fields to validate.
   */
  static public function fieldAttachValidate($entity_type, $entity) {
    try {
      field_attach_validate($entity_type, $entity);
    } catch (FieldValidationException $e) {
      $migration = Migration::currentMigration();
      foreach ($e->errors as $field_name => $error_list) {
        if (is_array($error_list)) {
          foreach ($error_list as $index => $error) {
            $message = $error['message'];
            $migration->saveMessage(t('Field validation error for !field_name: !message',
              array('!field_name' => $field_name, '!message' => $message)));
          }
        }
      }
    }
  }
}
