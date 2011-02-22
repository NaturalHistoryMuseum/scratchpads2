<?php
// $Id: date_api_elements.inc,v 1.2 2011/01/18 00:37:34 karens Exp $
/**
 * @file
 * Date API elements themes and validation.
 * This file is only included during the edit process to reduce memory usage.
 */

/**
 * Implementation of hook_element_info().
 *
 * Parameters for date form elements, designed to have sane defaults so any
 * or all can be omitted.
 *
 * Fill the element #default_value with a date in datetime format,
 * (YYYY-MM-DD HH:MM:SS), adjusted to the proper local timezone.
 *
 * NOTE - Converting a date stored in the database from UTC to the local zone
 * and converting it back to UTC before storing it is not handled by this
 * element and must be done in pre-form and post-form processing!!
 *
 * The date_select element will create a collection of form elements, with a
 * separate select or textfield for each date part. The whole collection will
 * get re-formatted back into a date value of the requested type during validation.
 *
 * The date_text element will create a textfield that can contain a whole
 * date or any part of a date as text. The user input value will be re-formatted
 * back into a date value of the requested type during validation.
 *
 * The date_timezone element will create a drop-down selector to pick a
 * timezone name.
 *
 * #date_timezone
 *   The local timezone to be used to create this date.
 *
 * #date_format
 *   A format string that describes the format and order of date parts to
 *   display in the edit form for this element. This makes it possible
 *   to show date parts in a custom order, or to leave some of them out.
 *   Be sure to add 'A' or 'a' to get an am/pm selector. Defaults to the
 *   short site default format.
 *
 * #date_label_position
 *   Handling option for date part labels, like 'Year', 'Month', and 'Day',
 *   can be 'above' the date part, 'within' it, or 'none', default is 'above' .
 *   The 'within' option shows the label as the first option in a select list
 *   or the default value for an empty textfield, taking up less screen space.
 *
 * #date_increment
 *   Increment minutes and seconds by this amount, default is 1.
 *
 * #date_year_range
 *   The number of years to go back and forward in a year selector,
 *   default is -3:+3 (3 back and 3 forward).
 *
 * #date_text_parts
 *   Array of date parts that should use textfields instead of selects
 *   i.e. array('year') will format the year as a textfield and other
 *   date parts as drop-down selects.
 */
function _date_api_element_info() {
  $date_base = array(
    '#input' => TRUE, '#tree' => TRUE,
    '#date_timezone' => date_default_timezone(),
    '#date_flexible' => 0,
    '#date_format' => variable_get('date_format_short', 'm/d/Y - H:i'),
    '#date_text_parts' => array(),
    '#date_increment' => 1,
    '#date_year_range' => '-3:+3',
    '#date_label_position' => 'above',
    );
  $type['date_select'] = array_merge($date_base, array(
    '#process' => array('date_select_element_process'),
    '#theme_wrappers' => array('date_select'),
    '#value_callback' => 'date_select_element_value_callback',
    ));
  $type['date_text'] = array_merge($date_base, array(
    '#process' => array('date_text_element_process'),
    '#theme_wrappers' => array('date_text'),
    '#value_callback' => 'date_text_element_value_callback',
    ));
  $type['date_timezone'] = array(
    '#input' => TRUE, '#tree' => TRUE,
    '#process' => array('date_timezone_element_process'),
    '#theme_wrappers' => array('date_text'),
    '#value_callback' => 'date_timezone_element_value_callback',
    );  
  return $type;
}

/**
 * Helper function to round minutes and seconds to requested value.
 */
function date_increment_round(&$date, $increment) {
  // Round minutes and seconds, if necessary.
  if (is_object($date) && $increment > 1) {
    $day = intval(date_format($date, 'j'));
    $hour = intval(date_format($date, 'H'));
    $second = intval(round(intval(date_format($date, 's')) / $increment) * $increment);
    $minute = intval(date_format($date, 'i'));
    if ($second == 60) {
      $minute += 1;
      $second = 0;
    }
    $minute = intval(round($minute / $increment) * $increment);
    if ($minute == 60) {
      $hour += 1;
      $minute = 0;
    }
    date_time_set($date, $hour, $minute, $second);
    if ($hour == 24) {
      $day += 1;
      $hour = 0;
      $year = date_format($date, 'Y');
      $month = date_format($date, 'n');
      date_date_set($date, $year, $month, $day);
    }
  }
  return $date;
}

