<?php

/**
 * @file
 *   Administrative pages for the Apache Solr Multi-Site Search framework.
 */

/**
 * Creates the form that allows the user to select which facets will be enabled.
 *
 * Only enabled facets are sent to solr.  Fewer enabled facets can reduce the
 * load on the search server.  Blocks are only offered for enabled facets, so
 * this also reduces the clutter on the blocks admin page.
 */
function apachesolr_multisitesearch_settings() {
  module_load_include('inc', 'apachesolr_multisitesearch', 'apachesolr_multisitesearch.index');
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

  if (count(apachesolr_multisitesearch_query_bundles()) > 0) {
    // Grab the bundle names from the metadata and load them as options.
    $query_exclusion_options = apachesolr_multisitesearch_query_bundles();
    // Grab the excluded item keys from the variables.
    $excluded_bundles = variable_get('apachesolr_multisitesearch_query_exclusions', array());

    $form['query_exclusions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search Result Exclusions'),
    );
    $form['query_exclusions']['exclusion_options'] = array(
      '#type' => 'select',
      '#prefix' => t('<p>Select content should which should be excluded from the search query. Hold the CTRL key to select more than one item.</p><div style="apache-multisitesearch-search-exclusion">'),
      '#title' => t('Content Types'),
      '#description' => t('Content types to exclude from search. Multiple selections are allowed.'),
      '#multiple' => TRUE,
      '#default_value' => $excluded_bundles ? $excluded_bundles : '',
      '#options' => $query_exclusion_options,
      '#suffix' => '</div>',
    );
    $form['query_exclusions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Exclude content'),
      '#submit' => array('apachesolr_multisitesearch_exclude_indexes'),
    );
    $form['query_exclusions']['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear all exclusions'),
      '#submit' => array('apachesolr_multisitesearch_exclude_indexes'),
    );
  }

  return $form;
}

/**
 * Submit handler for the "Exclude content types" button.
 */
function apachesolr_multisitesearch_exclude_indexes($form, &$form_state) {
  $fv = $form_state['values'];
  if ($fv['op'] == t('Clear all exclusions')) {
    variable_del('apachesolr_multisitesearch_query_exclusions');
    drupal_set_message('All content exclusions have been cleared.');
  }
  elseif (isset($fv['query_exclusions']['exclusion_options']) &&
    !empty($fv['query_exclusions']['exclusion_options'])) {
    // Set the variable to store the query filter keys and create a string
    // to display the excluded content.
    variable_set('apachesolr_multisitesearch_query_exclusions', $fv['query_exclusions']['exclusion_options']);
    $query_exclusion_options = apachesolr_multisitesearch_query_bundles();
    // Match the exclusion options' keys to their values and slap them together.
    $matches = array_intersect_key($query_exclusion_options, $fv['query_exclusions']['exclusion_options']);
    $output = implode($matches, ", ");
    drupal_set_message(t('The following content types will be excluded from search: <em>@output</em>', array('@output' => $output)));
  }
  else {
    // If nothing is selected to be excluded, delete the variable, and
    // let the user know.
    variable_del('apachesolr_multisitesearch_query_exclusions');
    drupal_set_message(t('<strong>No content</strong> will be excluded from search.'));
  }
}

/**
 * Submit handler for the "Delete selected indexes" button.
 */
function apachesolr_multisitesearch_delete_indexes($form, &$form_state) {
  module_load_include('inc', 'apachesolr_multisitesearch', 'apachesolr_multisitesearch.index');
  $fv = $form_state['values'];
  // Instantiate a new Solr object.
  $solr = apachesolr_get_solr();
  $env_id = apachesolr_default_environment();
  foreach ($fv['admin']['delete']['hashes'] as $hash) {
    if ($hash) {
      $query = "hash:$hash";
      $solr->deleteByQuery($query);
      drupal_set_message(t('The index for !hash has been deleted.', array('!hash' => $hash)));
      if (apachesolr_site_hash() == $hash) {
        //Todo : we might want to execute apachesolr_node_check_index_table();
        apachesolr_set_last_index_updated($env_id, time());
      }
    }
  }
  $solr->commit();
  apachesolr_multisitesearch_retrieve_metadata();
}

/**
 * Submit handler for the "Refresh metadata now" button.
 */
function apachesolr_multisitesearch_refresh_metadata_now() {
  module_load_include('inc', 'apachesolr_multisitesearch', 'apachesolr_multisitesearch.index');
  // Delete all current variables and refresh them
  variable_del('apachesolr_multisitesearch_last_metadata_update');
  variable_del('apachesolr_multisitesearch_last_metadata_fetch');
  apachesolr_multisitesearch_refresh_metadata();
  drupal_set_message(t('The metadata has been refreshed.'));
}
