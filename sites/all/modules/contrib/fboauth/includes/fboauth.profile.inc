<?php

/**
 * @file
 * Functions to assist with handling with Profile module data.
 */

/**
 * Return a list of all profile fields supported by Profile module.
 *
 * Profile module doesn't provide a "pretty" way to do this unfortunately,
 * so altogether this is a pretty ugly looking function.
 */
function fboauth_profile_fields() {
  static $profile_fields;

  if (!isset($profile_fields)) {
    $profile_fields = array();
    $profile_categories = db_select('profile_field')
      ->distinct()
      ->fields('profile_field', array('category'))
      ->orderBy('category')
      ->execute()
      ->fetchCol();
    foreach ($profile_categories as $profile_category) {
      $result = _profile_get_fields($profile_category);
      foreach ($result as $field) {
        $profile_fields[$field->name] = (array) $field;
      }
    }
  }

  return $profile_fields;
}

/**
 * Add options for Profile module to the Facebook OAuth settings form.
 */
function fboauth_profile_form_alter(&$form, &$form_state) {
  $form['fboauth_user_profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile field mapping'),
    '#description' => t('Each of your Profile fields are listed below. Map each one you would like to import into your site to a Facebook data source. <strong>Note that only profile fields configured to show on the user registration form may be imported!</strong>'),
    '#tree' => TRUE,
    '#weight' => 5,
  );

  $properties = fboauth_user_properties(TRUE);
  $property_options = array('' => t('- Do not import -'));
  foreach ($properties as $property => $property_info) {
    $property_options[$property] = '[' . $property . '] ' . $property_info['label'];
  }

  $field_defaults = variable_get('fboauth_user_profile', array());
  $field_select = array(
    '#type' => 'select',
    '#options' => $property_options,
  );

  $profile_fields = fboauth_profile_fields();
  foreach ($profile_fields as $profile_name => $profile_field) {
    $form['fboauth_user_profile'][$profile_name] = $field_select;
    $form['fboauth_user_profile'][$profile_name]['#title'] = t($profile_field['title']);
    $form['fboauth_user_profile'][$profile_name]['#default_value'] = isset($field_defaults[$profile_name]) ? $field_defaults[$profile_name] : '';
  }
}

/**
 * Submit handler for the Facebook OAuth settings form.
 */
function fboauth_profile_form_submit(&$form, &$form_state) {
  variable_set('fboauth_user_profile', array_filter($form_state['values']['fboauth_user_profile']));
}

/**
 * Add profile info to a Drupal user array (before account creation).
 */
function fboauth_profile_create_user(&$edit, $fbuser) {
  $profile_map = variable_get('fboauth_user_profile', array());
  $profile_fields = fboauth_profile_fields();

  foreach ($profile_map as $profile_field_name => $facebook_property_name) {
    if (isset($profile_fields[$profile_field_name])) {
      $profile_field = $profile_fields[$profile_field_name];
      switch ($profile_field['type']) {
        case 'date':
          $date = array();
          $fbdate = explode('/', $fbuser->$facebook_property_name);
          if (count($fbdate) == 3) {
            foreach (array('month', 'day', 'year') as $count => $part) {
              $date[$part] = (int) ltrim($fbdate[$count], '0');
            }
            $edit[$profile_field_name] = $date;
          }
          break;
        case 'selection':
          // We can't import anything other than strings into selects.
          if (!is_string($fbuser->$facebook_property_name)) {
            break;
          }

          // Mapping options is tricky business. We loop through all available
          // options and choose the closest one to match the incoming value.
          $options = explode("\n", $profile_field['options']);
          $best_match = 0.0;
          $best_option = NULL;
          $fb_option = is_string($fbuser->$facebook_property_name) ? $fbuser->$facebook_property_name : '';
          $match_fb = strtolower($fbuser->$facebook_property_name);
          foreach ($options as $option) {
            $option = trim($option);
            $match_option = strtolower($option);
            $this_match = 0;
            similar_text($match_option, $match_fb, $this_match);
            if ($this_match > $best_match) {
              $best_match = $this_match;
              $best_option = $option;
            }
          }
          if (isset($best_option)) {
            $edit[$profile_field_name] = $best_option;
          }
          break;
        case 'textfield':
        case 'textarea':
        case 'list':
        case 'url':
        default:
          // We can't import anything other than strings into textfields.
          if (!is_string($fbuser->$facebook_property_name)) {
            break;
          }

          $edit[$profile_field_name] = $fbuser->$facebook_property_name;
          break;
      }
    }
  }
}
