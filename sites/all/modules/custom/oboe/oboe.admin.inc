<?php

/**
 * Generates the profile type editing form.
 */
function oboe_form($form, &$form_state, $oboe, $op = 'edit'){
  if($op == 'clone'){
    $profile_type->label .= ' (cloned)';
    $profile_type->type = '';
  }
  return array(
    'label' => array(
      '#title' => t('Job name'),
      '#type' => 'textfield',
      '#default_value' => $oboe->is_new ? '' : $oboe->label,
      '#description' => t('A label for this job.'),
      '#required' => TRUE,
      '#size' => 30
    ),
    'type' => array(
      '#title' => t('Job type'),
      '#empty_option' => t('Select job type'),
      '#required' => TRUE,
      '#options' => OBOEService::get_job_types(),
      '#type' => 'select',
      '#default_value' => $oboe->is_new ? '' : $oboe->type
    ),
    'actions' => array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#weight' => 40
      )
    )
  );
}

/**
 * Form API submit callback for the type form.
 */
function oboe_form_submit(&$form, &$form_state){
  $oboe = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $oboe->save();
}

/**
 * Form API submit callback for the delete button.
 */
function oboe_form_submit_delete(&$form, &$form_state){
  $form_state['redirect'] = 'admin/structure/profiles/manage/' . $form_state['oboe']->type . '/delete';
}