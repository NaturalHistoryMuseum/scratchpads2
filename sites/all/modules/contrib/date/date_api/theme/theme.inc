<?php

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
  $attributes = $element['#attributes'];
  $wrapper_attributes = array();
  // Add an wrapper to mimic the way a single value field works, for ease in using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) .'>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) .'>' . theme('form_element', $element) . '</div>';
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
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());
  $attributes['class'][] = 'container-inline-date';
  $wrapper_attributes = array('class' => array('date-padding'));
  $wrapper_attributes['class'][] = 'clearfix';
  // Add an wrapper to mimic the way a single value field works, for ease in using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) .'>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) .'>' . theme('form_element', $element) . '</div>';
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
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());
  $attributes['class'][] = 'container-inline-date';
  // If there is no description, the floating date elements need some extra padding below them.
  $wrapper_attributes = array('class' => array('date-padding'));
  if (empty($element['date']['#description'])) {
    $wrapper_attributes['class'][] = 'clearfix';
  }
  // Add an wrapper to mimic the way a single value field works, for ease in using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) .'>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) .'>' . theme('form_element', $element) . '</div>';
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
  return '&nbsp;';
}
function theme_date_part_label_timezone($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Timezone');
}
function theme_date_part_label_date($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Date', array(), array('context' => 'datetime'));
}
function theme_date_part_label_time($vars) {
  $part_type = $vars['part_type'];
  $element = $vars['element'];
  return t('Time', array(), array('context' => 'datetime'));
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

/**
 *  Preprocessor to construct an ical vcalendar
 *
 * @param $events
 *   An array of events where each event is an array keyed on the uid:
 *    'start'
 *      Start date object,
 *    'end'
 *      End date object, optional, omit for all day event.
 *    'summary'
 *      Title of event (Text)
 *    'description'
 *      Description of event (Text)
 *    'location'
 *      Location of event (Text or vvenue id)
 *    'uid'
 *      ID of the event for use by calendaring program, usually the url of the node
 *    'url'
 *      URL of event information
 *
 *    'alarm'
 *      sub-array of alarm information for the event, including:
 *      - 'action' .- the action to take, either 'DISPLAY' .or 'EMAIL'
 *      - 'trigger' .- the time period for the trigger, like -P2D.
 *      - 'repeat' .- the number of times to repeat the alarm.
 *      - 'duration' .- the time period between repeated alarms, like P1D.
 *      - 'description' .- the description of the alarm.
 *      An email alarm should have two additional parts:
 *      - 'email' .- a comma-separated list of email recipients.
 *      - 'summary' .- the subject of the alarm email.
 *
 * @param $calname
 *   Name of the calendar.  Use site name if none is specified.
 *
 */
function template_preprocess_date_vcalendar(&$vars) {

  $vars['current_date'] = date_format(date_now(), DATE_FORMAT_ICAL);
  $vars['current_date_utc'] = date_format(date_now('UTC'), DATE_FORMAT_ICAL);
  $vars['site_timezone'] = date_default_timezone();
  $vars['calname'] = date_ical_escape_text(!empty($vars['calname']) ? $vars['calname'] : variable_get('site_name', ''));

  // Format the event results as iCal expects.
  $events_in = $vars['events'];
  $events = array();
  $rows = (array) $vars['rows'];
  foreach ($events_in as $uid => $event) {
    $row = array_shift($rows);
    // Omit any items with empty dates.
    if (!empty($event['start'])) {
      $events[$uid] = $event;
      $timezone = timezone_name_get(date_timezone_get($event['start']));
      if (!empty($timezone)) {
        $events[$uid]['timezone'] = "TZID=$timezone;";
      }
      else {
        $events[$uid]['timezone'] = '';
      }
      $date_format = (!empty($row->calendar_all_day) && $row->calendar_all_day == TRUE) ? DATE_FORMAT_ICAL_DATE : DATE_FORMAT_ICAL;
      $events[$uid]['start'] = date_format($event['start'], $date_format);
      if ($event['start'] && $event['end']) {
        $events[$uid]['end'] = date_format($event['end'], $date_format);
      }
      else {
        $events[$uid]['end'] = $events[$uid]['start'];
      }
      foreach ($event as $key => $value) {
        if (is_string($value)) {
          $event[trim($key)] = trim($value);
        }
      }

      // Escape text values.
      foreach ($event as $key => $value) {
        if ($key == 'alarm') {
          foreach ($value as $alarm_key => $alarm_value) {
            if (in_array($alarm_key, array('summary', 'description'))) {
              $events[$uid]['alarm'][$alarm_key] = date_ical_escape_text($alarm_value);
            }
          }
        }
        elseif (in_array($key, array('summary', 'description', 'location'))) {
          $events[$uid][$key] = date_ical_escape_text(html_entity_decode($value, ENT_QUOTES, 'UTF-8'));
        }
      }
    }
  }

  $vars['events'] = $events;
}

