<?php

/**
 * silver_taxonomy_excel_form
 */
function silver_taxonomy_excel_form(){
  $vocabularies = taxonomy_get_vocabularies();
  $taxonomies = array(
    '- SELECT VOCABULARY -'
  );
  foreach($vocabularies as $vocabulary){
    $taxonomies['taxonomy_importer_'.$vocabulary->machine_name] = $vocabulary->name;
  }
  return array(
    'silver_import_subform_feeds' => array(
      '#type' => 'select',
      '#title' => '',
      '#options' => $taxonomies,
      '#ajax' => array(
        'callback' => 'silver_ajax_callback',
        'wrapper' => 'silver_import_subform_feeds'
      )
    ),
    '#suffix' => '<div id="silver_import_subform_feeds"></div>'
  );
}