<?php

$plugin = array(
  'form' => 'feeds_tamper_unique_form',
  'callback' => 'feeds_tamper_unique_callback',
  'description' => 'feeds_tamper_unique_description',
  'machine_name' => 'feeds_tamper_unique_machine_name',
  'name' => 'Unique',
  'multi' => 'direct',
  'single' => 'skip',
  'category' => 'List',
);

function feeds_tamper_unique_form($importer, $element_key, $settings) {
  $form = array();
  $form['unique']['#markup'] = t('Makes the elements in a multivalued field unique.');
  return $form;
}

function feeds_tamper_unique_description($settings) {
  return 'Make the values unique.';
}

function feeds_tamper_unique_machine_name($settings) {
  return 'make_unique';
}

function feeds_tamper_unique_callback($result, $item_key, $element_key, &$field, $settings) {
  $unique = array();
  foreach ($field as $f) {
    if (!in_array($f, $unique)) {
      $unique[] = $f;
    }
  }
  $field = $unique;
}
