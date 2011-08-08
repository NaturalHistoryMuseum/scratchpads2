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
      )
    )
  );
}

/**
 * Submit for the above.
function silver_taxonomy_excel_form_validate($form, &$form_state){
  // Here we check the file, and ensure that it looks like it is roughly correct
  // FIXME - No we don't.
  //echo "THERE";
  //exit();
}
 */

/**
 * Validate function for the above form.
 */
function silver_taxonomy_excel_form_submit($form, &$form_state){
  // Now we need to submit the correct form.
  $file = file_save_upload('taxonomy_excel_import_file', array(
    'file_validate_extensions' => array(
      'xls xlsx'
    )
  ));
  $form_state = array(
    'build_info' => array(
      'args' => array(
        'taxonomy_importer_' . $form_state['values']['taxonomy_vid']
      ),
      'files' => array(
        drupal_get_path('module', 'feeds') . '/feeds.pages.inc'
      )
    ),
    'values' => array(
      'feeds' => array(
        'FeedsFileFetcher' => array(
          'fid' => 0,
          'source' => $file->uri,
          'upload' => '',
          'file' => $file
        ),
        'FeedsExcelParser' => array(
          'no_headers' => 0,
          'all_worksheets' => 1
        )
      )
    ),
    'submit' => 'Import',
    'op' => 'Import',
    'submit_handlers' => array(
      'feeds_import_form_submit'
    )
  );
  module_load_include('pages.inc', 'feeds');
  drupal_form_submit('feeds_import_form', $form_state);
}