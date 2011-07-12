<?php

$plugin = array(
  'form' => 'feeds_tamper_html_entity_decode_form',
  'callback' => 'feeds_tamper_html_entity_decode_callback',
  'description' => 'feeds_tamper_html_entity_decode_description',
  'machine_name' => 'feeds_tamper_html_entity_decode_machine_name',
  'name' => 'HTML entity decode',
  'multi' => 'loop',
  'category' => 'HTML',
);

function feeds_tamper_html_entity_decode_form($importer, $element_key, $settings) {
  $form = array();
  $form['html'] = array(
    '#markup' => t('Convert all HTML entities such as &amp;amp; and &amp;quot; to &amp; and &quot;.')
  );
  return $form;
}

function feeds_tamper_html_entity_decode_description($settings) {
  return 'Decode HTML entities';
}

function feeds_tamper_html_entity_decode_machine_name($settings) {
  return 'decode_html_entities';
}

function feeds_tamper_html_entity_decode_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = html_entity_decode($field, ENT_QUOTES, 'UTF-8');
}
