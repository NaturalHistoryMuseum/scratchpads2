<?php

/**
 * silver_pages_excel_form
 */
function silver_pages_excel_form(){
  $content_types = array(
    '- SELECT CONTENT TYPE -'
  );
  $entity_info = entity_get_info('node');
  foreach($entity_info['bundles'] as $node_type => $node_type_info){
    $content_types[$node_type] = $node_type_info['label'];
  }
  $form = array(
    'content_type_select' => array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#options' => $content_types,
      '#description' => t('Please select a content type to import into.')
    )
  );
  foreach($entity_info['bundles'] as $node_type => $node_type_info){
    $form['import_subform_fieldset_' . $node_type] = array(
      '#type' => 'fieldset',
      '#value' => t(''),
      '#tree' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="content_type_select"]' => array(
            'value' => "$node_type"
          )
        )
      ),
      'subform' => array(
        '#type' => 'subform',
        '#subform_id' => 'feeds_import_form',
        '#subform_file' => drupal_get_path('module', 'feeds') . '/feeds.pages.inc',
        '#subform_arguments' => array(
          'node_importer_' . $node_type
        ),
        '#limit_validation_errors' => TRUE
      )
    );    
  }
  return $form;
}