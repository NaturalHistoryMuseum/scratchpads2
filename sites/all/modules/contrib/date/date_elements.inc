<?php
// $Id: date_elements.inc,v 1.2 2011/01/18 00:37:34 karens Exp $
/**
 * @file
 * Date forms and form themes and validation.
 *
 * All code used in form editing and processing is in this file,
 * included only during form editing.
 */

/**
 * Private implementation of hook_widget().
 *
 * The widget builds out a complex date element in the following way:
 *
 * - A field is pulled out of the database which is comprised of one or
 *   more collections of from/to dates.
 *
 * - The dates in this field are all converted from the UTC values stored
 *   in the database back to the local time before passing their values
 *   to FAPI.
 *
 * - If values are empty, the field settings rules are used to determine
 *   if the default_values should be empty, now, the same, or use strtotime.
 *
 * - Each from/to combination is created using the date_combo element type
 *   defined by the date module. If the timezone is date-specific, a
 *   timezone selector is added to the first combo element.
 *
 * - If repeating dates are defined, a form to create a repeat rule is
 *   added to the field element.
 *
 * - The date combo element creates two individual date elements, one each
 *   for the from and to field, using the appropriate individual Date API
 *   date elements, like selects, textfields, or popups.
 *
 * - In the individual element validation, the data supplied by the user is
 *   used to update the individual date values.
 *
 * - In the combo date validation, the timezone is updated, if necessary,
 *   then the user input date values are used with that timezone to create
 *   date objects, which are used update combo date timezone and offset values.
 *
 * - In the field's submission processing, the new date values, which are in
 *   the local timezone, are converted back to their UTC values and stored.
 *
 */
function date_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $base) {
  $element = $base;

  module_load_include('inc', 'date_api', 'date_api_elements');  
  $timezone = date_get_timezone($field['settings']['tz_handling'], isset($items[0]['timezone']) ? $items[0]['timezone'] : date_default_timezone());

  // TODO see if there's a way to keep the timezone element from ever being
  // nested as array('timezone' => 'timezone' => value)). After struggling
  // with this a while, I can find no way to get it displayed in the form
  // correctly and get it to use the timezone element without ending up
  // with nesting.
  if (is_array($timezone)) {
    $timezone = $timezone['timezone'];
  }

  // Convert UTC dates to their local values in DATETIME format,
  // and adjust the default values as specified in the field settings.

  // It would seem to make sense to do this conversion when the data
  // is loaded instead of when the form is created, but the loaded
  // field data is cached and we can't cache dates that have been converted
  // to the timezone of an individual user, so we cache the UTC values
  // instead and do our conversion to local dates in the form and
  // in the formatters.
  $process = date_process_values($field, $instance);
  foreach ($process as $processed) {
    if (!isset($items[$delta][$processed])) {
      $items[$delta][$processed] = '';
    }
    $date = date_local_date($form, $form_state, $delta, $items[$delta], $timezone, $field, $instance, $processed);
    $items[$delta][$processed] = is_object($date) ? date_format($date, DATE_FORMAT_DATETIME) : '';
  }

  $element += array(
    '#type' => 'date_combo',
    '#theme_wrappers' => array('date_combo'),
    '#weight' => $delta,
    '#default_value' => isset($items[$delta]) ? $items[$delta] : '',
    '#date_timezone' => $timezone,
    '#element_validate' => array('date_combo_validate', 'date_widget_validate'),
  );

  if ($field['settings']['tz_handling'] == 'date') {
    $element['timezone'] = array(
      '#type' => 'date_timezone',
      '#delta' => $delta,
      '#default_value' => $timezone,
      '#weight' => $instance['widget']['weight'] + .2,
      );  
  }

  // Add a date repeat form element, if needed.
  if (module_exists('date_repeat') && $field['settings']['repeat'] == 1) {
    module_load_include('inc', 'date', 'date_repeat');    
    _date_repeat_widget($element, $field, $instance, $items, $delta);
    $element['rrule']['#weight'] = $instance['widget']['weight'] + .4;
  }

  return $element;
}

