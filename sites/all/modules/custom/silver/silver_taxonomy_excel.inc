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
  $form = array(
    'taxonomy_vocabulary_select' => array(
      '#type' => 'select',
      '#title' => t('Taxonomy'),
      '#options' => $taxonomies,
      '#description' => t('Please select a vocabulary to import into.')
    )
  );
  foreach($vocabularies as $vocabulary){
    $form['import_subform_fieldset_' . $vocabulary->vid] = array(
      '#type' => 'fieldset',
      '#value' => t(''),
      '#tree' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="taxonomy_vocabulary_select"]' => array(
            'value' => "{$vocabulary->vid}"
          )
        )
      ),
      'subform' => array(
        '#type' => 'subform',
        '#subform_id' => 'feeds_import_form',
        '#subform_file' => drupal_get_path('module', 'feeds') . '/feeds.pages.inc',
        '#subform_arguments' => array(
          'taxonomy_importer_' . $vocabulary->vid
        ),
        '#limit_validation_errors' => TRUE
      )
    );
  }
  return $form;
}