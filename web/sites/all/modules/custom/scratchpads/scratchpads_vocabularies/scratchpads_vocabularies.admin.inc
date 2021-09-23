<?php

/**
 * function scratchpads_vocabularies_admin_list
 * 
 * List all the available mappings
 */
function scratchpads_vocabularies_admin_list($form, &$form_state){
  // Return the confirmation forms where appropriate
  if(isset($form_state['storage']['confirm_existing_field'])){return confirm_form(array(), t("The field %field seems to already be mapped. This operation will delete the existing information, version number, pending updates and so on.", array(
      '%field' => $form_state['values']['new_vocabulary_field']
    )), 'admin/structure/vocabularies');}
  // The main form
  drupal_add_css(drupal_get_path('module', 'scratchpads_vocabularies') . '/theme/scratchpads_vocabularies.css', 'file');
  $output = array();
  // Build the list of mappings
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  $updates_available = FALSE;
  $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
  $field_options = _scratchpads_vocabularies_available_fields();
  $header = array(
    t('Mapping'),
    t('Status'),
    t('Operations')
  );
  $rows = array();
  foreach($all_info as $info_id => $info){
    list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
    if(!$vocabulary || !$receiver){
      continue;
    }
    $vocabulary->refresh(); // Ensure we re-fetch any data
    $status = array();
    if($vocabulary->version() !== $receiver->version()){
      $status[] = t('There is a new version of the vocabulary available. You need to upgrade this vocabulary.');
    }else{
      $status[] = t('The vocabulary is up to date.');
    }
    if($count_pu = $receiver->pendingUpdates()){
      $status[] = t('There are %count pending update(s) for this vocabulary. Please go to \'manage\' and edit the updates', array(
        '%count' => $count_pu
      ));
    }
    $operations = array(
      l(t('Manage'), 'admin/config/services/vocabularies/' . $info_id)
    );
    if(isset($our_vocs[$info_id])){
      global $user;
      if(user_access('delete any vocabulary mapping') || (!empty($our_vocs[$info_id]['uid']) && $our_vocs[$info_id]['uid'] == $user->uid)){
        $operations[] = l(t('Delete'), 'admin/config/services/vocabularies/' . $info_id . '/delete');
      }
    }
    $rows[] = array(
      t('Map from %source to %destination', array(
        '%source' => $vocabulary->name(),
        '%destination' => $receiver->name()
      )),
      '<ul><li>' . implode('</li><li>', $status) . ' </li></ul>',
      implode(' ', $operations)
    );
  }
  $output['list'] = _scratchpads_vocabularies_form_table($header, $rows);
  // Option for a new entry
  if(user_access('add vocabulary mappings')){
    $output['new_vocabulary'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Add new')
    );
    $output['new_vocabulary']['new_vocabulary_title'] = array(
      '#title' => t('Title'),
      '#description' => t('The title for this mapping'),
      '#type' => 'textfield',
      '#default_value' => ''
    );
    $output['new_vocabulary']['new_vocabulary_url'] = array(
      '#title' => t('Source'),
      '#description' => t('A URL to a valid GBIF XML vocabulary'),
      '#type' => 'textfield',
      '#default_value' => '',
      '#field_prefix' => 'http://'
    );
    $output['new_vocabulary']['new_vocabulary_field'] = array(
      '#title' => t('Destination'),
      '#description' => t('Select a List (text) field to receive the vocabulary. <strong>Warning:</strong> using this will change the allowed values of your field.'),
      '#type' => 'select',
      '#options' => $field_options
    );
    $output['new_vocabulary']['save'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      '#name' => 'add_new'
    );
  }
  return $output;
}

/**
 * scratchpads_vocabularies_admin_list_validate
 */
function scratchpads_vocabularies_admin_list_validate($form, &$form_state){
  $vocabularies = module_invoke_all('scratchpads_vocabulary_info');
  if(isset($form_state['storage']['triggering_element'])){
    $tr = $form_state['storage']['triggering_element'];
    $values = & $form_state['storage']['values'];
  }else{
    $tr = $form_state['triggering_element'];
    $values = & $form_state['values'];
  }
  if($tr['#name'] == 'add_new'){
    if(!user_access('add vocabulary mappings')){
      form_set_error('new_vocabulary_title', t("You do not have permission to add new vocabularies"));
    }else{
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
          $xml = new ScratchpadsVocabularies\GbifXmlVocabulary($url);
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
        if($test_field->persistentPropertiesExist() && !user_access('delete any vocabulary mapping')){
          form_set_error('new_vocabulary_field', "This field is already in use and you do not have permission to delete fields");
          $voc_failed = TRUE;
        }
        if(!$voc_failed){
          $values['new_vocabulary_url'] = $url;
        }
      }
      if(!in_array($values['new_vocabulary_field'], $field_options)){
        form_set_error('new_vocabulary_field', t('The field must be one of the available fields for which there isn\'t already a mapping'));
      }
    }
  }else if($tr['#name'] != 'all_updates' && empty($tr['#view_pending_updates'])){
    form_set_error($tr['#name'], 'An invalid option was selected. Please refresh the form and try again.');
  }
}

