<?php

/**
 * scratchpads_vocabularies_mapping_form
 * 
 * Page/Form callback for /admin/vocabularies/mapping
 */
function scratchpads_vocabularies_mapping_form($form, &$form_state){
  // Return the confirmation forms where appropriate
  if(isset($form_state['storage']['confirm_delete'])){
    return confirm_form(array(), t("Are you sure you want to delete the vocabulary %voc ?", array(
      '%voc' => $form_state['triggering_element']['#delete_name']
    )), 'admin/structure/vocabularies');
  }else if(isset($form_state['storage']['confirm_existing_field'])){return confirm_form(array(), t("The field %field seems to already be mapped. This operation will delete the existing information, version number, pending updates and so on.", array(
      '%field' => $form_state['values']['new_vocabulary_field']
    )), 'admin/structure/vocabularies');}
  // The main form
  drupal_add_css(drupal_get_path('module', 'scratchpads_vocabularies') . '/theme/scratchpads_vocabularies.css', 'file');
  $output = array();
  module_load_include('inc', 'scratchpads_vocabularies', 'includes/classes');
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  $updates_available = FALSE;
  $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
  $field_options = _scratchpads_vocabularies_available_fields();
  $header = array(
    '',
    t('Name'),
    t('Description'),
    t('Version')
  );
  foreach($all_info as $info_id => $info){
    list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
    if(!$vocabulary || !$receiver){
      continue;
    }
    $output[$info_id] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => $info['title']
    );
    $output[$info_id]['info'] = array(
      '#type' => 'markup',
      '#markup' => theme('scratchpads_vocabularies_info', array(
        'tite' => $info['title'],
        'receiver' => $receiver,
        'vocabulary' => $vocabulary,
        'info' => $info
      ))
    );
    if($vocabulary->version() != $receiver->version()){
      $updates_available = TRUE;
      $output[$info_id]['updatenow'] = array(
        '#markup' => '<strong class="vocabularies_status">' . t('A new version of this vocabulary is available') . '</strong>'
      );
      $output[$info_id]["update_$info_id"] = array(
        '#type' => 'submit',
        '#value' => t('Upgrade now'),
        '#update_voc_id' => $info_id
      );
    }
    $count = count($receiver->pendingUpdates());
    if($count){
      $output[$info_id]["view_$info_id"] = array(
        '#type' => 'submit',
        '#value' => t('View pending updates'),
        '#view_pending_updates' => TRUE
      );
    }
    if(isset($our_vocs[$info_id])){
      $output[$info_id]['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete this mapping'),
        '#delete_mapping' => $info_id,
        '#delete_name' => $all_info[$info_id]['title']
      );
    }
  }
  // Button for updating all
  if($updates_available){
    $output['all_updates'] = array(
      '#type' => 'submit',
      '#value' => t('Upgrade all'),
      '#name' => 'all_updates'
    );
  }
  // Option for a new entry
  $output['new_vocabulary_'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Add new')
  );
  $output['new_vocabulary_']['info'] = array(
    '#markup' => '',
    '#prefix' => '<div class="scratchpads_vocabularies_info">',
    '#suffix' => '</div>'
  );
  $output['new_vocabulary_']['info']['table'] = _scratchpads_vocabularies_form_table($header, array(
    array(
      '<strong>' . t('Title') . '</strong>',
      array(
        'new_vocabulary_title' => array(
          '#type' => 'textfield',
          '#default_value' => ''
        )
      ),
      t('The title for this mapping'),
      ''
    ),
    array(
      '<strong>' . t('Source') . '</strong>',
      array(
        'new_vocabulary_url' => array(
          '#type' => 'textfield',
          '#default_value' => '',
          '#field_prefix' => 'http://'
        )
      ),
      t('Enter a URL to a valid GBif XML vocabulary'),
      '(new)'
    ),
    array(
      '<strong>' . t('Destination') . '</strong>',
      array(
        'new_vocabulary_field' => array(
          '#type' => 'select',
          '#options' => $field_options
        )
      ),
      t('Select a List (text) field to receiver the vocabulary. <strong>Warning:</strong> using this will change the allowed values of your field'),
      '(new)'
    )
  ));
  $output['new_vocabulary_']['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#name' => 'add_new'
  );
  return $output;
}

/**
 * scratchpads_vocabularies_mapping_form_validate
 */
