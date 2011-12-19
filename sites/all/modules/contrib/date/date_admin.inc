<?php
/**
 * @file
 * Date administration code.
 * Moved to separate file since there is a lot of code here that is not needed often.
 */
/**
 * Settings for the default formatter.
 */
function date_default_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $formatter = $display['type'];
  $form = array();

  $form['format_type'] = array(
    '#title' => t('Choose how users view dates and times:'),
    '#type' => 'select',
    '#options' => date_format_type_options(),
    '#default_value' => $settings['format_type'],
    '#description' => t('To add or edit options, visit <a href="@date-time-page">Date and time settings</a>.', array('@date-time-page' => url('admin/config/regional/date-time'))),
    '#weight' => 0,
  );

  $form['fromto'] = array(
    '#title' => t('Display:'),
    '#type' => 'select',
    '#options' => array(
      'both' => t('Both Start and End dates'),
      'value' => t('Start date only'),
      'value2' => t('End date only'),
      ),
    '#access' => $field['settings']['todate'],
    '#default_value' => $settings['fromto'],
    '#weight' => 1,
  );

  // Make the string translatable by keeping it as a whole rather than
  // translating prefix and suffix separately.
  list($prefix, $suffix) = explode('@count', t('Show @count value(s)'));
  $form['multiple_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Multiple values:'),
    '#size' => 5,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_number'],
    '#weight' => 2,
    '#access' => ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
    '#description' => t('Identify a specific number of values to display, or leave blank to show all values.'),
  );

  list($prefix, $suffix) = explode('@isodate', t('starting from @isodate'));
  $form['multiple_from'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_from'],
    '#weight' => 3,
    '#access' => ($field['cardinality'] === FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
  );

  list($prefix, $suffix) = explode('@isodate', t('ending with @isodate'));
  $form['multiple_to'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_to'],
    '#weight' => 4,
    '#access' => ($field['cardinality'] === FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
    '#description' => t('Identify specific start and/or end dates in the format YYYY-MM-DDTHH:MM:SS, or leave blank for all available dates.'),
   );

  $form['show_repeat_rule'] = array(
    '#title' => t('Repeat rule:'),
    '#type' => 'select',
    '#options' => array(
      'show' => t('Display repeat rule'),
      'hide' => t('Hide repeat rule')),
    '#default_value' => $settings['show_repeat_rule'],
    '#access' => $field['settings']['repeat'],
    '#weight' => 5,
  );

  return $form;
}

/**
 * Settings for the interval formatter.
 */
function date_interval_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $form = array();
  $form['interval'] = array(
    '#title' => t('Interval'),
    '#description' => t("How many time units should be shown in the 'time ago' string."),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 6)),
    '#default_value' => $settings['interval'],
    '#weight' => 0,
  );
  return $form;
}

/**
 * Settings summary for the default formatter.
 */
function date_default_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $formatter = $display['type'];
  $format_types = date_format_type_options();
  $summary = array();
  switch($formatter) {
    case 'date_plain':
      $format = t('Plain');
      break;
    case 'format_interval':
      $format = t('Interval');
      break;
    default:
      $format = $format_types[$settings['format_type']];
  }
  $summary[] = t('Display dates using the @format format', array('@format' => $format));

  if (array_key_exists('fromto', $settings) && $field['settings']['todate']) {
    $options = array(
      'both' => t('Display both Start and End dates'),
      'value' => t('Display Start date only'),
      'value2' => t('Display End date only'),
    );
    $summary[] = $options[$settings['fromto']];
  }

  if (array_key_exists('multiple_number', $settings) && !empty($field['cardinality'])) {
    $summary[] = t('Show @count value(s) starting with @date1, ending with @date2', array(
      '@count' => !empty($settings['multiple_number']) ? $settings['multiple_number'] : t('all'),
      '@date1' => !empty($settings['multiple_from']) ? $settings['multiple_from'] : t('earliest'),
      '@date2' => !empty($settings['multiple_to']) ? $settings['multiple_to'] : t('latest'),
    ));
  }

  if (array_key_exists('show_repeat_rule', $settings) && !empty($field['repeat'])) {
    if (!empty($settings['show_repeat_rule'])) {
      $summary[] = t('Show repeat rule');
    }
    else {
      $summary[] = t('Do not show repeat rule');
    }
  }

  return implode('<br />', $summary);
}

/**
 * Settings summary for the interval formatter.
 * @TODO Add settings later.
 */
