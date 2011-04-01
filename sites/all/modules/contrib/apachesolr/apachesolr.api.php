
/**
 *
 * Exposed Hooks in 7.x:
 */

/**
  * Any module performing a search should call apachesolr_modify_query($query, 'modulename').
  * That function then invokes this hook. It allows modules to modify the query object and params array.
  * $caller indicates which module is invoking the hook. A return value of TRUE from
  * apachesolr_modify_query() indicates the search should be aborted. A module implementing
  * hook_apachesolr_modify_query() may return TRUE to flag the query to be aborted.
  */
function hook_apachesolr_modify_query($query, $caller) {
  // I only want to see articles by the admin!
  $query->add_filter("is_uid", 1);
}

/**
 * The module calling apachesolr_do_query() may implement a function that is run after
 * hook_apachesolr_modify_query() and allows the caller to make final changes to the
 * query and params before the query is sent to Solr.  The function name is built
 * from the $caller parameter to apachesolr_do_query().
 */
function CALLER_finalize_query($query) {
}

/**
 *  This is pretty much the same as hook_apachesolr_modify_query() but runs earlier
 * and before the query is statically cached. It can e.g. be used to add
 * available sorts to the query.
 */
function hook_apachesolr_prepare_query($query, $caller) {
  // Add a sort on the node ID.
  $query->set_available_sort('entity_id', array(
    'title' => t('Node ID'),
    'default' => 'asc',
  ));
}

/**
 * Alter hook for apachesolr_field_mappings().
 *
     Add or alter index mappings for Field API types.  The default mappings array handles just
    list fields and taxonomy term reference fields, such as:

    $mappings['list_text'] = array(
      'display_callback' => 'apachesolr_fields_list_display_callback',
      'indexing_callback' => 'apachesolr_fields_list_indexing_callback',
      'index_type' => 'string',
      'facets' => TRUE,
    ),

    In your _alter hook implementation you can add additional field types such as:

      $mappings['number_integer']['number'] = array('indexing_callback' => '', 'index_type' => 'integer', 'facets' => TRUE);

    You can allso add a mapping for a specific field.  This will take precedence over any
    mapping for a general field type. A field-specific mapping would look like:

      $mappings['per-field']['field_model_name'] = array('indexing_callback' => '', 'index_type' => 'string', 'facets' => TRUE);

    or

      $mappings['per-field']['field_model_price'] = array('indexing_callback' => '', 'index_type' => 'float', 'facets' => TRUE);

    If a custom field needs to be searchable but does not need to be faceted you can change the 'facets'
    parameter to FALSE, like:

      $mappings['number_integer']['number'] = array('callback' => '', 'index_type' => 'integer', 'facets' => FALSE);
 */
function hook_apachesolr_field_mappings_alter(&$mappings) {

}


/**
 *  Invoked by apachesolr.module when generating a list of nodes to index for a given
 * namespace.  Return an array of node types to be excldued from indexing for that namespace
 * (e.g. 'apachesolr_search'). This is used by apachesolr_search module to exclude
 * certain node types from the index.
 */
function hook_apachesolr_types_exclude($namespace) {
}

/**
 *  This is invoked by apachesolr.module for each node to be added to the index - if any module
 * returns TRUE, the node is skipped for indexing.
 */
function hook_apachesolr_node_exclude($node, $namespace) {
}

/**
 * Allows a module to change the contents of the $document object before it is sent to the Solr Server.
 * To add a new field to the document, you should generally use one of the pre-defined dynamic fields.
 * Follow the naming conventions for the type of data being added based on the schema.xml file.
 */
function hook_apachesolr_update_index($document, $node) {
}

/**
 * The is invoked by apachesolr_search.module for each document returned in a search - new in 6.x-beta7
 * as a replacement for the call to hook_nodeapi().
 */
function hook_apachesolr_search_result_alter($doc) {
}

/**
 *   Called by the sort link block code. Allows other modules to modify, add or remove sorts.
 */
function hook_apachesolr_sort_links_alter(&$sort_links) {
}
