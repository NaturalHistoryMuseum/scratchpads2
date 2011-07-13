<?php
/* 
 */


/**
 * @file declare how to load and import a SONZ (Services of New Zealand)
 * Taxonomy
 */
///////////// TODO
/**
 *
 */ 
function lsid_taxonomy_service_info() {
  $services = array();
  $services['lsid'] = array(
    'provider' => 'Biodiversity Information Standards',
    'name' => 'Life Science Identifier search',
    'id' => 'lsid',
    'description' => 'Biological taxonomy classifications (disabled).',
    'about' => 'http://lsid.tdwg.org/',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'lsid_taxonomy_service_form',
    'import_form_submit' => 'lsid_taxonomy_service_form_submit',
    // Internal use
    'servicetype' => 'lookup',
    'protocol' => 'URI',
    'pattern' => 'http://lsid.tdwg.org/!guid',
    'format' => 'lsid',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function lsid_taxonomy_service_form() {
  $form = array();

  $form['terms'] = array(
    '#type' => 'markup',
    '#markup' => t('
      To use this resource, you only have to agree to the terms and conditions.
    '),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Retrieve'),
    '#submit' => array('sonz_taxonomy_service_form_submit'),
  );

  return $form;
}

/**
 * What to do when loading from this service
 */
function lsid_taxonomy_service_form_submit($form, &$form_state) {
dpm('sonz_taxonomy_service_form_submit');
  // Invoke service and parse response
  $service_id = $form_state['values']['service_id'];
  $services = sonz_taxonomy_service_info();
  $service = $services[$service_id];
  taxonomy_xml_invoke_service_request($service, $form_state['values']);
}


/**
 * Helper function to check if a given string looks like an LSID.
 * If so, it returns it in an array of componant bits.
 * If not, returns NULL.
 *
 * LSID is a "Life Sciences Identifier" GUID used to identify Taxonomic
 * concepts.
 * It's not great, and it's probably not going to carry on living, but it's out
 * there in metadata and web services.
 *
 * For convenience, it also returns a namespaced 'type' so we can quickly see
 * what 'type' of resource the LSID is referring to.
 * eg an LSID starting with 'urn:lsid:ubio.org:classificationbank' is the type
 * of entity that Drupal will call a term, and TCS would call a TaxonConcept.
 */
function taxonomy_xml_parse_lsid($id) {
  $bits = split(":", $id);
  if (count($bits) < 5) {
    return NULL;
  }
  $lsid = array(
    'urn' => $bits[0],
    'schema' => $bits[1],
    'authority' => $bits[2],
    'namespace' => $bits[3],
    'identifier' => $bits[4],
    'version' =>  @$bits[5], # optional
    'type' => implode(':', array($bits[0], $bits[1], $bits[2], $bits[3])),
  );
  if ((count($bits) > 4) && $lsid['urn'] == 'urn' && $lsid['schema'] == 'lsid' ) {
    return $lsid;
  }
  return NULL;
}
