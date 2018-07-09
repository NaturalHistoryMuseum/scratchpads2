<?php

/**
 * @file
 * Contains the MigrateLanguagefieldFieldHandler class.
 */

// Avoid issues when migrate module is disabled.
if (!class_exists('MigrateSimpleFieldHandler')) {
  return;
}

/**
 * Migrate field handler for language fields.
 */
class MigrateLanguagefieldFieldHandler extends MigrateSimpleFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();
    $this->registerTypes(array('language_field'));
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    // Get the options for this field.
    $options = languagefield_options_list($field_info, $instance, $entity->type, $entity);

    // Filter out values that are not in the options.
    $values = array_intersect($values, array_keys($options));

    return parent::prepare($entity, $field_info, $instance, $values);
  }

}
