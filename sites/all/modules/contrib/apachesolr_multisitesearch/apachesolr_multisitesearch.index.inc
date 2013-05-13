<?php

function apachesolr_multisitesearch_refresh_metadata() {
  // Update meta data 1x per hour max.
  // Fetch meta data 1x per 5 minutes max.
  // TODO - make these intervals controllable.
  $last_update = variable_get('apachesolr_multisitesearch_last_metadata_update', 0);
  $last_fetch = variable_get('apachesolr_multisitesearch_last_metadata_fetch', 0);
  $time = time();
  if ($time - $last_update > 60*60) {
    if (apachesolr_multisitesearch_update_metadata()) {
      variable_set('apachesolr_multisitesearch_last_metadata_update', $time);
    }
  }
  if ($time - $last_fetch > 60*5) {
    apachesolr_multisitesearch_retrieve_metadata();
  }
  $env_id = apachesolr_default_environment();
  apachesolr_set_last_index_updated($env_id, $time);
}

/**
 * Create a document to send to the index that contains useful information
 * regarding the website.
 * @return ApacheSolrDocument
 */
function apachesolr_multisitesearch_generate_metadata() {
  $document = new ApacheSolrDocument();
  $document->id = apachesolr_document_id(1, 'multisite.metadata');
  $document->site = url(NULL, array('absolute' => TRUE));
  $document->hash = apachesolr_site_hash();
  $document->entity_id = 1;
  $document->entity_type = 'multisite.meta';
  $document->ss_multisite_meta_sitename = variable_get('site_name', 'Drupal');

  // Only refresh for the default environment
  $env_id = apachesolr_default_environment();

  // Add bundle information to our metadata
  foreach (entity_get_info() as $entity_type => $info) {
    // We index each name as a string for cross-site faceting
    foreach ($info['bundles'] as $bundle => $bundle_info) {
      if (!empty($bundle_info['apachesolr']['index'])) {
        $document->setMultiValue('sm_multisite_meta_bundles', $bundle_info['label']);
      }
    }
  }

  // Add taxonomy to our metadata
  if (module_exists('taxonomy')) {
    module_load_include('inc', 'apachesolr', 'apachesolr.index');
    $vocabs = taxonomy_get_vocabularies();
    foreach ($vocabs as $vid => $vocab) {
      // We index each name as a string for cross-site faceting
      // using the vocab name rather than vid in field construction.
      $document->setMultiValue('sm_multisite_meta_taxonomy', apachesolr_vocab_name($vid));
    }
  }
  // Allow alterations to this meta data
  drupal_alter('apachesolr_multisitesearch_metadata', $document);
  return $document;
}

function apachesolr_multisitesearch_retrieve_metadata() {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    $params['qt'] = 'standard';
    $params['fl'] = '*';
    $response = $solr->search('entity_type:multisite.meta', $params);
    foreach ($response->response->docs as $doc) {
      // Convert doc into a simple array.
      if (isset($doc->hash)) {
        foreach ($doc as $k => $v) {
          $data[$doc->hash][$k] = $v;
        }
        if (empty($data[$doc->hash]['sm_multisite_meta_taxonomy'])) {
          $data[$doc->hash]['sm_multisite_meta_taxonomy'] = array();
        }
        if (empty($data[$doc->hash]['sm_multisite_meta_bundles'])) {
          $data[$doc->hash]['sm_multisite_meta_bundles'] = array();
        }
      }
    }
    watchdog('Apache Solr Multisite', 'Fetched site meta data');
    variable_set('apachesolr_multisitesearch_metadata', $data);
  }
  catch (Exception $e) {
    watchdog('Apache Solr Multisite', 'Failed to fetch meta data <br /> !message',
      array('!message' => nl2br(strip_tags($e->getMessage()))), WATCHDOG_ERROR);
  }
}

/**
 * Creation of update of the metadata
 * @return type
 */
function apachesolr_multisitesearch_update_metadata() {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    $metadata_doc = apachesolr_multisitesearch_generate_metadata();
    $solr->addDocuments(array($metadata_doc));
    $solr->commit();
    watchdog('Apache Solr Multisite', 'Updated site meta data');
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr Multisite', 'Indexing failed for meta data <br /> !message',
             array('!message' => nl2br(strip_tags($e->getMessage()))), WATCHDOG_ERROR);
  }
  return FALSE;
}

function apachesolr_multisitesearch_delete_metadata($hash = NULL) {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    if (empty($hash)) {
      $query = "entity_type:multisite.meta";
    }
    else {
      $query = "hash:$hash AND entity_type:multisite.meta";
    }
    $solr->deleteByQuery($query);
    watchdog('Apache Solr Multisite', 'Removed meta data from the index');
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr Multisite', 'Removing meta data failed. <br /> !message',
      array('!message' => nl2br(strip_tags($e->getMessage()))), WATCHDOG_ERROR);
  }
  return FALSE;
}

function apachesolr_multisitesearch_get_site_hashes() {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    $params['qt'] = 'standard';
    $params['fl'] = '*';
    $params['facet'] = 'true';
    $params['facet.field'][] = 'hash';
    $params['facet.mincount'] = 1;
    $params['facet.limit'] = '1000';
    $response = $solr->search('*:*', $params);
    $results = (array)$response->facet_counts->facet_fields->hash;
    return $results;
  }
  catch (Exception $e) {
    watchdog('Apache Solr Multisite', 'Failed to fetch hash facet count <br /> !message',
             array('!message' => nl2br(strip_tags($e->getMessage()))), WATCHDOG_ERROR);
  }
}