/**
 * Create a date object from a datetime string value.
 */
function date_default_date($element) {
  $granularity = date_format_order($element['#date_format']);
  $date = new DateObject($element['#default_value'], $element['#date_timezone'], DATE_FORMAT_DATETIME);
  if (is_object($date)) {
    $date->limitGranularity($granularity);
    if ($date->validGranularity($granularity, $element['#date_flexible'])) {
      date_increment_round($date, $element['#date_increment']); 
      return $date;
    }
  }    
  return NULL;
}

/**
 * Element value callback for date_timezone element.
 */
function date_timezone_element_value_callback($element, $input = FALSE, &$form_state) {
  //if (!$input) {
    return array();
  //}
}

/**
 * Create a timezone form element.
 *
 * @param array $element
 * @return array
 *   the timezone form element
 */
function date_timezone_element_process($element, $form_state, $form) {
  $element['#tree'] = TRUE;
  $element['timezone'] = array(
    '#type' => 'select',
    '#title' => theme('date_part_label_timezone', array('part_type' => 'select', 'element' => $element)),
    '#default_value' => $element['#default_value'],
    '#options' => date_timezone_names($element['#required']),
    '#weight' => $element['#weight'],
    '#required' => $element['#required'],
    '#theme' => 'date_select_element',
    '#theme_wrappers' => array('form_element'),
  );
  if (isset($element['#element_validate'])) {
    array_push($element['#element_validate'], 'date_timezone_validate');
  }
  else {
    $element['#element_validate'] = array('date_timezone_validate');
  }
  // TODO This sometimes causes problems, do we need it?
  //$element['#attributes'] = array('class' => array('date-timezone clear-block'));
  return $element;
}

/**
 *  Validation for timezone input
 *
 *  Move the timezone value from the nested field back to the original field.
 */
function date_timezone_validate($element, &$form_state) {
  form_set_value($element, $element['#value']['timezone'], $form_state);
}

/**
 * Element value callback for date_text element.
 */
function date_text_element_value_callback($element, $input = FALSE, &$form_state) {
  $return = array('date' => ''); 
  $date = NULL;
  if ($input !== FALSE) {
    $return = $input;
    $date = date_text_input_date($element, $input);  
  }
  elseif (!empty($element['#default_value'])) {
    $date = date_default_date($element);
  }
  if (is_object($date) && empty($date->errors)) {
    $return['date'] = $date->format($element['#date_format']);
  }
  return $return;
}

/**
 * Text date input form.
 *
 * Display all or part of a date in a single textfield.
 *
 * The exact parts displayed in the field are those in #date_granularity.
 * The display of each part comes from #date_format.
 * 
 */
function date_text_element_process($element, $form_state, $form) {

  $element['#tree'] = TRUE;
  $element['#theme_wrappers'] = array('date_text');
  $element['date']['#value'] = $element['#value']['date'];
  $element['date']['#type'] = 'textfield';
  $element['date']['#weight'] = !empty($element['date']['#weight']) ? $element['date']['#weight'] : $element['#weight'];
  $element['date']['#attributes'] = array('class' => isset($element['#attributes']['class']) ? $element['#attributes']['class'] += array('date-date') : array('date-date'));
  $element['date']['#description'] = ' ' . t('Format: @date', array('@date' => date_now()->format($element['#date_format'])));

  // Keep the system from creating an error message for the sub-element.
  // We'll set our own message on the parent element.
  //$element['date']['#required'] = $element['#required'];
  $element['date']['#theme'] = 'date_textfield_element';
  if (isset($element['#element_validate'])) {
    array_push($element['#element_validate'], 'date_text_validate');
  }
  else {
    $element['#element_validate'] = array('date_text_validate');
  }
  if (!empty($element['#force_value'])) {
    $element['date']['#value'] = $element['date']['#default_value'];
  }

  return $element;
}

