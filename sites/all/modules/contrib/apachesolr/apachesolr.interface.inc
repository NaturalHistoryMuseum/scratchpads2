<?php

/**
 * The interface for all 'query' objects.
 */
interface DrupalSolrQueryInterface {
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
   *   An array keyed with aliases with the real Solr index field name as value.
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

