<?php

/**
 * @file
 * Encode and Decode a text with a set of defined functions.
 */

$plugin = array(
  'form' => 'feeds_tamper_encode_form',
  'callback' => 'feeds_tamper_encode_callback',
  'name' => 'Encode/Decode Plugin',
  'multi' => 'direct',
  'category' => 'Text',
);

/**
 * Form callback for the 'encode' plugin.
 */
function feeds_tamper_encode_form($importer, $element_key, $settings) {
  $form = array();

  $form['help']['#value'] = t('Encode (or Decode) the field contents.');

  $form['mode'] = array(
    '#title' => t('Serialization mode:'),
    '#description' => t('Serialization/Encoding mode'),
    '#type' => 'radios',
    '#options' => array(
      'serialize' => t('PHP Serialize'),
      'unserialize' => t('PHP Unserialize'),
      'json_encode' => t('Json Encode'),
      'json_decode' => t('Json Decode'),
      'base64_encode' => t('Base64 Encode'),
      'base64_decode' => t('Base64 Decode'),
    ),
    '#default_value' => isset($settings['mode']) ? $settings['mode'] : 'serialize',
  );

  return $form;
}

/**
 * Callback for the 'encode' plugin.
 */
function feeds_tamper_encode_callback($source, $item_key, $element_key, &$field, array $settings) {
  if (empty($settings['mode'])) {
    $settings['mode'] = 'serialize';
  }

  $function = $settings['mode'];

  if (function_exists($function)) {
    $field = call_user_func($function, $field);
  }
}
