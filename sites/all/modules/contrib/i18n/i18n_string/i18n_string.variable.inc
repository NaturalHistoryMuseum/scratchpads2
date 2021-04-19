<?php
/**
 * @file
 * Variable information
 */

/**
 * Implements hook_variable_info().
 */
function i18n_string_variable_info($options = array()) {
  $variables['i18n_string_translate_langcode_[language]'] = array(
    'type' => 'multiple_language',
    'title' => t('Enable translation for language'),
    'multiple values' => array('type' => 'boolean'),
    'group' => 'i18n',
  );
  $variables['i18n_string_allowed_formats'] = array(
    'title' => t('Translatable text formats'),
    'options callback' => 'i18n_string_variable_format_list',
    'type' => 'options',
    'default callback' => 'i18n_string_variable_format_default',
    'access' => 'administer filters',
    'description' => t('The translation system only translates strings with the selected text formats. All other strings will be ignored and removed from the list of translatable strings.'),
  );
  $variables['i18n_string_source_language'] = array(
    'title' => t('Source language'),
    'type' => 'language',
    'default callback' => 'i18n_string_source_language',
    'description' => t('Language that will be used as the source language for string translations. The default is the site default language.'),
  );
  $variables['i18n_string_debug'] = array(
    'type' => 'enable',
    'title' => t('Debug string translation', array(), $options),
    'default' => 0,
    'group' => 'debug',
  );
  $variables['i18n_string_textgroup_class_[textgroup]'] = array(
    'title' => t('Class to use for the text group'),
    'description' => t('Determines which the class will be use for string translation in the text group.', array(), $options),
    'repeat' => array(
      'type' => 'select',
      'default' => 'i18n_string_textgroup_default',
      'options callback' => 'i18n_string_variable_textgroup_class_list',
    ),
    'submit callback' => 'i18n_string_variable_textgroup_class_submit_callback',
    'group' => 'i18n',
  );
  return $variables;
}

/**
 * Implements hook_variable_type_info().
 */
function i18n_string_variable_type_info() {
  $type['textgroup'] = array(
    'title' => t('Text group'),
    'type' => 'select',
    'options callback' => 'i18n_string_variable_textgroup_list',
  );
  return $type;
}

/**
 * Options callback, format list
 */
function i18n_string_variable_format_list() {
  $list = array();
  // As the user has administer filters permissions we get a full list here
  foreach (filter_formats() as $fid => $format) {
    $list[$fid] = $format->name;
  }
  return $list;
}

/**
 * Allowed formats default value
 */
function i18n_string_variable_format_default() {
  return array(filter_fallback_format());
}

/**
 * Options callback, text groups list.
 */
function i18n_string_variable_textgroup_list() {
  $groups = array();
  foreach (i18n_string_group_info() as $name => $info) {
    $groups[$name] = $info['title'];
  }
  return $groups;
}

/**
 * Options callback, text group classes list.
 */
function i18n_string_variable_textgroup_class_list($variable, $options = array()) {
  return array(
    'i18n_string_textgroup_default' => t('Text group handler default.', array(), $options),
    'i18n_string_textgroup_cached' => t('Text group handler which include persistent caching.', array(), $options),
  );
}

/**
 * Submit callback. Execute Reset the persistent caches after save the text group class variables.
 */
function i18n_string_variable_textgroup_class_submit_callback($variable, $options, $form, $form_state) {
  // Reset the persistent caches.
  cache_clear_all('i18n:string:' , 'cache', TRUE);
}
