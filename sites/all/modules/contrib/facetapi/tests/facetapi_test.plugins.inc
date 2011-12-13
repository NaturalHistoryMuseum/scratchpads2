<?php

/**
 * Test plugins for Facet API.
 */

/**
 * Facet API adapter for the Apache Solr Search Integration module.
 */
class FacetapiTestAdapter extends FacetapiAdapter {

  /**
   * The total number of results in the query.
   */
  protected $resultCount;

  /**
   * Returns a boolean flagging whether $this->searcher executed a search.
   */
  public function searchExecuted() {
    return !empty($this->keys);
  }

  /**
   * Returns a boolean flagging whether facets in a realm shoud be displayed.
   */
  public function suppressOutput($realm_name) {

  }

  /**
   * Sets the result count.
   */
  public function setResultCount($count) {
    $this->resultCount = $count;
    return $this;
  }

  /**
   * Returns the nmber of total results found for the current search.
   */
  public function getResultCount() {
    return $this->resultCount;
  }
}

/**
 * Plugin for "term" query types.
 */
class FacetapiTestTerm extends FacetapiQueryType implements FacetapiQueryTypeInterface {

  static public function getType() {
    return 'term';
  }

  public function execute($query) {
    // Nothing to do...
  }

  public function build() {
    $build = array();

    $build['testone'] = array('#count' => 1);
    $build['testtwo'] = array('#count' => 2);
    $build['testthree'] = array('#count' => 3);

    // Returns initialized build.
    return $build;
  }
}
