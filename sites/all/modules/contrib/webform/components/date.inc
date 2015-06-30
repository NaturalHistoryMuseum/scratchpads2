<?php

/**
 * @file
 * Webform module date component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_date() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => 0,
    'extra' => array(
      'timezone' => 'user',
      'exclude' => array(),
      'start_date' => '-2 years',
      'end_date' => '+2 years',
      'year_textfield' => 0,
      'datepicker' => 1,
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
function _webform_theme_date() {
  return array(
    'webform_date' => array(
      'render element' => 'element',
      'file' => 'components/date.inc',
    ),
    'webform_display_date' => array(
      'render element' => 'element',
      'file' => 'components/date.inc',
    ),
    'webform_calendar' => array(
      'variables' => array('component' => NULL, 'calendar_classes' => NULL),
      'template' => 'templates/webform-calendar',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_date($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . '<br />' . t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>. Strings such as today, +2 months, and Dec 9 2004 are all valid.'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $form['extra']['timezone'] = array(
    '#type' => 'radios',
    '#title' => t('Default value timezone'),
    '#default_value' => empty($component['extra']['timezone']) ? 'user' : $component['extra']['timezone'],
    '#description' => t('If using relative dates for a default value (e.g. "today") base the current day on this timezone.'),
    '#options' => array('user' => t('User timezone'), 'site' => t('Website timezone')),
    '#weight' => 2,
    '#access' => variable_get('configurable_timezones', 1),
  );

  $form['extra']['exclude'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Hide'),
    '#default_value' => $component['extra']['exclude'],
    '#options' => array(
      'day' => t('Day'),
      'month' => t('Month'),
      'year' => t('Year'),
    ),
    '#description' => t('A hidden day or month will be set to 1. A hidden year will be set to the year of the default value.'),
    '#weight' => 3,
  );

  $form['display']['datepicker'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable popup calendar'),
    '#default_value' => $component['extra']['datepicker'],
    '#description' => t('Enable a JavaScript date picker next to the date field.'),
    '#weight' => 2,
    '#parents' => array('extra', 'datepicker'),
  );

  $form['display']['year_textfield'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a textfield for year'),
    '#default_value' => $component['extra']['year_textfield'],
    '#description' => t('If checked, the generated date field will use a textfield for the year. Otherwise it will use a select list.'),
    '#weight' => 5,
    '#parents' => array('extra', 'year_textfield'),
  );

  $form['validation']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Start date'),
    '#default_value' => $component['extra']['start_date'],
    '#description' => t('The earliest date that may be entered into the field.') . ' ' . t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>.'),
    '#size' => 30,
    '#weight' => 3,
    '#parents' => array('extra', 'start_date'),
  );
  $form['validation']['end_date'] = array(
    '#type' => 'textfield',
    '#title' => t('End date'),
    '#default_value' => $component['extra']['end_date'],
    '#description' => t('The latest date that may be entered into the field.') . ' ' . t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>.'),
    '#size' => 30,
    '#weight' => 4,
    '#parents' => array('extra', 'end_date'),
  );

  $form['#validate'] = array('_webform_edit_date_validate');
  return $form;
}

/**
 * Implements hook_form_id_validate.
 *
 * Warns user about hiding all the date fields and not using the date picker.
 */
