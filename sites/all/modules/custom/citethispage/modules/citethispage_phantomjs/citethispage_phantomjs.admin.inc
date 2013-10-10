<?php

/**
 * citethispage_phantomjs_settings_form
 */
function citethispage_phantomjs_settings_form($form, &$form_state){
  $form['citethispage_phantomjs_preview'] = array(
    '#type' => 'select',
    '#title' => t('Preview button'),
    '#required' => TRUE,
    '#description' => t('Select which users should get a preview option'),
    '#options' => array(
      0 => t('None'),
      1 => t('Logged-in users only'),
      2 => t('All users')
    ),
    '#default_value' => variable_get('citethispage_phantomjs_preview', 2)
  );
  $inject_default = _citethispage_phantomjs_var_get('citethispage_phantomjs_inject');
  $form['citethispage_phantomjs_inject'] = array(
    '#type' => 'text_format',
    '#required' => FALSE,
    '#title' => t('snapshot message'),
    '#description' => "The text that users can add to snapshot pages",
    '#default_value' => $inject_default['value'],
    '#format' => $inject_default['format']
  );
  $form['citethispage_phantomjs_tool'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Command line'),
    '#description' => t('The command line to execute. Note that this is expected to use the bundled snapshot.js, tested with PhantomJS 1.5. Only change this if you know what you are doing.'),
    '#default_value' => _citethispage_phantomjs_var_get('citethispage_phantomjs_tool'),
    '#maxlength' => 2048
  );
  $form['citethispage_phantomjs_destination'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Destination'),
    '#description' => t('The destination path for the generated file. Use ".pdf" extention to generate a PDF, and ".png" to generate a PNG. Leave this empty if you want to manage the destination via your phantomjs script instead.'),
    '#default_value' => _citethispage_phantomjs_var_get('citethispage_phantomjs_destination')
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
        'citethispage_phantomjs'
      )
    ))
  );
  return system_settings_form($form);
}
