<?php

/**
 * silver_taxonomy_excel_form
 */
function silver_taxonomy_excel_form(){
  $vocabularies = taxonomy_get_vocabularies();
  $taxonomies = array(
    '- SELECT VOCABULARY -'
  );
  foreach($vocabularies as $vid => $vocabulary){
    $taxonomies[$vid] = $vocabulary->name;
  }
  return array(
    'taxonomy_vid' => array(
      '#type' => 'select',
      '#title' => t('Taxonomy'),
      '#options' => $taxonomies
    ),
    'taxonomy_excel_import_file' => array(
      '#type' => 'file',
      '#title' => t('Excel file'),
      '#description' => t('Template files for the import can be <a href="!url">downloaded</a>.', array(
        '!url' => url()
      ))
    ),
    'submit_silver_taxonomy_excel_form' => array(
      '#type' => 'submit',
      '#value' => t('Import'),
      '#name' => 'silver_taxonomy',
      '#states' => array(
        'invisible' => array(
          '#edit-taxonomy-excel-import-file' => array(
            'value' => ''
          )
        )
      ),
      '#submit' => array(
        'silver_taxonomy_excel_form_submit'
      ),
      '#validate' => array(
        'silver_taxonomy_excel_form_validate'      
      )
    )
  );
}

/**
 * Submit for the above.
 */
function silver_taxonomy_excel_form_submit($form, &$form_state){
  // Here we check the file, and ensure that it looks like it is roughly correct
  // FIXME - No we don't.
  
}

/**
 * Validate function for the above form.
 */
function silver_taxonomy_excel_form_validate($form, &$form_state){
  echo "HERE";exit;  
}