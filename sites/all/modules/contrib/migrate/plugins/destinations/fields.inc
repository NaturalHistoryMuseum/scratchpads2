<?php

/**
 * @file
 * Support for processing entity fields
 */

class MigrateFieldsEntityHandler extends MigrateDestinationHandler {

  public function __construct() {
    $this->registerTypes(array('entity'));
  }

  /**
   * Implementation of MigrateDestinationHandler::fields().
   */
  public function fields($entity_type, $bundle, $migration = NULL) {
    $fields = array();
    $field_instance_info = field_info_instances($entity_type, $bundle);
    foreach ($field_instance_info as $machine_name => $instance) {
      $field_info = field_info_field($machine_name);
      $type = $field_info['type'];

      if (user_access(MIGRATE_ACCESS_ADVANCED)) {
        $fields[$machine_name] = $instance['label'] . ' (' . $field_info['type'] . ')';
      }
      else {
        $fields[$machine_name] = $instance['label'];
      }

      // Look for subfields
      $class_list = _migrate_class_list('MigrateFieldHandler');
      $disabled = unserialize(variable_get('migrate_disabled_handlers', serialize(array())));
      $fields_found = FALSE;
      foreach ($class_list as $class_name => $handler) {
        if (!in_array($class_name, $disabled) && $handler->handlesType($type)
          && method_exists($handler, 'fields')) {
          migrate_instrument_start($class_name . '->fields');
          $subfields = call_user_func(array($handler, 'fields'), $type,
            $instance, $migration);
          migrate_instrument_stop($class_name . '->fields');
          foreach ($subfields as $subfield_name => $subfield_label) {
            $fields[$machine_name . ':' . $subfield_name] = $subfield_label;
          }
          $fields_found = TRUE;
        }
      }
      if (!$fields_found) {
        // Check the default field handler last.
        migrate_instrument_start('MigrateDefaultFieldHandler->fields');
        $subfields = call_user_func(
          array(new MigrateDefaultFieldHandler, 'fields'), $type, $instance,
          $migration);
        migrate_instrument_stop('MigrateDefaultFieldHandler->fields');
        foreach ($subfields as $subfield_name => $subfield_label) {
          $fields[$machine_name . ':' . $subfield_name] = $subfield_label;
        }
      }
    }

    return $fields;
  }

  public function prepare($entity, stdClass $row) {
    migrate_instrument_start('MigrateDestinationEntity->prepareFields');
    // Look for Field API fields attached to this destination and handle appropriately
    $migration = Migration::currentMigration();
    $destination = $migration->getDestination();
    $entity_type = $destination->getEntityType();
    $bundle = $destination->getBundle();
    $instances = field_info_instances($entity_type, $bundle);
    foreach ($instances as $machine_name => $instance) {
      if (property_exists($entity, $machine_name)) {
        // Normalize to an array
        if (!is_array($entity->{$machine_name})) {
          $entity->{$machine_name} = array($entity->{$machine_name});
        }
        $field_info = field_info_field($machine_name);
        $entity->{$machine_name} = migrate_field_handler_invoke_all($entity, $field_info,
          $instance, $entity->{$machine_name});
      }
    }
    migrate_instrument_stop('MigrateDestinationEntity->prepareFields');
  }

  public function complete($entity, stdClass $row) {
    migrate_instrument_start('MigrateDestinationEntity->completeFields');
    // Look for Field API fields attached to this destination and handle appropriately
    $migration = Migration::currentMigration();
    $destination = $migration->getDestination();
    $entity_type = $destination->getEntityType();
    $bundle = $destination->getBundle();
    $instances = field_info_instances($entity_type, $bundle);
    foreach ($instances as $machine_name => $instance) {
      if (property_exists($entity, $machine_name)) {
        // Normalize to an array
        if (!is_array($entity->{$machine_name})) {
          $entity->{$machine_name} = array($entity->{$machine_name});
        }
        $field_info = field_info_field($machine_name);
        migrate_field_handler_invoke_all($entity, $field_info,
          $instance, $entity->{$machine_name}, 'complete');
      }
    }
    migrate_instrument_stop('MigrateDestinationEntity->completeFields');
  }
}

