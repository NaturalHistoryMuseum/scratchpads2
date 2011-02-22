<?php
// $Id: date_repeat_form.inc,v 1.39 2011/01/18 00:37:34 karens Exp $
/**
 * @file
 * Code to add a date repeat selection form to a date field and create
 * an iCal RRULE from the chosen selections.
 *
 * Moved to a separate file since it is not used on most pages
 * so the code is not parsed unless needed.
 *
 * Currently implemented:
 * INTERVAL, UNTIL, EXDATE, RDATE, BYDAY, BYMONTHDAY, BYMONTH,
 * YEARLY, MONTHLY, WEEKLY, DAILY
 *
 * Currently not implemented:
 *
 * BYYEARDAY, MINUTELY, HOURLY, SECONDLY, BYMINUTE, BYHOUR, BYSECOND
 *   These could be implemented in the future.
 *
 * COUNT
 *   The goal of this module is to create a way we can parse an iCal
 *   RRULE and pull out just dates for a specified date range, for
 *   instance with a date that repeats daily for several years, we might
 *   want to only be able to pull out the dates for the current year.
 *
 *   Adding COUNT to the rules we create makes it impossible to do that
 *   without parsing and computing the whole range of dates that the rule
 *   will create. COUNT is left off of the user form completely for this
 *   reason.
 *
 * BYSETPOS
 *   Seldom used anywhere, so no reason to complicated the code.
 */
/**
 * Generate the repeat setting form.
 */
