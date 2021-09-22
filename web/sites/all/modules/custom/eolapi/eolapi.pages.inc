<?php

function eolapi_config_form($form, &$form_state){
  return array(
    'delete_all_content' => array(
      '#type' => 'fieldset',
      '#title' => t('Delete'),
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      'message' => array(
        '#markup' => '<p>' . t('There are currently %number EOL API objects on this site.', array(
          '%number' => db_select('eolapi', 'e')->countQuery()->execute()->fetchField()
        )) . '</p>'
      ),
      'delete' => array(
        '#type' => 'submit',
        '#value' => t('Delete all EOL API objects')
      )
    ),
    'other_options' => array(
      '#type' => 'fieldset',
      '#title' => t('Other options'),
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      'disable' => array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('eolapi_is_disabled', 0),
        '#title' => t('Disabled')
      ),
      'save' => array(
        '#type' => 'submit',
        '#value' => t('Save')
      )
    )
  );
}

/**
 * Submit function for the above form.
 */
function eolapi_config_form_submit(&$form, &$form_state){
  switch($form_state['values']['op']){
    case $form_state['values']['delete']:
      // Disable here ALWAYS
      variable_set('eolapi_is_disabled', TRUE);
      batch_set(array(
        'operations' => array(
          array(
            'eolapi_batch_delete_entities',
            array()
          )
        ),
        'finished' => 'eolapi_batch_finished',
        'title' => t('Deleting EOL API entities'),
        'file' => drupal_get_path('module', 'eolapi') . '/eolapi.pages.inc'
      ));
      break;
    case $form_state['values']['save']:
      variable_set('eolapi_is_disabled', $form_state['values']['disable']);
      break;
  }
}

/**
 * eolapi_batch_delete_entities
 */
function eolapi_batch_delete_entities(&$context){
  if(!isset($context['sandbox']['ids'])){
    $context['sandbox']['ids'] = db_select('eolapi', 'e')->fields('e', array(
      'eid'
    ))->execute()->fetchCol();
    $context['sandbox']['max'] = count($context['sandbox']['ids']);
    if(!$context['sandbox']['max']){
      $context['finished'] = 1;
      return;
    }
  }
  $ids = array();
  $i = 0;
  // Only do a small number here, because the files are deleted too.
  while($i < 20 && ($id = array_pop($context['sandbox']['ids'])) != FALSE){
    $ids[] = $id;
    $i++;
  }
  entity_delete_multiple('eolapi', $ids);
  $context['message'] = t('Deleted !num of !total', array(
    '!num' => $context['sandbox']['max'] - count($context['sandbox']['ids']),
    '!total' => $context['sandbox']['max']
  ));
  $context['finished'] = ($context['sandbox']['max'] - count($context['sandbox']['ids'])) / $context['sandbox']['max'];
}

/**
 * eolapi_batch_finished
 */
function eolapi_batch_finished(){}