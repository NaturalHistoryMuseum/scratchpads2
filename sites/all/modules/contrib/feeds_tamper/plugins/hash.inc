<?php
$plugin = array(
  'form' => 'feeds_tamper_hash_form',
  'callback' => 'feeds_tamper_hash_callback',
  'name' => 'Hash',
  'multi' => 'loop',
  'category' => 'Other',
  'description' => 'feeds_tamper_hash_description',
  'machine_name' => 'feeds_tamper_hash_machine_name'
);

function feeds_tamper_hash_form($importer, $element_key, $settings){
  return array(
    'random' => array(
      '#markup' => t('Makes the element a hash of the values being imported.  This makes it easy to import new content without setting a GUID for it.')
    )
  );
}

function feeds_tamper_hash_machine_name($settings){
  return 'make_hash';
}

function feeds_tamper_hash_description($settings){
  return 'Make the value a hash.';
}

function feeds_tamper_hash_callback($result, $item_key, $element_key, &$field, $settings){
  $field = $field ? $field : md5(serialize($result->items[$item_key]));
}