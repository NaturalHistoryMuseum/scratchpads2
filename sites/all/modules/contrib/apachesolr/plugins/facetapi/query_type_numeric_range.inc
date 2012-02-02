<?php

/**
 * @file
 * Numeric range query type plugin for the Apache Solr adapter.
 */

/**
 * Plugin for "numeric_range" query types.
 */
class ApacheSolrFacetapiNumericRange extends FacetapiQueryType implements FacetapiQueryTypeInterface {

  private $single_key;
  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'numeric_range';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param DrupalSolrQueryInterface $query
   *   An object containing the query in the backend's native API.
   * @todo Cache the values based on the filter query or any other way?
   */
  public function execute($query) {
    // Check if we have a cache of this field
    //
    $settings = $this->adapter->getFacet($this->facet)->getSettings();
    $active = $this->adapter->getActiveItems($this->facet);

    $singular_field_info = $this->facet['map options'];
    $singular_field_info['multiple'] = FALSE;
    $this->single_key = apachesolr_index_key($singular_field_info);
    // See:  http://wiki.apache.org/solr/StatsComponent
    $query->addParam('stats', 'true');
    $query->addParam('stats.field', $this->single_key);
    $query->addParam('stats.facet', $this->single_key);
    // Range filters don't support OR operator.
    foreach ($active as $value => $item) {
      $query->addFilter($this->single_key, $value);
    }
  }

  /**
   * Initializes the facet's build array.
   *
   * Any calls to this method need to be wrapped in a try-catch block.
   *
   * @return array
   *   The initialized render array.
   */
  public function build() {
    $build = array();
    // Per key we save our statistics result
    $cache = cache_get('stats_' . $this->single_key, 'cache_apachesolr');
    $stats_minmax = array();

    if (!isset($cache->data)) {
      // we need an additional query for the statistics of the field
      // We can optionally specify a Solr object.
      $solr = apachesolr_get_solr();

      // We possibly need some caching for this query
      $query_stats = apachesolr_drupal_query('apachesolr_stats', array(), '', '', $solr);
      $query_stats->addParam('stats', 'true');
      $query_stats->addParam('stats.field', $this->single_key);
      $query_stats->addParam('stats.facet', $this->single_key);
      $response_stats = $query_stats->search();

      if ($response_stats->response) {
        $stats_minmax = $response_stats->stats->stats_fields->{$this->single_key};
        cache_set('stats_' . $this->single_key, $stats_minmax, 'cache_apachesolr');
      }
    }
    else {
      // Set our statistics from the cache
      $stats_minmax = $cache->data;
    }

    if ($response = apachesolr_static_response_cache($this->adapter->getSearcher())) {
      if (isset($response->stats->stats_fields->{$this->single_key})) {
        $stats = (array) $response->stats->stats_fields->{$this->single_key};
        foreach ($stats as $key => $val) {
          $build[$this->facet['field']]['#range_' . $key] = $val;
          $build[$this->facet['field']]['#global_range_' . $key] = $stats_minmax->$key;
        }
      }
    }
    return $build;
  }
}
