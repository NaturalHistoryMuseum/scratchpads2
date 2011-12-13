<?php

/**
 * @file
 *   Provides a multi-site search admin pages and functionality
 */

function apachesolr_multisitesearch_metadata() {
  $document = new ApacheSolrDocument();
  $document->id = apachesolr_document_id(0, 'multisite_metadata');
  $document->site = url(NULL, array('absolute' => TRUE));
  $document->hash = apachesolr_site_hash();
  $document->entity_id = 1;
  $document->entity_type = 'multisite_meta';
  $document->ss_multisite_meta_sitename = variable_get('site_name', 'Drupal');
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  if (module_exists('taxonomy')) {
    $vocabs = taxonomy_get_vocabularies();
    foreach ($vocabs as $vid => $vocab) {
      // We index each name as a string for cross-site faceting
      // using the vocab name rather than vid in field construction.
      $document->setMultiValue('sm_multisite_meta_taxonomy', apachesolr_vocab_name($vid));
    }
  }
  drupal_alter('apachesolr_multisitesearch_metadata', $document);
  return $document;
}

function apachesolr_multisitesearch_update_metadata() {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    $metadata_doc = apachesolr_multisitesearch_metadata();
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

function apachesolr_multisitesearch_get_metadata() {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    $params['qt'] = 'standard';
    $params['fl'] = '*';
    $response = $solr->search('entity_type:multisite_meta', $params);
    foreach ($response->response->docs as $doc) {
      // Convert doc into a simple array.
      if (isset($doc->hash)) {
        foreach ($doc as $k => $v) {
          $data[$doc->hash][$k] = $v;
        }
        if (empty($data[$doc->hash]['sm_multisite_meta_taxonomy'])) {
          $data[$doc->hash]['sm_multisite_meta_taxonomy'] = array();
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

/**
 * Creates the form that allows the user to select which facets will be enabled.
 *
 * Only enabled facets are sent to solr.  Fewer enabled facets can reduce the
 * load on the search server.  Blocks are only offered for enabled facets, so
 * this also reduces the clutter on the blocks admin page.
 */
function apachesolr_multisitesearch_settings() {
  $form = array();
  $form['#tree'] = TRUE;
  $form['submit_message'] = array(
    '#type' => 'value',
    '#value' => t('The Apache Solr Multisite Search filter settings were changed. To arrange the blocks for your enabled filters, visit the <a href="@url">blocks administration page</a>.', array('@url' => url('admin/structure/block'))),
  );
  $form['admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administrative functions'),
  );
  $form['admin']['refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh metadata now'),
    '#prefix' => '<p>' . t('Multisite metadata is used to communicate between all of the sites in a multisite setup. If site names are not showing properly in the search results and facet blocks try refreshing the metadata. Metadata is also refreshed periodically on cron runs.') . '</p>',
    '#submit' => array('apachesolr_multisitesearch_refresh_metadata_now'),
  );
  
  // Use the metadata and a list of all the hashes in the index
  // to build up checkboxes for deleting site indexes.
  // This is only necessary because sometimes hashes get
  // stranded in the index and deleting the index from the normal
  // admin screen doesn't rectify the problem.
  $metadata = variable_get('apachesolr_multisitesearch_metadata', array());
  $hashes = apachesolr_multisitesearch_get_site_hashes();
  $options = array();
  foreach ($hashes as $hash => $count) {
    if ($hash == apachesolr_site_hash()) {
      $options[$hash] = t('This site (!site, !count documents)', array('!site' => variable_get('site_name', 'Drupal'), '!count' => $count));
    }
    elseif (!empty($metadata[$hash])) {
      $options[$hash] = $metadata[$hash]['site'] . ' ' . t('(!count documents)', array('!count' => $count));
    }
    else {
      $options[$hash] = $hash . ' ' . t('(!count documents)', array('!count' => $count));
    }
  }
  
  if (count($options) > 0) {
    $form['admin']['delete']['hashes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Delete data from sites using this index'),
      '#options' => $options,
      '#description' => t('If you end up in a situation where the index has hashes that no longer map to real active sites, use this option to delete the outdated data. If you delete another site\'s index from this site, that site will have to first trigger a re-index, and then run cron until the index is rebuilt.'),
    );
    $form['admin']['delete']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete selected indexes'),
      '#submit' => array('apachesolr_multisitesearch_delete_indexes'),
    );
  }
  return $form;
}

/**
 * Submit handler for the "Delete selected indexes" button.
 */
function apachesolr_multisitesearch_delete_indexes($form, &$form_state) {
  // Instantiate a new Solr object.
  $solr = apachesolr_get_solr();
  foreach ($form_state['values']['admin']['delete']['hashes'] as $hash) {
    if ($hash) {
      $query = "hash:$hash";
      $solr->deleteByQuery($query);
      drupal_set_message(t('The index for !hash has been deleted.', array('!hash' => $hash)));
      if (apachesolr_site_hash() == $hash) {
        // Rebuild our node-tracking table.
        apachesolr_rebuild_index_table();
        apachesolr_index_set_last_updated(time());
      }
    }
  }
  $solr->commit();
  apachesolr_multisitesearch_get_metadata();
}

/**
 * Submit handler for the "Refresh metadata now" button.
 */
function apachesolr_multisitesearch_refresh_metadata_now() {
  variable_del('apachesolr_multisitesearch_last_metadata_update');
  variable_del('apachesolr_multisitesearch_last_metadata_fetch');
  apachesolr_multisitesearch_refresh_metadata();
  drupal_set_message(t('The metadata has been refreshed.'));
}
