<?php

/**
 * @file
 * Webform module markup component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_markup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'format' => NULL,
      'private' => FALSE,
      'display_on' => 'form',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_markup() {
  return array(
    'webform_display_markup' => array(
      'render element' => 'element',
      'file' => 'components/markup.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_markup($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'text_format',
    '#title' => t('Value'),
    '#default_value' => $component['value'],
    '#description' => t('Markup allows you to enter custom HTML or PHP logic into your form.') . theme('webform_token_help'),
    '#weight' => -1,
    '#format' => $component['extra']['format'],
    '#element_validate' => array('_webform_edit_markup_validate'),
  );

  $form['display']['display_on'] = array(
    '#type' => 'select',
    '#title' => t('Display on'),
    '#default_value' => $component['extra']['display_on'],
    '#options' => array(
      'form' => t('form only'),
      'display' => t('viewed submission only'),
      'both' => t('both form and viewed submission'),
    ),
    '#weight' => 1,
    '#parents' => array('extra', 'display_on'),
  );

  return $form;
}

/**
 * Element validate handler; Set the text format value.
 */
function _webform_edit_markup_validate($form, &$form_state) {
  if (is_array($form_state['values']['value'])) {
    $form_state['values']['extra']['format'] = $form_state['values']['value']['format'];
    $form_state['values']['value'] = $form_state['values']['value']['value'];
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_markup($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? NULL : $component['name'],
    '#weight' => $component['weight'],
    '#markup' => $filter ? check_markup(webform_replace_tokens($component['value'], $node, NULL, NULL, TRUE), $component['extra']['format'], '', TRUE) : $component['value'],
    '#format' => $component['extra']['format'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'markup'),
    '#access' => $component['extra']['display_on'] != 'display',
  );

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_markup($component, $value, $format = 'html') {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  return array(
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_markup',
    '#format' => $format,
    '#value' => check_markup(webform_replace_tokens($component['value'], $node, NULL, NULL, TRUE), $component['extra']['format'], '', TRUE),
    '#translatable' => array('title'),
    '#access' => $component['extra']['display_on'] != 'form',
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_markup($variables) {
  $element = $variables['element'];
  return $element['#access'] ? $element['#value'] : '';
}
