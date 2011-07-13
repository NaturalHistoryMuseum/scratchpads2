<?php
/* 
 */

/**
 * @file declare how to import a taxonomy from remote url.
 */

/**
 * hook_taxonomy_service_info()
 */ 
function url_taxonomy_service_info() {
  $services = array();
  $services['url'] = array(
    'provider' => 'Remote URL',
    'name' => 'The Web',
    'id' => 'url',
    'description' => '
      Download a file in one of the supported formats.
    ',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'url_taxonomy_service_form',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function url_taxonomy_service_form($form_values, $service_info) {
  $form = array(
    '#type' => 'fieldset',
  );

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to import from'),
    '#description' => t('
      Enter the URL of a file or web service containing a vocabulary definition.
      <br/>eg <em>http://www.w3.org/TR/2003/PR-owl-guide-20031215/food</em>
      <br/>The URL will be fetched requesting a content-type rdf/xml if available,
      this means sometimes you can enter URLs that look like web pages
      but we will actually retrieve the raw data.
      (On well-behaved sources that provide content-negotiation)
    '),
    '#default_value' => variable_get('taxonomy_xml_url', ''),
  );

  $formats = taxonomy_xml_formats();
  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Format of file'),
    '#default_value' => variable_get('taxonomy_xml_format', 'xml_format'),
    '#options' => $formats,
  );

  $form['format']['#description'] = t('
    The RDF parser can also do a pretty good job of importing RDFa-enhanced XHTML. 
    Try it on a Drupal7 taxonomy term page.
  ');

  return $form;
}



/**
 * What to do when loading from this service
 */
function url_taxonomy_service_form_submit($form, &$form_state) {

  $service_id = $form_state['values']['service_id'];
  $parameters = $form_state['values'];
  $parameters['format'] = $parameters[$service_id]['format'];
  $url = $parameters[$service_id]['url'];

  // Invoke service and parse response
  // Need to break it into steps, to try and keep the times manageable.

  // Prepare a batch config
  $batch_settings = array(
    'title' => t('Invoking a request for %url.', array('%url' => $url)),
    'operations' => array(),
    // The last operation will be to see if any more jobs were queued in the meantime.
    // unlimited batch recursion.
    'finished' => 'taxonomy_xml_batch_import_finished',
    'file' => drupal_get_path('module', 'taxonomy_xml') . '/taxonomy_xml.process.inc',
  );

  // Break down the steps.
  // #1, Fetch a reponse from the URL
  $batch_settings['operations'][] = array('taxonomy_xml_cached_get_contents', array($url, $parameters));

  // #2 That would have cached the response, so next time we open that file will be quicker
  $batch_settings['operations'][] = array('taxonomy_xml_invoke_import_on_url', array($url, $parameters));

  // Ensure that any pending jobs in the queue get found and done.
  $batch_settings['operations']['final'] = array('taxonomy_xml_batch_requeue_more', array());

  batch_set($batch_settings);
  watchdog('taxonomy_xml', 'Queued retrieval of remote URL. It will happen in stages to avoid timeout <a href="!url">!url</a>', array('!url' => $url), WATCHDOG_DEBUG);
}