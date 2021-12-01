<?php

/**
 * The interface for all 'query' objects.
 */
interface DrupalSolrQueryInterface {

  /**
   * Get query name.
   */
  function getName();

  /**
   * Get query searcher name (for facetapi, views, pages, etc).
   */
  function getSearcher();

  /**
   * Get context values.
   */
  function getContext();

  /**
   * Set context value.
   */
  function addContext(array $context);

  /**
   * Returns all filters matching $name, if set; otherwise, returns all filters.
   *
   * @param string $name
   *   The facet field name to match. If NULL, all filters will be returned.
   *
   * @return array
   *   All matching filters.
   */
  function getFilters($name = NULL);

  /**
   * Tests whether a filter is already present in the query.
   *
   * @param string $name
   *   The facet field name to check.
   * @param string $value
   *   The facet value to check.
   * @param boolean $exclude
   *   Optional, defaults to FALSE, must match the filter.
   *
   * @return boolean
   *   TRUE or FALSE.
   */
  function hasFilter($name, $value, $exclude = FALSE);

  /**
   * Adds a filter to the query.
   *
   * @param string $name
   *   The facet field name.
   * @param string $value
   *   The facet field value.
   * @param boolean $exclude
   *   Set to TRUE to filter out documents matching $value.
   * @param string $local
   *   Solr LocalParams.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function addFilter($name, $value, $exclude = FALSE, $local = '');

  /**
   * Removes a filter from the query.
   *
   * @param string $name
   *   The name of the facet field to remove.
   * @param string $value
   *   The value of the facet field to remove. If NULL, all filters matching
   *   $name are removed.
   * @param boolean $exclude
   *   If $value is not NULL, only filters matching both $value and $exclude are
   *   removed. Ignored if $value is NULL.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function removeFilter($name, $value = NULL, $exclude = FALSE);

  /**
   * Returns all subqueries to the query.
   *
   * @return array
   *   All subqueries to the query.
   */
  function getFilterSubQueries();

  /**
   * Adds a subquery to the query.
   *
   * @param SolrFilterSubQuery $query
   *   The query to add to the orginal query - may have keywords or filters.
   * @param string $fq_operator
   *   The operator to use within the filter part of the subquery
   * @param string $q_operator
   *   The operator to use in joining the subquery to the main keywords. Note:
   *   this is unlikely to work with the Dismax handler when the main query is
   *   only keywords.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function addFilterSubQuery(SolrFilterSubQuery $query);

  /**
   * Removes a specific subquery.
   *
   * @param DrupalSolrQueryInterface $query
   *   The query to remove.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function removeFilterSubQuery(SolrFilterSubQuery $query);

  /**
   * Removes all subqueries.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function removeFilterSubQueries();

  /**
   * Transforms a single filter in a form suitable for use in a Solr query.
   *
   * @param array $filter
   *   A filter as an array with the keys '#name', for the facet field name,
   *   '#value', for the facet field value, '#local', for Solr LocalParams, and
       '#exclude' set to TRUE if it is an exclusion filter.
   *
   * @return string
   *   A Solr fq parameter value.
   */
  function makeFilterQuery(array $filter);

  /**
   * Gets the value of a parameter.
   *
   * @param string $name
   *   The parameter name.
   *
   * @return
   *   The value of the parameter.
   */
  function getParam($name);

  /**
   * Gets all parameters in normalized form.
   *
   * @return array
   *   All parameters as key-value pairs.
   */
  function getParams();

  /**
   * Gets parameters in a form suitable for use in a Solr query.
   *
   * @return array
   *   All parameters as key-value pairs, where values have been transformed
   *   into Solr parameter values.
   */
  function getSolrParams();

  /**
   * Adds a param to be sent when running the Solr search.
   *
   * If the param is single-valued, this will replace rather than add the value.
   *
   * @param string $name
   *   A Solr param name, e.g. 'q' or 'fl'.
   * @param $value
   *   A Solr param value: an array of values, or a string for a single value.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function addParam($name, $value);

  /**
   * Adds multiple params to be sent when running the Solr search.
   *
   * If the param is single-valued, this will replace rather than add the value.
   *
   * @param $params
   *   An array where the keys are param names, and the values may be strings or
   *   arrays of strings.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function addParams(array $params);

  /**
   * Removes all values for one Solr param.
   *
   * @param string $name
   *   A Solr param name, e.g. 'q' or 'fl'.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function removeParam($name);

  /**
   * Replaces a param to be sent when running the Solr search.
   *
   * Basically a shortcut for removeParam() plus addParam().
   *
   * @param string $name
   *   A Solr param name, e.g. 'q' or 'fl'.
   * @param $value
   *   A Solr param value: an array of values, or a string for a single value.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function replaceParam($name, $value);

  /**
   * Handles aliases for field to make nicer URLs.
   *
   * @param $field_map
   *   An array keyed with real Solr index field names with the alias as value.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function addFieldAliases($field_map);

  function getFieldAliases();

  function clearFieldAliases();

  function getAvailableSorts();

  /**
   * Adds an available sort.
   *
   * @param string $name
   *  The name of the field in the Solr index to sort on.
   * @param array $sort
   *  An array with the keys 'title', for the human name of the sort, and
   *  'default', for the default sort direction ('asc' or 'desc').
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function setAvailableSort($name, $sort);

  /**
   * Removes an available sort.
   *
   * @param string $name
   *  The name of the field in the Solr index to sort on.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function removeAvailableSort($name);

  /**
   * Gets the current sort.
   *
   * @return array
   *   The current sort as an array with the keys '#name', for the name of
   *   the field, and '#direction', for the sort direction ('asc' or 'desc').
   */
  function getSolrsort();