function _webform_edit_date_validate($form, &$form_state) {
  // Reduce checkbox values to simple non-associative array of values.
  form_set_value($form['extra']['exclude'], array_filter(array_values($form_state['values']['extra']['exclude'])), $form_state);

  // Note that Drupal 7 doesn't support setting errors no checkboxes due to
  // browser issues. @see https://www.drupal.org/node/222380
  if (count($form_state['values']['extra']['exclude']) == 3) {
    form_set_error('extra][exclude', 'The day, month and year can\'t all be hidden.');
  }
  if ($form_state['values']['extra']['exclude'] == array('month')) {
    form_set_error('extra][exclude', 'You cannot hide just the month.');
  }

  // Validate that the start and end dates are valid. Don't validate the default
  // date because with token substitution, it might not be valid at component
  // definition time. Also note that validation was introduced in 7.x-4.8 and
  // previously-defined webform may have invalid start and end dates.
  foreach (array('start_date', 'end_date') as $field) {
    if (trim($form_state['values']['extra'][$field]) && !($date[$field] = webform_strtodate('c', $form_state['values']['extra'][$field]))) {
      form_set_error("extra][$field", t('The @field could not be interpreted in <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>.',
                                                 array('@field' => $form['validation'][$field]['#title'])));
    }
  }
  if (!empty($date['start_date']) && !empty($date['end_date']) && $date['end_date'] < $date['start_date']) {
    form_set_error('extra][end_date', t('The End date must be on or after the Start date.'));
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_date($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'date',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#required' => $component['required'],
    '#start_date' => trim($component['extra']['start_date']),
    '#end_date' => trim($component['extra']['end_date']),
    '#reference_timestamp' => $submission && $submission->completed ? $submission->completed : NULL,
    '#year_textfield' => $component['extra']['year_textfield'],
    '#default_value' => $filter ? webform_replace_tokens($component['value'], $node) : $component['value'],
    '#timezone' => $component['extra']['timezone'],
    '#exclude' => $component['extra']['exclude'],
    '#process' => array('webform_expand_date'),
    '#theme' => 'webform_date',
    '#theme_wrappers' => array('webform_element'),
    '#element_validate' => array('webform_validate_date'),
    '#translatable' => array('title', 'description'),
  );

  if ($component['extra']['datepicker']) {
    $element['#datepicker'] = TRUE;
    $element['#attached'] = array(
      'library' => array(
        array('system', 'ui.datepicker'),
      ),
    );
  }

  // Set the value from Webform.
  if (isset($value[0]) && $value[0] !== '') {
    $value = webform_date_array($value[0], 'date');
    $element['#default_value'] = $value;
  }

  return $element;
}

/**
 * Form API #process function for Webform date fields.
 */
function webform_expand_date($element) {
  $timezone = $element['#timezone'] != 'user' ? NULL : 'user';
  
  // Accept a string or array value for #default_value.
  if (!empty($element['#default_value']) && is_string($element['#default_value'])) {
    $timestring = webform_strtodate('c', $element['#default_value'], $timezone);
    $element['#default_value'] = webform_date_array($timestring, 'date');
  }
  // Prevent an error in PHP 5.4 caused by core's treatment of the #value.
  if (isset($element['#value'])) {
    unset($element['#value']);
  }

  // Set defaults according to existing #default_value (set by Form API)
  if (isset($element['#default_value']['month']) || isset($element['#default_value']['day']) || isset($element['#default_value']['year'])) {
    $default_values = array(
      'month' => $element['#default_value']['month'],
      'day' => $element['#default_value']['day'],
      'year' => $element['#default_value']['year'],
    );
  }
  else {
    $default_values = array(
      'day' => NULL,
      'month' => NULL,
      'year' => NULL,
    );
  }

  // Let Drupal do it's normal expansion.
  $element = form_process_date($element);

  // Convert relative dates to absolute and calculate the year, month and day.
  $timezone = $element['#timezone'] != 'user' ? NULL : 'user';
  foreach (array('start', 'end') as $start_end) {
    $element_field = &$element["#{$start_end}_date"];
    $element_field = $element_field ? webform_strtodate('Y-m-d', $element_field, $timezone, $element['#reference_timestamp']) : '';
    if ($element_field) {
      $parts = explode('-', $element_field);
    }
    else {
      $parts = $start_end == 'start' ? array(webform_strtodate('Y', '-2 years'), 1, 1) : array(webform_strtodate('Y', '+2 years'), 12, 31);
      $element_field = '';
    }
    unset($element_field); // Drop PHP reference.
    $parts[3] = $parts[0] . '-' . $parts[1] . '-' . $parts[2];
    $range[$start_end] = array_combine(array('year', 'month', 'day', 'date'), $parts);
  }

  // The start date is not guaranteeed to be early than the end date for
  // historical reasons.
  if ($range['start']['date'] > $range['end']['date']) {
    $temp = $range['start'];
    $range['start'] = $range['end'];
    $range['end'] = $temp;
  }

  // Restrict the months and days when not all options are valid choices.
  if ($element['#start_date'] && $element['#end_date']) {
    $delta_months = ($range['end']['year'] * 12 + $range['end']['month']) - ($range['start']['year'] * 12 + $range['start']['month']);
    if ($delta_months < 11) {
      // There are 10 or fewer months between the start and end date. If there
      // were 11, then every month would be possible, and the menu select
      // should not be pruned.
      $month_options = &$element['month']['#options'];
      if ($range['start']['month'] <= $range['end']['month']) {
        $month_options = array_intersect_key($month_options, array_flip(range($range['start']['month'], $range['end']['month'])));
      }
      else {
        $month_options = array_intersect_key($month_options, array_flip(range($range['start']['month'], 12))) +
                         array_intersect_key($month_options, array_flip(range(1, $range['end']['month'])));
      }
      unset($month_options); // Drop PHP reference.
      if ($delta_months <= 1) {
        // The start and end date are either on the same month or consequtive
        // months. See if the days should be pruned.
        $day_options = &$element['day']['#options'];
        if ($range['start']['month'] == $range['end']['month']) {
          // Range is within the same month. The days are a simple range from
          // start day to end day.
          $day_options = array_intersect_key($day_options, array_flip(range($range['start']['day'], $range['end']['day'])));
        }
        elseif ($range['start']['day'] > $range['end']['day'] + 1) {
          // Range spans two months and at least one day would be omitted.
          $days_in_month = date('t', mktime(0, 0, 0, $range['start']['month'], 1, $range['start']['year']));
          $day_options = array_intersect_key($day_options, array_flip(range($range['start']['day'], $days_in_month))) +
                         array_intersect_key($day_options, array_flip(range(1, $range['end']['day'])));
        }
        unset($day_options); // Drop PHP reference.
      }
    }
  }

  // Set default values.
  foreach ($default_values as $type => $value) {
    switch ($type) {
      case 'month':
        $none = t('Month');
        $hidden_default = 1;
        break;
      case 'day':
        $none = t('Day');
        $hidden_default = 1;
        break;
      case 'year':
        $none = t('Year');
        $hidden_default = !empty($element['#default_value']['year']) ? $element['#default_value']['year'] : webform_strtodate('Y', 'today', $timezone);
        break;
    }
    unset($element[$type]['#value']);
    $element[$type]['#title'] = $none;
    $element[$type]['#title_display'] = 'invisible';
    $element[$type]['#default_value'] = $default_values[$type];
    $element[$type]['#options'] = array('' => $none) + $element[$type]['#options'];
    if (in_array($type, $element['#exclude'])) {
      $element[$type] += array(
        '#prefix' => '<div class="webform-date-field-wrapper element-invisible">',
        '#suffix' => '</div>',
      );
      $element[$type]['#default_value'] = $hidden_default;
    }
  }

  // Tweak the year field.
  if ($element['#year_textfield']) {
    $element['year']['#type'] = 'textfield';
    $element['year']['#size'] = 5;
    $element['year']['#maxlength'] = 4;
    unset($element['year']['#options']);
  }
  elseif ($element['#start_date'] || $element['#end_date']) {
    $element['year']['#options'] = array('' => t('Year')) + drupal_map_assoc(range($range['start']['year'], $range['end']['year']));
  }

  return $element;
}

/**
 * Theme a webform date element.
 */
function theme_webform_date($variables) {
  $element = $variables['element'];

  $element['year']['#attributes']['class'][] = 'year';
  $element['month']['#attributes']['class'][] = 'month';
  $element['day']['#attributes']['class'][] = 'day';

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['year']['#attributes']['class'][] = 'error';
    $element['month']['#attributes']['class'][] = 'error';
    $element['day']['#attributes']['class'][] = 'error';
  }

  // Add HTML5 required attribute, if needed.
  if ($element['#required']) {
    $element['year']['#attributes']['required'] = 'required';
    $element['month']['#attributes']['required'] = 'required';
    $element['day']['#attributes']['required'] = 'required';
  }

  $class = array('webform-container-inline');

  // Add the JavaScript calendar if available (provided by Date module package).
  if (!empty($element['#datepicker'])) {
    $class[] = 'webform-datepicker';
    $calendar_class = array('webform-calendar');
    if ($element['#start_date']) {
      $calendar_class[] = 'webform-calendar-start-' . $element['#start_date'];
    }
    if ($element['#end_date']) {
      $calendar_class[] = 'webform-calendar-end-' . $element['#end_date'];
    }
    $calendar_class[] ='webform-calendar-day-' . variable_get('date_first_day', 0);

    $calendar = theme('webform_calendar', array('component' => $element['#webform_component'], 'calendar_classes' => $calendar_class));
  }

  $output = '';
  $output .= '<div class="' . implode(' ', $class) . '">';
  $output .= drupal_render_children($element);
  $output .= isset($calendar) ? $calendar : '';
  $output .= '</div>';

  return $output;
}

