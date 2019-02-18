<?php

/**
 * @defgroup spamicide Spamicide: another tool to eliminate spam.
 *
 * The spamicide module provides the ability to prevent spam being submitted to your site on various drupal forms.
 *
 * Author:  Wes Roepken aka lipcpro < wes at lipcpro dot com >
 * Date:    05/01/2011
 */


/**
 * @file
 * This module provides yet another tool to eliminate spam.
 *
 * @ingroup spamicide
 */

/**
 * Get the spamicide field name and .css file_name
 * @param $form
 * @return the spamicide field name and .css file_name to call or FALSE
 * @todo re-work this to prevent errors showing up
 */
function _spamicide_get_field(&$form) {
  $spamicide = db_query('SELECT enabled, form_field FROM {spamicide} WHERE form_id = :form', array(':form' => $form))->fetchObject();
  if (is_object($spamicide) && $spamicide->enabled) {
    return $spamicide->form_field;
  }
  else {
    return FALSE;
  }
}

/**
 * Set the spamicide field name and .css file_name
 * @param array $form_field
 * @param string $op
 */
function _spamicide_set_css_file($form_field, $op) {
  $spam_path = 'public://' . variable_get('spamicide_dir', 'spamicide');
  $css_file = $spam_path . '/' . $form_field . '.css';
  if ($op == 'delete') {
    $file_check = db_query('SELECT DISTINCT(form_field) FROM {spamicide} WHERE form_field = :form_field', array(':form_field' => $form_field))->fetchCol();
    if (!$file_check || !in_array($form_field , $file_check)) {
      file_unmanaged_delete($css_file);
    }
  }
  if ($op == 'create' && !file_exists($css_file)) {
    $css = 'div.' . _spamicide_get_css_class($form_field) . ' { display:none; }';
    file_save_data($css, $css_file, FILE_EXISTS_REPLACE);
  }
}

/**
 * Show translation if available
 * @param $lang_code
 * @return translated field description
 * @todo change $default to be able to change it in the admin interface
 * 	 with the possibility of adding per form default description
 */
function _spamicide_get_description($lang_code=NULL) {
  global $language;
  $lang_code = isset($lang_code) ? $lang_code : $language->language;
  $default = t('To prevent automated spam submissions leave this field empty.');
  if (module_exists('locale')) {
    $description = variable_get("spamicide_description_$lang_code", $default);
  }
  else {
    $description = variable_get('spamicide_description', $default);
  }
  return $description;
}

/**
 * #post_render callback on the spamicide field. This will wrap the field
 * in a div so it can be hidden with CSS.
 *
 */
function _spamicide_field_post_render($content, $element) {
    return '<div class="' . _spamicide_get_css_class($element['#name']) . '">' . $content . '</div>';
}

/**
 * Returns a CSS class name based on the field's name. Should be used
 * to wrap around the field so it can be hidden.
 *
 * Note that it should be pretty generic so it can't be sniffed out
 * by a spammer.
 *
 * @param string $name
 * 		The field's name
 * @return string
 * 		The CSS class
 */
function _spamicide_get_css_class($name) {
    return 'edit-' . str_replace('_', '-', $name) . '-wrapper';
}

function _spamicide_set_title($form_field) {
  return str_replace('_', ' ', $form_field);
}