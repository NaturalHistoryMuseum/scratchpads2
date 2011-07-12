<?php

$plugin = array(
  'form' => 'feeds_tamper_html_entity_encode_form',
  'callback' => 'feeds_tamper_html_entity_encode_callback',
  'description' => 'feeds_tamper_html_entity_encode_description',
  'machine_name' => 'feeds_tamper_html_entity_encode_machine_name',
  'name' => 'HTML entity encode',
  'multi' => 'loop',
  'category' => 'HTML',
);

function feeds_tamper_html_entity_encode_form($importer, $element_key, $settings) {
  $form = array();
  $form['html'] = array(
    '#markup' => t('This will convert all HTML special characters such as &gt; and &amp; to &amp;gt; and &amp;apm;.')
  );
  return $form;
}

function feeds_tamper_html_entity_encode_description($settings) {
  return 'Encode HTML entities';
}

function feeds_tamper_html_entity_encode_machine_name($settings) {
  return 'encode_html_entities';
}

function feeds_tamper_html_entity_encode_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = check_plain($field);
}