abstract class MigrateFieldHandler extends MigrateHandler {

  // Derived classes are expected to implement one or both of the prepare/complete
  // handlers.

  // abstract public function prepare($entity, array $field_info, array $instance, array $values);
  // abstract public function complete($entity, array $field_info, array $instance, array $values);

  /**
   * Determine the language of the field
   *
   * @param $entity
   * @param $field_info
   * @param $arguments
   *
   * @return string language code
   */
  function getFieldLanguage($entity, $field_info, array $arguments) {
    $migration = Migration::currentMigration();
    switch (TRUE) {
      case !field_is_translatable($migration->getDestination()
        ->getEntityType(), $field_info):
        return LANGUAGE_NONE;
      case isset($arguments['language']):
        return $arguments['language'];
      case !empty($entity->language) && $entity->language != LANGUAGE_NONE:
        return $entity->language;
        break;
      default:
        return $migration->getDestination()->getLanguage();
    }
  }
}

/**
 * A fallback field handler to do basic copying of field data.
 */
class MigrateDefaultFieldHandler extends MigrateFieldHandler {

  public function __construct() {
  }

  /**
   * Implements MigrateFieldHandler::fields().
   *
   * @param $field_type
   * @param $field_instance
   *
   * @return array
   */
  public function fields($field_type, $field_instance) {
    $field_info = field_info_field($field_instance['field_name']);
    $fields = array();
    $first = TRUE;
    foreach ($field_info['columns'] as $column_name => $column_info) {
      // The first column is the primary value, which is mapped directly to
      // the field name - so, don't include it here among the subfields.
      if ($first) {
        $first = FALSE;
      }
      else {
        $fields[$column_name] = empty($column_info['description']) ?
          $column_name : $column_info['description'];
      }
    }
    return $fields;
  }

  /**
   * Implements MigrateFieldHandler::prepare().
   *
   * @param $entity
   * @param array $field_info
   * @param array $instance
   * @param array $values
   *
   * @return null
   */
  public function prepare($entity, array $field_info, array $instance,
                          array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Get the name of the primary (first) column, which is mapped separately.
    reset($field_info['columns']);
    $primary_column = key($field_info['columns']);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      // Handle multivalue arguments (especially for subfields).
      $delta_arguments = array();
      foreach ($arguments as $name => $argument) {
        if (is_array($argument) && array_key_exists($delta, $argument)) {
          $delta_arguments[$name] = $argument[$delta];
        }
        else {
          $delta_arguments[$name] = $argument;
        }
      }
      $return[$language][$delta] = array($primary_column => $value) +
        array_intersect_key($delta_arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }

  /**
   * Overrides MigrateHandler::handlesType().
   *
   * @param string $type
   *
   * @return bool
   */
  public function handlesType($type) {
    // We claim to handle any type.
    return TRUE;
  }
}

/**
 * Base class for creating field handlers for fields with a single value.
 *
 * To use this class just extend it and pass key where the field's value should
 * be stored to the constructor, then register the type(s):
 *
 * @code
 *   class MigrateLinkFieldHandler extends MigrateSimpleFieldHandler {
 *     public function __construct() {
 *       parent::__construct('url');
 *       $this->registerTypes(array('link'));
 *     }
 *   }
 * @endcode
 */
abstract class MigrateSimpleFieldHandler extends MigrateFieldHandler {

  protected $fieldValueKey = 'value';

  protected $skipEmpty = FALSE;

  /**
   * Construct a simple field handler.
   *
   * @param $options
   *   Array of options (rather than unamed parameters so you don't have to
   *   what TRUE or FALSE means). The following keys are used:
   *   - 'value_key' string with the name of the key in the fields value array.
   *   - 'skip_empty' Boolean indicating that empty values should not be saved.
   */
  public function __construct($options = array()) {
    if (isset($options['value_key'])) {
      $this->fieldValueKey = $options['value_key'];
    }
    if (isset($options['skip_empty'])) {
      $this->skipEmpty = $options['skip_empty'];
    }
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Let the derived class skip empty values.
    if ($this->skipEmpty) {
      $values = array_filter($values, array($this, 'notNull'));
    }

    // Setup the Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      if (is_array($language)) {
        $current_language = $language[$delta];
      }
      else {
        $current_language = $language;
      }
      $return[$current_language][] = array($this->fieldValueKey => $value);
      $delta++;
    }
    return isset($return) ? $return : NULL;
  }

  /**
   * Returns TRUE only for values which are not NULL.
   *
   * @param $value
   *
   * @return bool
   */
  protected function notNull($value) {
    return !is_null($value);
  }
}

