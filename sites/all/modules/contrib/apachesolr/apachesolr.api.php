<?php
/**
 * @file
 *   Exposed Hooks in 7.x:
 */

/**
 * Prepare the query by adding parameters, sorts, etc.
 *
 * This hook is invoked before the query is cached. The cached query is used
 * after the search such as for building facet and sort blocks, so parameters
 * added during this hook may be visible to end users.
 *
 * This is otherwise the same as HOOK_apachesolr_query_alter(), but runs before
 * it.
 *
 * @param object $query
 *  An object implementing DrupalSolrQueryInterface. No need for &.
 */
function hook_apachesolr_query_prepare($query) {
  // Add a sort on the node ID.
  $query->setAvailableSort('entity_id', array(
    'title' => t('Node ID'),
    'default' => 'asc',
  ));
}

/**
 * Alter the query after it's prepared and cached.
 *
 * Any module performing a search should call
 * drupal_alter('apachesolr_query', $query). That function then invokes this
 * hook. It allows modules to modify the query object and its parameters.
 *
 * A module implementing HOOK_apachesolr_query_alter() may set
 * $query->abort_search to TRUE to flag the query to be aborted.
 *
 * @param object $query
 *   An object implementing DrupalSolrQueryInterface. No need for &.
 */
function hook_apachesolr_query_alter($query) {
  // I only want to see articles by the admin!
  $query->addFilter("is_uid", 1);

  // Only search titles.
  $query->replaceParam('qf', 'label');
}

/**
 * Assigns a readable name to your custom solr field
 *
 * @param array $map
 */
function hook_apachesolr_field_name_map_alter(&$map) {
  $map['xs_node'] = t('The full node object');
}

/**
 * Alter hook for apachesolr_field_mappings().
 *
 * Add index mappings for Field API types. The default mappings array
 * handles just list fields and taxonomy term reference fields, such as:
 *
 * $mappings['list_text'] = array(
 *   'display_callback' => 'apachesolr_fields_list_display_callback',
 *   'indexing_callback' => 'apachesolr_fields_list_indexing_callback',
 *   'index_type' => 'string',
 *   'facets' => TRUE,
 * ),
 *
 * In your implementation you can add additional field types such
 * as:
 *
 * $mappings['number_integer']['number'] = array('indexing_callback' => '', 'index_type' => 'integer', 'facets' => TRUE);
 *
 * You can also add a mapping for a specific field. This will take precedence
 * over any mapping for a general field type. A field-specific mapping would
 * look like:
 *
 * $mappings['per-field']['field_model_name'] = array('indexing_callback' => '', 'index_type' => 'string', 'facets' => TRUE);
 *
 * or:
 *
 * $mappings['per-field']['field_model_price'] = array('indexing_callback' => '', 'index_type' => 'float', 'facets' => TRUE);
 *
 * If a custom field needs to be searchable but does not need to be faceted you
 * can change the 'facets' parameter to FALSE, like:
 *
 * $mappings['number_integer']['number'] = array('callback' => '', 'index_type' => 'integer', 'facets' => FALSE);

 * @return array $mappings
 *   An associative array of mappings as defined by modules that implement
 *   hook_apachesolr_field_mappings().
 */
function hook_apachesolr_field_mappings() {
  $mappings = array();
  $default = array(
    'indexing_callback' => 'apachesolr_date_default_indexing_callback',
    'index_type' => 'date',
    'facets' => TRUE,
    'query types' => array('date'),
    'query type' => 'date',
    'min callback' => 'apachesolr_get_min_date',
    'max callback' => 'apachesolr_get_max_date',
    'map callback' => 'facetapi_map_date',
  );

  // DATE and DATETIME fields can use the same indexing callback.
  $mappings['date'] = $default;
  $mappings['datetime'] = $default;

  // DATESTAMP fields need a different callback.
  $mappings['datestamp'] = $default;
  $mappings['datestamp']['indexing_callback'] = 'apachesolr_datestamp_default_indexing_callback';

  return $mappings;
}

/**
 * Alter hook for apachesolr_field_mappings().
 *
 * Add or alter index mappings for Field API types. The default mappings array
 * handles just list fields and taxonomy term reference fields, in the same way
 * as documented in hook_apachesolr_field_mappings.
 *
 * @param array $mappings
 *   An associative array of mappings as defined by modules that implement
 *   hook_apachesolr_field_mappings().
 */