/**
 * scratchpads_vocabularies_admin_list_submit
 */
function scratchpads_vocabularies_admin_list_submit($form, &$form_state){
  if(isset($form_state['storage']['triggering_element'])){
    $tr = $form_state['storage']['triggering_element'];
    $values = & $form_state['storage']['values'];
  }else{
    $tr = $form_state['triggering_element'];
    $values = & $form_state['values'];
  }
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if($tr['#name'] == 'add_new'){
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
    global $user;
    $our_vocs[$final_id] = array(
      'title' => $values['new_vocabulary_title'],
      'url' => $values['new_vocabulary_url'],
      'field' => $values['new_vocabulary_field'],
      'uid' => $user->uid
    );
    variable_set('scratchpads_vocabularies_ui_vocabularies', $our_vocs);
    drupal_set_message(t('New mapping was added'), 'info');
  }else if($tr['#name'] == 'all_updates'){
    $all_info = module_invoke_all('scratchpads_vocabulary_info');
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
 * scratchpads_vocabularies_admin_form
 */
function scratchpads_vocabularies_admin_form($form, &$form_state){
  // Get the right definition array
  if(!isset($form_state['build_info']['args'][0])){
    $info_id = null;
  }else{
    $info_id = $form_state['build_info']['args'][0];
  }
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if(!isset($all_info[$info_id])){
    drupal_access_denied();
    return;
  }
  $info = $all_info[$info_id];
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
  if(!$vocabulary || !$receiver){return array(
      '#markup' => t('There was an error loading the mapping')
    );}
  $vocabulary->refresh(); // Ensure we re-fetch any data
  // Prepare the form
  drupal_add_css(drupal_get_path('module', 'scratchpads_vocabularies') . '/theme/scratchpads_vocabularies.css', 'file');
  drupal_set_title($info['title']);
  $form['voc_groups'] = array(
    '#type' => 'vertical_tabs'
  );
  $form['voc_gen'] = array(
    '#type' => 'fieldset',
    '#group' => 'voc_groups',
    '#title' => 'General'
  );
  $form['voc_gen']['title'] = array(
    '#markup' => '<h3>' . check_plain($info['title']) . '</h3>'
  );
  $form['voc_gen']['description'] = array(
    '#type' => 'markup',
    '#markup' => theme('scratchpads_vocabularies_info', array(
      'tite' => $info['title'],
      'receiver' => $receiver,
      'vocabulary' => $vocabulary,
      'info' => $info
    ))
  );
  $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
  if($vocabulary->version() != $receiver->version() && user_access('update vocabularies')){
    $form['voc_gen']['updatemsg'] = array(
      '#markup' => '<strong class="vocabularies_status">' . t('A new version of this vocabulary is available') . '</strong>'
    );
    $form['voc_gen']['update'] = array(
      '#type' => 'submit',
      '#value' => t('Upgrade now'),
      '#update_voc_id' => $info_id
    );
  }
  if(count($receiver->pendingUpdates()) && user_access('update vocabularies')){
    $form['voc_gen']['updates'] = array(
      '#markup' => l(t('Updates'), 'admin/config/services/vocabularies/' . $info_id . '/updates')
    );
  }
  if(isset($our_vocs[$info_id])){
    global $user;
    if(user_access('delete any vocabulary mapping') || (!empty($our_vocs[$info_id]['uid']) && $our_vocs[$info_id]['uid'] == $user->uid)){
      $form['voc_gen']['delete'] = array(
        '#markup' => l(t('Delete'), 'admin/config/services/vocabularies/' . $info_id . '/delete')
      );
    }
  }
  // Items tab
  $form['voc_items'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabulary items'),
    '#group' => 'voc_groups'
  );
  if($receiver->version() == $vocabulary->version()){
    $form['voc_items']['items'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Items'),
      '#prefix' => '<div class="scratchpads-vocabularies-items">',
      '#suffix' => '</div>',
      1 => array(
        '#markup' => $receiver->structure()->render()
      )
    );
  }else{
    $form['voc_items']['items_receiver'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Items currently in use'),
      '#prefix' => '<div class="scratchpads-vocabularies-items">',
      '#suffix' => '</div>',
      1 => array(
        '#markup' => $receiver->structure()->render()
      )
    );
    $form['voc_items']['items_source'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Items available'),
      '#prefix' => '<div class="scratchpads-vocabularies-items">',
      '#suffix' => '</div>',
      1 => array(
        '#markup' => $vocabulary->structure()->render()
      )
    );
  }
  return $form;
}

/**
 * scratchpads_vocabularies_admin_form_submit
 */
function scratchpads_vocabularies_admin_form_submit($form, &$form_state){
  $info_id = $form_state['triggering_element']['#update_voc_id'];
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if(!isset($all_info[$info_id])){return;}
  $info = $all_info[$info_id];
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($info);
  if(!$vocabulary || !$receiver){
    drupal_set_message(t('There was an error upgrading the mapping.'), 'error');
    return;
  }
  _scratchpads_vocabularies_update_vocabulary($vocabulary, $receiver);
}

/**
 * scratchpads_vocabularies_delete_form
 */
function scratchpads_vocabularies_delete_form($form, &$form_state){
  return confirm_form(array(), t("Are you sure you want to delete the vocabulary %voc ?", array(
    '%voc' => $form_state['build_info']['args'][0]
  )), 'admin/config/services/vocabularies');
}

/**
 * scratchpads_vocabularies_delete_form_submit
 */
function scratchpads_vocabularies_delete_form_submit($form, &$form_state){
  $info_id = $form_state['build_info']['args'][0];
  $our_vocs = variable_get('scratchpads_vocabularies_ui_vocabularies', array());
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if(!isset($our_vocs[$info_id])){
    drupal_set_message(t('Could not delete unknown vocabulary mapping %voc', array(
      '%voc' => $info_id
    )));
    drupal_goto('admin/config/services/vocabularies');
    return;
  }
  global $user;
  if(!user_access('delete any vocabulary mapping') && (empty($our_vocs[$info_id]['uid']) || $our_vocs[$info_id]['uid'] != $user->uid)){
    drupal_set_message(t('You do not have permission to delete this vocabulary'));
    drupal_goto('admin/config/services/vocabularies');
    return;
  }
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($all_info[$info_id]);
  if($vocabulary){
    $vocabulary->deletePersistentProperties();
  }
  if($receiver){
    $receiver->deletePersistentProperties();
  }
  unset($our_vocs[$info_id]);
  variable_set('scratchpads_vocabularies_ui_vocabularies', $our_vocs);
  drupal_set_message(t('Mapping was deleted'), 'info');
  drupal_goto('admin/config/services/vocabularies');
}

/**
 * scratchpads_vocabularies_update_page
 */
function scratchpads_vocabularies_update_page($info_id){
  $all_info = module_invoke_all('scratchpads_vocabulary_info');
  if(!isset($all_info[$info_id])){
    drupal_set_message(t('Unknown vocabulary mapping %voc', array(
      '%voc' => $info_id
    )));
    drupal_goto('admin/config/services/vocabularies');
    return;
  }
  drupal_add_css(drupal_get_path('module', 'scratchpads_vocabularies') . '/theme/scratchpads_vocabularies.css', 'file');
  $form = array();
  list($vocabulary, $receiver) = _scratchpads_vocabularies_info_build($all_info[$info_id]);
  if(!$vocabulary || !$receiver){
    drupal_set_message(t('There was a problem loading the vocabulary mapping %voc', array(
      '%voc' => $info_id
    )));
    drupal_goto('admin/config/services/vocabularies');
    return;
  }
  if(!count($receiver->pendingUpdates())){return array(
      '#type' => 'markup',
      '#markup' => t('There are no pending updates.')
    );}
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
      $action_table = drupal_get_form('scratchpads_vocabularies_modification_form_' . $update_id, $modification);
    }
    $update_table[] = array(
      check_plain($update->type()),
      $reason,
      $action_table
    );
  }
  $form['updates'] = _scratchpads_vocabularies_form_table(array(
    t('Update type'),
    t('Reason this is pending'),
    t('Action')
  ), $update_table);
  return $form;
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