function scratchpads_vocabularies_mapping_form_validate($form, &$form_state){
  $vocabularies = module_invoke_all('scratchpads_vocabulary_info');
  if(isset($form_state['storage']['triggering_element'])){
    $tr = $form_state['storage']['triggering_element'];
    $values = & $form_state['storage']['values'];
  }else{
    $tr = $form_state['triggering_element'];
    $values = & $form_state['values'];
  }
  if(isset($tr['#delete_mapping'])){
    $info_id = $tr['#delete_mapping'];
    $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
    if(!isset($vocabularies[$info_id]) || !isset($our_vocs[$info_id])){
      form_set_error($tr['#name'], 'An invalid option was selected. Please refresh the form and try again.');
    }
  }else if(isset($tr['#update_voc_id'])){
    if(empty($vocabularies[$tr['#update_voc_id']])){
      form_set_error($tr['#name'], 'An invalid option was selected. Please refresh the form and try again.');
    }
  }else if($tr['#name'] == 'add_new'){
    $field_options = _scratchpads_vocabularies_available_fields();
    if(empty($values['new_vocabulary_title'])){
      form_set_error('new_vocabulary_title', t('The title is required'));
    }
    if(empty($values['new_vocabulary_url'])){
      form_set_error('new_vocabulary_url', t('The URL is required'));
    }else{
      $url = 'http://' . preg_replace('/^(http:)?\/+/', '', $values['new_vocabulary_url']);
      $voc_failed = FALSE;
      try{
        $xml = new ScratchpadsVocabularies\GbifXmlVocabulary('test', $url);
      }
      catch(Exception $e){
        form_set_error('new_vocabulary_url', "This does not seem to be a valid GBif XML file. Error message: " . $e->getMessage());
        $voc_failed = TRUE;
      }
      try{
        $test_field = new ScratchpadsVocabularies\SelectFieldReceiver($values['new_vocabulary_field']);
      }
      catch(Exception $e){
        form_set_error('new_vocabulary_field', "It was not possible to use this field. Error message: " . $e->getMessage());
        $voc_failed = TRUE;
      }
      if(!$voc_failed){
        $values['new_vocabulary_url'] = $url;
      }
    }
    if(!in_array($values['new_vocabulary_field'], $field_options)){
      form_set_error('new_vocabulary_field', t('The field must be one of the available fields for which there isn\'t already a mapping'));
    }
  }else if($tr['#name'] != 'all_updates' && empty($tr['#view_pending_updates'])){
    form_set_error($tr['#name'], 'An invalid option was selected. Please refresh the form and try again.');
  }
}

/**
 * scratchpads_vocabularies_mapping_form_submit
 */
function scratchpads_vocabularies_mapping_form_submit($form, &$form_state){
  if(isset($form_state['storage']['triggering_element'])){
    $tr = $form_state['storage']['triggering_element'];
    $values = & $form_state['storage']['values'];
  }else{
    $tr = $form_state['triggering_element'];
    $values = & $form_state['values'];
  }
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if(!empty($tr['#view_pending_updates'])){
    drupal_goto('admin/structure/vocabularies/updates');
    return;
  }else if(isset($tr['#delete_mapping'])){
    if(empty($form_state['storage']['confirm_delete'])){
      $form_state['storage']['triggering_element'] = $tr;
      $form_state['storage']['values'] = $values;
      $form_state['storage']['confirm_delete'] = TRUE;
      $form_state['rebuild'] = TRUE;
      return;
    }
    $info_id = $tr['#delete_mapping'];
    $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
    if(isset($our_vocs[$info_id])){
      try{
        $delete_field = new ScratchpadsVocabularies\SelectFieldReceiver($our_vocs[$info_id]['field']);
        $delete_field->deletePersistentProperties();
      }
      catch(Exception $e){
      }
      unset($our_vocs[$tr['#delete_mapping']]);
      variable_set('scratchpads_vocabularies_ui_vocabularies', $our_vocs);
      drupal_set_message(t('Mapping was deleted'), 'info');
    }
  }else if($tr['#name'] == 'add_new'){
    $test_field = new ScratchpadsVocabularies\SelectFieldReceiver($values['new_vocabulary_field']);
    if($test_field->persistentPropertiesExist()){
      if(empty($form_state['storage']['confirm_existing_field'])){
        $form_state['storage']['triggering_element'] = $tr;
        $form_state['storage']['values'] = $values;
        $form_state['storage']['confirm_existing_field'] = TRUE;
        $form_state['rebuild'] = TRUE;
        return;
      }else{
        $test_field->deletePersistentProperties();
      }
    }
    $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
    $id = preg_replace('/[^a-zA-Z0-9_]+/', '_', strtolower($values['new_vocabulary_title']));
    $final_id = $id;
    $count = 0;
    while(isset($our_vocs[$final_id])){
      $count++;
      $final_id = $id . $count;
    }
    $our_vocs[$final_id] = array(
      'title' => $values['new_vocabulary_title'],
      'url' => $values['new_vocabulary_url'],
      'field' => $values['new_vocabulary_field']
    );
    variable_set('scratchpads_vocabularies_ui_vocabularies', $our_vocs);
    drupal_set_message(t('New mapping was added'), 'info');
  }else if(isset($tr['#update_voc_id']) || $tr['#name'] == 'all_updates'){
    $all_info = module_invoke_all('scratchpads_vocabulary_info');
    if(isset($tr['#update_voc_id'])){
      $all_info = array(
        $all_info[$tr['#update_voc_id']]
      );
    }
    foreach($all_info as $info_id => $info){
      list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
      if(!$vocabulary || !$receiver){
        continue;
      }
      _scratchpads_vocabularies_update_vocabulary($vocabulary, $receiver, FALSE);
    }
  }
}

