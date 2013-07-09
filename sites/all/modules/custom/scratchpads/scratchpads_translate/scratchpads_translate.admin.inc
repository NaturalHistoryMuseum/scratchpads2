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
    $values = $default_value = $fields_for_bundles = $fields_options = $fields_default_value = array();
    foreach($entity_info['bundles'] as $bundle_name => $bundle_details){
      // Get all the fields for this entity/bundle, and let the user know which
      // ones may be translated, and which ones may not.
      $fields = field_info_instances($entity_type, $bundle_name);
      foreach($fields as $field){
        if(!isset($fields_options[$field['field_name']])){
          $full_field = field_info_field($field['field_name']);
          if($full_field['translatable']){
            $fields_options[$field['field_name']] = $field['label'];
            $fields_default_value[$field['field_name']] = field_is_translatable($entity_type, $full_field) ? $field['field_name'] : 0;
          }
        }
      }
      switch($entity_type){
        case 'node':
          $values[$bundle_name] = $bundle_details['label'];
          $default_value[$bundle_name] = locale_multilingual_node_type($bundle_name) ? $bundle_name : 0;
          break;
        case 'taxonomy_term':
          $values[$bundle_name] = $bundle_details['label'];
          $vocabulary = taxonomy_vocabulary_machine_name_load($bundle_name);
          $default_value[$bundle_name] = i18n_taxonomy_vocabulary_mode($vocabulary) ? $bundle_name : 0;
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
          'bundles' => array(
            '#type' => 'checkboxes',
            '#title' => t('Bundles'),
            '#options' => $values,
            '#default_value' => $default_value,
            '#description' => t('The checked types may be translated.')
          ),
          'fields' => array(
            '#type' => 'fieldset',
            '#title' => t('Fields'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            'fields' => array(
              '#type' => 'checkboxes',
              '#title' => '',
              '#options' => $fields_options,
              '#default_value' => $fields_default_value
            )
          )
        );
        break;
      case 'user':
        break;
    }
  }
  return $form;
}

/**
 * Submit function for the above form.
 */
function scratchpads_translate_setup_translation_form_submit(&$form, &$form_state){}