/**
 * Element validation for Webform date fields.
 */
function webform_validate_date($element, $form_state) {
  if ($element['month']['#value'] !== '' || $element['day']['#value'] !== '' || $element['year']['#value'] !== '') {
    // Check that each part of the date has been filled in.
    foreach (array('day', 'month', 'year') as $field_type) {
      if (empty($element[$field_type]['#value'])) {
        form_error($element[$field_type], t('!part in !name is missing.', array('!name' => $element['#title'], '!part' => $element[$field_type]['#title'])));
        $missing_fields = TRUE;
      }
    }
    if (isset($missing_fields)) {
      return;
    }

    // Check for a valid date.
    if (!checkdate($element['month']['#value'], $element['day']['#value'], $element['year']['#value'])) {
      form_error($element, t('Entered !name is not a valid date.', array('!name' => $element['#title'])));
      return;
    }

    // Create a timestamp of the entered value for comparison.
    $timestamp = strtotime($element['year']['#value'] . '-' . $element['month']['#value'] . '-' . $element['day']['#value']);
    $format = webform_date_format('short');

    // Flip start and end if needed. Prior to 7.x-4.8, it was possible to save
    // a date component with the end date earlier than the start date.
    $date1 = strtotime($element['#start_date']);
    $date2 = strtotime($element['#end_date']);
    if ($date1 !== FALSE && $date2 !== FALSE) {
      $start_date = $date1 < $date2 ? $date1 : $date2;
      $end_date = $date1 > $date2 ? $date1 : $date2;
    }
    else {
      $start_date = $date1;
      $end_date = $date2;
    }

    // Check that the date is after the start date.
    if ($start_date !== FALSE && $timestamp < $start_date) {
      form_error($element, t('The entered date must be @start_date or later.', array('@start_date' => date($format, $start_date))));
    }

    // Check that the date is before the end date.
    if ($end_date !== FALSE && $timestamp > $end_date) {
      form_error($element, t('The entered date must be @end_date or earlier.', array('@end_date' => date($format, $end_date))));
    }
  }
  elseif ($element['#required']) {
    form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
  }
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_date($component, $value) {
  // Convert the date to an ISO 8601 format.
  return ($value['year'] && $value['month'] && $value['day']) ? webform_date_string($value, 'date') : '';
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_date($component, $value, $format = 'html') {
  $value = webform_date_array(isset($value[0]) ? $value[0] : '', 'date');

  return array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_date',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#exclude' => $component['extra']['exclude'],

    '#value' => $value,
    '#translatable' => array('title'),
  );
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_date($variables) {
  $element = $variables['element'];
  $output = ' ';
  if ($element['#value']['year'] && $element['#value']['month'] && $element['#value']['day']) {
    $timestamp = webform_strtotime($element['#value']['month'] . '/' . $element['#value']['day'] . '/' . $element['#value']['year']);
    $format = webform_date_format(NULL, $element['#exclude']);
    $output = format_date($timestamp, 'custom', $format, 'UTC');
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_date($component, $sids = array(), $single = FALSE, $join = NULL) {
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

  $dates = array();
  $submissions = 0;
  foreach ($result as $row) {
    $submissions++;
    if ($row['data']) {
      $dates[] = webform_date_array($row['data']);
    }
  }

  // Display stats.
  $nonblanks = count($dates);
  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);

  return array(
    'table_rows' => $rows,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_date($component, $value) {
  if ($value[0]) {
    $timestamp = webform_strtotime($value[0]);
    $format = webform_date_format('short', $component['extra']['exclude']);
    return format_date($timestamp, 'custom', $format, 'UTC');
  }
  else {
    return '';
  }
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_date($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_date($component, $export_options, $value) {
  if ($value[0]) {
    $timestamp = webform_strtotime($value[0]);
    if (!empty($export_options['iso8601_date'])) {
      $format = 'Y-m-d'; // ISO 8601 date: 2004-02-12
    }
    else {
      $format = webform_date_format('short');
    }
    return format_date($timestamp, 'custom', $format, 'UTC');
  }
  else {
    return '';
  }
}