function date_interval_formatter_settings_summary($field, $instance, $view_mode) {
  $summary = array();
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $formatter = $display['type'];
  return t('Display time ago, showing @interval units.', array('@interval' => $settings['interval']));

}

function _date_field_instance_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $instance['settings'];
  $widget_settings = $instance['widget']['settings'];

  $form['default_value'] = array(
    '#type' => 'select',
    '#title' => t('Default date'),
    '#default_value' => $settings['default_value'],
    '#options' => array('blank' => t('No default value'), 'now' => t('Now'), 'strtotime' => t('Relative')),
    '#weight' => 1,
    '#fieldset' => 'default_values',
    );

  $description = t("Describe a time by reference to the current day, like '+90 days' (90 days from the day the field is created) or '+1 Saturday' (the next Saturday). See !strtotime for more details.", array('!strtotime' => l(t('strtotime'), 'http://www.php.net/manual/en/function.strtotime.php')));
  $form['default_value_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Relative default value'),
    '#description' => $description,
    '#default_value' => $settings['default_value_code'],
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][default_value]"]' => array('value' => 'strtotime'))
      ),
    '#weight' => 1.1,
    '#fieldset' => 'default_values',
    );
  $form['default_value2'] = array(
    '#type' => !empty($field['settings']['todate']) ? 'select' : 'hidden',
    '#title' => t('Default end date'),
    '#default_value' => $settings['default_value2'],
    '#options' => array('same' => t('Same as Default date'), 'blank' => t('No default value'), 'now' => t('Now'), 'strtotime' => t('Relative')),
    '#weight' => 2,
    '#fieldset' => 'default_values',
    );
  $form['default_value_code2'] = array(
    '#type' => !empty($field['settings']['todate']) ? 'textfield' : 'hidden',
    '#title' => t('Relative default value for end date'),
    '#description' => $description,
    '#default_value' => $settings['default_value_code2'],
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][default_value2]"]' => array('value' => 'strtotime'))
      ),
    '#weight' => 2.1,
    '#fieldset' => 'default_values',
    );

  $form['#element_validate'] = array('date_field_instance_settings_form_validate');
  return $form;
}

function date_field_instance_settings_form_validate(&$form, &$form_state) {

  $settings = $form_state['values']['instance']['settings'];

  if ($settings['default_value'] == 'strtotime') {
    $is_strtotime = @strtotime($settings['default_value_code']);
    if (!$is_strtotime) {
      form_set_error('instance][settings][default_value_code', t('The Strtotime default value is invalid.'));
    }
  }
  if (isset($settings['default_value2']) && $settings['default_value2'] == 'strtotime') {
    $is_strtotime = @strtotime($settings['default_value_code2']);
    if (!$is_strtotime) {
      form_set_error('instance][settings][default_value_code2', t('The Strtotime default value for the End Date is invalid.'));
    }
  }
}

