<?php

/**
 * citethispage_nodes_settings_form
 * 
 * The settings form
 */
function citethispage_nodes_settings_form($form, &$form_state){
  $types = node_type_get_names();
  foreach($types as $type_machine_name => $type_name){
    $form['citethispage_nodes_types_' . $type_machine_name] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('citethispage_nodes_types_' . $type_machine_name, 1),
      '#title' => t('Provide title, authors and date for content type %content_type', array(
        '%content_type' => $type_name
      ))
    );
  }
  return system_settings_form($form);
}