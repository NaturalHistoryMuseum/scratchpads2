<?php

/**
 * silver_taxonomy_excel_form
 */
function silver_taxonomy_excel_form(){
  return array(
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
      )
    )
  );
}

/**
 * Submit for the above.
 */
function silver_taxonomy_excel_form_submit($form, &$form_state){
  echo "TAXONOMY:\n";
  print_r($form_state);
  print_r($form);
  exit();
}