  /**
   * Sets the sort.
   *
   * @param string $field
   *  The name of the field in the Solr index to sort on.
   * @param string $direction
   *  'asc' or 'desc'
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
  function setSolrsort($name, $direction);

  /**
   * Returns an array representing the URL query string for the current sort.
   *
   * @return array
   *   The URL query string for the current sort.
   */
  function getSolrsortUrlQuery();

  /**
   * Returns the search path (including the search keywords).
   *
   * @param string $new_keywords
   *   Optional. When set, this string overrides the query's current keywords.
   *
   * @return string
   *   The search path.
   */
  function getPath($new_keywords = NULL);

  /**
   * Sends the search request to Solr, unless $query->abort_search is TRUE.
   *
   * @param string $keys
   *   The search keys.
   *
   * @return
   *   A stdClass response object.
   */
  function search($keys = NULL);

  /**
   * Calls a method, without arguments, on the Solr object with which the query
   * object was initialized.
   *
   * @param string $method
   *   The method to call on the Solr object.
   *
   * @return
   *   Any method return.
   */
  function solr($method);
}

/**
 * The interface for all 'Service' objects.
 */
interface DrupalApacheSolrServiceInterface {
  /**
   * Call the /admin/ping servlet, to test the connection to the server.
   *
   * @param $timeout
   *   maximum time to wait for ping in seconds, -1 for unlimited (default 2).
   * @return
   *   (float) seconds taken to ping the server, FALSE if timeout occurs.
   */
  function ping($timeout = 2);

  /**
   * Get information about the Solr Core.
   *
   * @return
   *   (string) system info encoded in json
   */
  function getSystemInfo();

  /**
   * Get just the field meta-data about the index.
   */
  function getFields($num_terms = 0);

  /**
   * Get meta-data about the index.
   */
  function getLuke($num_terms = 0);

  /**
   * Get information about the Solr Core.
   *
   * Returns a Simple XMl document
   */
  function getStats();

  /**
   * Get summary information about the Solr Core.
   */
  function getStatsSummary();

  /**
   * Clear cached Solr data.
   */
  function clearCache();

  /**
   * Constructor
   *
   * @param $url
   *   The URL to the Solr server, possibly including a core name.  E.g. http://localhost:8983/solr/
   *   or https://search.example.com/solr/core99/
   * @param $env_id
   *   The machine name of a corresponding saved configuration used for loading
   *   data like which facets are enabled.
   */
  function __construct($url, $env_id = NULL);

  function getId();

  /**
   * Make a request to a servlet (a path) that's not a standard path.
   *
   * @param string $servlet
   *   A path to be added to the base Solr path. e.g. 'extract/tika'
   *
   * @param array $params
   *   Any request parameters when constructing the URL.
   *
   * @param array $options
   *  @see drupal_http_request() $options.
   *
   * @return
   *  response object
   *
   * @thows Exception
   */
  function makeServletRequest($servlet, $params = array(), $options = array());

  /**
   * Get the Solr url
   *
   * @return string
   */
  function getUrl();

  /**
   * Set the Solr url.
   *
   * @param $url
   *
   * @return $this
   */
  function setUrl($url);

  /**
   * Raw update Method. Takes a raw post body and sends it to the update service. Post body
   * should be a complete and well formed xml document.
   *
   * @param string $rawPost
   * @param float $timeout Maximum expected duration (in seconds)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function update($rawPost, $timeout = FALSE);

  /**
   * Add an array of Solr Documents to the index all at once
   *
   * @param array $documents Should be an array of ApacheSolrDocument instances
   * @param boolean $allowDups
   * @param boolean $overwritePending
   * @param boolean $overwriteCommitted
   *
   * @return response objecte
   *
   * @throws Exception If an error occurs during the service call
   */
  function addDocuments($documents, $overwrite = NULL, $commitWithin = NULL);

  /**
   * Send a commit command.  Will be synchronous unless both wait parameters are set to false.
   *
   * @param boolean $optimize Defaults to true
   * @param boolean $waitFlush Defaults to true
   * @param boolean $waitSearcher Defaults to true
   * @param float $timeout Maximum expected duration (in seconds) of the commit operation on the server (otherwise, will throw a communication exception). Defaults to 1 hour
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function commit($optimize = TRUE, $waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600);

  /**
   * Create a delete document based on document ID
   *
   * @param string $id Expected to be utf-8 encoded
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function deleteById($id, $timeout = 3600);

  /**
   * Create and post a delete document based on multiple document IDs.
   *
   * @param array $ids Expected to be utf-8 encoded strings
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function deleteByMultipleIds($ids, $timeout = 3600);

  /**
   * Create a delete document based on a query and submit it
   *
   * @param string $rawQuery Expected to be utf-8 encoded
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   * @return stdClass response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function deleteByQuery($rawQuery, $timeout = 3600);

  /**
   * Send an optimize command.  Will be synchronous unless both wait parameters are set
   * to false.
   *
   * @param boolean $waitFlush
   * @param boolean $waitSearcher
   * @param float $timeout Maximum expected duration of the commit operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function optimize($waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600);

  /**
   * Simple Search interface
   *
   * @param string $query The raw query string
   * @param array $params key / value pairs for other query parameters (see Solr documentation), use arrays for parameter keys used more than once (e.g. facet.field)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function search($query = '', array $params = array(), $method = 'GET');

  /**
   * Get the current solr version. This could be 1, 3 or 4
   *
   * @return int
   *   1, 3 or 4. Does not give a more details version, for that you need
   *   to get the system info.
   */
  function getSolrVersion();

}
