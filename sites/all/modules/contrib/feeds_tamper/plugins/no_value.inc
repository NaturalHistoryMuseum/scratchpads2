<?php
/**
 * @file
 * Deal with empty value fields.
 */
$plugin = array(
  'form' => 'feeds_tamper_no_value_form',
  'callback' => 'feeds_tamper_no_value_callback',
  'name' => 'No value',
  'multi' => 'skip',
  'category' => 'Filter'
);

function feeds_tamper_no_value_form($importer, $element_key, $settings){
  $form = array();
  $form['input'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Input conditions'),
    '#description' => t('What is considered as not having a value?'),
    '#options' => array(
      'null' => 'Case insensitive "Null"',
      'zero' => 'Character "0" (zero)',
      'length' => 'Zero length string'
    ),
    '#default_value' => isset($settings['input']) ? $settings['input'] : array(),
    '#required' => TRUE
  );
  $form['output'] = array(
    '#type' => 'radios',
    '#title' => t('Output value'),
    '#description' => t('If the input value matches a condition what should happen to the output value?'),
    '#options' => array(
      'zero' => 'Return number 0 (zero)',
      'empty' => 'Return an empty string',
      'null' => 'Return a NULL value'
    ),
    '#default_value' => isset($settings['output']) ? $settings['output'] : '',
    '#required' => TRUE
  );
  return $form;
}

function feeds_tamper_no_value_callback($result, $item_key, $element_key, &$field, $settings){
  $no_value = FALSE;
  $checks = $settings['input'];
  if(in_array('null', $checks)){
    if(preg_match('/^null$/i', $field)){
      $no_value = TRUE;
    }
  }
  if(in_array('zero', $checks)){
    if($field == '0'){
      $no_value = TRUE;
    }
  }
  if(in_array('length', $checks)){
    if(strlen($field) == 0){
      $no_value = TRUE;
    }
  }
  if($no_value){
    switch($settings['output']){
      case 'zero':
        $field = 0;
        break;
      case 'empty':
        $field = '';
        break;
      case 'null':
        $field = NULL;
    }
  }
}