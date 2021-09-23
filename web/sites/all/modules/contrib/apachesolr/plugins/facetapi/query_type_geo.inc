<?php

/**
 * Plugin for "apachesolr_geo" query types.
 */
class ApacheSolrFacetapiGeo extends FacetapiQueryType implements FacetapiQueryTypeInterface {
  // Center point is Denver.
  public $center_point = '39.7391667,-104.9841667';
  public $facet_options = '0.5,0.1,0.01';
  public $default_radius = 100;

  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'geo';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param DrupalSolrQueryInterface $query
   *   An object containing the query in the backend's native API.
   */
  public function execute($query) {
    // Retrieve settings of the facet.
    // We should be able to get all constants as facet options.
    $settings = $this->adapter->getFacet($this->facet)->getSettings();

    $facet_distances = explode(',', $this->facet_options);

    $active_items = $this->adapter->getActiveItems($this->facet);

    if (empty($active_items)) {
      $distance = $this->default_radius;
    }
    else {
      $active_item = array_pop($active_items);
      $distance = substr($active_item['value'], 1);
      // Add current selected distance to have possibility to unselect it.
      $facet_distances[] = 1;
    }

    // Search center point.
    $query->addParam('pt', $this->center_point);

    // Set location field name.
    $query->addParam('sfield', $this->facet['field']);
    $query->addParam('fq', '{!geofilt sfield=' . $this->facet['field'] . '}');

    // Set search radius.
    $query->addParam('d', $distance);

    // Set facets.
    foreach ($facet_distances as $facet_option) {
      $facet_distance = $distance * $facet_option;
      $query->addParam('facet.query', '{!geofilt d=' . $facet_distance . ' key=d' . $facet_distance . '}');
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
      if (isset($response->facet_counts->facet_queries)) {
        foreach ($response->facet_counts->facet_queries as $value => $count) {
          // Skip zero results values.
          if ($count > 0) {
            $build[$value] = array('#count' => $count);
          }
        }
      }
    }
    return $build;
  }
}