function date_combo_value_callback($element, $input = FALSE, &$form_state) {
  if (!$input) {
    return array();
  }
}

/**
 * Create local date object.
 *
 * Create a date object set to local time from the field and
 * widget settings and item values, using field settings to
 * determine what to do with empty values.
 */
function date_local_date($form, $form_state, $delta, $item, $timezone, $field, $instance, $part = 'value') {
  if (!empty($form['nid']['#value'])) {
    $default_value = '';
    $default_value_code = '';
  }
  elseif ($part == 'value') {
    $default_value = $instance['settings']['default_value'];
    $default_value_code = $instance['settings']['default_value_code'];
  }
  else {
    $default_value = $instance['settings']['default_value2'];
    $default_value_code = $instance['settings']['default_value_code2'];
  }
  if (empty($item) || empty($item[$part])) {
    if (empty($default_value) || $default_value == 'blank' || $delta > 0) {
      return NULL;
    }
    elseif ($part == 'value2' && $default_value == 'same') {
      if ($instance['settings']['default_value'] == 'blank' || empty($item['value'])) {
        return NULL;
      }
      else {
        $date = new DateObject($item['value'], $timezone, DATE_FORMAT_DATETIME);
        $date->limitGranularity($field['settings']['granularity']);
      }
    }
    // Special case for 'now' when using dates with no timezone,
    // make sure 'now' isn't adjusted to UTC value of 'now' .
    elseif ($field['settings']['tz_handling'] == 'none') {
      $date = date_now();
    }
    else {
      $date = date_now($timezone);
    }
  }
  else {
    $value = $item[$part];

    // @TODO Figure out how to replace date_fuzzy_datetime() function.
    // Special case for ISO dates to create a valid date object for formatting.
    // Is this still needed?
    /*
    if ($field['type'] == DATE_ISO) {
      $value = date_fuzzy_datetime($value);
    }
    else {
      $db_timezone = date_get_timezone_db($field['settings']['tz_handling']);
      $value = date_convert($value, $field['type'], DATE_DATETIME, $db_timezone);
    }
    */

    $date = new DateObject($value, date_get_timezone_db($field['settings']['tz_handling']));
    $date->limitGranularity($field['settings']['granularity']);
    if (empty($date)) {
      return NULL;
    }
    date_timezone_set($date, timezone_open($timezone));
  }
  if (is_object($date) && empty($item[$part]) && $default_value == 'strtotime' && !empty($default_value_code)) {
    date_modify($date, $default_value_code);
  }
  return $date;
}

/**
 * Process an individual date element.
 */
