<?php

/**
 * @file
 * Contains custom SolrBaseQuery class for Apachesolr Views.
 */

class ApachesolrViewsSolrBaseQuery extends SolrBaseQuery {

  // The view related with this query object.
  protected $view;

  /**
   * Constructs a new archiver instance.
   *
   * @param string $name
   *   The search name, used for finding the correct blocks and other config.
   *   Typically "apachesolr".
   *
   * @param string $solr
   *   An instantiated DrupalApacheSolrService Object.
   *   Can be instantiated from apachesolr_get_solr().
   *
   * @param array $params
   *   Array of params to initialize the object (typically 'q' and 'fq').
   *
   * @param string $sortstring
   *   Visible string telling solr how to sort - added to GET query params.
   *
   * @param string $base_path
   *   The search base path (without the keywords) for this query,
   *   without trailing slash.
   *
   * @param array $context
   *   The context related with this query.
   *
   * @param object $view
   *   The view object related with this query.
   */
  public function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = '', $context = array(), $view = NULL) {
    parent::__construct($name, $solr, $params, $sortstring, $base_path, $context);
    $this->view = $view;
  }

  /**
   * Need to set proper base path for facets.
   */
  public function getPath($new_keywords = NULL) {
    if (isset($new_keywords)) {
      return $this->base_path . '/' . $new_keywords;
    }
    return $this->base_path;
  }

  /**
   * Returns the view related with this query object.
   */
  public function getView() {
    return $this->view;
  }
}