/**
 *  Validation for text input.
 * 
 * When used as a Views widget, the validation step always gets triggered,
 * even with no form submission. Before form submission $element['#value']
 * contains a string, after submission it contains an array.
 * 
 */
function date_text_validate($element, &$form_state) {
  if (is_string($element['#value'])) {
    return;
  }
  $input_exists = NULL;
  $input = drupal_array_get_nested_value($form_state['input'], $element['#parents'], $input_exists);

  $label = !empty($element['#date_title']) ? $element['#date_title'] : (!empty($element['#title']) ? $element['#title'] : '');
  $date = date_text_input_date($element, $input);
  if (empty($date) && !empty($element['#required'])) {
    form_error($element, t('A valid date is required for %title.', array('%title' => $label))); 
  }
  elseif (empty($date) && !empty($element['#value']['date'])) {
    form_error($element, t('%title is invalid.', array('%title' => $label))); 
  }
  elseif (!empty($date->errors)) {
    form_error($element, implode('<br />', $date->errors));
  }
  elseif (!empty($date)) {
    $value = $date->format(DATE_FORMAT_DATETIME);
    form_set_value($element, $value, $form_state);
  }
}

/**
 * Helper function for creating a date object out of user input.
 */
function date_text_input_date($element, $input) {
  if (empty($input) || !is_array($input) || !array_key_exists('date', $input)) {
    return NULL;
  }  
  $granularity = date_format_order($element['#date_format']);
  $date = new DateObject($input['date'], $element['#date_timezone'], $element['#date_format']);
  if (is_object($date)) {
    $date->limitGranularity($granularity);
    if ($date->validGranularity($granularity, $element['#date_flexible'])){
      date_increment_round($date, $element['#date_increment']); 
      return $date;
    }  
  }
  return NULL;
}

/**
 * Element value callback for date_select element.
 */
function date_select_element_value_callback($element, $input = FALSE, &$form_state) {
  $return = array('year' => '', 'month' => '', 'day' => '', 'hour' => '', 'minute' => '', 'second' => ''); 
  $date = NULL;
  if ($input !== FALSE) {
    $return = $input;
    $date = date_select_input_date($element, $input);  
  }
  elseif (!empty($element['#default_value'])) {
    $date = date_default_date($element);
  }
  $granularity = date_format_order($element['#date_format']);
  $formats = array('year' => 'Y', 'month' => 'n', 'day' => 'j', 'hour' => 'H', 'minute' => 'i', 'second' => 's');
  foreach ($granularity as $field) {
    if ($field != 'timezone') {
      $return[$field] = is_object($date) ? $date->format($formats[$field]) : '';
    }
  }
  return $return;
}

/**
 * Flexible date/time drop-down selector.
 *
 * Splits date into a collection of date and time sub-elements, one
 * for each date part. Each sub-element can be either a textfield or a
 * select, based on the value of ['#date_settings']['text_fields'].
 *
 * The exact parts displayed in the field are those in #date_granularity.
 * The display of each part comes from ['#date_settings']['format'].
 * 
 */
function date_select_element_process($element, $form_state, $form) {
  $date = NULL;
  $granularity = date_format_order($element['#date_format']);

  $input_exists = NULL;
  $input = drupal_array_get_nested_value($form_state['input'], $element['#parents'], $input_exists);

  if (is_array($element['#default_value'])) {
    $date = date_select_input_date($element, $element['#default_value']);
  }
  elseif (!empty($element['#default_value'])) {
    $date = date_default_date($element);
  }

  $element['#tree'] = TRUE;
  $element['#theme_wrappers'] = array('date_select');

  $element += (array) date_parts_element($element, $date, $element['#date_format']);

  // Store a hidden value for all date parts not in the current display.
  $granularity = date_format_order($element['#date_format']);
  $formats = array('year' => 'Y', 'month' => 'n', 'day' => 'j', 'hour' => 'H', 'minute' => 'i', 'second' => 's');
  foreach (date_nongranularity($granularity) as $field) {
    if ($field != 'timezone') {
      $element[$field] = array(
        '#type' => 'value',
        '#value' => 0,
        );
    }
  }
  if (isset($element['#element_validate'])) {
    array_push($element['#element_validate'], 'date_select_validate');
  }
  else {
    $element['#element_validate'] = array('date_select_validate');
  }

  return $element;
}

