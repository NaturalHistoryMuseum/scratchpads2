<?php

/**
 * @file
 * Performs basic mathematical calculations on the imported value
 */

$plugin = array(
  'form' => 'feeds_tamper_math_form',
  'validate' => 'feeds_tamper_math_validate',
  'callback' => 'feeds_tamper_math_callback',
  'name' => 'Mathematical operation',
  'category' => 'Number',
  'multi' => 'loop',
);

function feeds_tamper_math_form($importer, $element_key, $settings) {
  $form = array();
  $form['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#description' => t('The operation to apply to the imported value.'),
    '#required' => TRUE,
    '#options' => array(
      'addition'        =>  '+',
      'subtraction'     =>  '-',
      'multiplication'  =>  '*',
      'division'        =>  '/',
    ),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : '',
  );
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#required' => TRUE,
    '#description' => t('A numerical value.'),
    '#default_value' => isset($settings['value']) ? $settings['value'] : '',
  );
  return $form;
}

function feeds_tamper_math_validate(&$settings) {
  if (!is_numeric($settings['value'])) {
    form_set_error('settings][value', t('The value must be numeric.'));
  }
  elseif ($settings['operation'] == 'division' && $settings['value'] == 0) {
    form_set_error('settings][value', t('Cannot divide by zero.'));
  }
}

function feeds_tamper_math_callback($result, $item_key, $element_key, &$field, $settings, $source) {
  switch ($settings['operation']) {
    case 'addition':
      $field = $field + $settings['value'];
      break;
    case 'subtraction':
      $field = $field - $settings['value'];
      break;
    case 'multiplication':
      $field = $field * $settings['value'];
      break;
    case 'division':
      $field = $field / $settings['value'];
      break;
  }
}
