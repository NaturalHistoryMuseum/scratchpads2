<?php

/**
 * @file
 * Numeric range query type plugin for the Apache Solr adapter.
 */

/**
 * Plugin for "numeric_range" query types.
 */
class ApacheSolrFacetapiNumericRange extends FacetapiQueryType implements FacetapiQueryTypeInterface {

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
    // See:  http://wiki.apache.org/solr/StatsComponent
    $query->addParam('stats', 'true');
    $query->addParam('stats.field', $this->facet['field']);
    $query->addParam('stats.facet', $this->facet['field']);
    // Range filters don't support OR operator.
    foreach ($active as $value => $item) {
      $query->addFilter($this->facet['field'], $value);
    }
  }

  /**
   * Initializes the facet's build array.
   *
   * @return array
   *   The initialized render array.
   */
  public function build() {
    $build = array();
    if ($response = apachesolr_static_response_cache($this->adapter->getSearcher())) {

      //we need an additional query for the statistics of the field
      // We can optionally specify a Solr object.
      $solr = apachesolr_get_solr();
      $query_stats = apachesolr_drupal_query('apachesolr_stats', array(), '', '', $solr);
      $query_stats->addParam('stats', 'true');
      $query_stats->addParam('stats.field', $this->facet['field']);
      $query_stats->addParam('stats.facet', $this->facet['field']);
      $response_stats = $query_stats->search();
      if ($response_stats->response) {
        $stats_minmax = $response_stats->stats->stats_fields->{$this->facet['field']};
      }

      if (isset($response->stats->stats_fields->{$this->facet['field']})) {
        $stats = (array) $response->stats->stats_fields->{$this->facet['field']};
        foreach ($stats as $key => $val) {
          $build[$this->facet['field']]['#range_' . $key] = $val;
          $build[$this->facet['field']]['#global_range_' . $key] = $stats_minmax->$key;
        }
      }
    }
    return $build;
  }
}
