<?php

/**
 * @file
 * Webform module date component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_date() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'timezone' => 'user',
      'year_start' => '-2',
      'year_end' => '+2',
      'year_textfield' => 0,
      'datepicker' => 1,
      'title_display' => 0,
      'description' => '',
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_date() {
  return array(
    'webform_date' => array(
      'render element' => 'element',
    ),
    'webform_display_date' => array(
      'render element' => 'element',
    ),
    'webform_calendar' => array(
      'variables' => array('component' => NULL, 'calendar_classes' => NULL),
      'template' => 'templates/webform-calendar',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
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
    '#description' => t('Adjust the default time value according to a specific timezone.'),
    '#options' => array('user' => t('User timezone'), 'site' => t('Website timezone')),
    '#weight' => -1,
    '#access' => variable_get('configurable_timezones', 1),
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

  $form['validation']['year_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start year'),
    '#default_value' => $component['extra']['year_start'],
    '#description' => t('The first year that is allowed to be entered. May be relative (i.e. -2 or +2) or simply the year (i.e. 1950).'),
    '#size' => 10,
    '#maxlength' => 4,
    '#weight' => 3,
    '#parents' => array('extra', 'year_start'),
  );
  $form['validation']['year_end'] = array(
    '#type' => 'textfield',
    '#title' => t('End year'),
    '#default_value' => $component['extra']['year_end'],
    '#description' => t('The last year that is allowed to be entered.  May be relative (i.e. -2 or +2) or simply the year (i.e. 1950).'),
    '#size' => 10,
    '#maxlength' => 4,
    '#weight' => 4,
    '#parents' => array('extra', 'year_end'),
  );

  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_date($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#type' => 'date',
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#required' => $component['mandatory'],
    '#year_start' => $component['extra']['year_start'],
    '#year_end' => $component['extra']['year_end'],
    '#process' => array('webform_expand_date'),
    '#theme' => 'webform_date',
    '#theme_wrappers' => array('webform_element'),
    '#element_validate' => array('webform_validate_date'),
    '#webform_component' => $component,
  );

  if ($component['extra']['datepicker']) {
    $element['#datepicker'] = TRUE;
    $element['#attached'] = array(
      'library' => array(
        array('system', 'ui.datepicker'),
      ),
    );
  }

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
  $component = $element['#webform_component'];

  // Accept a string or array value for #default_value.
  if (isset($element['#default_value']) && is_string($element['#default_value'])) {
    $element['#default_value'] = webform_date_array($element['#default_value'], 'date');
  }

  // Set defaults according to existing #default_value (set by Form API)
  if (isset($element['#default_value']['month']) || isset($element['#default_value']['day']) || isset($element['#default_value']['year'])) {
    $default_values = array(
      'month' => $element['#default_value']['month'],
      'day' => $element['#default_value']['day'],
      'year' => $element['#default_value']['year'],
    );
  }
  // Or, if none, use set the defaults of the component.
  elseif (drupal_strlen($component['value']) > 0) {
    $timezone = $component['extra']['timezone'] != 'user' ? NULL : 'user';
    $default_values = webform_date_array(webform_strtodate('c', $component['value'], $timezone), 'date');
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

  // Set default values.
  foreach ($default_values as $type => $value) {
    switch ($type) {
      case 'month':
        $none = t('Month');
        break;
      case 'day':
        $none = t('Day');
        break;
      case 'year':
        $none = t('Year');
        break;
    }
    unset($element[$type]['#value']);
    $element[$type]['#default_value'] = isset($default_values[$type]) ? $default_values[$type] : NULL;
    $element[$type]['#options'] = array('' => $none) + $element[$type]['#options'];
  }

  // Convert relative dates to absolute ones.
  foreach (array('year_start', 'year_end') as $start_end) {
    $year = $element['#' . $start_end];
    if (strpos($year, '-') === 0 || strpos($year, '+') === 0) {
      $timezone = $component['extra']['timezone'] != 'user' ? NULL : 'user';
      $element['#' . $start_end] = webform_strtodate('Y', $year . ' years', $timezone);
    }
  }

  // Tweak the year field.
  if ($component['extra']['year_textfield']) {
    $element['year']['#type'] = 'textfield';
    $element['year']['#size'] = 5;
    $element['year']['#maxlength'] = 4;
    unset($element['year']['#options']);
  }
  elseif (is_numeric($element['#year_start']) && is_numeric($element['#year_end'])) {
    $element['year']['#options'] = array('' => t('Year')) + drupal_map_assoc(range($element['#year_start'], $element['#year_end']));
  }

  return $element;
}

/**
 * Theme a webform date element.
 */
