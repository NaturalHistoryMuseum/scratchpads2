<?php

/**
 * 
 */
function taxonomy_get_tree($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $max_terms = 1000){
  $result = db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
    ':vid' => $vid
  ))->fetchCol();
  if(array_pop($result) < $max_terms){return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);}
  // We've a large number of terms.  We can attempt the tree if the $max_depth 
  // is 1, which it almost always is.
  if($max_depth == 1){
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $results = $query->addTag('translatable')->addTag('term_access')->fields('t')->fields('h', array(
      'parent'
    ))->condition('t.vid', $vid)->condition('h.parent', $parent)->orderBy('t.weight')->orderBy('t.name')->execute();
    $terms = array();
    foreach($results as $row){
      $terms[$row->tid] = $row;
    }
    if($load_entities){return taxonomy_term_load_multiple(array_keys($terms));}
    return $terms;
  }
  // We're asking too much. Set an error, and return an empty tree.
  watchdog('Taxonomy', 'Attempted to get a large tree.', array(), WATCHDOG_ERROR);
  return array();
}

/**
 * Implements hook_field_settings_form().
 */
function taxonomy_field_settings_form($field, $instance, $has_data){
  // Get proper values for 'allowed_values_function', which is a core setting.
  $vocabularies = taxonomy_get_vocabularies();
  $form['allowed_values'] = array(
    '#tree' => TRUE
  );
  $i = 0;
  $default_values = array();
  if(isset($field['settings']['allowed_values'])){
    foreach($field['settings']['allowed_values'] as $value){
      $default_values[$value['vocabulary']] = $value;
    }
  }
  foreach($vocabularies as $vocabulary){
    $form['allowed_values'][$i]['vocabulary'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($vocabulary->name),
      '#default_value' => isset($default_values[$vocabulary->machine_name]['vocabulary']) ? $default_values[$vocabulary->machine_name]['vocabulary'] : 0,
      '#return_value' => $vocabulary->machine_name,
      '#disabled' => $has_data
    );
    $form['allowed_values'][$i]['parent'] = array(
      '#type' => 'value',
      '#value' => isset($default_values[$vocabulary->machine_name]['parent']) ? $default_values[$vocabulary->machine_name]['parent'] : 0
    );
    $i++;
  }
  return $form;
}

/**
 * Clear all static cache variables for terms.
 */
function taxonomy_terms_static_reset(){
  drupal_static_reset('taxonomy_get_tree_original');
  drupal_static_reset('taxonomy_get_tree_original:parents');
  drupal_static_reset('taxonomy_get_tree_original:terms');
  taxonomy_terms_static_reset_original();
}

/**
 * Implementations of hook_form_FORM_ID_alter
 */
function taxonomy_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id){
  taxonomy_form_field_ui_field_settings_form_alter($form, $form_state, $form_id);
}

function taxonomy_form_field_ui_field_settings_form_alter(&$form, &$form_state, $form_id){
  if(isset($form['field']['type']) && $form['field']['type']['#value'] == 'taxonomy_term_reference'){
    // We tweak the submit so that we can alter the values array early on.
    $form['#submit'] = array_merge(array(
      'taxonomy_field_ui_field_settings_form_submit'
    ), $form['#submit']);
  }
}

/**
 * Tweak the submitted values to ensure the array is keyed from 0.
 */
function taxonomy_field_ui_field_settings_form_submit(&$form, &$form_state){
  $new_settings = array();
  foreach($form_state['values']['field']['settings']['allowed_values'] as $key => $value){
    if($value['vocabulary']){
      $new_settings[] = $value;
    }
  }
  $form_state['values']['field']['settings']['allowed_values'] = $new_settings;
}