function _date_repeat_rrule_process($element, $form_state, $form) {
  module_load_include('inc', 'date_api', 'date_api_ical');

  if (empty($element['#date_repeat_widget'])) {
    $element['#date_repeat_widget'] = module_exists('date_popup') ? 'date_popup' : 'date_select';
  }
  if (is_array($element['#default_value'])) {
    $element['#value'] = date_repeat_merge($element['#value'], $element);
    $rrule = date_api_ical_build_rrule($element['#value']);
  }
  else {
    $rrule = $element['#default_value'];
  }

  // Empty the original string value of the RRULE so we can create
  // an array of values for the form from the RRULE's contents.
  $element['#value'] = '';

  $parts = date_repeat_split_rrule($rrule);
  $rrule = $parts[0];
  $exceptions = $parts[1];
  $additions = $parts[2];
  $timezone = !empty($element['#date_timezone']) ? $element['#date_timezone'] : date_default_timezone();
  $merged_values = date_repeat_merge($rrule, $element);

  $UNTIL = '';
  if (!empty($merged_values['UNTIL']['datetime'])) {
    $until_date = new DateObject($merged_values['UNTIL']['datetime'], $merged_values['UNTIL']['tz']);
    date_timezone_set($until_date, timezone_open($timezone));
    $UNTIL = date_format($until_date, DATE_FORMAT_DATETIME);
  }

  $element['INTERVAL'] = array(
    '#type' => 'select',
    //'#title' => t('Interval'),
    '#default_value' => (!empty($rrule['INTERVAL']) ? $rrule['INTERVAL'] : 0),
    '#options' => INTERVAL_options(),
    '#prefix' => '<div class="date-repeat-input">',
    '#suffix' => '</div>',
  );

  $element['FREQ'] = array(
    '#type' => 'select',
    //'#title' => t('Frequency'),
    '#default_value' => !empty($rrule['FREQ']) ? $rrule['FREQ'] : 'NONE',
    '#options' => FREQ_options(),
    '#prefix' => '<div class="date-repeat-input">',
    '#suffix' => '</div>',
  );
  $element['UNTIL'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="date-clear">',
    '#suffix' => '</div>',
    'datetime' => array(
      '#type' => $element['#date_repeat_widget'],
      '#title' => t('Until'),
      '#description' => t('Date to stop repeating this item.'),
      '#default_value' => $UNTIL,
      '#date_format' => !empty($element['#date_format']) ? date_limit_format($element['#date_format'], array('year', 'month', 'day')) : 'Y-m-d',
      '#date_timezone' => $timezone,
      '#date_text_parts'  => !empty($element['#date_text_parts']) ? $element['#date_text_parts'] : array(),
      '#date_year_range'  => !empty($element['#date_year_range']) ? $element['#date_year_range'] : '-3:+3',
      '#date_label_position' => !empty($element['#date_label_position']) ? $element['#date_label_position'] : 'within',
      ),
    'tz' => array('#type' => 'hidden', '#value' => $element['#date_timezone']),
    'all_day' => array('#type' => 'hidden', '#value' => 1),
    'granularity' => array('#type' => 'hidden', '#value' => serialize(array('year', 'month', 'day'))),
    );

  $collapsed = TRUE;
  if (!empty($merged_values['BYDAY']) || !empty($merged_values['BYMONTH'])) {
    $collapsed = FALSE;
  }
  // start the advanced fieldset
  $element['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#description' => t("If no advanced options are selected, the date will repeat on the day of week of the start date for weekly repeats, otherwise on the month and day of the start date. Use the options below to override that behavior to select specific months and days to repeat on. Use the 'Except' box to input dates that should be omitted from the results.") . ' ',
    '#prefix' => '<div class="date-clear">',
    '#suffix' => '</div>',
    );

  $element['advanced']['BYMONTH'] = array(
    '#type' => 'select',
    '#title' => t('Month', array(), array('context' => 'datetime')),
    '#default_value' => !empty($rrule['BYMONTH']) ? $rrule['BYMONTH'] : '',
    '#options' => array('' => t('-- Any')) + date_month_names(TRUE),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="date-repeat-input">',
    '#suffix' => '</div>',
  );

  $element['advanced']['BYMONTHDAY'] = array(
    '#type' => 'select',
    '#title' => t('Day of Month'),
    '#default_value' => !empty($rrule['BYMONTHDAY']) ? $rrule['BYMONTHDAY'] : '',
    '#options' => array('' => t('-- Any')) + drupal_map_assoc(range(1, 31)) + drupal_map_assoc(range(-1, -31)),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="date-repeat-input">',
    '#suffix' => '</div>',
  );

  $element['advanced']['BYDAY'] = array(
    '#type' => 'select',
    '#title' => t('Day of Week'),
    '#default_value' => !empty($rrule['BYDAY']) ? $rrule['BYDAY'] : '',
    '#options' => array('' => t('-- Any')) + date_repeat_dow_options(),
    //'#attributes' => array('size' => '5'),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="date-repeat-input">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_exceptions'])) {
    $form_state['num_exceptions'] = count($exceptions);
  }
  $max = empty($form_state['num_exceptions']) ? 1 : $form_state['num_exceptions'];
  $element['exceptions'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => empty($form_state['num_exceptions']) ? TRUE : FALSE,
    '#title' => t('Except'),
    '#description' => t('Dates to omit from the list of repeating dates.'),
    '#prefix' => '<div id="date-repeat-exceptions" class="date-repeat">',
    '#suffix' => '</div>',
    );  
  for ($i = 0; $i < $max; $i++) {
    $EXCEPT = '';
    if (!empty($exceptions[$i]['datetime'])) {
      $EXCEPT = $exceptions[$i]['datetime'];
    }
    $element['exceptions']['EXDATE'][$i] = array(
      '#tree' => TRUE,
      'datetime' => array(
        '#type' => $element['#date_repeat_widget'],
        '#default_value' => !empty($EXCEPT) ? $EXCEPT : '0000-00-00 11:59:59',
        '#date_timezone' => !empty($element['#date_timezone']) ? $element['#date_timezone'] : date_default_timezone(),
        '#date_format' => !empty($element['#date_format']) ? date_limit_format($element['#date_format'], array('year', 'month', 'day')) : 'Y-m-d',
        '#date_text_parts'  => !empty($element['#date_text_parts']) ? $element['#date_text_parts'] : array(),
        '#date_year_range'  => !empty($element['#date_year_range']) ? $element['#date_year_range'] : '-3:+3',
        '#date_label_position' => !empty($element['#date_label_position']) ? $element['#date_label_position'] : 'within',
        ),
      'tz' => array('#type' => 'hidden', '#value' => $element['#date_timezone']),
      'all_day' => array('#type' => 'hidden', '#value' => 1),
      'granularity' => array('#type' => 'hidden', '#value' => serialize(array('year', 'month', 'day'))),
      );
  }

  // collect additions in the same way as exceptions - implements RDATE.
  if (empty($form_state['num_additions'])) {
    $form_state['num_additions'] = count($additions);
  }
  $max = empty($form_state['num_additions']) ? 1 : $form_state['num_additions'];
  $element['additions'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => empty($form_state['num_additions']) ? TRUE : FALSE,
    '#title' => t('Additional'),
    '#description' => t('Dates to add to the list of repeating dates.'),
    '#prefix' => '<div id="date-repeat-additions" class="date-repeat">',
    '#suffix' => '</div>',
    );
  for ($i = 0; $i < $max; $i++) {
    $RDATE = '';
    if (!empty($additions[$i]['datetime'])) {
      $RDATE = $additions[$i]['datetime'];
    }
    $element['additions']['RDATE'][$i] = array(
      '#tree' => TRUE,
      'datetime' => array(
        '#type' => $element['#date_repeat_widget'],
        '#default_value' => $RDATE,
        '#date_timezone' => !empty($element['#date_timezone']) ? $element['#date_timezone'] : date_default_timezone_name(),
        '#date_format' => !empty($element['#date_format']) ? date_limit_format($element['#date_format'], array('year', 'month', 'day')) : 'Y-m-d',
        '#date_text_parts'  => !empty($element['#date_text_parts']) ? $element['#date_text_parts'] : array(),
        '#date_year_range'  => !empty($element['#date_year_range']) ? $element['#date_year_range'] : '-3:+3',
        '#date_label_position' => !empty($element['#date_label_position']) ? $element['#date_label_position'] : 'within',
        ),
      'tz' => array('#type' => 'hidden', '#value' => $element['#date_timezone']),
      'all_day' => array('#type' => 'hidden', '#value' => 1),
      'granularity' => array('#type' => 'hidden', '#value' => serialize(array('year', 'month', 'day'))),
      );
  }

  $element['exceptions']['exceptions_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add exception'),
    '#submit' => array('date_repeat_add_exception'),
    '#ajax' => array(
      'callback' => 'date_repeat_add_exception_callback',
      'wrapper' => 'date-repeat-exceptions',
    ),
  ); 
  $element['additions']['additions_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add addition'),
    '#submit' => array('date_repeat_add_addition'),
    '#ajax' => array(
      'callback' => 'date_repeat_add_addition_callback',
      'wrapper' => 'date-repeat-additions',
    ),
  ); 

  $element['#date_repeat_collapsed'] = !empty($rrule['INTERVAL']) || !empty($rrule['FREQ']) ? 0 : (!empty($element['#date_repeat_collapsed']) ? $element['#date_repeat_collapsed'] : 0);

  return $element;
}

