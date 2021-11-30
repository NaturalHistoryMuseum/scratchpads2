<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for date.module.
 */

/**
 * Implements hook_feeds_processor_targets().
 */
function date_feeds_processor_targets($entity_type, $bundle_name) {
  $targets = array();

  $field_types = array(
    'date' => TRUE,
    'datestamp' => TRUE,
    'datetime' => TRUE,
  );

  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (!isset($field_types[$info['type']])) {
      continue;
    }

    $targets[$name . ':start'] = array(
      'name' => check_plain($instance['label']),
      'callback' => 'date_feeds_set_target',
      'description' => t('The start date for the @name field. Also use if mapping both start and end.', array('@name' => $instance['label'])),
      'real_target' => $name,
      'summary_callbacks' => array('date_feeds_summary_callback'),
      'form_callbacks' => array('date_feeds_form_callback'),
    );

    if (!empty($info['settings']['todate'])) {
      // Change the label for the start date.
      $targets[$name . ':start']['name'] = t('@name: Start', array('@name' => $instance['label']));

      $targets[$name . ':end'] = array(
        'name' => t('@name: End', array('@name' => $instance['label'])),
        'callback' => 'date_feeds_set_target',
        'description' => t('The end date for the @name field.', array('@name' => $instance['label'])),
        'real_target' => $name,
        'summary_callbacks' => array('date_feeds_summary_callback'),
        'form_callbacks' => array('date_feeds_form_callback'),
      );
    }
  }

  return $targets;
}

/**
 * Callback for setting date values.
 */
function date_feeds_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  $language = $mapping['language'];
  list($target, $sub_field) = explode(':', $target, 2);

  $value_key = $sub_field === 'start' ? 'value' : 'value2';
  $offset_key = $sub_field === 'start' ? 'offset' : 'offset2';

  $field = isset($entity->$target) ? $entity->$target : array($language => array());

  $info = field_info_field($target);
  $format = date_type_format($info['type']);

  $db_tz = new DateTimeZone(date_get_timezone_db($info['settings']['tz_handling']));
  $default_tz = new DateTimeZone(_date_feeds_get_default_timezone($mapping));

  $delta = 0;
  foreach ($values as $value) {
    $value = _date_feeds_get_date_object($value, $default_tz);

    if (!$value || !empty($value->errors)) {
      $field[$language][$delta][$value_key] = NULL;
    }
    else {
      if (!isset($field[$language][$delta]['timezone'])) {
        $timezone = $value->getTimezone()->getName();
        if ($timezone == 'Z') {
          // PHP < 5.5.10 doesn't know about the Zulu time zone (which equals
          // the UTC time zone). Set timezone to "UTC".
          $timezone = 'UTC';
        }
        $field[$language][$delta]['timezone'] = $timezone;
      }

      $value->setTimezone($db_tz);

      $field[$language][$delta][$value_key] = $value->format($format, TRUE);
      $field[$language][$delta][$offset_key] = $value->getOffset();

      // Ensure that both value keys always exist to prevent php notices in
      // date_field_validate().
      if (!array_key_exists('value', $field[$language][$delta])) {
        $field[$language][$delta]['value'] = NULL;
      }
      if (!array_key_exists('value2', $field[$language][$delta])) {
        $field[$language][$delta]['value2'] = NULL;
      }
    }

    $delta++;
  }

  $entity->$target = $field;
}

/**
 * Summary callback for date field targets.
 */
function date_feeds_summary_callback(array $mapping, $target, array $form, array $form_state) {
  $mapping += array('timezone' => 'UTC');

  $options = _date_feeds_timezone_options();

  return t('Default timezone: %zone', array('%zone' => $options[$mapping['timezone']]));
}

/**
 * Form callback for date field targets.
 */
function date_feeds_form_callback(array $mapping, $target, array $form, array $form_state) {
  $mapping += array('timezone' => 'UTC');

  return array(
    'timezone' => array(
      '#type' => 'select',
      '#title' => t('Timezone handling'),
      '#options' => _date_feeds_timezone_options(),
      '#default_value' => $mapping['timezone'],
      '#description' => t('This value will only be used if the timezone is mising.'),
    ),
  );
}

/**
 * Returns the timezone options.
 *
 * @return array
 *   A map of timezone options.
 */
function _date_feeds_timezone_options() {
  return array(
    '__SITE__' => t('Site default'),
  ) + system_time_zones();
}

/**
 * Returns the timezone to be used as the default.
 *
 * @param array $mapping
 *   The mapping array.
 *
 * @return string
 *   The timezone to use as the default.
 */
function _date_feeds_get_default_timezone(array $mapping) {
  $mapping += array('timezone' => 'UTC');

  if ($mapping['timezone'] === '__SITE__') {
    return variable_get('date_default_timezone', 'UTC');
  }

  return $mapping['timezone'];
}

/**
 * Converts a date string or object into a DateObject.
 *
 * @param DateTime|string|int $value
 *   The date value or object.
 * @param DateTimeZone $default_tz
 *   The default timezone.
 *
 * @return DateObject
 *   The converted DateObject.
 */
function _date_feeds_get_date_object($value, DateTimeZone $default_tz) {
  if ($value instanceof DateObject) {
    return $value;
  }

  // Convert DateTime and FeedsDateTime.
  if ($value instanceof DateTime) {
    if (!$value->getTimezone() || !preg_match('/[a-zA-Z]/', $value->getTimezone()->getName())) {
      $value->setTimezone($default_tz);
    }
    return new DateObject($value->format(DATE_FORMAT_ISO), $value->getTimezone());
  }

  if (is_string($value) || is_object($value) && method_exists($value, '__toString')) {
    $value = trim($value);
  }

  // Filter out meaningless values.
  if (empty($value) || !is_string($value) && !is_int($value)) {
    return FALSE;
  }

  // Support year values.
  if ((string) $value === (string) (int) $value) {
    if ($value >= variable_get('date_min_year', 100) && $value <= variable_get('date_max_year', 4000)) {
      return new DateObject('January ' . $value, $default_tz);
    }
  }

  return new DateObject($value, $default_tz);
}
