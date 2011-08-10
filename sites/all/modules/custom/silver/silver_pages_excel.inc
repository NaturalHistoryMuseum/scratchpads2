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
    $content_types['node_importer_' . $node_type] = $node_type_info['label'];
  }
  return array(
    'silver_import_subform_feeds' => array(
      '#weight' => -100,
      '#type' => 'select',
      '#title' => '',
      '#options' => $content_types,
      '#ajax' => array(
        'callback' => 'silver_ajax_callback',
        'wrapper' => 'silver_import_subform_feeds'
      )
    ),
    '#suffix' => '<div id="silver_import_subform_feeds"></div>'
  );
}