function date_repeat_add_exception_callback($form, &$form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);
  return $element;
}

function date_repeat_add_addition_callback($form, &$form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);
  return $element;
}

function date_repeat_add_exception($form, &$form_state) {
  $form_state['num_exceptions']++;
  $form_state['rebuild'] = TRUE;
}

function date_repeat_add_addition($form, &$form_state) {
  $form_state['num_additions']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Regroup values back into a consistant array, no matter what state it is in.
 */
function date_repeat_merge($form_values, $element) {
  if (empty($form_values) || !is_array($form_values)) {
    return $form_values;
  }
  if (array_key_exists('advanced', $form_values) || array_key_exists('exceptions', $form_values) || array_key_exists('additions', $form_values)) {
    if (!array_key_exists('advanced', $form_values)) $form_values['advanced'] = array();
    if (!array_key_exists('exceptions', $form_values)) $form_values['exceptions'] = array();
    if (!array_key_exists('additions', $form_values)) $form_values['additions'] = array();
    $form_values = array_merge($form_values, (array) $form_values['advanced'], (array) $form_values['exceptions'], (array) $form_values['additions']);
    unset($form_values['advanced']);
    unset($form_values['exceptions']);
    unset($form_values['additions']);
  }
  if (array_key_exists('BYDAY', $form_values)) unset($form_values['BYDAY']['']);
  if (array_key_exists('BYMONTH', $form_values)) unset($form_values['BYMONTH']['']);
  if (array_key_exists('BYMONTHDAY', $form_values)) unset($form_values['BYMONTHDAY']['']);

  if (array_key_exists('UNTIL', $form_values) && is_array($form_values['UNTIL']['datetime'])) {
    $function = $element['#date_repeat_widget'] . '_input_date';
    $until_element = $element;
    $until_element['#date_format'] = date_limit_format($element['#date_format'], array('year', 'month', 'day'));
    $date = $function($until_element, $form_values['UNTIL']['datetime']);
    $form_values['UNTIL']['datetime'] = is_object($date) ? $date->format(DATE_FORMAT_DATETIME) : '';
  }
  if (array_key_exists('EXDATE', $form_values) && is_array($form_values['EXDATE'])) {
    $function = $element['#date_repeat_widget'] . '_input_date';
    $exdate_element = $element;
    foreach ($form_values['EXDATE'] as $delta => $value) {
      if (is_array($value['datetime'])) {
        $exdate_element['#date_format'] = date_limit_format($element['#date_format'], array('year', 'month', 'day'));
        $date = $function($exdate_element, $form_values['EXDATE'][$delta]['datetime']);
        $form_values['EXDATE'][$delta]['datetime'] = is_object($date) ? $date->format(DATE_FORMAT_DATETIME) : '';
      }
    }
  }
  if (array_key_exists('RDATE', $form_values) && is_array($form_values['RDATE'])) {
    $function = $element['#date_repeat_widget'] .'_input_date';
    $rdate_element = $element;
    foreach ($form_values['RDATE'] as $delta => $value) {
      if (is_array($value['datetime'])) {
        $rdate_element['#date_format'] = date_limit_format($element['#date_format'], array('year', 'month', 'day'));
        $form_values['RDATE'][$delta]['datetime'] = $function($rdate_element, $form_values['RDATE'][$delta]['datetime']);
      }
    }
  }
  return $form_values;
}

/**
 * Build a RRULE out of the form values.
 */
function date_repeat_rrule_validate($element, &$form_state) {
  module_load_include('inc', 'date_api', 'date_api_ical');

  $form_values = $form_state['values'];
  $item = $form_values;
  foreach ($element['#parents'] as $key) {
    $item = $item[$key];
  }
  $item = date_repeat_merge($item, $element);
  if (!empty($item['UNTIL']['datetime']) && is_object($item['UNTIL']['datetime'])) {
    $item['UNTIL']['datetime']->setTime(23, 59, 59);
    $item['UNTIL']['datetime']->setTimeZone(new DateTimeZone('UTC'));
    $item['UNTIL']['tz'] = 'UTC';
  }
  $rrule = date_api_ical_build_rrule($item);
  form_set_value($element, $rrule, $form_state);
}

/**
 * Theme the exception list as a table so the buttons line up
 */
function theme_date_repeat_current_exceptions($vars) {
  $rows = $vars['rows'];
  $rows_info = array();
  foreach ($rows as $key => $value) {
    if (substr($key, 0, 1) != '#') {
      $rows_info[] = array(drupal_render($value['action']), drupal_render($value['display']));
    }
  }
  return theme('table', array('header' => array(t('Delete'), t('Current exceptions')), 'rows' => $rows_info));
}

 /**
 * Theme the exception list as a table so the buttons line up
 */
function theme_date_repeat_current_additions($rows = array()) {
  $rows_info = array();
  foreach ($rows as $key => $value) {
    if (substr($key, 0, 1) != '#') {
      $rows_info[] = array(drupal_render($value['action']), drupal_render($value['display']));
    }
  }
  return theme('table', array('header' => array(t('Delete'), t('Current additions')), 'rows' => $rows_info));
} 

/**
 * Wrapper fieldset for repeat rule.
 */
function theme_date_repeat_rrule($vars) {
  $element = $vars['element'];
  $class = $element['#date_repeat_collapsed'] ? array('collapsible', 'collapsed') : array('collapsible');
  $fieldset = array(
    '#title' => t('Repeat'),
    '#value' => '',
    '#description' => theme('advanced_help_topic', 'date_api', 'date-repeat-form') . t('Choose a frequency and period to repeat this date. If nothing is selected, the date will not repeat.'),
    '#attributes' => array('class' => $class),
    '#children' => $element['#children'],
  );
  // Make sure we don't get floating parts where we don't want them.
  $element['#prefix'] = '<div class="date-clear">';
  $element['#suffix'] = '</div>';

  return theme('fieldset', array('element' => $fieldset));
}