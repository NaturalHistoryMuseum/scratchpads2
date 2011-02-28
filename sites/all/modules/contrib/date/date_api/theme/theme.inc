<?php
// $Id: theme.inc,v 1.3 2011/01/18 00:37:34 karens Exp $

/**
 * @file
 * Theme files for Date API.
 */
/**
 * Format a date timezone element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_timezone($vars) {
  $element = $vars['element'];
  return '<div class="date-clear">' . theme('form_element', $element, $element['#children']) . '</div>';
}

/**
 * Format a date selection element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_select($vars) {
  $element = $vars['element'];
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' .date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="' . $class . '">' . theme('form_element', $element, $output) . '</div>';
}

/**
 * Format a date text element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_text($vars) {
  $element = $vars['element'];
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' .date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="' . $class . '">' . theme('form_element', $element, $output) . '</div>';
}

/**
 *  Themes for date input form elements
 */
function theme_date_select_element($vars) {
  $element = $vars['element'];
  $parents = $element['#parents'];
  $part = array_pop($parents);
  return '<div class="date-' . $part . '">' . theme('select', $element) . '</div>';
}

function theme_date_textfield_element($vars) {
  $element = $vars['element'];
  $parents = $element['#parents'];
  $part = array_pop($parents);
  return '<div class="date-' . $part . '">' . theme('textfield', $element) . '</div>';
}

/**
 * Functions to separate date parts in form.
 *
 * Separators float up to the title level for elements with titles,
 * so won't work if this element has titles above the element date parts.
 */
function theme_date_part_hour_prefix($vars) {
  $element = $vars['element'];
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">&nbsp;-&nbsp;</span>';
  }
}

function theme_date_part_minsec_prefix($vars) {
  $element = $vars['element'];
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">:</span>';
  }
}

/**
 * Format labels for each date part in a date_select.
 *
 * @param $part_type
 *   the type of field used for this part, 'textfield' .or 'select'
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 */
function theme_date_part_label_year($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Year', array(), array('context' => 'datetime'));
}
function theme_date_part_label_month($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Month', array(), array('context' => 'datetime'));
}
function theme_date_part_label_day($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Day', array(), array('context' => 'datetime'));
}
function theme_date_part_label_hour($vars) {
  return t('Hour', array(), array('context' => 'datetime'));
}
function theme_date_part_label_minute($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Minute', array(), array('context' => 'datetime'));
}
function theme_date_part_label_second($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Second', array(), array('context' => 'datetime'));
}
function theme_date_part_label_ampm($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return ' . ';
}
function theme_date_part_label_timezone($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Timezone');
}

/**
 * Theme for a date block that looks like a mini calendar day.
 * Pass in a date object already set to the right timezone, 
 * format as a calendar page date. The calendar styling is created in css.
 */
function theme_date_calendar_day($vars) {
  $date = $vars['date'];
  if (empty($date)) {
    return NULL;
  }
  return '<div class="date-calendar-day">' .
    '<span class="month">' . date_format_date($date, 'custom', 'M') . '</span>' .
    '<span class="day">' . date_format_date($date, 'custom', 'j') . '</span>' .
    '<span class="year">' . date_format_date($date, 'custom', 'Y') . '</span>' .
  '</div>';
}

function theme_date_time_ago($vars) {
  $start_date = $vars['start_date'];
  $end_date = $vars['end_date'];
  $interval = !empty($vars['interval']) ? $vars['interval'] : 2;

  // If no date is sent, then return nothing
  if (empty($start_date) || empty($end_date)) {
    return NULL;
  }

  // Time to compare dates to
  $now = date_format(date_now(), DATE_FORMAT_DATETIME);
  $start = date_format($start_date, DATE_FORMAT_DATETIME);
  $end = date_format($end_date, DATE_FORMAT_DATETIME);

  // 1) The date is entirely in the future
  if ($now < $start) {
    return t('!time from now', array('!time' => date_format_interval($start_date, $interval)));
  }
  // 2) Ongoing date
  elseif ($now > $start && $now <= $end) {
    //return t('Started !time ago', array('!time' => $dates['value']['interval']));
    return t('ongoing');
  }
  // 3) Date is in the past (format_interval added 'ago' .to the value).
  else {
    return date_format_interval($start_date, $interval);
  }
}
