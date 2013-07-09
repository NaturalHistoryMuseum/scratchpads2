<?php

/**
 * Translation setup
 */
function scratchpads_translate_setup_translation_form(){
  $form = array();
  // We only allow translation of Nodes, Users and Terms.  We therefore need to
  // check each of the bundles for those entities, and display whether or not
  // they have translations enabled.
  // NODE
  foreach(array(
    'node',
    'taxonomy_term',
    'user'
  ) as $entity_type){
    $entity_info = entity_get_info($entity_type);
    $values = $default_value = $fields_options = $fields_default_value = array();
    foreach($entity_info['bundles'] as $bundle_name => $bundle_details){
      switch($entity_type){
        case 'node':
          $values[$bundle_name] = $bundle_details['label'];
          if(locale_multilingual_node_type($bundle_name)){
            $default_value[] = $bundle_name;
          }
          break;
        case 'taxonomy_term':
          $biological_vids = variable_get('biological_vids', array());
          $vocabulary = taxonomy_vocabulary_machine_name_load($bundle_name);
          if(!empty($biological_vids[$vocabulary->vid])){
            continue 2;
          }
          $values[$bundle_name] = $bundle_details['label'];
          if(i18n_taxonomy_vocabulary_mode($vocabulary)){
            $default_value[] = $bundle_name;
          }
          break;
      }
    }
    // Currently 'user' only has one bundle (user), so we have no need of the
    // form for that entity.
    switch($entity_type){
      default:
        $form[$entity_type] = array(
          '#type' => 'fieldset',
          '#title' => $entity_info['label'],
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#tree' => TRUE,
          'bundles' => array(
            '#type' => 'checkboxes',
            '#title' => t('Bundles'),
            '#options' => $values,
            '#tree' => TRUE,
            '#default_value' => $default_value,
            '#description' => t('The checked types may be translated.')
          )
        );
        break;
      case 'user':
        break;
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Fix fields and bundles')
  );
  return $form;
}

/**
 * Submit function for the above form.
 */
function scratchpads_translate_setup_translation_form_submit(&$form, &$form_state){
  $fields_to_translate = array();
  foreach($form_state['values'] as $entity_type => $values){
    if(isset($values['bundles']) && isset($values['fields']['fields'])){
      foreach($values['bundles'] as $bundle_name => $set){
        switch($entity_type){
          case 'node':
            // Only set it if we need to, as we don't want to override
            // customisation.
            if(!$set || !locale_multilingual_node_type($bundle_name)){
              variable_set('language_content_type_' . $bundle_name, $set ? 4 : 0);
              variable_set('entity_translation_hide_translation_links_' . $bundle_name, 1);
            }
            break;
          case 'taxonomy_term':
            db_update('taxonomy_vocabulary')->fields(array(
              'i18n_mode' => $set ? 1 : 0
            ))->condition('machine_name', $bundle_name)->execute();
            break;
        }
      }
    }
  }
}