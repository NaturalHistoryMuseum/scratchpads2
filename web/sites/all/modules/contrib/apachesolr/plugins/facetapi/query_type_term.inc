<?php

/**
 * @file
 * Term query type plugin for the Apache Solr adapter.
 */

/**
 * Plugin for "term" query types.
 */
class ApacheSolrFacetapiTerm extends FacetapiQueryType implements FacetapiQueryTypeInterface {

  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'term';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param DrupalSolrQueryInterface $query
   *   An object containing the query in the backend's native API.
   */
  public function execute($query) {
    $settings = $this->adapter->getFacet($this->facet)->getSettings();
    // Adds the operator parameter.
    $operator = $settings->settings['operator'];
    $ex = (FACETAPI_OPERATOR_OR != $operator) ? '' : "{!ex={$this->facet['field']}}";
    $query->addParam('facet.field', $ex . $this->facet['field']);

    if (!empty($settings->settings['facet_missing'])) {
      $query->addParam('f.' . $this->facet['field'] . '.facet.missing', 'true');
    }
    // Adds "hard limit" parameter to prevent too many return values.
    $limit = empty($settings->settings['hard_limit']) ? 20 : (int) $settings->settings['hard_limit'];
    $query->addParam('f.' . $this->facet['field'] . '.facet.limit', $limit);

    // Adds "facet mincount" parameter to limit the number of facets.
    if (isset($settings->settings['facet_mincount'])) {
      $count = $settings->settings['facet_mincount'];
      $query->addParam('f.' . $this->facet['field'] . '.facet.mincount', $count);
    }

    $active = $this->adapter->getActiveItems($this->facet);

    // Adds filter based on the operator.
    if (FACETAPI_OPERATOR_OR != $operator) {
      foreach ($active as $value => $item) {
        // Handle facet missing:
        if ($value === '_empty_' && !empty($settings->settings['facet_missing'])) {
          $query->addFilter($this->facet['field'], '[* TO *]', TRUE);
        }
        elseif (strlen($value)) {
          $query->addFilter($this->facet['field'], $value);
        }
      }
    }
    else {
      // OR facet.
      $local = "tag={$this->facet['field']}";
      $values = array_keys($active);
      if ($values) {
        // Quote any values that have white space or colons.
        foreach ($values as &$v) {
          if (preg_match('/[:\s]/', $v) || strlen($v) == 0) {
            $v = '"' . $v . '"';
          }
        }
        $query->addFilter($this->facet['field'], '(' . implode(' OR ', $values) . ')', FALSE, $local);
      }
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
      $settings = $this->adapter->getFacet($this->facet)->getSettings();
      if (isset($response->facet_counts->facet_fields->{$this->facet['field']})) {
        $values = (array) $response->facet_counts->facet_fields->{$this->facet['field']};
        foreach ($values as $value => $count) {
          // Facet missing may return 0 even if mincount is 1.
          if (empty($settings->settings['facet_mincount']) || $count) {
            $build[$value] = array('#count' => $count);
          }
        }
      }
    }
    return $build;
  }
}