function _date_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form = array(
    '#element_validate' => array('date_field_widget_settings_form_validate'),
    );

  $options = array();
  if ($widget['type'] == 'date_popup' && module_exists('date_popup')) {
    $formats = date_popup_formats();
  }
  else {
    // example input formats must show all possible date parts, so add seconds.
    $formats = str_replace('i', 'i:s', array_keys(system_get_date_formats('short')));
    $formats = drupal_map_assoc($formats);
  }
  $now = date_example_date();
  foreach ($formats as $f) {
    $options[$f] = date_format_date($now, 'custom', $f);
  }
  $form['input_format'] = array(
    '#type' => 'select',
    '#title' => t('Date entry options'),
    '#default_value' => $settings['input_format'],
    '#options' => $options,
    '#description' => t('Control the order and format of the options users see.'),
    '#weight' => 3,
    '#fieldset' => 'date_format',
  );
  // Only a limited set of formats is available for the Date Popup module
  if ($widget['type'] != 'date_popup') {
    $form['input_format']['#options']['custom'] = t('Custom format');
    $form['input_format_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom input format'),
      '#default_value' => $settings['input_format_custom'],
      '#description' => t("Override the input format selected above. Define a php date format string like 'm-d-Y H:i' (see <a href=\"@link\">http://php.net/date</a> for more details).", array('@link' => 'http://php.net/date')),
      '#weight' => 5,
      '#fieldset' => 'date_format',
      '#attributes' => array('class' => array('indent')),
      '#states' => array(
        'visible' => array(
          ':input[name="instance[widget][settings][input_format]"]' => array('value' => 'custom')
        ),
      ),
    );
  }
  else {
    $form['input_format_custom'] = array(
      '#type' => 'hidden',
      '#value' => '',
      );
  }

  if (in_array($widget['type'], array('date_select', 'date_popup'))) {
    $form['year_range'] = array(
      '#type' => 'date_year_range',
      '#default_value' => $settings['year_range'],
      '#fieldset' => 'date_format',
      '#weight' => 6,
    );
    $form['increment'] = array(
      '#type' => 'select', '#title' => t('Time increments'),
      '#default_value' => $settings['increment'],
      '#options' => array(
        1 => t('1 minute'),
        5 => t('5 minute'),
        10 => t('10 minute'),
        15 => t('15 minute'),
        30 => t('30 minute')),
      '#weight' => 7,
      '#fieldset' => 'date_format',
    );
  }
  else {
    $form['year_range'] = array(
      '#type' => 'hidden',
      '#value' => $settings['year_range'],
    );
    $form['increment'] = array(
      '#type' => 'hidden',
      '#value' => $settings['increment'],
    );
  }

  $form['label_position'] = array(
    '#type' => 'value',
    '#value' => $settings['label_position'],
    );
  $form['text_parts'] = array(
    '#type' => 'value',
    '#value' => $settings['text_parts'],
    );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#fieldset' => 'date_format',
    '#weight' => 9,
    );
  if (in_array($widget['type'], array('date_select'))) {
    $options = array('above' => t('Above'), 'within' => t('Within'), 'none' => t('None'));
    $description = t("The location of date part labels, like 'Year', 'Month', or 'Day' . 'Above' displays the label as titles above each date part. 'Within' inserts the label as the first option in the select list and in blank textfields. 'None' doesn't label any of the date parts. Theme functions like 'date_part_label_year' and 'date_part_label_month' control label text.");
  }
  else {
    $options = array('above' => t('Above'), 'none' => t('None'));
    $description = t("The location of date part labels, like 'Year', 'Month', or 'Day' . 'Above' displays the label as titles above each date part. 'None' doesn't label any of the date parts. Theme functions like 'date_part_label_year' and 'date_part_label_month' control label text.");
  }
  $form['advanced']['label_position'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $settings['label_position'],
    '#title' => t('Position of date part labels'),
    '#description' => $description,
    );
  $form['advanced']['text_parts'] = array(
    '#theme' => $widget['type'] == 'date_select' ? 'date_text_parts' : '',
    );
  $text_parts = (array) $settings['text_parts'];
  foreach (date_granularity_names() as $key => $value) {
    if ($widget['type'] == 'date_select') {
      $form['advanced']['text_parts'][$key] = array(
        '#type' => 'radios',
        '#default_value' => in_array($key, $text_parts) ? 1 : 0,
        '#options' => array(0 => '', 1 => ''),
        );
    }
    else {
      $form['advanced']['text_parts'][$key] = array(
        '#type' => 'value',
        '#value' => in_array($key, (array) $settings['text_parts']) ? 1 : 0,
        );
    }
  }
  if (module_exists('date_repeat') && date_is_repeat_field($field, $instance)) {
    $form['repeat_collapsed'] = array(
      '#type' => 'value',
      '#default_value' => 1,
      '#options' => array(
        0 => t('Expanded', array(), array('context' => 'Date repeat')),
        1 => t('Collapsed', array(), array('context' => 'Date repeat'))
      ),
      '#title' => t('Repeat display', array(), array('context' => 'Date repeat')),
      '#description' => t("Should the repeat options form start out expanded or collapsed? Set to 'Collapsed' to make those options less obtrusive.", array(), array('context' => 'Date repeat')),
      '#fieldset' => 'date_format',
      );
  }

  $context = array(
    'field' => $field,
    'instance' => $instance,
  );
  drupal_alter('date_field_settings_form', $form, $context);

  return $form;
}

function date_field_widget_settings_form_validate(&$form, &$form_state) {
  // The widget settings are in the wrong place in the form because of #tree on the top level.
  $settings = $form_state['values']['instance']['widget']['settings'];
  $settings = array_merge($settings, $settings['advanced']);
  unset($settings['advanced']);
  form_set_value(array('#parents' => array('instance', 'widget', 'settings')), $settings, $form_state);

  $widget = &$form_state['values']['instance']['widget'];
  // Munge the table display for text parts back into an array of text parts.
  if (is_array($widget['settings']['text_parts'])) {
    form_set_value($form['text_parts'], array_keys(array_filter($widget['settings']['text_parts'])), $form_state);
  }

  if ($widget['settings']['input_format'] === 'custom' && empty($widget['settings']['input_format_custom'])) {
    form_set_error('instance][widget][settings][input_format_custom', t('Please enter a custom date format, or choose one of the preset formats.'));
  }
}

