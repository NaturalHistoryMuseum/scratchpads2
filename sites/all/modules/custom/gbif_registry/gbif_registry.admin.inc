<?php

/**
 * Return the form which allows us to set the Registry to use.
 */
function gbif_registry_admin_form(){
  return system_settings_form(array(
    'gbif_registry_web_service_url' => array(
      '#type' => 'textfield',
      '#title' => t('GBIF Registry web service URL'),
      '#default_value' => variable_get('gbif_registry_web_service_url', 'http://gbrds.gbif.org/registry/')
    )
  ));
}

/**
 * Validate the data sent from the settings form.
 */
function gbif_registry_admin_form_validate(&$form, &$form_state){
  // We check that the Registry URL ends with a slash.
  if(substr($form_state['values']['gbif_registry_web_service_url'], -1) != '/'){
    form_set_error('gbif_registry_web_service_url', t('The web service URL must end with a "/".'));
  }
}

/**
 * Display the test page.
 */
function gbif_registry_test_page(){
  // We need to register our own autoloader, as it appears the Drupal one does
  // not support namespaces (or I've screwed up with my configuration).
  $registry = GBIFRegistry::singleton();
  $nodes = array();
  foreach($registry->getOrganizations() as $organization){
    $nodes[] = array(
      'title' => $organization->getTitle(),
      'description' => l($organization->getHomepage(),$organization->getHomepage())
    );
  }
  return theme('table', array(
    'header' => array(
      t('Title'),
      t('Homepage')
    ),
    'rows' => $nodes
  ));
}