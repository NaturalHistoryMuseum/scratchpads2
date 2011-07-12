<?php

$plugin = array(
  'form' => 'feeds_tamper_convert_case_form',
  'callback' => 'feeds_tamper_convert_case_callback',
  'name' => 'Convert case',
  'multi' => 'loop',
  'category' => 'Text',
  'description' => 'feeds_tamper_convert_case_description',
  'machine_name' => 'feeds_tamper_convert_case_machine_name',
);

function feeds_tamper_convert_case_form($importer, $element_key, $settings) {
  $form = array();
  $form['mode'] = array(
    '#type' => 'radios',
    '#title' => t('How to convert case'),
    '#default_value' => isset($settings['mode']) ? $settings['mode'] : MB_CASE_TITLE,
    '#options' => array(MB_CASE_TITLE => t('Title case'), MB_CASE_LOWER => t('Lower Case'), MB_CASE_UPPER => t('Upper case'))
  );
  return $form;
}

function feeds_tamper_convert_case_machine_name($settings) {
  $text = 'convert_to_';
  switch($settings['mode']) {
    case MB_CASE_TITLE:
      return $text .= 'titlecase';
    case MB_CASE_LOWER:
      return $text .= 'lowercase';
    case MB_CASE_UPPER:
      return $text .= 'uppercase';
  }
}

function feeds_tamper_convert_case_description($settings) {
  $text = 'Convert text to ';
  switch($settings['mode']) {
    case MB_CASE_TITLE:
      return $text .= 'title case.';
    case MB_CASE_LOWER:
      return $text .= 'lowercase.';
    case MB_CASE_UPPER:
      return $text .= 'uppercase.';
  }
}

function feeds_tamper_convert_case_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = mb_convert_case($field, $settings['mode'], 'UTF-8');
}