function hook_apachesolr_field_mappings_alter(&$mappings, $entity_type) {
  // Enable indexing for text fields
  $mappings['text'] = array(
    'indexing_callback' => 'apachesolr_fields_default_indexing_callback',
    'map callback' => '',
    'index_type' => 'string',
    'facets' => TRUE,
    'facet missing allowed' => TRUE,
    'dependency plugins' => array('bundle', 'role'),
    'hierarchy callback' => FALSE,
    'name_callback' => '',
    'facet mincount allowed' => FALSE,
    // Field API allows any field to be multi-valued.
    // If we set this to false we are able to sort
    'multiple' => FALSE,
  );

  // Add our per field mapping here so we can sort on the
  // price by making it single. Solr cannot sort on multivalued fields
  // field_price is our identifier of a custom field, and it was decided to
  // index in the same way as a number_float field.
  $mappings['per-field']['field_price'] = $mappings['number_float'];
  $mappings['per-field']['field_price']['multiple'] = FALSE;
}

/**
 * Add information to index other entities
 *
 * @param array $entity_info
 */
function hook_apachesolr_entity_info_alter(&$entity_info) {
  $entity_info['myentity']['indexable'] = TRUE;
  $entity_info['myentity']['status callback'] = 'my_module_status_callback';
  $entity_info['myentity']['document callback'][] = 'my_module_document';
  $entity_info['myentity']['reindex callback'] = 'my_module_reindex';

  // Following values are optional
  $entity_info['myentity']['index_table'] = 'apachesolr_index_entities_myentity';
  $entity_info['myentity']['cron_check'] = 'my_module_cron_check';
  $entity_info['myentity']['apachesolr']['result callback'] = 'my_module_result_processing';
}

/**
 * Allows a module to modify the delete query.
 *
 * @param string $query
 *   Defaults to *:*
 */
function hook_apachesolr_delete_index_alter($query) {
  // use the site hash so that you only delete this site's content
  $query = 'hash:' . apachesolr_site_hash();
}

/**
 * The is invoked by apachesolr_search.module for each document returned in a
 * search. This has been introduced in 6.x-beta7 as a replacement for the call
 * to HOOK_nodeapi().
 *
 * @param object $document
 *   The ApacheSolrDocument instance.
 * @param array $extra
 * @param $query
 */
function hook_apachesolr_search_result_alter($document, &$extra, DrupalSolrQueryInterface $query) {
}

/**
 * This is invoked by apachesolr_search.module for the whole resultset returned
 * in a search.
 *
 * @param array $results
 *   The returned search results.
 */
function hook_apachesolr_process_results(&$results, DrupalSolrQueryInterface $query) {
  foreach ($results as $id => $result) {
    $results[$id]['title'] = t('[Result] !title', array('!title' => $result['title']));
  }
}

/**
 * Respond to search environment deletion.
 *
 * This hook is invoked from apachesolr_environment_delete() after the
 * environment is removed from the database.
 *
 * @param array $environment
 *   The environment object that is being deleted.
 */
function hook_apachesolr_environment_delete($environment) {
}

/**
 *
 * Modify the build array for any search output build by Apache Solr
 * This includes core and custom pages and makes it very easy to modify both
 * of them at once
 *
 * @param array $build
 * @param array $search_page
 */
function hook_apachesolr_search_page_alter(&$build, $search_page) {
  // Adds a text to the top of the page
  $info = array('#markup' => t('Add information to every search page'));
  array_unshift($build, $info);
}

/**
 * Modify the search types as found in the search pages administration
 *
 * @param array $search_types
 */
function hook_apachesolr_search_types_alter(&$search_types) {
  $search_types['ss_language'] = array(
    'name' => apachesolr_field_name_map('ss_language'),
    'default menu' => 'search/language/%',
    'title callback' => 'custom_title_callback',
  );
}

/**
 * Build the documents before sending them to Solr.
 *
 * @param integer $document_id
 * @param array $entity
 * @param string $entity_type
 */
function hook_apachesolr_index_document_build(ApacheSolrDocument $document, $entity, $entity_type, $env_id) {

}

/**
 * Build the documents before sending them to Solr.
 *
 * Supports all types of
 * hook_apachesolr_index_document_build_' . $entity_type($documents[$id], $entity, $env_id);
 *
 * @param $document
 * @param $entity
 * @param $entity_type
 */
function hook_apachesolr_index_document_build_node(ApacheSolrDocument $document, $entity, $env_id) {

}

/**
 * Alter the prepared documents from one entity before sending them to Solr.
 *
 * @param $documents
 *   Array of ApacheSolrDocument objects.
 * @param $entity
 * @param $entity_type
 * @param string $env_id
 */
function hook_apachesolr_index_documents_alter(array &$documents, $entity, $entity_type, $env_id) {

}