function _date_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form = array(
    '#element_validate' => array('date_field_settings_validate'),
  );

  if (module_exists('date_repeat')) {
    $form['repeat'] = array(
      '#type' => 'select',
      '#title' => t('Repeating date'),
      '#default_value' => $field['settings']['repeat'],
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#attributes' => array('class' => array('container-inline')),
      '#description' => t("Repeating dates use an 'Unlimited' number of values. Instead of the 'Add more' button, they include a form to select when and how often the date should repeat."),
      '#disabled' => $has_data,
    );
  }
  else {
    $form['repeat'] = array(
      '#type' => 'hidden',
      '#value' => 0,
    );
  }

  // Make sure granularity is in the right format and has no empty values.
  if (!empty($settings['granularity']) && is_array($settings['granularity'])) {
    $granularity = array_filter($settings['granularity']);
  }
  $tz_handling = $settings['tz_handling'];

  $description = t('Select the date attributes to collect and store.');
  $options = date_granularity_names();
  $checkbox_year = array(
    '#type' => 'checkbox',
    '#title' => $options['year'],
    '#value' => 'year',
    '#return_value' => 'year',
    '#disabled' => TRUE,
  );
  unset($options['year']);
  $form['granularity'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Date attributes to collect'),
    '#default_value' => $granularity,
    '#options' => $options,
    '#attributes' => array('class' => array('container-inline')),
    '#description' => $description,
    '#disabled' => $has_data,
    'year' => $checkbox_year,
  );

  $description = t('End dates are used to collect duration. E.g., allow an event to start on September 15, and end on September 16.');
  $form['enddate_get'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collect an end date'),
    '#description' => $description,
    '#default_value' => (empty($settings['todate']) ? FALSE : TRUE),
    '#disabled' => $has_data,
  );
  $form['enddate_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#default_value' => ((isset($settings['todate']) && $settings['todate'] === 'required') ? TRUE : FALSE),
    '#disabled' => $has_data,
    '#states' => array(
      'invisible' => array(
        'input[name="field[settings][enddate_get]"]' => array('checked' => FALSE)
      ),
    ),
  );
  $description = t('Select the timezone handling method for this date field.');
  $form['tz_handling'] = array(
    '#type' => 'select',
    '#title' => t('Time zone handling'),
    '#default_value' => $tz_handling,
    '#options' => date_timezone_handling_options(),
    '#description' => $description,
    '#disabled' => $has_data,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'date') . '/date_admin.js'),
    ),
  );
  // Force this value to hidden because we don't want to allow it to be changed right now,
  // but allow it to be a variable if needed.
  $form['timezone_db'] = array(
    '#type' => 'hidden',
    '#value' => date_get_timezone_db($tz_handling),
    );

  return $form;
}

/**
 * Custom field settings manipulation.
 */
function date_field_settings_validate(&$form, &$form_state) {
  $field = &$form_state['values']['field'];

  if ($field['settings']['tz_handling'] == 'none') {
    form_set_value($form['timezone_db'], '', $form_state);
  }
  else {
    form_set_value($form['timezone_db'], date_get_timezone_db($field['settings']['tz_handling']), $form_state);
  }
  if ($field['settings']['tz_handling'] != 'none' && !in_array('hour', array_filter($field['settings']['granularity']))) {
    form_set_error('field][settings][tz_handling', t('Dates without hours granularity must not use any timezone handling.'));
  }
  // Extract the correct 'todate' value out of the two end date checkboxes.
  if ($field['settings']['enddate_get']) {
    if ($field['settings']['enddate_required']) {
      $field['settings']['todate'] = 'required';
    }
    else {
      $field['settings']['todate'] = 'optional';
    }
  }
  else {
    $field['settings']['todate'] = '';
  }
  // Don't save the pseudo values created in the UI.
  unset($field['settings']['enddate_get'], $field['settings']['enddate_required']);
}

/**
 *  Timezone handling options
 *
 *  the 'none' option will do no timezone conversions and will store and display dates exactly as entered
 *  useful in locales or situations where timezone conversions are not working reliably,
 *  for dates with no times, for historical dates where timezones are irrelevant,
 *  or anytime conversion is unnecessary or undesirable
 */
function date_timezone_handling_options() {
  return array(
    'site' => t("Site's time zone"),
    'date' => t("Date's time zone"),
    'user' => t("User's time zone"),
    'utc' => 'UTC',
    'none' => t('No time zone conversion'),
    );
}

