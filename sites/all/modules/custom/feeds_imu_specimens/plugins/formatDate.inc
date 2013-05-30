<?php
/**
 * @file
 * Convert string to timestamp.
 */
$plugin = array(
  'form' => 'feeds_tamper_dateFormat_form',
  'callback' => 'feeds_tamper_dateFormat_callback',
  'validate' => 'feeds_tamper_dateFormat_validate',
  'name' => 'Date format',
  'multi' => 'loop',
  'category' => 'Text'
);

function feeds_tamper_dateFormat_form($importer, $element_key, $settings){
  $form = array();
  $form['date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('PHP Date Format'),
    '#default_value' => isset($settings['date_format']) ? $settings['date_format'] : '',
    '#description' => t('A user-defined php date format string like "m-d-Y H:i". See the <a href="@url">PHP manual</a> for available options.', array(
      '@url' => 'http://www.php.net/manual/function.date.php'
    ))
  );
  return $form;
}

function feeds_tamper_dateFormat_callback($result, $item_key, $element_key, &$field, $settings){
  $field = date($settings['date_format'], $field);
}

function feeds_tamper_dateFormat_validate(&$settings){
  if(!empty($settings['date_format'])){
    form_set_error('settings][date_format', t('Please enter a valid date format string.'));
  }else{
    $settings['date_format'] = (string)$settings['date_format'];
  }
}