/**
 * scratchpads_vocabularies_pending_update_form
 *
 * Page callback for /admin/vocabularies/updates
 */
function scratchpads_vocabularies_pending_update_form($form, &$form_state){
  drupal_add_css(drupal_get_path('module', 'scratchpads_vocabularies') . '/theme/scratchpads_vocabularies.css', 'file');
  $output = array();
  module_load_include('inc', 'scratchpads_vocabularies', 'includes/classes');
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  foreach($all_info as $info_id => $info){
    list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
    if(!$vocabulary || !$receiver || !count($receiver->pendingUpdates())){
      continue;
    }
    $update_table = array();
    foreach($receiver->pendingUpdates() as $update_id => $update){
      // Work out why it's pending
      $modification = $update->modification();
      if(!$modification){
        $reason = t('Unknown. Please report an issue on the Scratchpads issue queue about this problem');
        $action_table = array(
          '#markup' => t('No available actions')
        );
      }else{
        $reason = $modification->explanation();
        $action_table = array();
        $action_table["apply_form_" . $update_id . "_" . $info_id] = $modification->form(array(
          'vocabulary_id' => $info_id,
          'update_id' => $update_id
        ), $form, $form_state);
        $action_table["apply_" . $update_id . "_" . $info_id] = array(
          '#type' => 'submit',
          '#value' => t('Apply Pending Update'),
          '#update_id' => $update_id,
          '#vocabulary_id' => $info_id
        );
      }
      $update_table[] = array(
        check_plain($update->type()),
        $reason,
        $action_table
      );
    }
    $output[$info_id] = array(
      '#type' => 'fieldset',
      '#title' => $info['title'],
      '#collapsible' => FALSE,
      'updates' => _scratchpads_vocabularies_form_table(array(
        t('Update type'),
        t('Reason this is pending'),
        t('Action')
      ), $update_table)
    );
  }
  return $output;
}

/**
 * scratchpads_vocabularies_pending_update_form_submit
 */
function scratchpads_vocabularies_pending_update_form_submit($form, &$form_state){
  // Get the update
  $vocabulary_id = $form_state['triggering_element']['#vocabulary_id'];
  $update_id = $form_state['triggering_element']['#update_id'];
  module_load_include('inc', 'scratchpads_vocabularies', 'includes/classes');
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($all_info[$vocabulary_id]);
  if(!$vocabulary || !$receiver){return;}
  $updates = $receiver->pendingUpdates();
  $update = $updates[$update_id];
  $modification = $update->modification();
  $operations = $modification->submit(array(
    'vocabulary_id' => $vocabulary_id,
    'update_id' => $update_id
  ), $form, $form_state);
  batch_set(array(
    'title' => t('Applying updates. Please do not close this window until the process is complete.'),
    'progress_message' => t('Completed @percentage% of updates, estimated time left @estimate'),
    'operations' => $operations,
    'finished' => '_scratchpads_vocabularies_finish_update',
    'file' => drupal_get_path('module', 'scratchpads_vocabularies') . '/scratchpads_vocabularies.admin.inc'
  ));
}

/**
 * function _scratchpads_vocabularies_available_fields
 *
 * Returns the list of select fields that do not have a
 * mapping associated via the UI.
 *
 * Note that this may include fields that already have
 * an associated mapping via a hook
 */
function _scratchpads_vocabularies_available_fields(){
  // Get all the fields
  $matching_fields = field_read_fields(array(
    'type' => 'list_text',
    'module' => 'list'
  ));
  $field_options = array();
  foreach($matching_fields as $field){
    $field_options[$field['field_name']] = $field['field_name'];
  }
  // Now filter out those already used
  $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  foreach($our_vocs as $info_id => $our_info){
    list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($all_info[$info_id]);
    if(!$vocabulary || !$receiver){
      continue;
    }
    unset($field_options[$receiver->id()]);
  }
  return $field_options;
}

/**
 * _scratchpads_vocabularies_remap
 *
 * Batch callback for remaping values on entities
 */
function _scratchpads_vocabularies_remap($vocabulary_id, $update_id, $type, $entities, $map){
  module_load_include('inc', 'scratchpads_vocabularies', 'includes/classes');
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($all_info[$vocabulary_id]);
  if(!$vocabulary || !$receiver){return;}
  $updates = $receiver->pendingUpdates();
  $update = $updates[$update_id];
  $receiver->applyModifications(array(
    $type => $entities
  ), array(
    $update
  ), $map);
}

/**
 * _scratchpads_vocabularies_finish_update
 * 
 * Batch finish callback
 */
function _scratchpads_vocabularies_finish_update(){
  // Changes may impact menus and so on, so do a full cache clear.
  drupal_flush_all_caches();
}
