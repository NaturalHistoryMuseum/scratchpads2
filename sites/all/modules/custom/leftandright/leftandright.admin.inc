<?php

/**
 * Display form for adding and editing vocabularies.
 */
function leftandright_rebuild_tree_form($form, &$form_state, $edit = array()) {
  echo "HERE";exit;
  // During initial form build, add the entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['vocabulary'])) {
    $vocabulary = is_object($edit) ? $edit : (object) $edit;
    $defaults = array(
      'name' => '',
      'machine_name' => '',
      'description' => '',
      'hierarchy' => 0,
      'weight' => 0,
    );
    foreach ($defaults as $key => $value) {
      if (!isset($vocabulary->$key)) {
        $vocabulary->$key = $value;
      }
    }
    $form_state['vocabulary'] = $vocabulary;
  }
  else {
    $vocabulary = $form_state['vocabulary'];
  }

  // @todo Legacy support. Modules are encouraged to access the entity using
  //   $form_state. Remove in Drupal 8.
  $form['#vocabulary'] = $form_state['vocabulary'];

  // Check whether we need a deletion confirmation form.
  if (isset($form_state['confirm_delete']) && isset($form_state['values']['vid'])) {
    return taxonomy_vocabulary_confirm_delete($form, $form_state, $form_state['values']['vid']);
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $vocabulary->name,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $vocabulary->machine_name,
    '#maxlength' => 21,
    '#machine_name' => array(
      'exists' => 'taxonomy_vocabulary_machine_name_load',
    ),
  );
  $form['old_machine_name'] = array(
    '#type' => 'value',
    '#value' => $vocabulary->machine_name,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $vocabulary->description,
  );
  // Set the hierarchy to "multiple parents" by default. This simplifies the
  // vocabulary form and standardizes the term form.
  $form['hierarchy'] = array(
    '#type' => 'value',
    '#value' => '0',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  if (isset($vocabulary->vid)) {
    $form['actions']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
    $form['vid'] = array('#type' => 'value', '#value' => $vocabulary->vid);
    $form['module'] = array('#type' => 'value', '#value' => $vocabulary->module);
  }
  return $form;
}