<?php

/**
 * Primary value passed to this field must be the 'given' name.
 * it cannot be NULL, but may be an empty string.
 *
 * Arguments are used to specify all the other values:
 * 'title' => array('source_field' => 'title'),
 * 'middle' => array('source_field' => 'middle'),
 * 'family' => array('source_field' => 'family'),
 * 'generational' => array('source_field' => 'generational'),
 * 'credentials' => array('source_field' => 'credentials'),
 *
 * Add the source field mappings to the argument array then add null mappings to
 * avoid having fields flagged as as unmapped:
 * @code
 *   $arguments = array(
 *    'title' => array('source_field' => 'profile_title'),
 *    'middle' => array('source_field' => 'profile_middle_name'),
 *    'family' => array('source_field' => 'profile_last_name'),
 *   );
 *   // The given name should be passed in as the primary value.
 *   $this->addFieldMapping('field_name', 'profile_first_name')
 *        ->arguments($arguments);
 *   // Since the excerpt is mapped via an argument, add a null mapping so it's
 *   // not flagged as unmapped.
 *   $this->addFieldMapping(NULL, 'profile_title');
 *   $this->addFieldMapping(NULL, 'profile_middle_name');
 *   $this->addFieldMapping(NULL, 'profile_last_name');
 * @endcode
 */
class MigrateNameHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('name'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array('given' => $value) + array_intersect_key($arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