class MigrateTextFieldHandler extends MigrateFieldHandler {

  public function __construct() {
    $this->registerTypes(array('text', 'text_long', 'text_with_summary'));
  }

  static function arguments($summary = NULL, $format = NULL, $language = NULL) {
    $arguments = array();
    if (!is_null($summary)) {
      $arguments['summary'] = $summary;
    }
    if (!is_null($format)) {
      $arguments['format'] = $format;
    }
    if (!is_null($language)) {
      $arguments['language'] = $language;
    }
    return $arguments;
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param $type
   *  The field type.
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   *
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = array();
    if ($type == 'text_with_summary') {
      $fields['summary'] = t('Subfield: <a href="@doc">Summary of field contents</a>',
        array('@doc' => 'http://drupal.org/node/1224042#summary'));
    }
    if ($instance['settings']['text_processing']) {
      $fields['format'] = t('Subfield: <a href="@doc">Text format for the field</a>',
        array('@doc' => 'http://drupal.org/node/1224042#format'));
    }
    $field = field_info_field($instance['field_name']);
    if (field_is_translatable($instance['entity_type'], $field)) {
      $fields['language'] = t('Subfield: <a href="@doc">Language for the field</a>',
        array('@doc' => 'http://drupal.org/node/1224042#language'));
    }
    return $fields;
  }


  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $migration = Migration::currentMigration();
    $destination = $migration->getDestination();
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $max_length = isset($field_info['settings']['max_length']) ?
      $field_info['settings']['max_length'] : 0;

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $item = array();
      if (isset($arguments['summary'])) {
        if (is_array($arguments['summary'])) {
          $item['summary'] = $arguments['summary'][$delta];
        }
        else {
          $item['summary'] = $arguments['summary'];
        }
      }
      if (isset($arguments['format'])) {
        if (is_array($arguments['format'])) {
          $format = $arguments['format'][$delta];
        }
        else {
          $format = $arguments['format'];
        }
      }
      elseif (!empty($instance['settings']['text_processing'])) {
        $format = $destination->getTextFormat();
      }
      else {
        $format = NULL;
      }
      $item['format'] = $item['value_format'] = $format;
      // If the value is an array, which it might be if the destination entity
      // was loaded via entity_load(), ensure we're handling a string.
      if (is_array($value) && !empty($value[0]) && array_key_exists('value', $value[0])) {
        $value = $value[0]['value'];
      }
      // Make sure the value will fit
      if ($max_length) {
        $item['value'] = drupal_substr($value, 0, $max_length);
        if (!empty($arguments['track_overflow'])) {
          $value_length = drupal_strlen($value);
          if ($value_length > $max_length) {
            $migration->saveMessage(
              t('Value for field !field exceeds max length of !max_length, actual length is !length',
                array(
                  '!field' => $instance['field_name'],
                  '!max_length' => $max_length,
                  '!length' => $value_length,
                )),
              Migration::MESSAGE_INFORMATIONAL);
          }
        }
      }
      else {
        $item['value'] = $value;
      }

      if (is_array($language)) {
        $current_language = $language[$delta];
      }
      else {
        $current_language = $language;
      }
      $return[$current_language][] = $item;
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}

class MigrateValueFieldHandler extends MigrateSimpleFieldHandler {

  public function __construct() {
    parent::__construct(array(
      'value_key' => 'value',
      'skip_empty' => FALSE,
    ));
    $this->registerTypes(array(
      'value',
      'list',
      'list_boolean',
      'list_integer',
      'list_float',
      'list_text',
      'number_integer',
      'number_decimal',
      'number_float',
    ));
  }
}

class MigrateTaxonomyTermReferenceFieldHandler extends MigrateFieldHandler {