/**
 * Create form elements for one or more date parts.
 *
 * Get the order of date elements from the provided format.
 * If the format order omits any date parts in the granularity, alter the
 * granularity array to match the format, then flip the $order array
 * to get the position for each element. Then iterate through the
 * elements and create a sub-form for each part.
 *
 * @param array $element
 * @param object $date
 * @param array $granularity
 * @param string $format
 * @return array
 *   the form array for the submitted date parts
 */
function date_parts_element($element, $date, $format) {
  $granularity = date_format_order($format);
  $sub_element = array('#granularity' => $granularity);
  $order = array_flip($granularity);

  $hours_format  = strpos(strtolower($element['#date_format']), 'a') ? 'g': 'G';
  $month_function  = strpos($element['#date_format'], 'F') !== FALSE ? 'date_month_names' : 'date_month_names_abbr';
  $count = 0;
  $increment = min(intval($element['#date_increment']), 1);
  foreach ($granularity as $field) {
    // Allow empty value as option if date is not required 
    // or if empty value was provided as a starting point.
    $part_required = ($element['#required'] && is_object($date)) ? TRUE : FALSE;
    $part_type = in_array($field, $element['#date_text_parts']) ? 'textfield' : 'select';
    $sub_element[$field] = array(
      '#weight' => $order[$field],
      '#required' => $element['#required'],
      '#attributes' => array('class' => isset($element['#attributes']['class']) ? $element['#attributes']['class'] += array('date-' . $field) : array('date-' . $field)),
      );
    switch ($field) {
      case 'year':
        $range = date_range_years($element['#date_year_range'], $date);
        $min_year = $range[0];
        $max_year = $range[1];

        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format('Y') : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = drupal_map_assoc(date_years($min_year, $max_year, $part_required));
        }
        break;
      case 'month':
        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format('n') : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = $month_function($part_required);
        }
        break;
      case 'day':
        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format('j') : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = drupal_map_assoc(date_days($part_required));
        }
        break;
      case 'hour':
        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format($hours_format) : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = drupal_map_assoc(date_hours($hours_format, $part_required));
        }
        $sub_element[$field]['#prefix'] = theme('date_part_hour_prefix', $element);
        break;
      case 'minute':
        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format('i') : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = drupal_map_assoc(date_minutes('i', $part_required, $element['#date_increment']));
        }
        $sub_element[$field]['#prefix'] = theme('date_part_minsec_prefix', $element);
        break;
      case 'second':
        $sub_element[$field]['#default_value'] = is_object($date) ? $date->format('s') : '';
        if ($part_type == 'select') {
          $sub_element[$field]['#options'] = drupal_map_assoc(date_seconds('s', $part_required, $element['#date_increment']));
        }
        $sub_element[$field]['#prefix'] = theme('date_part_minsec_prefix', $element);
        break;
    }

    // Add handling for the date part label.
    $label = theme('date_part_label_' . $field, array('part_type' => $part_type, 'element' => $element));
    if (in_array($field, $element['#date_text_parts'])) {
      $sub_element[$field]['#type'] = 'textfield';
      $sub_element[$field]['#theme'] = 'date_textfield_element';
      $sub_element[$field]['#size'] = 7;
      if ($element['#date_label_position'] == 'within') {
        if (!empty($sub_element[$field]['#options']) && is_array($sub_element[$field]['#options'])) {
          $sub_element[$field]['#options'] = array(
            '-' . $label => '-' . $label) + $sub_element[$field]['#options'];
        }
        if (empty($sub_element[$field]['#default_value'])) {
          $sub_element[$field]['#default_value'] = '-' . $label;
        }
      }
      elseif ($element['#date_label_position'] != 'none') {
        $sub_element[$field]['#title'] = $label;
      }
    }
    else {
      $sub_element[$field]['#type'] = 'select';
      $sub_element[$field]['#theme'] = 'date_select_element';
      if ($element['#date_label_position'] == 'within') {
        $sub_element[$field]['#options'] = array(
          '' => '-' . $label) + $sub_element[$field]['#options'];
      }
      elseif ($element['#date_label_position'] != 'none') {
        $sub_element[$field]['#title'] = $label;
      }
    }
  }

  // Views exposed filters are treated as submitted even if not,
  // so force the #default value in that case. Make sure we set
  // a default that is in the option list.
  if (!empty($element['#force_value'])) {
    $options = $sub_element[$field]['#options'];
    $default = !empty($sub_element[$field]['#default_value']) ? $sub_element[$field]['#default_value'] : array_shift($options);
    $sub_element[$field]['#value'] = $default;
  }

  if (($hours_format == 'g' || $hours_format == 'h') && date_has_time($granularity)) {
    $sub_element['ampm'] = array(
      '#type' => 'select',
      '#default_value' => is_object($date) ? (date_format($date, 'G') >= 12 ? 'pm' : 'am') : '',
      '#options' => drupal_map_assoc(date_ampm()),
      '#weight' => 8,
      '#attributes' => array('class' => array('date-ampm')),
    );
    if ($element['#date_label_position'] == 'within') {
      $sub_element['ampm']['#options'] = array('' => '-' . theme('date_part_label_ampm', array('part_type' => 'ampm', 'eleement' => $element))) + $sub_element['ampm']['#options'];
    }
    elseif ($element['#date_label_position'] != 'none') {
      $sub_element['ampm']['#title'] = theme('date_part_label_ampm', array('part_type' => 'ampm', 'element' => $element));
    }
  }
  return $sub_element;
}

