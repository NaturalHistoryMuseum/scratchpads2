<?php

/**
 * 
 */
function taxonomy_get_tree($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $max_terms = 1000){
  if(array_pop(db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
    ':vid' => $vid
  ))->fetchCol()) < $max_terms){
    return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);
  }
  // We've a large number of terms.  We can attempt the tree if the $max_depth 
  // is 1, which it almost always is.
  if($max_depth == 1){
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $query->addField('t', 'tid');
    $query->condition('h.parent', $parent);
    $query->condition('t.vid', $vid);
    $query->addTag('term_access');
    $query->orderBy('t.weight');
    $query->orderBy('t.name');
    $tids = $query->execute()->fetchCol();
    return taxonomy_term_load_multiple($tids);
  }
  // We're asking too much. Set an error, and return an empty tree.
  watchdog('taxonomy', 'Attempted to get a large tree.', array(), WATCHDOG_ERROR);
  return array();
}

/**
 * Implements hook_field_settings_form().
 */
function taxonomy_field_settings_form($field, $instance, $has_data){
  // Get proper values for 'allowed_values_function', which is a core setting.
  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach($vocabularies as $vocabulary){
    $options[$vocabulary->machine_name] = $vocabulary->name;
  }
  $form['allowed_values'] = array(
    '#tree' => TRUE
  );
  foreach($vocabularies as $vocabulary){
    $form['allowed_values'][$vocabulary->machine_name]['vocabulary'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($vocabulary->name),
      '#default_value' => isset($field['settings']['allowed_values'][$vocabulary->machine_name]['vocabulary']) ? $field['settings']['allowed_values'][$vocabulary->machine_name]['vocabulary'] : '',
      '#return_value' => $vocabulary->machine_name,
      '#disabled' => $has_data
    );
    $form['allowed_values'][$vocabulary->machine_name]['parent'] = array(
      '#type' => 'value',
      '#value' => isset($field['settings']['allowed_values'][$vocabulary->machine_name]['parent']) ? $field['settings']['allowed_values'][$vocabulary->machine_name]['parent'] : 0
    );
  }
  return $form;
}