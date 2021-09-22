<?php

/**
 * silver_taxonomy_excel_form
 */
function character_editor_import_form($form, &$form_state){
  $vocabularies = taxonomy_get_vocabularies();
  foreach($vocabularies as $vocabulary){
    $taxonomies['taxonomy_importer_' . $vocabulary->machine_name] = $vocabulary->name;
  }
  $form_state['programmed'] = TRUE;
  return array(
    'character_editor_import_sdd_vid' => array(
      '#type' => 'select',
      '#title' => '',
      '#options' => $taxonomies,
      '#empty_option' => t('Select vocabulary')
    ),
    'character_editor_import_sdd_file' => array(
      '#type' => 'file',
      '#title' => t('SDD file')
    ),
    'character_editor_import_sdd_submit' => array(
      '#type' => 'submit',
      '#value' => t('Import SDD')
    )
  );
}

/**
 * Submit for the above form
 */
function character_editor_import_form_submit($form, &$form_state){
  $file = file_save_upload('character_editor_import_sdd_file', array(
    'file_validate_extensions' => array()
  ));
  // We load the file and bish bash bosh, we parse it.
  $dom = DOMDocument::load($file->uri);
  _character_editor_import_characters($dom);
}

/**
 * Helper function which imports the Characters and Character states.
 */
function _character_editor_import_characters($dom){}