function date_combo_element_process($element, &$form_state, $form) {
  if (isset($element['#access']) && empty($element['#access'])) {
    return;
  }

  $field_name = $element['#field_name'];
  $delta = $element['#delta'];
  $bundle = $element['#bundle'];
  $entity_type = $element['#entity_type'];
  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);

  $columns = $element['#columns'];
  if (isset($columns['rrule'])) {
    unset($columns['rrule']);
  }
  $from_field = 'value';
  $to_field = 'value2';
  $tz_field = 'timezone';
  $offset_field = 'offset';
  $offset_field2 = 'offset2';

  if ($field['settings']['todate'] != 'required' 
  && !empty($element['#default_value'][$to_field]) 
  && $element['#default_value'][$to_field] == $element['#default_value'][$from_field]) {
    unset($element['#default_value'][$to_field]);
  }

  $element[$from_field] = array(
    '#field'         => $field,
    '#title'         => t($instance['label']),
    '#weight'        => $instance['widget']['weight'],
    '#required'      => ($instance['required'] && $delta == 0) ? 1 : 0,
    '#default_value' => isset($element['#default_value'][$from_field]) ? $element['#default_value'][$from_field] : '',
    '#field'         => $field,
    '#delta'         => $delta,
    '#date_timezone' => $element['#date_timezone'],
    '#date_format'      => date_limit_format(date_input_format($element, $field, $instance), $field['settings']['granularity']),
    '#date_text_parts'  => (array) $instance['widget']['settings']['text_parts'],
    '#date_increment'   => $instance['widget']['settings']['increment'],
    '#date_year_range'  => $instance['widget']['settings']['year_range'],
    '#date_label_position' => $instance['widget']['settings']['label_position'],
    );

  $description =  !empty($instance['description']) ? t($instance['description']) : ''; 

  // Give this element the right type, using a Date API
  // or a Date Popup element type.

  switch ($instance['widget']['type']) {
    case 'date_select':
    case 'date_select_repeat':
      // From/to selectors with lots of parts will look better if displayed 
      // on two rows instead of in a single row.
      if (!empty($field['settings']['todate']) && count($field['settings']['granularity']) > 3) {
        $element[$from_field]['#attributes'] = array('class' => array('date-clear'));
      }
      $element[$from_field]['#type'] = 'date_select';
      $element[$from_field]['#theme_wrappers'] = array('date_select');
      break;
    case 'date_popup':
    case 'date_popup_repeat':  
      $element[$from_field]['#type'] = 'date_popup';
      $element[$from_field]['#theme_wrappers'] = array('date_popup');
      break;
    default:
      $element[$from_field]['#type'] = 'date_text';
      $element[$from_field]['#theme_wrappers'] = array('date_text');
      break;
  }

  // If this field uses the 'To', add matching element
  // for the 'To' date, and adapt titles to make it clear which
  // is the 'From' and which is the 'To' .

  if (!empty($field['settings']['todate'])) {
    $element['#date_float'] = TRUE;
    $element[$from_field]['#title']  = t('From date');
    $element[$to_field] = $element[$from_field];
    $element[$to_field]['#title'] = t('To date');
    $element[$to_field]['#default_value'] = isset($element['#default_value'][$to_field]) ? $element['#default_value'][$to_field] : '';
    $element[$to_field]['#required'] = FALSE;
    $element[$to_field]['#weight'] += .1;
    if ($instance['widget']['type'] == 'date_select') {
      $description .= ' ' . t("Empty 'To date' values will use the 'From date' values.");
    }
    $element['#fieldset_description'] = $description;
  }
  else {
    $element[$from_field]['#description'] = $description;
  }

  // Create label for error messages that make sense in multiple values
  // and when the title field is left blank.
  if (!empty($field['cardinality']) && empty($field['settings']['repeat'])) {
    $element[$from_field]['#date_title'] = t('@field_name From date value #@delta', array('@field_name' => $instance['label'], '@delta' => $delta + 1));
    if (!empty($field['settings']['todate'])) {
      $element[$to_field]['#date_title'] = t('@field_name To date value #@delta', array('@field_name' => $instance['label'], '@delta' => $delta + 1));
    }
  }
  elseif (!empty($field['settings']['todate'])) {
    $element[$from_field]['#date_title'] = t('@field_name From date', array('@field_name' => $instance['label']));
    $element[$to_field]['#date_title'] = t('@field_name To date', array('@field_name' => $instance['label']));
  }
  else {
    $element[$from_field]['#date_title'] = $instance['label'];
  }

  // Make sure field info will be available to the validator which
  // does not get the values in $form.
  $form_state['#field_info'][$field['field_name']] = $field;

  return $element;
}

function date_element_empty($element, &$form_state) {
  $item = array();
  $item['value'] = NULL;
  $item['value2']   = NULL;
  $item['timezone']   = NULL;
  $item['offset'] = NULL;
  $item['offset2'] = NULL;
  $item['rrule'] = NULL;
  form_set_value($element, $item, $form_state);
  return $item;
}

