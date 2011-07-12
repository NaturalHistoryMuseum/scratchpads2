<?php
$plugin = array(
  'form' => 'feeds_tamper_random_form',
  'callback' => 'feeds_tamper_random_callback',
  'description' => 'feeds_tamper_random_description',
  'machine_name' => 'feeds_tamper_random_machine_name',
  'name' => 'Random',
  'multi' => 'direct',
  'single' => 'skip',
  'category' => 'Other'
);

function feeds_tamper_random_form($importer, $element_key, $settings){
  $form = array();
  $form['random']['#markup'] = t('Makes the element a random value.');
  return $form;
}

function feeds_tamper_random_description($settings){
  return 'Make the values random.';
}

function feeds_tamper_random_machine_name($settings){
  return 'make_random';
}

function feeds_tamper_random_callback($result, $item_key, $element_key, &$field, $settings){
  watchdog('tamper', 'Tampered');
  $field = $random;
}