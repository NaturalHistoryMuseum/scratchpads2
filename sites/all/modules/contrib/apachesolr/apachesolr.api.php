<?php
/**
 * @file
 *   Exposed Hooks in 7.x:
 */


/**
 * Add index mappings for Field API types. The default mappings array
 * handles just list fields and taxonomy term reference fields, such as:
 *
 * $mappings['list_text'] = array(
 *   'indexing_callback' => 'apachesolr_fields_list_indexing_callback',
 *   'index_type' => 'string',
 *   'map callback' => 'apachesolr_fields_list_display_callback',
 *   'facets' => TRUE,
 * ),
 *
 * In your implementation you can add additional field types such as:
 * $mappings['number_integer']['number'] = array(...);
 *
 * You can also add mapping for a specific field. This will take precedence
 * over any mapping for a general field type. A field-specific mapping would
 * looks like:
 * $mappings['per-field']['field_model_name'] = array(...);
 *
 * Much more information can be found below in the example implementation or in
 * facetapi.api.php. If you feel restricted with the options as set below
 * there is nothing that stops you from implementing facetapi directly. However
 * it is recommended to not directly talk to solr fields since this could break
 * in the future.
 *
 * @return array $mappings
 *   An associative array of mappings as defined by modules that implement
 *   hook_apachesolr_field_mappings().
 */
function hook_apachesolr_field_mappings() {
  $mappings = array(
    // Example for a field API type. See extensive documentation below
    'number_float' => array(
      'indexing_callback' => 'apachesolr_fields_default_indexing_callback',
      'index_type' => 'tfloat',
      'facets' => TRUE,
      'query types' => array('term', 'numeric_range'),
      'query type' => 'term',
      'facet mincount allowed' => TRUE,
    ),
    // Example for a field API field
    'per-field' => array(
      // machine name of the field in Field API
      'field_price' => array(
        // REQUIRED FIELDS //
        // Function callback to return the value that will be put in to
        // the solr index
        'indexing_callback' => 'apachesolr_fields_default_indexing_callback',

        // NON REQUIRED FIELDS //
        // See apachesolr_index_key() for the correct type. Defaults string
        'index_type' => 'string',
        // How to display the values when they return as a facet
        'map callback' => 'apachesolr_fields_list_facet_map_callback',
        // Does your facet have a dynamic name? Add function call here and will
        // have the name of the return value
        'name callback' => FALSE,
        // If a custom field needs to be searchable but does not need to be faceted you
        // can change the 'facets' parameter to FALSE.
        'facets' => FALSE,
        // Do you want to allow items without value
        'facet missing allowed' => FALSE,
        // (optional)  Whether or not the facet supports the
        //    "minimum facet count" setting. Defaults to TRUE.
        'facet mincount allowed' => FALSE,
        // Field API allows any field to be multi-valued.
        // If we set this to false we are able to sort
        'dependency plugins' => array('bundle', 'role'),
        // Does your solr index has a hierarchy?
        // See facetapi_get_taxonomy_hierarchy for details or
        // view the mapping of taxonomy_term_reference
        'hierarchy callback' => FALSE,
        // There are different query types to return information from Solr
        // term : Regular strings
        // date : Everything regarding dates
        // numeric_range : Useful when you have widgets that depend
        //   on statistics coming from Solr
        'query types' => array('term', 'numeric_range'),
        // Backwards compatible with previous facetapi versions.
        // Pick the main query type
        'query type' => 'term',
        // What dependencies do you have (see facetapi)
        'multiple' => TRUE,
      ),
    ),
  );
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
    'name callback' => '',
    'facet mincount allowed' => FALSE,
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
 * Assigns a readable name to your custom solr field
 *
 * @param array $map
 */
function hook_apachesolr_field_name_map_alter(&$map) {
  $map['xs_node'] = t('The full node object');
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
 * Add information to index other entities.
 * There are some modules in http://drupal.org that can give a good example of
 * custom entity indexing such as apachesolr_user_indexer, apachesolr_term
 * @param array $entity_info
 */
function hook_apachesolr_entity_info_alter(&$entity_info) {
  // REQUIRED VALUES
  // myentity should be replaced with user/node/custom entity
  $entity_info['node'] = array();
  // Set this entity as indexable
  $entity_info['node']['indexable'] = TRUE;
  // Validate each entity if it can be indexed or not
  $entity_info['node']['status callback'] = 'apachesolr_index_node_status_callback';
  // Build up a custom document.
  $entity_info['node']['document callback'][] = 'apachesolr_index_node_solr_document';
  // What to do when a reindex is issued. Most probably this will reset all the
  // items in the index_table
  $entity_info['node']['reindex callback'] = 'apachesolr_index_node_solr_reindex';

  // OPTIONAL VALUES
  // Index in a separate table? Useful for huge datasets.
  $entity_info['node']['index_table'] = 'apachesolr_index_entities_node';
  // Execute custom callback on each cron run.
  // See apachesolr_index_node_check_table
  $entity_info['node']['cron_check'] = 'apachesolr_index_node_check_table';
  // Specific output processing for the results
  $entity_info['node']['apachesolr']['result callback'] = 'apachesolr_search_node_result';
}


/**
 * The is invoked by apachesolr_search.module for each document returned in a
 * search. This has been introduced in 6.x-beta7 as a replacement for the call
 * to HOOK_nodeapi().
 *
 * @param object $document
 *   The ApacheSolrDocument instance.
 * @param array $extra
 * @param array $query
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
 * The function is the follow-up for apachesolr_update_index
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
 * The function is the follow-up for apachesolr_update_index but then for
 * specific entity types
 *
 * @param $document
 * @param $entity
 * @param $entity_type
 */
function hook_apachesolr_index_document_build_ENTITY_TYPE(ApacheSolrDocument $document, $entity, $env_id) {
  // Index book module data.
  if (!empty($entity->book['bid'])) {
    // Hard-coded - must change if apachesolr_index_key() changes.
    $document->is_book_bid = (int) $entity->book['bid'];
  }
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
  // Do whatever altering you need here
}