function theme_webform_date($variables) {
  $element = $variables['element'];

  $element['year']['#attributes']['class'] = array('year');
  $element['month']['#attributes']['class'] = array('month');
  $element['day']['#attributes']['class'] = array('day');

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['year']['#attributes']['class'][] = 'error';
    $element['month']['#attributes']['class'][] = 'error';
    $element['day']['#attributes']['class'][] = 'error';
  }

  $class = array('webform-container-inline');

  // Add the JavaScript calendar if available (provided by Date module package).
  if (!empty($element['#datepicker'])) {
    $class[] = 'webform-datepicker';
    $calendar_class = array('webform-calendar');
    if ($element['#year_start'] && is_numeric($element['#year_start'])) {
      $calendar_class[] = 'webform-calendar-start-' . $element['#year_start'];
    }
    if ($element['#year_start'] && is_numeric($element['#year_end'])) {
      $calendar_class[] = 'webform-calendar-end-' . $element['#year_end'];
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
  $component = $element['#webform_component'];
  $form_key = $component['form_key'];
  $name = $component['name'];

  // Check if the user filled the required fields.
  foreach (array('day', 'month', 'year') as $field_type) {
    if (!is_numeric($element[$field_type]['#value']) && $element['#required']) {
      form_error($element, t('!name field is required.', array('!name' => $name)));
      return;
    }
  }
  // Check for a valid date.
  if ($element['month']['#value'] !== '' || $element['day']['#value'] !== '' || $element['year']['#value'] !== '') {
    if (!checkdate((int)$element['month']['#value'], (int)$element['day']['#value'], (int)$element['year']['#value'])) {
      form_error($element, t('Entered !name is not a valid date.', array('!name' => $name)));
      return;
    }
  }
  // Check the date is between allowed years.
  if ($element['year']['#value'] !== '' && is_numeric($element['#year_start']) && is_numeric($element['#year_end'])) {
    // Allow years to be in reverse order.
    $start = $element['#year_start'] < $element['#year_end'] ? $element['#year_start'] : $element['#year_end'];
    $end   = $element['#year_start'] > $element['#year_end'] ? $element['#year_start'] : $element['#year_end'];
    if ($element['year']['#value'] < $start || $element['year']['#value'] > $end) {
      form_error($element['year'], t('The entered date needs to be between the years @start and @end.', array('@start' => $start, '@end' => $end)));
      return;
    }
  }
}

/**
 * Implementation of _webform_submit_component().
 */
function _webform_submit_date($component, $value) {
  // Convert the date to an ISO 8601 format.
  return ($value['year'] && $value['month'] && $value['day']) ? webform_date_string($value, 'date') : '';
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_date($component, $value, $format = 'html') {
  $value = webform_date_array(isset($value[0]) ? $value[0] : '', 'date');

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_date',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value,
    '#webform_component' => $component,
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
    $format = webform_date_format('medium');
    $output = format_date($timestamp, 'custom', $format, 'UTC');
  }

  return $output;
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_date($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->orderBy('sid');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
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
  return $rows;
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_date($component, $value) {
  if ($value[0]) {
    $timestamp = webform_strtotime($value[0]);
    $format = webform_date_format('short');
    return format_date($timestamp, 'custom', $format, 'UTC');
  }
  else {
    return '';
  }
}

/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_date($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_date($component, $export_options, $value) {
  if ($value[0]) {
    $timestamp = webform_strtotime($value[0]);
    $format = webform_date_format('short');
    return format_date($timestamp, 'custom', $format, 'UTC');
  }
  else {
    return '';
  }
}
