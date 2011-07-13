<?php
/* 
 */

/**
 * @file declare how to load and import a SONZ (Services of New Zealand)
 * Taxonomy
 * 
 * This simple "service" example is also easily available by entering the URL
 * directly into the taxonomy_xml importer, and is provided as a basic example
 * service definition
 */

/**
 *
 */ 
function sonz_taxonomy_service_info() {
  $services = array();
  $services['sonz'] = array(
    'provider' => 'New Zealand Government Locator Service (NZGLS)',
    'name' => 'Services Of New Zealand (SONZ)',
    'id' => 'sonz',
    'description' => '
      A list of topics and classifications used to describe subjects 
      of interest to government and government agencies. 
      Subjects include terms relating to administration, national affairs, 
      education, infrastructure, law, economics and industry.
    ',
    'about' => 'http://www.e.govt.nz/standards/nzgls/thesauri',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'sonz_taxonomy_service_form',
    'import_form_submit' => 'sonz_taxonomy_service_form_submit',
    // Internal use
    'servicetype' => 'lookup',
    'protocol' => 'URI',
    'pattern' => 'http://www.e.govt.nz/standards/nzgls/thesauri/sonz150.txt/download',
    'format' => 'csv',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function sonz_taxonomy_service_form($form_values, $service_info) {
  $form = array();

  $form['terms'] = array(
    '#type' => 'markup',
    '#markup' => t('
      To use this resource, you only have to agree to the terms and conditions.
    '),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import from '. $service_info['name']),
    '#submit' => array('sonz_taxonomy_service_form_submit'),
  );

  return $form;
}

/**
 * What to do when loading from this service
 */
function sonz_taxonomy_service_form_submit($form, &$form_state) {
  // Invoke service and parse response
  $service_id = $form_state['values']['service_id'];
  $services = sonz_taxonomy_service_info();
  $service = $services[$service_id];
  module_load_include('inc', 'taxonomy_xml', 'taxonomy_xml.process');
  taxonomy_xml_invoke_service_request($service, $form_state['values']);
}