/**
 * Validate and update a combo element.
 * Don't try this if there were errors before reaching this point.
 */
function date_combo_validate($element, &$form_state) {

  $form_values = $form_state['values'];
  $field_name = $element['#field_name'];
  $delta = $element['#delta'];
  $langcode = $element['#language'];

  // If the whole field is empty and that's OK, stop now.
  if (empty($form_state['input'][$field_name]) && !$element['#required']) {
    return;
  }

  // Repeating dates have a different form structure, so get the
  // right item values.
  $item = isset($form_values[$field_name][$langcode]['rrule']) ? $form_values[$field_name][$langcode] : $form_values[$field_name][$langcode][$delta];
  $posted = isset($form_values[$field_name][$langcode]['rrule']) ? $form_state['input'][$field_name][$langcode] : $form_state['input'][$field_name][$langcode][$delta];

  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);

  $from_field = 'value';
  $to_field = 'value2';
  $tz_field = 'timezone';
  $offset_field = 'offset';
  $offset_field2 = 'offset2';

  // Unfortunately, due to the fact that much of the processing is already
  // done by the time we get here, it is not possible highlight the field
  // with an error, we just try to explain which element is creating the
  // problem in the error message.
  $parent = $element['#parents'];
  $error_field = array_pop($parent);
  $errors = array();

  // Check for empty 'From date', which could either be an empty
  // value or an array of empty values, depending on the widget.
  $empty = TRUE;
  if (!empty($item[$from_field])) {
    if (!is_array($item[$from_field])) {
      $empty = FALSE;
    }
    else {
      foreach ($item[$from_field] as $key => $value) {
        if (!empty($value)) {
          $empty = FALSE;
          break;
        }
      }
    }
  }

  // A 'To' date without a 'From' date is a validation error.
  if ($empty && !empty($item[$to_field])) {
    if (!is_array($item[$to_field])) {
      form_set_error($error_field, t("A 'From date' date is required if a 'To date' is supplied for field %field #%delta.", array('%delta' => $field['cardinality'] ? intval($delta + 1) : '', '%field' => t($instance['label']))));
      $empty = FALSE;
    }
    else {
      foreach ($item[$to_field] as $key => $value) {
        if (!empty($value)) {
          form_set_error($error_field, t("A 'From date' date is required if a 'To date' is supplied for field %field #%delta.", array('%delta' => $field['cardinality'] ? intval($delta + 1) : '', '%field' => t($instance['label']))));
          $empty = FALSE;
          break;
        }
      }
    }
  }

  if ($empty) {
    $item = date_element_empty($element, $form_state);
    if (!$element['#required']) {
      return;
    }
  }
  // Don't look for further errors if errors are already flagged
  // because otherwise we'll show errors on the nested elements
  // more than once.
  elseif (!form_get_errors()) {

    // Check todate input for blank values and substitute in fromdate
    // values where needed, then re-compute the todate with those values.
    if ($field['settings']['todate']) {
      $merged_date = array();
      $to_date_empty = TRUE;
      foreach ($posted[$to_field] as $part => $value) {
        $to_date_empty = $to_date_empty && empty($value) && !is_numeric($value);
        $merged_date[$part] = empty($value)  && !is_numeric($value) ? $posted[$from_field][$part] : $value;
        if ($part == 'ampm' && $merged_date['ampm'] == 'pm' && $merged_date['hour'] < 12) {
          $merged_date['hour'] += 12;
        }
        elseif ($part == 'ampm' && $merged_date['ampm'] == 'am' && $merged_date['hour'] == 12) {
          $merged_date['hour'] -= 12;
        }
      }

      // If all date values were empty and a date is required, throw 
      // an error on the first element. We don't want to create 
      // duplicate messages on every date part, so the error will 
      // only go on the first.  
      if ($to_date_empty && $field['settings']['todate'] == 'required') {
        $errors[] = t('Some value must be entered in the To date.');
      }

      $element[$to_field]['#value'] = $merged_date;

      // Call the right function to turn this altered user input into
      // a new value for the todate.
      $item[$to_field] = $merged_date;
    }
    else {
      $item[$to_field] = $item[$from_field];
    }

    $from_date = date_input_date($field, $instance, $element[$from_field], $posted[$from_field]);

    if (!empty($field['settings']['todate'])) {
      $to_date = date_input_date($field, $instance, $element[$to_field], $merged_date);
    }
    else {
      $to_date = $from_date;
    }

    // Neither the from date nor the to date should be empty at this point
    // unless they held values that couldn't be evaluated.
    if (!$instance['required'] && (empty($from_date) || empty($to_date))) {
      $item = date_element_empty($element, $form_state);
      $errors[] = t('The dates are invalid.');
    }
    elseif (!empty($field['settings']['todate']) && $from_date > $to_date) {
      form_set_value($element[$to_field], $to_date, $form_state);
      $errors[] = t('The To date must be greater than the From date.');
    }
    else {
      // Convert input dates back to their UTC values and re-format to ISO
      // or UNIX instead of the DATETIME format used in element processing.
      $timezone = !empty($item[$tz_field]) ? $item[$tz_field] : $element['#date_timezone'];
      $timezone_db = date_get_timezone_db($field['settings']['tz_handling']);
      $item[$tz_field] = $timezone;

      $item[$offset_field] = date_offset_get($from_date);

      $test_from = date_format($from_date, 'r');
      $test_to = date_format($to_date, 'r');

      $item[$offset_field2] = date_offset_get($to_date);
      date_timezone_set($from_date, timezone_open($timezone_db));
      date_timezone_set($to_date, timezone_open($timezone_db));
      $item[$from_field] = date_format($from_date, date_type_format($field['type']));
      $item[$to_field] = date_format($to_date, date_type_format($field['type']));
      if (isset($form_values[$field_name]['rrule'])) {
        $item['rrule'] = $form_values[$field['field_name']]['rrule'];
      }

      // Test a roundtrip back to the original timezone to catch
      // invalid dates, like 2AM on the day that spring daylight savings
      // time begins in the US.
      date_timezone_set($from_date, timezone_open($timezone));
      date_timezone_set($to_date, timezone_open($timezone));
      if ($test_from != date_format($from_date, 'r')) {
        $errors[] = t('The From date is invalid.');
      }
      if ($test_to != date_format($to_date, 'r')) {
        $errors[] = t('The To date is invalid.');
      }
      if (empty($errors)) {
        form_set_value($element, $item, $form_state);
      }
    }
  }
  if (!empty($errors)) {
    if ($field['cardinality']) {
      form_set_error($error_field, t('There are errors in @field_name value #@delta:', array('@field_name' => $instance['label'], '@delta' => $delta + 1)) . theme('item_list', array('items' => $errors)));
    }
    else {
      form_set_error($error_field, t('There are errors in @field_name:', array('@field_name' => $instance['label'])) . theme('item_list', array('items' => $errors)));      
    }
  }
}

/**
 * Handle widget processing.
 */
function date_widget_validate($element, &$form_state) {
  $field = field_widget_field($element, $form_state);
  if (module_exists('date_repeat') && $field['settings']['repeat']) {
    module_load_include('inc', 'date', 'date_repeat');
    return _date_repeat_widget_validate($element, $form_state);
  }
}

/**
 * Determine the input format for this element.
 */
function date_input_format($element, $field, $instance) {
  if (!empty($instance['widget']['settings']['input_format_custom'])) {
    return $instance['widget']['settings']['input_format_custom'];
  }
  elseif (!empty($instance['widget']['settings']['input_format']) && $instance['widget']['settings']['input_format'] != 'site-wide') {
    return $instance['widget']['settings']['input_format'];
  }
  return variable_get('date_format_short', 'm/d/Y - H:i');
}