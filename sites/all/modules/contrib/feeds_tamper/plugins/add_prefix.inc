<?php

/**
 * @file
 * Remove text or whitespace from the beginning, middle, or both sides of text.
 */

$plugin = array(
  'form' => 'feeds_tamper_add_prefix_form',
  'callback' => 'feeds_tamper_add_prefix_callback',
  'name' => 'Add Prefix',
  'multi' => 'loop',
  'category' => 'Text',
);

function feeds_tamper_add_prefix_form($importer, $element_key, $settings) {
  $form = array();
  $form['mask'] = array(
    '#type' => 'textfield',
    '#title' => t('Text to prefix'),
    '#default_value' => isset($settings['mask']) ? $settings['mask'] : '',
    '#description' => t('Text (e.g. path) to prefix the value'),
  );
  return $form;
}

function feeds_tamper_add_prefix_callback($source, $item_key, $element_key, &$field, $settings) {
   // Prefix any non-empty string
    if (strlen($field) > 0) {
      $field = $settings['mask'] . $field;
    }
}
