<?php

/**
 * @file
 * Classes used by the Facet API module.
 */

/**
 * Facet API adapter for the Apache Solr Search Integration module.
 */
class ApacheSolrFacetapiAdapter extends FacetapiAdapter {

  /**
   * Returns the path to the admin settings for a given realm.
   *
   * @param string $realm_name
   *   The name of the realm.
   *
   * @return string
   *   The path to the admin settings.
   */
  public function getPath($realm_name) {
    $path = 'admin/config/search/apachesolr/settings';
    // $adapter will be an instance of class FacetapiAdapter
    if ($adapter = menu_get_object('facetapi_adapter', 4)) {
      // Get the environment ID from the machine name of the searcher.
      $env_id = ltrim(strstr($adapter->getSearcher(), '@'), '@');
      $path .= '/' . $env_id . '/facets';
      // Add the realm name to the path if it is not the first one in the list.
      if (key(facetapi_get_realm_info()) != $realm_name) {
        $path .= '/' . $realm_name;
      }
    }
    return $path;
  }

  /**
   * Allows the backend to initialize its query object before adding the facet
   * filters.
   *
   * @param mixed $query
   *   The backend's native object.
   */
  function initActiveFilters($query) {
    $enabled_facets = facetapi_get_enabled_facets($this->info['name']);
    if ($enabled_facets) {
      $query->addParam('facet', 'true');
      $query->addParam('facet.sort', 'count');
      $query->addParam('facet.mincount', '1');
    }
  }

  /**
   * Returns a boolean flagging whether $this->_searcher executed a search.
   */
  public function searchExecuted() {
    // Initial check - has ANY solr query run in our environment.
    $env_id = $this->info['instance'];
    $this_has_searched = apachesolr_has_searched($env_id);
    // Secondary check - do we have results for this searcher?
    $this_has_searched = $this_has_searched && apachesolr_static_response_cache($this->getSearcher());
    return $this_has_searched;
  }

  /**
   * Suppress output of the realm
   *
   * @param string $realm_name
   *
   * @return bool $flag
   *   Returns if it was suppressed or not
   */
  public function suppressOutput($realm_name) {
    $flag = FALSE;
    if ($realm_name == 'block') {
      $env_id = $this->info['instance'];
      $flag = apachesolr_suppress_blocks($env_id);
    }
    return $flag || !$this->searchExecuted();
  }

  /**
   * Returns the search keys.
   *
   * @return string
   */
  public function getSearchKeys() {
    if (NULL === $this->keys) {
      $env_id = $this->info['instance'];
      if ($query = apachesolr_current_query($env_id)) {
        return $query->getParam('q');
      }
    }
    else {
      return $this->keys;
    }
    return FALSE;
  }

  /**
   * Returns the search path.
   *
   * @return string
   *   A string containing the search path.
   *
   * @todo D8 should provide an API function for this.
   */
  public function getSearchPath() {
    $env_id = $this->info['instance'];
    $query = apachesolr_current_query($env_id);
    if (!$query || (NULL === $this->searchPath && NULL === $query->getPath())) {
      if ($path = module_invoke($this->info['module'] . '_search', 'search_info')) {
        $this->searchPath = 'search/' . $path['path'];
        if (!isset($_GET['keys']) && ($keys = $this->getSearchKeys())) {
          $this->searchPath .= '/' . $keys;
        }
      }
    }
    if (!$query || NULL === $query->getPath()) {
      return $this->searchPath;
    }
    else {
      return $query->getPath();
    }

  }

  /**
   * Returns the number of total results found for the current search.
   *
   * @return bool|int
   *   Number of results or false if no search response was found
   */
  public function getResultCount() {
    $response = apachesolr_static_response_cache($this->getSearcher());
    if ($response) {
      return $response->response->numFound;
    }
    return FALSE;
  }

  /**
   * Allows for backend specific overrides to the settings form.
   *
   * @param array $form
   * @param array $form_state
   */
  public function settingsForm(&$form, &$form_state) {
    if (in_array('date', $form['#facetapi']['facet']['query types']) ) {
      $granularity = array(
        FACETAPI_DATE_YEAR => t('Year'),
        FACETAPI_DATE_MONTH => t('Month'),
        FACETAPI_DATE_DAY => t('Day'),
        FACETAPI_DATE_HOUR => t('Hour'),
        FACETAPI_DATE_MINUTE => t('Minute'),
      );

      $facet = $form['#facetapi']['facet'];
      $settings = $this->getFacet($facet)->getSettings()->settings;

      $form['global']['date_granularity'] = array(
        '#type' => 'select',
        '#title' => t('Granularity'),
        '#description' => t('Time intervals smaller than this will not be displayed in the facet.'),
        '#options' => $granularity,
        '#default_value' => isset($settings['date_granularity']) ? $settings['date_granularity'] : FACETAPI_DATE_MINUTE,
      );
    }

    $form['#validate'][] = 'apachesolr_facet_form_validate';
  }
}
