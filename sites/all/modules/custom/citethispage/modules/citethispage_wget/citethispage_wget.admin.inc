<?php

/**
 * citethispage_wget_settings_form
 */
function citethispage_wget_settings_form($form, &$form_state){
  $form['citethispage_wget_tool'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Command line'),
    '#description' => t('The command line to execute. This is expected to archive [citethispage_wget:url] into the folder [citethispage_wget:destination]. The destination file should then be obtained by concatenating the original url (including the host name) to the destination.'),
    '#default_value' => variable_get('citethispage_wget_tool', CITETHISPAGE_WGET_DEFAULT_TOOL)
  );
  $form['citethispage_wget_destination'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Destination'),
    '#description' => t('The destination folder for the archive.'),
    '#default_value' => variable_get('citethispage_wget_destination', CITETHISPAGE_WGET_DEFAULT_DESTINATION)
  );
  $form['citethispage_wget_inject'] = array(
    '#type' => 'textarea',
    '#required' => FALSE,
    '#title' => t('Inject'),
    '#description' => "The following text will be injected at the top of the page's body",
    '#default_value' => variable_get('citethispage_wget_inject', CITETHISPAGE_WGET_DEFAULT_INJECT)
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
