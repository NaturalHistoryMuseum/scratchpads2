<?php
/**
 * Exposed Hooks in 7.x:
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
 * @param $query
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
 * @param $query
 *   An object implementing DrupalSolrQueryInterface. No need for &.
 */
function hook_apachesolr_query_alter($query) {
  // I only want to see articles by the admin!
  $query->addFilter("is_uid", 1);

  // Only search titles.
  $query->replaceParam('qf', 'label');
}

/**
 * Alter hook for apachesolr_field_mappings().
 *
 * Add or alter index mappings for Field API types. The default mappings array
 * handles just list fields and taxonomy term reference fields, such as:
 *
 * $mappings['list_text'] = array(
 *   'display_callback' => 'apachesolr_fields_list_display_callback',
 *   'indexing_callback' => 'apachesolr_fields_list_indexing_callback',
 *   'index_type' => 'string',
 *   'facets' => TRUE,
 * ),
 *
 * In your _alter hook implementation you can add additional field types such
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
 *
 * @param array $mappings
 *   An associative array of mappings as defined by modules that implement
 *   hook_apachesolr_field_mappings().
 */
function hook_apachesolr_field_mappings_alter(&$mappings) {
}

/**
 * Invoked by apachesolr.module when generating a list of nodes to index for a
 * given namespace.  Return an array of node types to be excluded from indexing
 * for that namespace (e.g. 'apachesolr_search'). This is used by
 * apachesolr_search module to exclude certain node types from the index.
 *
 * @param string $namespace
 *   Usually the calling module (eg. 'apachesolr_search').
 *
 * @return array
 *   An array containing node types to be excluded from indexing.
 */
function hook_apachesolr_types_exclude($namespace) {
  // Do not index any nodes of type 'Basic Page'.
  return array('page');
}

/**
 * This is invoked by apachesolr.module for each node to be added to the index.
 * If any module returns TRUE, the node is skipped for indexing. Note that nodes
 * which are already present in the index and subsequently qualify to be
 * excluded will not be removed from the index automatically. This hook can be
 * used to remove them prior to returning TRUE.
 *
 * @param object $node
 *   The node object which is being indexed.
 * @param string $namespace
 *   Usually the calling module (eg. 'apachesolr_search').
 *
 * @return bool
 *   Return TRUE to skip the indexing of the node.
 */
function hook_apachesolr_node_exclude($node, $namespace) {
  // Exclude nodes from uid 1.
  if ($node->uid == 1) {
    apachesolr_delete_node_from_index($node);
    return TRUE;
  }
}

/**
 * Allows a module to change the contents of the $document object before it is
 * sent to the Solr Server. To add a new field to the document you should
 * generally use one of the pre-defined dynamic fields. Follow the naming
 * conventions for the type of data being added based on the schema.xml file.
 *
 * @param object $document
 *   The ApacheSolrDocument instance. No need for &.
 * @param object $node
 *   The node object which is being indexed.
 * @param string $namespace
 *   Usually the calling module (eg. 'apachesolr_search').
 */
function hook_apachesolr_update_index($document, $node, $namespace) {
  // Add the full node object of 'story' nodes to the index.
  if ($node->type == 'story') {
    $document->addField('tm_node', urlencode(serialize(node_load($node->nid))));
  }
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
function hook_apachesolr_search_result_alter($document, $extra, DrupalSolrQueryInterface $query) {
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
 * @param $environment
 *   The environment object that is being deleted. No need for &.
 */
function hook_apachesolr_environment_delete($environment) {
}

/**
 * Modify the build array for any search output build by Apache Solr
 * This includes core and custom pages and makes it very easy to modify both
 * of them at once
 */
function hook_apachesolr_search_page_alter(&$build, $search_page) {
  // Adds a text to the top of the page
  $info = array('#markup' => t('Add information to every search page'));
  array_unshift($build, $info);
}