  public function __construct() {
    $this->registerTypes(array('taxonomy_term_reference'));
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param $type
   *  The field type.
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   *
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    return array(
      'source_type' => t('Option: <a href="@doc">Set to \'tid\' when the value is a source ID</a>',
        array('@doc' => 'http://drupal.org/node/1224042#source_type')),
      'create_term' => t('Option: <a href="@doc">Set to TRUE to create referenced terms when necessary</a>',
        array('@doc' => 'http://drupal.org/node/1224042#create_term')),
      'ignore_case' => t('Option: <a href="@doc">Set to TRUE to ignore case differences between source data and existing term names</a>',
        array('@doc' => 'http://drupal.org/node/1224042#ignore_case')),
    );
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }
    if (count($values) == 1 && empty($values[0])) {
      $values = array();
    }

    $tids = array();
    if (isset($arguments['source_type']) && $arguments['source_type'] == 'tid') {
      // Nothing to do. We have tids already.
      $tids = $values;
    }
    elseif ($values) {
      $vocab_name = $field_info['settings']['allowed_values'][0]['vocabulary'];
      $names = taxonomy_vocabulary_get_names();

      // Get the vocabulary for this term
      if (isset($field_info['settings']['allowed_values'][0]['vid'])) {
        $vid = $field_info['settings']['allowed_values'][0]['vid'];
      }
      else {
        $vid = $names[$vocab_name]->vid;
      }

      // Remove leading and trailing spaces in term names
      $values = array_map('trim', $values);

      // Cannot use taxonomy_term_load_multiple() since we have an array of names.
      // It wants a singular value. This query may return case-insensitive
      // matches.
      $existing_terms = db_select('taxonomy_term_data', 'td')
        ->fields('td', array('tid', 'name'))
        ->condition('td.name', $values, 'IN')
        ->condition('td.vid', $vid)
        ->execute()
        ->fetchAllKeyed(1, 0);
      // If we're ignoring case, change both the matched term name keys and the
      // source values to lowercase.
      if (isset($arguments['ignore_case']) && $arguments['ignore_case']) {
        $ignore_case = TRUE;
        $existing_terms = array_change_key_case($existing_terms);
        foreach ($values as $value) {
          $lower_values[$value] = strtolower($value);
        }
      }
      else {
        $ignore_case = FALSE;
      }
      foreach ($values as $value) {
        if (isset($existing_terms[$value])) {
          $tids[] = $existing_terms[$value];
        }
        elseif ($ignore_case && isset($existing_terms[$lower_values[$value]])) {
          $tids[] = $existing_terms[$lower_values[$value]];
        }
        elseif (!empty($arguments['create_term'])) {
          $new_term = new stdClass();
          $new_term->vid = $vid;
          $new_term->name = $value;
          $new_term->vocabulary_machine_name = $vocab_name;

          // This term is being created with no fields, but we should still call
          // field_attach_validate() before saving, as that invokes
          // hook_field_attach_validate().
          MigrateDestinationEntity::fieldAttachValidate('taxonomy_term', $new_term);

          taxonomy_term_save($new_term);
          $tids[] = $new_term->tid;
          // Add newly created term to existing array.
          $existing_terms[$value] = $new_term->tid;
        }
        else {
          // No term is found for the source value and none is set to be
          // created: warn that data has not been imported.
          $migration = Migration::currentMigration();
          $migration->saveMessage(t("No matching taxonomy term found for source value '@value' in vocabulary %vocab.", array(
            '@value' => $value,
            '%vocab' => $names[$vocab_name]->name,
          )), MigrationBase::MESSAGE_INFORMATIONAL);
        }
      }
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $result = array();
    $delta = 0;
    foreach ($tids as $tid) {
      if (is_array($language)) {
        $current_language = $language[$delta];
      }
      else {
        $current_language = $language;
      }
      $result[$current_language][] = array('tid' => $tid);
      $delta++;
    }
    return $result;
  }
}

/**
 * The next generation of file field handler. This class focuses on the file
 * field itself, and offloads understanding of obtaining the actual file and
 * dealing with the file entity to an embedded MigrateFileInterface instance.
 */
