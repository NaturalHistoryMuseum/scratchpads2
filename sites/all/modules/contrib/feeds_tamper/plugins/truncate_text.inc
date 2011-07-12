<?php

$plugin = array(
  'form' => 'feeds_tamper_truncate_text_form',
  'callback' => 'feeds_tamper_truncate_text_callback',
  'description' => 'feeds_tamper_truncate_text_description',
  'machine_name' => 'feeds_tamper_truncate_text_machine_name',
  'validate' => 'feeds_tamper_truncate_text_validate',
  'name' => 'Truncate',
  'multi' => 'loop',
  'category' => 'Text',
);

function feeds_tamper_truncate_text_form($importer, $element_key, $settings) {
  $form = array();
  $form['num_char'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of characters'),
    '#default_value' => isset($settings['num_char']) ? $settings['num_char'] : '',
    '#description' => t('The characters of characters a string can be.'),
  );
  $form['ellipses'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ellipses'),
    '#default_value' => isset($settings['ellipses']) ? $settings['ellipses'] : FALSE,
    '#description' => t('Add ellipses (...) to the end of the truncated text.'),
  );
  return $form;
}

function feeds_tamper_truncate_text_machine_name($settings) {
  $text = 'truncate_text_to_' . $settings['num_char'] . '_characters';
  $text .= $settings['ellipses'] ? '_with_ellipses' : '';
  return $text;
}

function feeds_tamper_truncate_text_description($settings) {
  $desc = 'Truncate to ' . $settings['num_char'] . ' characters';
  $desc .= $settings['ellipses'] ? ' with ellipses.' : '.';
  return $desc;
}

function feeds_tamper_truncate_text_validate($settings) {
  $settings['num_char'] = trim($settings['num_char']);
  if (!ctype_digit($settings['num_char'])) {
    form_set_error('truncate_text][num_char', t('Needs to be an integer.'));
  }
}

function feeds_tamper_truncate_text_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = substr($field, 0, $settings['num_char']);
  if ($settings['ellipses']) {
    $field .= '...';
  }
}
