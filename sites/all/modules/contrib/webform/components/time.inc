<?php

/**
 * @file
 * Webform module time component.
 */

// Time depends on functions provided by date.
webform_component_include('date');

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_time() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => 0,
    'extra' => array(
      'timezone' => 'user',
      'start_time' => '',
      'end_time' => '',
      'hourformat' => '12-hour',
      'minuteincrements' => 1,
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      'analysis' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_time() {
  return array(
    'webform_time' => array(
      'render element' => 'element',
      'file' => 'components/time.inc',
    ),
    'webform_display_time' => array(
      'render element' => 'element',
      'file' => 'components/time.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_time($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . '<br />' . t('Accepts a time in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>. Strings such as now, +2 hours, and 10:30pm are all valid.'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $form['validation']['start_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Start time'),
    '#default_value' => $component['extra']['start_time'],
    '#description' => t('The earliest time that may be entered into the field.'),
    '#size' => 10,
    '#weight' => 3,
    '#parents' => array('extra', 'start_time'),
  );
  $form['validation']['end_time'] = array(
    '#type' => 'textfield',
    '#title' => t('End time'),
    '#default_value' => $component['extra']['end_time'],
    '#description' => t('The latest time that may be entered into the field.'),
    '#size' => 10,
    '#weight' => 4,
    '#parents' => array('extra', 'end_time'),
  );
  $form['extra']['timezone'] = array(
    '#type' => 'radios',
    '#title' => t('Default value timezone'),
    '#default_value' => $component['extra']['timezone'],
    '#description' => t('If using relative dates for a default value (e.g. "now") base the current time on this timezone.'),
    '#options' => array('user' => t('User timezone'), 'site' => t('Website timezone')),
    '#weight' => 2,
    '#access' => variable_get('configurable_timezones', 1),
  );
  $form['display']['hourformat'] = array(
    '#type' => 'radios',
    '#title' => t('Time format'),
    '#default_value' => $component['extra']['hourformat'],
    '#options' => array('12-hour' => t('12-hour (am/pm)'), '24-hour' => t('24-hour')),
    '#weight' => 2,
    '#parents' => array('extra', 'hourformat'),
  );
  $form['display']['minuteincrements'] = array(
    '#type' => 'select',
    '#title' => t('Minute increments'),
    '#default_value' => $component['extra']['minuteincrements'],
    '#options' => array(
      1 => t('1 minute'),
      5 => t('5 minute'),
      10 => t('10 minute'),
      15 => t('15 minute'),
      30 => t('30 minute'),
    ),
    '#weight' => 3,
    '#parents' => array('extra', 'minuteincrements'),
  );
  $form['#validate'] = array('_webform_edit_time_validate');
  return $form;
}

/**
 * Implements hook_form_id_validate.
 *
 * Validate start and end times.
 */
function _webform_edit_time_validate($form, &$form_state) {
  // Validate that the start and end times are valid. Don't validate the default
  // time because with token substitution, it might not be valid at component
  // definition time. The end time may be before the start time to faciliate
  // time ranges spanning midnight.
  foreach (array('start_time', 'end_time') as $field) {
    $time[$field] = FALSE;
    if (trim($form_state['values']['extra'][$field]) && ($time[$field] = strtotime('1-1-1970 UTC ' . $form_state['values']['extra'][$field])) === FALSE) {
      form_set_error("extra][$field", t('The @field isn\'t a valid time.', array('@field' => $form['validation'][$field]['#title'])));
    }
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_time($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'webform_time',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#element_validate' => array('webform_validate_time'),
    '#start_time' => trim($component['extra']['start_time']),
    '#end_time' => trim($component['extra']['end_time']),
    '#hourformat' => $component['extra']['hourformat'],
    '#minuteincrements' => $component['extra']['minuteincrements'],
    '#default_value' => $filter ? webform_replace_tokens($component['value'], $node) : $component['value'],
    '#timezone' => $component['extra']['timezone'],
    '#process' => array('webform_expand_time'),
    '#theme' => 'webform_time',
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  // Set the value from Webform if available.
  if (!empty($value[0])) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Form API #process function for Webform time fields.
 */
function webform_expand_time($element) {
  // Expand the default value from a string into an array.
  if (!empty($element['#default_value'])) {
    // Adjust the time based on the user or site timezone.
    if (variable_get('configurable_timezones', 1) && $element['#timezone'] == 'user') {
      $timezone_name = isset($GLOBALS['user']->timezone) ? $GLOBALS['user']->timezone : 'UTC';
    }
    else {
      $timezone_name = variable_get('date_default_timezone', 'UTC');
    }

    $default_values = webform_date_array(webform_strtodate('c', $element['#default_value'], $timezone_name), 'time');
  }
  else {
    $default_values = array(
      'hour' => '',
      'minute' => '0',
      'second' => '',
    );
  }
  $start_hour = $element['#start_time'] ? date('G', strtotime('1-1-1970 ' . $element['#start_time'])) : FALSE;
  $end_hour = $element['#end_time'] ? date('G', strtotime('1-1-1970 ' . $element['#end_time'])) : FALSE;
  $reduced_range = ($start_hour !== FALSE && $start_hour > 0) || ($end_hour !== FALSE && $end_hour < 23);
  $format_12_hour = $element['#hourformat'] == '12-hour';

  // Generate the choices for the hour drop-down select.
  $hours = $format_12_hour && !$reduced_range ? array_slice(range(0, 12), 1, 12, TRUE) : range(0, 23);
  if ($format_12_hour && $reduced_range) {
    $hours = array_map(function($hour) {
      return (1 + ($hour + 11) % 12) . ($hour < 12 ? ' am' : ' pm');
    }, $hours);
  }

  // Prune the hours to the allowed range.
  if ($reduced_range) {
    // $start_hour of FALSE type-juggles nicely to 0.
    $end_hour = $end_hour === FALSE ? 23 : $end_hour;
    if ($start_hour <= $end_hour) {
      $hours = array_intersect_key($hours, array_flip(range($start_hour, $end_hour)));
    }
    else {
      $hours = array_intersect_key($hours, array_flip(range($start_hour, 23))) +
               array_intersect_key($hours, array_flip(range(0, $end_hour)));
    }
  }

  // Generate the choices for the minute drop-down select.
  $minutes = range(0, 59, $element['#minuteincrements']);
  $minutes = array_combine($minutes, array_map(function ($minute) {
    return substr('00' . $minute, -2);
  }, $minutes));

  // Add the labels to the drop-down selects.
  $hours = array('' => t('Hour')) + $hours;
  $minutes = array('' => t('Minute')) + $minutes;

  // Adjust the default for minutes if needed, rounding down if needed.
  // Rounding down eliminate the problem of rounding up going to the next hour.
  // Worse, rounding 23:59 up would actually be the next day, which can't be
  // represented because time components aren't linked to date components.
  if (!isset($minutes[$default_values['minute']])) {
    $default_values['minute'] -= $default_values['minute'] % $element['#minuteincrements'];
  }

  // Set the overall default value.
  if ($default_values['hour'] !== '') {
    $element['#default_value'] = webform_date_string($default_values);
  }

  // Convert default to 12-hour if needed.
  if ($format_12_hour && !$reduced_range) {
    $default_values = webform_time_convert($default_values, '12-hour');
  }

  $element['hour'] = array(
    '#prefix' => '',
    '#type' => 'select',
    '#title' => t('Hour'),
    '#title_display' => 'invisible',
    '#default_value' => $default_values['hour'],
    '#options' => $hours,
  );
  $element['minute'] = array(
    '#prefix' => ':',
    '#type' => 'select',
    '#title' => t('Minute'),
    '#title_display' => 'invisible',
    '#default_value' => $default_values['minute'],
    '#options' => $minutes,
  );
  if ($format_12_hour && !$reduced_range) {
    $element['ampm'] = array(
      '#type' => 'radios',
      '#default_value' => $default_values['ampm'] ? $default_values['ampm'] : 'am',
      '#options' => array('am' => t('am'), 'pm' => t('pm')),
    );
  }

  return $element;
}

/**
 * Theme a webform time element.
 */
function theme_webform_time($variables) {
  $element = $variables['element'];

  $element['hour']['#attributes']['class'][] = 'hour';
  $element['minute']['#attributes']['class'][] = 'minute';

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['hour']['#attributes']['class'][] = 'error';
    $element['minute']['#attributes']['class'][] = 'error';
  }

  // Add HTML5 required attribute, if needed.
  if ($element['#required']) {
    $element['hour']['#attributes']['required'] = 'required';
    $element['minute']['#attributes']['required'] = 'required';
    if (!empty($element['ampm'])) {
      $element['ampm']['am']['#attributes']['required'] = 'required';
      $element['ampm']['pm']['#attributes']['required'] = 'required';
    }
  }

  $output = '<div class="webform-container-inline">' . drupal_render($element['hour']) . drupal_render($element['minute']) . drupal_render($element['ampm']) . '</div>';

  return $output;
}

function webform_validate_time($element, $form_state) {
  $form_key = $element['#webform_component']['form_key'];

  // Check if the user filled the required fields.
  if ($element['#required']) {
    foreach (array('hour', 'minute', 'ampm') as $field_type) {
      if (isset($element[$field_type]) && $element[$field_type]['#value'] === '') {
        form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
        return;
      }
    }
  }

  // Check for a valid time. Allow a minute with no hour as "no time set".
  if ($element['hour']['#value'] !== '' ) {
    if (!is_numeric($element['hour']['#value']) || !is_numeric($element['minute']['#value']) || (isset($element['ampm']) && $element['ampm']['#value'] === '')) {
      form_error($element, t('Entered !name is not a valid time.', array('!name' => $element['#title'])));
      return;
    }

    // Enforce the start and end times, if any.
    $timestamp = strtotime($element['hour']['#value'] . ':' . $element['minute']['#value'] . ' ' .
                           (isset($element['ampm']) ?  $element['ampm']['#value'] : ''));
    $start_time = strtotime($element['#start_time']);
    $end_time = strtotime($element['#end_time']);
    $subs = array(
      '@start_time' => $element['#start_time'],
      '@end_time' => $element['#end_time'],
    );
    if ($start_time !== FALSE && $end_time !== FALSE && $start_time > $end_time) {
      // Validate as "over midnight" date range.
      if ($end_time < $timestamp && $timestamp < $start_time) {
        form_error($element, t('The entered time must be from @start_time to midnight to @end_time.', $subs));
      }
    }
    else {
      // Validate the start and end times are a regular (over noon) time range.
      if ($start_time !== FALSE && $timestamp < $start_time) {
        form_error($element, t('The entered time must be no earlier than @start_time.', $subs));
      }
      if ($end_time !== FALSE && $timestamp > $end_time) {
        form_error($element, t('The entered time must be no later than @end_time.', $subs));
      }
    }
  }
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_time($component, $value) {
  // Convert to 24-hour time before string conversion.
  if ($component['extra']['hourformat'] == '12-hour') {
    $value = webform_time_convert($value, '24-hour');
  }

  // Convert the value into a ISO 8601 string.
  return $value['hour'] !== '' ? webform_date_string($value, 'time') : '';
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_time($component, $value, $format = 'html') {
  $value = webform_date_array(isset($value[0]) ? $value[0] : '', 'time');
  if ($component['extra']['hourformat'] == '12-hour') {
    $value = webform_time_convert($value, '12-hour');
  }

  return array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_time',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#hourformat' => $component['extra']['hourformat'],
    '#value' => $value,
    '#translatable' => array('title'),
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_time($variables) {
  $element = $variables['element'];
  $output = ' ';
  if (isset($element['#value']['hour']) && $element['#value']['hour'] !== '' && isset($element['#value']['minute']) && $element['#value']['minute'] !== '') {
    if ($element['#hourformat'] == '24-hour') {
      $output = sprintf('%02d', $element['#value']['hour']) . ':' . sprintf('%02d', $element['#value']['minute']);
    }
    else {
      $output = $element['#value']['hour'] . ':' . sprintf('%02d', $element['#value']['minute']) . ' ' . $element['#value']['ampm'];
    }
  }
  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_time($component, $sids = array(), $single = FALSE, $join = NULL) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid'])
    ->orderBy('wsd.sid');

  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $result = $query->execute();

  $times = array();
  $submissions = 0;
  foreach ($result as $row) {
    $submissions++;
    if ($row['data']) {
      $times[] = webform_date_array($row['data']);
    }
  }

  // Display stats.
  $nonblanks = count($times);
  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);

  return array(
    'table_rows' => $rows,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_time($component, $value) {
  if ($value[0]) {
    $time = webform_date_array($value[0], 'time');
    if ($component['extra']['hourformat'] == '24-hour') {
      return sprintf('%02d', $time['hour']) . ':' . sprintf('%02d', $time['minute']);
    }
    else {
      $time = webform_time_convert($time, '12-hour');
      return $time['hour'] . ':' . sprintf('%02d', $time['minute']) . ' ' . $time['ampm'];
    }
  }
  else {
    return '';
  }
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_time($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_time($component, $export_options, $value) {
  if ($value[0]) {
    $time = webform_date_array($value[0], 'time');
    // An ISO 8601 time is the same as 24-hour time.
    if (!empty($export_options['iso8601_time']) || $component['extra']['hourformat'] == '24-hour') {
      return sprintf('%02d', $time['hour']) . ':' . sprintf('%02d', $time['minute']);
    }
    else {
      $time = webform_time_convert($time, '12-hour');
      return $time['hour'] . ':' . sprintf('%02d', $time['minute']) . ' ' . $time['ampm'];
    }
  }
  else {
    return '';
  }
}

/**
 * Convert a time between a 24-hour and a 12-hour value.
 *
 * @param $array
 *   An array of hour, minute, second, and optionally ampm.
 * @param $format
 *   Either 12-hour or 24-hour.
 * @return
 *   An array with hour, minute, second, and ampm (if using "12-hour").
 */
function webform_time_convert($array, $format) {
  if ($array['hour'] !== '') {
    if ($format == '12-hour') {
      $array['ampm'] = ($array['hour'] >= 12 && $array['hour'] < 24) ? 'pm' : 'am';
      $array['hour'] = ($array['hour'] > 12 || $array['hour'] == 0) ? abs($array['hour'] - 12) : (int) $array['hour'];
    }
    elseif ($format == '24-hour' && isset($array['ampm'])) {
      $array['hour'] = ($array['hour'] < 12 && $array['ampm'] == 'pm') ? $array['hour'] + 12 : (int) $array['hour'];
      $array['hour'] = ($array['hour'] == 12 && $array['ampm'] == 'am') ? 0 : $array['hour'];
    }
  }

  if ($format == '12-hour' && !isset($array['ampm'])) {
    $array['ampm'] = '';
  }
  elseif ($format == '24-hour' && isset($array['ampm'])) {
    unset($array['ampm']);
  }

  return $array;
}