abstract class MigrateFileFieldBaseHandler extends MigrateFieldHandler {

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param $type
   *  The file field type - 'file', 'image', etc.
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   *
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = array(
      'file_class' => t('Option: <a href="@doc">Implementation of MigrateFile to use</a>',
        array('@doc' => 'http://drupal.org/node/1540106#file_class')),
    );

    $field = field_info_field($instance['field_name']);
    if (field_is_translatable($instance['entity_type'], $field)) {
      $fields['language'] = t('Subfield: Language for the field');
    }

    // If we can identify the file class mapped to this field, pick up the
    // subfields specific to that class.
    if ($migration) {
      $field_mappings = $migration->getFieldMappings();
      $class_mapping = $instance['field_name'] . ':file_class';
      if (isset($field_mappings[$class_mapping])) {
        $mapping = $field_mappings[$class_mapping];
        $file_class = $mapping->getDefaultValue();
      }
    }
    if (empty($file_class)) {
      $file_class = 'MigrateFileUri';
    }
    $fields += call_user_func(array($file_class, 'fields'));
    return $fields;
  }

  /**
   * Implementation of MigrateFieldHandler::prepare().
   *
   * Prepare file data for saving as a Field API file field.
   *
   * @return array
   *  Field API array suitable for inserting in the destination object.
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $default_language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $migration = Migration::currentMigration();

    // One can override the source class via CLI or drushrc.php (the
    // option is named file_function for historical reasons)
    if ($migration->getOption('file_function')) {
      $file_class = $migration->getOption('file_function');
    }
    elseif (!empty($arguments['file_class'])) {
      $file_class = $arguments['file_class'];
    }
    else {
      $file_class = 'MigrateFileUri';
    }

    // If a destination directory (relative to the Drupal public files directory)
    // is not explicitly provided, use the default for the field.
    if (empty($arguments['destination_dir'])) {
      $arguments['destination_dir'] = $this->destinationDir($field_info, $instance);
    }

    $return = array();

    // Note that what $value represents depends on the file class -
    // MigrateFileUri expects a filespec/URI, MigrateFileFid expects a file ID,
    // etc.
    foreach ($values as $delta => $value) {
      if ($value) {
        // Handle potentially multiple arguments
        $instance_arguments = array();
        foreach ($arguments as $key => $argument) {
          // For a scalar argument, pass it directly
          if (!is_array($argument)) {
            $instance_arguments[$key] = $argument;
          }
          else {
            if (isset($argument[$delta])) {
              $instance_arguments[$key] = $argument[$delta];
            }
            else {
              $migration->saveMessage(
                t('No data for subfield %key at row %delta for field %field',
                  array(
                    '%key' => $key,
                    '%delta' => $delta,
                    '%field' => $field_info['field_name'],
                  )),
                Migration::MESSAGE_WARNING);
            }
          }
        }
        // If the parent entity doesn't have an explicit uid, give ownership
        // to the anonymous account
        $owner = isset($entity->uid) ? $entity->uid : 0;
        // Call the MigrateFileInterface implementation to do the real work
        $source = new $file_class($instance_arguments);
        $file = $source->processFile($value, $owner);

        // Assuming we got back a valid file ID, build the proper field
        // array out of it. We assume that if we did not get back a fid, the
        // MigrateFile class has saved a message indicating why.
        if ($file) {
          $field_array = array('fid' => $file->fid);
          $language = isset($instance_arguments['language']) ? $instance_arguments['language'] : $default_language;
          if (is_array($language)) {
            $language = $language[$delta];
          }
          $return[$language][] = $this->buildFieldArray($field_array, $instance_arguments, $delta);
        }
      }
    }

    return $return;
  }

  /**
   * Determine where the migrated file should go.
   *
   * @param $field_info
   *  Field API info on the general field.
   * @param $instance
   *  Field API info on the field instance for this entity type.
   *
   * @return string
   *  Directory relative to the Drupal public files directory.
   */
  protected function destinationDir($field_info, $instance) {
    // Only apply for file/image types
    if (isset($instance['settings']['file_directory'])) {
      $destination_dir = file_field_widget_uri($field_info, $instance);
    }
    else {
      $destination_dir = 'public://';
    }
    return $destination_dir;
  }

  /**
   * Add any type-specific subfields to a file field array.
   *
   * @param $field_array
   *  The field array so far (generally will just contain a fid).
   * @param $arguments
   *  Array of arguments passed to the field handler, from which we'll extract
   *  our own subfields.
   * @param $delta
   *  Index of field values being worked on, for pulling the corresponding
   *  subfield values if we have an array of them.
   */
  abstract protected function buildFieldArray($field_array, $arguments, $delta);
}

