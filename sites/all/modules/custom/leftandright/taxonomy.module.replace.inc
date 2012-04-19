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
 * Identical to the original, but with the field_attach_delete code removed.
 * Enter description here ...
 * @param unknown_type $tid
 * @throws Exception
 */
function taxonomy_term_delete($tid){
  $transaction = db_transaction();
  try{
    $tids = array(
      $tid
    );
    while($tids){
      $children_tids = $orphans = array();
      foreach($tids as $tid){
        // See if any of the term's children are about to be become orphans:
        if($children = taxonomy_get_children($tid)){
          foreach($children as $child){
            // If the term has multiple parents, we don't delete it.
            $parents = taxonomy_get_parents($child->tid);
            if(count($parents) == 1){
              $orphans[] = $child->tid;
            }
          }
        }
        if($term = taxonomy_term_load($tid)){
          db_delete('taxonomy_term_data')->condition('tid', $tid)->execute();
          db_delete('taxonomy_term_hierarchy')->condition('tid', $tid)->execute();
          // SDRycroft
          //field_attach_delete('taxonomy_term', $term);
          $taxonomy_terms_delete_cron = variable_get('taxonomy_terms_delete_cron', array());
          $taxonomy_terms_delete_cron[] = $term;
          variable_set('taxonomy_terms_delete_cron', $taxonomy_terms_delete_cron);
          // End SDRycroft
          module_invoke_all('taxonomy_term_delete', $term);
          module_invoke_all('entity_delete', $term, 'taxonomy_term');
          taxonomy_terms_static_reset();
        }
      }
      $tids = $orphans;
    }
    return SAVED_DELETED;
  }
  catch(Exception $e){
    $transaction->rollback();
    watchdog_exception('taxonomy', $e);
    throw $e;
  }
}

/**
 * Run the field_attach_delete function that wasn't run when deleting a term.
 */
function taxonomy_cron(){
  $taxonomy_terms_delete_cron = variable_get('taxonomy_terms_delete_cron', array());
  foreach($taxonomy_terms_delete_cron as $term){
    field_attach_delete('taxonomy_term', $term);
  }
  variable_del('taxonomy_terms_delete_cron');
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