<?php

/**
 * @file
 * Date query type plugin for the Apache Solr adapter.
 */

/**
 * Plugin for "date" query types.
 */
class ApacheSolrFacetapiDate extends FacetapiQueryTypeDate implements FacetapiQueryTypeInterface {

  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'date';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param DrupalSolrQueryInterface $query
   *   An object containing the query in the backend's native API.
   */
  public function execute($query) {
    // Gets the data range in formats that Solr understands.
    $date_range = $this->getDateRange($query);
    if (empty($date_range)) {
      return NULL;
    }
    list($start, $end, $gap) = $date_range;
    $query->addParam('facet.date', $this->facet['field']);
    $query->addParam('f.' . $this->facet['field'] . '.facet.date.start', $start);
    $query->addParam('f.' . $this->facet['field'] . '.facet.date.end', $end);
    $query->addParam('f.' . $this->facet['field'] . '.facet.date.gap', $gap);

    // Adds "hard limit" parameter to prevent too many return values.
    $settings = $this->adapter->getFacet($this->facet)->getSettings();
    $limit = empty($settings->settings['hard_limit']) ? 20 : (int) $settings->settings['hard_limit'];
    $query->addParam('f.' . $this->facet['field'] . '.facet.limit', $limit);

    $active = $this->adapter->getActiveItems($this->facet);
    // Date filters don't support OR operator.
    foreach ($active as $value => $item) {
      $query->addFilter($this->facet['field'], $value);
    }
  }

  /**
   * Gets the range of dates we are using.
   *
   * @param DrupalSolrQueryInterface $query
   *   A SolrBaseQuery object.
   *
   * @return bool|array
   *   An array containing the gap and range information or false if not present
   */
  function getDateRange(DrupalSolrQueryInterface $query) {
    $return = NULL;
    $gap = NULL;

    // Attempts to get next gap from passed date filters.
    foreach ($this->adapter->getActiveItems($this->facet) as $item) {
      if ($gap = facetapi_get_date_gap($item['start'], $item['end'])) {
        $next_gap = facetapi_get_next_date_gap($gap, FACETAPI_DATE_SECOND);
        if ($next_gap == $gap) {
          $next_gap = NULL;
          return NULL;
        }
        $return = array(
          "{$item['start']}/$next_gap",
          "{$item['end']}+1$next_gap/$next_gap",
          "+1$next_gap",
        );
      }
    }

    // If no filters were passed, get default range.
    if (NULL === $return) {

      // Builds SQL that gets minimum and maximum values from node table.
      $minimum = $maximum = FALSE;
      if ($this->facet['min callback'] && is_callable($this->facet['min callback'])) {
        $minimum = $this->facet['min callback']($this->facet);
      }
      if ($this->facet['max callback'] && is_callable($this->facet['max callback'])) {
        $maximum = $this->facet['max callback']($this->facet);
      }

      // Gets the default gap.
      //$gap = FACETAPI_DATE_YEAR;
      if ($minimum && $maximum) {
        $gap = facetapi_get_timestamp_gap($minimum, $maximum);
        $minimum = facetapi_isodate($minimum, $gap);
        $maximum = facetapi_isodate($maximum, $gap);
        $return = array(
          "$minimum/$gap",
          "$maximum+1$gap/$gap",
          "+1$gap",
        );
      }
    }
    // Returns the range information.
    return $return;
  }

  /**
   * Initializes the facet's build array.
   *
   * @return array
   *   The initialized render array.
   */
  public function build() {

    // Initializes build and gets static response.
    if (!$response = apachesolr_static_response_cache($this->adapter->getSearcher())) {
      return array();
    }
    $build = array();

    // Gets total number of documents matched in search.
    $total = $response->response->numFound;

    // Gets the active date facets, starts to builds the "parent - child"
    // relationships.
    $parent = NULL;
    foreach ($this->adapter->getActiveItems($this->facet) as $value => $item) {
      // Builds the raw facet "value", the count for selected items will be the
      // total number of rows returned in the query.
      $build[$value] = array('#count' => $total);

      // If there is a previous item, there is a parent, uses a reference so the
      // arrays are populated when they are updated.
      if (NULL !== $parent) {
        $build[$parent]['#item_children'][$value] = &$build[$value];
        $build[$value]['#item_parents'][$parent] = $parent;
      }

      // Stores the last value iterated over.
      $parent = $value;
    }

    // Gets raw facet data from the Solr server.
    if (isset($response->facet_counts->facet_dates) && isset($response->facet_counts->facet_dates->{$this->facet['field']})) {
      $raw_data = (array) $response->facet_counts->facet_dates->{$this->facet['field']};
    }
    else {
      $raw_data = array();
    }
    //$end = (!empty($raw_data['end'])) ? $raw_data['end'] : '';
    //$start = (!empty($raw_data['start'])) ? $raw_data['start'] : '';
    $gap = (!empty($raw_data['gap'])) ? $raw_data['gap'] : '';

    $settings = $this->getSettings()->settings;
    $granularity = isset($settings['date_granularity']) ? $settings['date_granularity'] : FACETAPI_DATE_MINUTE;

    // We cannot list anything below a minute (range of 00 seconds till 59
    // seconds). Milliseconds are not possible. Also, anything below the set
    // granularity should not be shown either.
    if ($gap != "+1" . FACETAPI_DATE_SECOND && (!$gap || strtotime($gap, 0) >= strtotime("+1" . $granularity, 0))) {
      unset($raw_data['start']);
      unset($raw_data['end']);
      unset($raw_data['gap']);

      // Treat each date facet as a range start, and use the next date facet
      // as range end.  Use 'end' for the final end.
      $previous = NULL;

      // Builds facet counts object used by the server.
      foreach ($raw_data as $value => $count) {
        if ($count) {
          $from = $value;
          $to = facetapi_isodate(strtotime($value . $gap));
          $new_value = '[' . $from . ' TO ' . $to . ']';
          $build[$new_value] = array('#count' => $count, '#active' => 0);
          if (NULL !== $parent) {
            $build[$parent]['#item_children'][$new_value] = &$build[$new_value];
            $build[$new_value]['#item_parents'][$parent] = $parent;
          }
        }
      }
    }
    return $build;
  }
}