/**
 *  Validation function for date selector.
 * 
 * When used as a Views widget, the validation step always gets triggered,
 * even with no form submission. Before form submission $element['#value']
 * contains a string, after submission it contains an array.
 * 
 */
function date_select_validate($element, &$form_state) {
  if (is_string($element['#value'])) {
    return;
  }

  $input_exists = NULL;
  $input = drupal_array_get_nested_value($form_state['input'], $element['#parents'], $input_exists);

  // Strip field labels out of the results.
  foreach ($element['#value'] as $field => $field_value) {
    if (substr($field_value, 0, 1) == '-') {
      $input[$field] = '';
    }
  }

  $label = !empty($element['#date_title']) ? $element['#date_title'] : (!empty($element['#title']) ? $element['#title'] : '');

  if (isset($element['#value']['ampm'])) {
    if ($input['ampm'] == 'pm' && $input['hour'] < 12) {
      $input['hour'] += 12;
    }
    elseif ($input['ampm'] == 'am' && $input['hour'] == 12) {
      $input['hour'] -= 12;
    }
  }
  unset($input['ampm']);
  $date = date_select_input_date($element, $input);

  if (empty($date) && $element['#required']) {
    form_error($element, t('A valid value is required.'));
  }
  elseif (!empty($date->errors)) {
    form_error($element, implode('<br />', $date->errors));
  }
  elseif (!empty($date)) {
    form_set_value($element, $date->format(DATE_FORMAT_DATETIME), $form_state);
  }
}

/**
 * Helper function for creating a date object out of user input.
 */
function date_select_input_date($element, $input) {
  if (empty($input) || !is_array($input) || !array_key_exists('year', $input)) {
    return NULL;
  }  
  $granularity = date_format_order($element['#date_format']);
  $date = new DateObject($input, $element['#date_timezone']);
  if (is_object($date)) {
    $date->limitGranularity($granularity);
    if ($date->validGranularity($granularity, $element['#date_flexible'])) {
      date_increment_round($date, $element['#date_increment']); 
      return $date;
    }  
  }
  return NULL;
}
