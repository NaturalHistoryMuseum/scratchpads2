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
  module_load_include('crud.inc', 'field');
  $fields = field_read_fields();
  $links = array();
  unset($_SESSION['scratchpads_translate']);
  foreach($fields as $field){
    if(in_array('node', $field['entity_types'])){
      if(!field_is_translatable('node', $field)){
        if($field['locked']){
          // We need to unlock the field to ensure the user can edit it
          if(!isset($_SESSION['scratchpads_translate'])){
            $_SESSION['scratchpads_translate'] = array();
          }
          $_SESSION['scratchpads_translate'][] = $field['field_name'];
        }
        // Load the full proper field.
        $field = field_info_field($field['field_name']);
        $links[] = '<strong>' . $field['field_name'] . '</strong> - ' . l(t('edit'), 'admin/structure/types/manage/' . array_pop($field['bundles']['node']) . '/fields/' . $field['field_name'], array(
          'query' => array(
            'destination' => 'admin/config/regional/translate-setup'
          )
        ));
      }
    }else if(in_array('taxonomy_term', $field['entity_types'])){
      if(!field_is_translatable('taxonomy_term', $field)){
        if($field->locked){
          // We need to unlock the field to ensure the user can edit it
          if(!isset($_SESSION['scratchpads_translate'])){
            $_SESSION['scratchpads_translate'] = array();
          }
          $_SESSION['scratchpads_translate'][] = $field['field_name'];
        }
        // Load the full proper field.
        $field = field_info_field($field['field_name']);
        $links[] = l(t('edit'), 'admin/structure/taxonomy/' . array_pop($field['bundles']['taxonomy_term']) . '/fields/' . $field['field_name'], array(
          'query' => array(
            'destination' => 'admin/config/regional/translate-setup'
          )
        ));
      }
    }else if(in_array('user', $field['entity_types'])){
    }
  }
  foreach(node_type_get_types() as $bundle => $info){
    if(!title_field_replacement_enabled('node', $bundle, 'title')){
      $links[] = '<strong>' . check_plain($info->name) . ' - title</strong> - ' . l(t('edit'), 'admin/structure/types/manage/' . $bundle . '/fields/replace/title', array(
        'query' => array(
          'destination' => 'admin/config/regional/translate-setup'
        )
      ));
    }
  }
  if(count($links)){
    $form['links'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fields not set as translatable'),
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
      'links' => array(
        '#markup' => theme('item_list', array(
          'items' => $links
        ))
      )
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enable translation of checked node types and taxonomy vocabularies')
  );
  return $form;
}

/**
 * Submit function for the above form.
 */
function scratchpads_translate_setup_translation_form_submit(&$form, &$form_state){
  $fields_to_translate = array();
  foreach($form_state['values'] as $entity_type => $values){
    if(isset($values['bundles']) && is_array($values['bundles'])){
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