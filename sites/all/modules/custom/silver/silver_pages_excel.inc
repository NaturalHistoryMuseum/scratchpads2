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
    'submit_silver_pages_excel_form' => array(
      '#type' => 'submit',
      '#value' => t('Import'),
      '#op' => 'silver_pages',
      '#states' => array(
        'invisible' => array(
          '#edit-pages-excel-import-file' => array(
            'value' => ''
          )
        )
      ),
      '#submit' => array(
        'silver_pages_excel_form_submit'
      )
    )
  );
}

/**
 * Submit for the above form.
 */
function silver_pages_excel_form_submit($form, &$form_state){
  echo "PAGES:\n";
  print_r($form_state);
  print_r($form);
  exit();
}