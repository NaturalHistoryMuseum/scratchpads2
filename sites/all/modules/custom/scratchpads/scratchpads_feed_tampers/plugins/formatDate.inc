<?php
/**
 * feeds_tamper 
 */
$plugin = array(
  'form' => 'feeds_tamper_dateFormat_form',
  'callback' => 'feeds_tamper_dateFormat_callback',
  'validate' => 'feeds_tamper_dateFormat_validate',
  'category' => 'Date',
  'name' => 'Date format',
  'multi' => 'loop',
  'category' => 'Text'
);

function feeds_tamper_dateFormat_form($importer, $element_key, $settings){
  $form = array();
  $form['incoming_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Incoming Date Format'),
    '#default_value' => isset($settings['incoming_date_format']) ? $settings['incoming_date_format'] : '',
    '#description' => t('Enter the php date format of the incoming string. For example "d/m/Y". See the <a href="@url">PHP manual</a> for available options.', array(
      '@url' => 'http://www.php.net/manual/function.date.php'
    ))
  );
  $form['new_date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('New Date Format'),
      '#default_value' => isset($settings['new_date_format']) ? $settings['new_date_format'] : '',
      '#description' => t('Enter the format you want the incoming date converted to. For example "Y/m/d". See the <a href="@url">PHP manual</a> for available options.', array(
          '@url' => 'http://www.php.net/manual/function.date.php'
      ))
  );
  return $form;
}

function feeds_tamper_dateFormat_callback($result, $item_key, $element_key, &$field, $settings){
  if(!empty($field)){
    
    try{
      $field = date_create_from_format($settings['incoming_date_format'], $field)->format($settings['new_date_format']);
    }
    catch(Exception $e) {
      echo $e->getMessage();
      //print_r(date_get_last_errors());
    }
  //  if (!$date) {
   //   $e = date_get_last_errors();
    //  foreach ($e['errors'] as $error) {
     //   echo "$error\n";
     // }
     // exit(1);
   // } 
    
  }
}

function feeds_tamper_dateFormat_validate(&$settings){
  if(empty($settings['incoming_date_format'])){
    form_set_error('settings][incoming_date_format', t('Please enter the incoming date format string.'));
  }
  if(empty($settings['new_date_format'])){
    form_set_error('settings][new_date_format', t('Please enter a new date format string.'));
  }
}