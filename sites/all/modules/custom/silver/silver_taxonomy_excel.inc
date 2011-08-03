<?php

/**
 * silver_taxonomy_excel_form
 */
function silver_taxonomy_excel_form(){
  return array(
    'taxonomy_excel_import_file' => array(
      '#type' => 'file',
      '#title' => t('Excel file'),
      '#description' => t('Template files for the import can be <a href="!url">downloaded</a>.', array('!url'=> url()))
    ),
    'submit' => array(
      '#type' => 'submit',
      '#title' => t('Import'),
      '#value' => t('Import'),
      '#states' => array(
        'invisible' => array(
          '#edit-taxonomy-excel-import-file' => array(
            'value' => ''
          )
        )
      ),
      '#submit' => array('silver_taxonomy_excel_form_submit')
    )
  );
}