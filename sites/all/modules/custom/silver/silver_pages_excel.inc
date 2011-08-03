<?php

/**
 * silver_pages_excel_form
 */
function silver_pages_excel_form(){
  return array(
    'pages_excel_import_file' => array(
      '#type' => 'file',
      '#title' => t('Excel file')
    ),
    'submit' => array(
      '#type' => 'submit',
      '#title' => t('Import'),
      '#value' => t('Import'),
      '#states' => array(
        'invisible' => array(
          '#edit-pages-excel-import-file' => array(
            'value' => ''
          )
        )
      )
    )
  );
}