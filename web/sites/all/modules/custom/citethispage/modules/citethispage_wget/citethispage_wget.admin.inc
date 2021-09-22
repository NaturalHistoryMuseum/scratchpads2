<?php

/**
 * citethispage_wget_settings_form
 */
function citethispage_wget_settings_form($form, &$form_state){
  $form['citethispage_wget_preview'] = array(
    '#type' => 'select',
    '#title' => t('Preview button'),
    '#required' => TRUE,
    '#description' => t('Select which users should get a preview option'),
    '#options' => array(
      0 => t('None'),
      1 => t('Logged-in users only'),
      2 => t('All users')
    ),
    '#default_value' => variable_get('citethispage_wget_preview', 2)
  );
  $inject_default = variable_get('citethispage_wget_inject', array(
    'value' => CITETHISPAGE_WGET_DEFAULT_INJECT,
    'format' => 'filtered_html'
  ));
  $form['citethispage_wget_inject'] = array(
    '#type' => 'text_format',
    '#required' => FALSE,
    '#title' => t('snapshot message'),
    '#description' => "The text that users can add to snapshot pages",
    '#default_value' => $inject_default['value'],
    '#format' => $inject_default['format']
  );
  $form['citethispage_wget_tool'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Command line'),
    '#description' => t('The command line to execute. The module relies on the command having a certain behaviour - only change this if you know what you are doing. This is expected to archive [citethispage_wget:url] into the folder [citethispage_wget:destination]. The destination file should then be obtained by concatenating the original url (including the host name) to the destination.'),
    '#default_value' => variable_get('citethispage_wget_tool', CITETHISPAGE_WGET_DEFAULT_TOOL),
    '#maxlength' => 2048
  );
  $form['citethispage_wget_destination'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Destination'),
    '#description' => t('The destination folder for the archive.'),
    '#default_value' => variable_get('citethispage_wget_destination', CITETHISPAGE_WGET_DEFAULT_DESTINATION)
  );
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['token_help']['help'] = array(
    '#type' => 'markup',
    '#markup' => theme('token_tree', array(
      'token_types' => array(
        'citethispage_wget'
      )
    ))
  );
  return system_settings_form($form);
}
