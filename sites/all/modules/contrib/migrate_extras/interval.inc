<?php

/**
 * Primary value passed to this field must be interval value itself (number of
 * periods).
 *
 * Pass the period as an argument, e.g.:
 *
 * @code
 *   $arguments = array('period' => 'month');
 *   $this->addFieldMapping('field_subscription_duration', 'num_months')
 *        ->arguments($arguments);
 * @endcode
 */
class MigrateIntervalFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('interval'));
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
      $return[$language][$delta] = array(
        'interval' => $value,
        'period' => $arguments['period'],
      );
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