/**
 * Handle for file fields.
 */
class MigrateFileFieldHandler extends MigrateFileFieldBaseHandler {

  public function __construct() {
    $this->registerTypes(array('file'));
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   * Note that file and image fields support slightly different field lists.
   *
   * @param $type
   *  The file field type - 'file' or 'image'
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   *
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = parent::fields($type, $instance, $migration);
    $fields += array(
      'description' => t('Subfield: <a href="@doc">String to be used as the description value</a>',
        array('@doc' => 'http://drupal.org/node/1224042#description')),
      'display' => t('Subfield: <a href="@doc">String to be used as the display value</a>',
        array('@doc' => 'http://drupal.org/node/1224042#display')),
    );
    return $fields;
  }

  /**
   * Implementation of MigrateFileFieldBaseHandler::buildFieldArray().
   */
  protected function buildFieldArray($field_array, $arguments, $delta) {
    if (isset($arguments['description'])) {
      if (is_array($arguments['description'])) {
        $field_array['description'] = $arguments['description'][$delta];
      }
      else {
        $field_array['description'] = $arguments['description'];
      }
    }
    else {
      $field_array['description'] = '';
    }

    if (isset($arguments['display'])) {
      if (is_array($arguments['display'])) {
        $field_array['display'] = $arguments['display'][$delta];
      }
      else {
        $field_array['display'] = $arguments['display'];
      }
    }
    else {
      $field_array['display'] = 1;
    }
    return $field_array;
  }
}

/**
 * Handle for image fields;
 */
class MigrateImageFieldHandler extends MigrateFileFieldBaseHandler {

  public function __construct() {
    $this->registerTypes(array('image'));
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   * Note that file and image fields support slightly different field lists.
   *
   * @param $type
   *  The file field type - 'file' or 'image'
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   *
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = parent::fields($type, $instance, $migration);
    $fields += array(
      'alt' => t('Subfield: <a href="@doc">String to be used as the alt value</a>',
        array('@doc' => 'http://drupal.org/node/1224042#alt')),
      'title' => t('Subfield: <a href="@doc">String to be used as the title value</a>',
        array('@doc' => 'http://drupal.org/node/1224042#title')),
    );
    return $fields;
  }

  /**
   * Implementation of MigrateFileFieldBaseHandler::buildFieldArray().
   */
  protected function buildFieldArray($field_array, $arguments, $delta) {
    if (isset($arguments['alt'])) {
      if (is_array($arguments['alt'])) {
        $field_array['alt'] = $arguments['alt'][$delta];
      }
      else {
        $field_array['alt'] = $arguments['alt'];
      }
    }
    if (isset($arguments['title'])) {
      if (is_array($arguments['title'])) {
        $field_array['title'] = $arguments['title'][$delta];
      }
      else {
        $field_array['title'] = $arguments['title'];
      }
    }
    return $field_array;
  }
}

class MigrateNodeReferenceFieldHandler extends MigrateSimpleFieldHandler {

  public function __construct() {
    parent::__construct(array(
      'value_key' => 'nid',
      'skip_empty' => TRUE,
    ));
    $this->registerTypes(array('node_reference'));
  }

  protected function notNull($value) {
    return !is_null($value) && $value !== FALSE;
  }
}

class MigrateUserReferenceFieldHandler extends MigrateSimpleFieldHandler {

  public function __construct() {
    parent::__construct(array(
      'value_key' => 'uid',
      'skip_empty' => TRUE,
    ));
    $this->registerTypes(array('user_reference'));
  }

  protected function notNull($value) {
    return !is_null($value) && $value !== FALSE;
  }
}
