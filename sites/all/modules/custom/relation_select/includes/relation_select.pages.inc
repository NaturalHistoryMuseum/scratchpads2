<?php

/**
 * Form builder function for the add relation form.
 */
function relation_select_add_relation_form($form, &$form_state, $entity_type, $entity_id, $default_relation_type = NULL, $view_name = NULL, $view_display = NULL) {
  // Prevent exposed filters subform from caching
  unset($form_state['input']['subform-exposed_filters']);
  $entity = entity_load($entity_type, array(
    $entity_id
  ));
  if (!$entity) {
    drupal_set_message(t("Sorry, the entity could not be loaded."), 'error');
    return;
  }
  $entity_uri = entity_uri($entity_type, $entity[$entity_id]);
  $form['#redirect'] = $entity_uri['path'];
  $form_state['bundle'] = relation_get_available_types($entity_type, $entity[$entity_id]);
  $form_state['entity_type'] = $entity_type;
  $form_state['entity_id'] = $entity_id;
  // If there's no relation type, allow user to choose one here
  if (!$default_relation_type) {
    if (isset($form_state['values']['relation_type'])) {
      $default_relation_type = $form_state['values']['relation_type'];
    }
    $relation_types = relation_get_available_types($entity_type, $form_state['bundle']);
    $form['relation_type'] = array(
      '#type' => 'select',
      '#title' => t('Relation type'),
      '#options' => array(
        '' => t('< Please select >')
      ),
      '#description' => t('Please select the type of relation you want to create.'),
      //      '#default_value' => $relation_type,
      '#ajax' => array(
        'callback' => 'relation_select_add_relation_form_js',
        'wrapper' => 'relation-type-wrapper',
        'method' => 'replace'
      )
    );
    foreach ($relation_types as $relation_type) {
      $form['relation_type']['#options'][$relation_type->relation_type] = $relation_type->label;
    }
  }
  $form['relation_type_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'relation-type-wrapper'
    )
  );
  if ($default_relation_type) {
    $form['relation_type_wrapper']['relation_select'] = array(
      '#type' => 'relation_select',
      '#entity_id' => $form_state['entity_id'],
      '#entity_type' => $form_state['entity_type'],
      '#bundle' => $form_state['bundle'],
      '#relation_type' => $default_relation_type,
      '#required' => TRUE,
      '#title' => t('Relation'),
      '#description' => t('Please select the entity you would like to use as the relation.')
    );    
    if ($view_name) {
      $form['relation_type_wrapper']['relation_select']['#view'] = array(
        'name' => $view_name
      );
      if ($view_display) {
        $form['relation_type_wrapper']['relation_select']['#view']['display'] = $view_display;
      }
    }
    // Subform for any extra fields
    $relation = new stdClass();
    $relation->relation_type = $default_relation_type;
    field_attach_form('relation', $relation, $form['relation_type_wrapper'], $form_state);
    $form_state['relation'] = $relation;
    unset($form['relation_type_wrapper']['endpoints']);
    $form['relation_type_wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 10
    );
  }

  return $form;
}

function relation_select_add_relation_form_js($form, $form_state) {
  return ($form['relation_type_wrapper']);
}

function relation_select_add_relation_form_submit($form, &$form_state) {
  // Get the relation keys
  $entity_keys = relation_select_build_entity_keys($form_state['entity_type'], $form_state['entity_id'], $form_state['values']['relation_select']['endpoints']);
  // Build the relation object
  if ($relation = relation_create($form_state['relation']->relation_type, $entity_keys)) {
    entity_form_submit_build_entity('relation', $relation, $form, $form_state);
    // Save the new relation
    $form_state['rid'] = relation_save($relation);
    if (isset($form['#redirect'])) {
      $form_state['redirect'] = $form['#redirect'];
    }
    elseif ($relation->rid) {
      $form_state['redirect'] = 'relation/' . $relation->rid;
    }
    drupal_set_message(t('New relation created'));
  }
  else { // Coult not create relation - prob cos it already exists
    drupal_set_message(t('Sorry this relation could not be created.'), 'error');
  }
}
