<?php

/**
 * @file
 * Facebook to Field module mapping.
 */

/**
 * Add options for Field module to the Facebook OAuth settings form.
 */
function fboauth_field_form_alter(&$form, &$form_state) {
  $form['fboauth_user_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('User field mapping'),
    '#description' => t('Each of your <a href="!url">fields that are attached to users</a> are listed below. Map each one you would like to import into your site to a Facebook data source.', array('!url' => url('admin/config/people/accounts/fields'))),
    '#tree' => TRUE,
    '#weight' => 5,
  );

  // Each field type can only map to certain type Facebook properties. Build a
  // list for each type that includes reasonable options.
  $properties = fboauth_user_properties(TRUE);
  $property_options = array();
  foreach ($properties as $property => $property_info) {
    if (isset($property_info['field_types'])) {
      foreach ($property_info['field_types'] as $field_type) {
        $property_options[$field_type][$property] = '[' . $property . '] ' . $property_info['label'];
      }
    }
  }

  $field_defaults = variable_get('fboauth_user_fields', array());
  $instances = field_info_instances('user', 'user');
  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($instance['field_name']);
    if (isset($property_options[$field['type']])) {
      $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
      $form['fboauth_user_fields'][$field_name] = array(
        '#title' => t($instance['label']),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($field_defaults[$field_name]) ? $field_defaults[$field_name] : '',
      );
    }
    else {
      $form['fboauth_user_fields'][$field_name] = array(
        '#title' => t($instance['label']),
        '#type' => 'form_element',
        '#children' => '<em>' . t('No mappable Facebook properties.') . '</em>',
        '#theme_wrappers' => array('form_element'),
      );
    }
  }
}

/**
 * Submit handler for the Facebook OAuth settings form.
 */
function fboauth_field_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['fboauth_user_fields'])) {
    variable_set('fboauth_user_fields', array_filter($form_state['values']['fboauth_user_fields']));
  }
}

/**
 * Add field info to a Drupal user array (before account creation).
 */
function fboauth_field_create_user(&$edit, $fbuser) {
  $field_map = variable_get('fboauth_user_fields', array());
  $field_convert_info = fboauth_field_convert_info();
  $instances = field_info_instances('user', 'user');
  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($instance['field_name']);
    if (isset($field_map[$field_name]) && isset($field_convert_info[$field['type']]['callback'])) {
      $callback = $field_convert_info[$field['type']]['callback'];
      $facebook_property_name = $field_map[$field_name];
      if ($value = $callback($facebook_property_name, $fbuser, $field, $instance)) {
        $edit[$field_name][LANGUAGE_NONE][0] = $value;
      }
    }
  }
}

/**
 * Provide a callback map for converting Facebook data to fields.
 */
function fboauth_field_convert_info() {
  $convert_info = array(
    'text' => array(
      'label' => t('Text'),
      'callback' => 'fboauth_field_convert_text',
    ),
    'text_long' => array(
      'label' => t('Long text'),
      'callback' => 'fboauth_field_convert_text',
    ),
    'list_text' => array(
      'label' => t('List (\'text\')'),
      'callback' => 'fboauth_field_convert_list',
    ),
    'datetime' => array(
      'label' => t('Date'),
      'callback' => 'fboauth_field_convert_date',
    ),
    'date' => array(
      'label' => t('Date'),
      'callback' => 'fboauth_field_convert_date',
    ),
    'datestamp' => array(
      'label' => t('Date'),
      'callback' => 'fboauth_field_convert_date',
    ),
  );
  drupal_alter('fboauth_field_convert_info', $convert_info);
  return $convert_info;
}

/**
 * Facebook data conversion function.
 */
function fboauth_field_convert_text($facebook_property_name, $fbuser, $field, $instance) {
  $value = NULL;
  if (isset($fbuser->$facebook_property_name)) {
    // Most Facebook properties are simple strings.
    if (is_string($fbuser->$facebook_property_name)) {
      $value = $fbuser->$facebook_property_name;
    }
    // If the property is an object, try common keys that are strings.
    elseif (is_object($fbuser->$facebook_property_name)) {
      $object = $fbuser->$facebook_property_name;
      // Locations, Signficant Other, Hometown, and other properties use "name"
      // as the human-readable value in the object.
      if (isset($object->name)) {
        $value = $object->name;
      }
    }
  }

  return $value ? array('value' => $value) : NULL;
}

/**
 * Facebook data conversion function.
 *
 * Converts an incoming Facebook location (which is an object) into a string.
 */
function fboauth_field_convert_location($facebook_property_name, $fbuser, $field, $instance) {
}

/**
 * Facebook data conversion function.
 *
 * Select the best value from a list field based on a Facebook property value.
 */
function fboauth_field_convert_list($facebook_property_name, $fbuser, $field, $instance) {
  // We can't import anything other than strings into selects.
  if (!is_string($fbuser->$facebook_property_name)) {
    return;
  }

  // Mapping options is tricky business. We loop through all available
  // options and choose the closest one to match the incoming value.
  $options = list_allowed_values($field);
  $best_match = 0.0;
  $best_option = NULL;
  $fb_option = is_string($fbuser->$facebook_property_name) ? $fbuser->$facebook_property_name : '';
  $match_fb = strtolower($fbuser->$facebook_property_name);
  foreach ($options as $key => $option) {
    $option = trim($option);
    $match_option = strtolower($option);
    $this_match = 0;
    similar_text($match_option, $match_fb, $this_match);
    if ($this_match > $best_match) {
      $best_match = $this_match;
      $best_option = $option;
      $best_key = $key;
    }
  }

  return isset($best_key) ? array('value' => $best_key) : NULL;
}

/**
 * Facebook data conversion function.
 *
 * Convert a Facebook date into Date module formats. This function takes all
 * three date formats (date, datetime, and datestamp) and returns the
 * appropriate data. Both date and datetime use ISO format of
 * YYYY-MM-DDTHH:MM:SS. Datestamp uses a UNIX timestamp.
 */
function fboauth_field_convert_date($facebook_property_name, $fbuser, $field, $instance) {
  $value = NULL;
  $fbdate = explode('/', $fbuser->$facebook_property_name);
  if (count($fbdate) == 3) {
    $date = new DateObject($fbuser->$facebook_property_name);
    if (date_is_date($date)) {
      $format = $field['type'] == 'datestamp' ? DATE_FORMAT_UNIX : DATE_FORMAT_ISO;
      $value = array(
        'value' => $date->format($format, TRUE),
        'date_type' => $field['type'],
      );
    }
  }
  return $value;
}
