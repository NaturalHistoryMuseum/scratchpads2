<?php

/**
 * @file
 * Block realm code and hook implementations.
 */

/**
 * Returns block information.
 */
function facetapi_get_block_info($realm_name = 'block') {
  $blocks = array();

  // Gets delta map, iterates over all enabled facets.
  $map = facetapi_get_delta_map();
  foreach (facetapi_get_searcher_info() as $searcher => $info) {
    // Gets cache settings for the searcher.
    $cache = variable_get('facetapi:block_cache:' . $searcher, DRUPAL_NO_CACHE);

    // Adds "Current Search" blocks.
    $delta = array_search($searcher . ':current_search', $map);
    $blocks[$delta] = array(
      'info' => 'Facet API: ' . $info['label'] . ' : ' . t('Current Search'),
      'cache' => $cache,
    );

    // Adds blocks for facets that are enabled or whose delta mapping is forced.
    foreach (facetapi_get_delta_map_queue($searcher, $realm_name) as $facet_name) {
      if ($facet = facetapi_facet_load($facet_name, $searcher)) {
        // Gets the delta from the delta map.
        $string = facetapi_build_delta($searcher, $realm_name, $facet_name);
        $delta = array_search($string, $map);

        // Defines the block.
        $blocks[$delta] = array(
          'info' => 'Facet API: ' . $info['label'] . ' : ' . $facet['label'],
          'cache' => $cache,
        );
      }
    }
  }

  // Returns available blocks.
  return $blocks;
}

/**
 * Returns the content for a facet based on the delta.
 */
function facetapi_get_block($delta) {
  $builds = &drupal_static(__FUNCTION__, array());

  // Bails if delta is not mapped.
  $map = facetapi_get_delta_map();
  if (!isset($map[$delta])) {
    return;
  }

  // Extracts the searcher, realm name, and facet name from $delta.
  // Process the parts from the end in case the searcher includes a ':'.
  $parts = explode(':', $map[$delta]);
  $facet_name = array_pop($parts);
  $facet_name = rawurldecode($facet_name);
  $realm_name = array_pop($parts);
  $searcher = implode(':', $parts);

  // If we are viewing the current search block, set variable names accordingly.
  if (!$searcher && 'current_search' == $facet_name) {
    $searcher = $realm_name;
    $realm_name = $facet_name = FALSE;
  }

  // Bails if adapter can't be loaded.
  if (!$adapter = facetapi_adapter_load($searcher)) {
    return;
  }

  // If there is no realm, we are rendering the curent search block.
  if (FALSE === $realm_name) {
    if (!$adapter->searchExecuted()) {
      return;
    }
    return array(
      'subject' => t('Current search'),
      'content' => $adapter->buildCurrentSearch(),
    );
  }

  // Bails if the output should be suppressed.
  if ($adapter->suppressOutput($realm_name)) {
    return;
  }

  // Bails if the facet isn't enabled.
  if (!facetapi_facet_enabled($searcher, $realm_name, $facet_name)) {
    return;
  }

  // Builds and caches the entire realm per searcher / realm combination.
  $group = $searcher . ':' . $realm_name;
  if (!isset($builds[$group])) {
    $builds[$group] = facetapi_build_realm($searcher, $realm_name);
  }

  // Returns the individual block.
  if (isset($builds[$group][$facet_name])) {

    // Adds contextual links.
    $builds[$group][$facet_name]['#contextual_links'] = array(
      'facetapi' => array('admin/config/search/facetapi', array($searcher, $realm_name, $facet_name)),
    );

    // Returns the subject and content of the block.
    $variables = array('title' => $builds[$group][$facet_name]['#title']);
    return array(
      'subject' => theme('facetapi_title', $variables),
      'content' => $builds[$group][$facet_name]
    );
  }
}

/**
 * Returns a cached delta map of hashes to names.
 *
 * Sometimes our deltas are longer than 32 chars and need to be passed to hash().
 * Due to the block table's schema, deltas cannot be longer than 32 characters.
 * However, hashes are nasty as CSS IDs, so we can use the map to convert
 * the hashes back to a nicer value in facetapi_preprocess_block().
 *
 * @return
 *   An array containing the delta map.
 */
function facetapi_get_delta_map() {
  $map = &drupal_static(__FUNCTION__);
  if (NULL === $map) {
    if ($data = cache_get('facetapi:delta_map')) {
      $map = $data->data;
    }
    else {

      $map = array();
      foreach (facetapi_get_searcher_info() as $searcher => $info) {

        // Maps current search block.
        $delta = $searcher . ':current_search';
        $map[facetapi_hash_delta($delta)] = $delta;

        // Maps facet deltas.
        // @todo - some other way to define realms that are block-like.
        foreach (array('block') as $realm_name) {
          foreach (facetapi_get_delta_map_queue($searcher, $realm_name) as $facet_name) {
            $delta = facetapi_build_delta($searcher, $realm_name, $facet_name);
            $map[facetapi_hash_delta($delta)] = $delta;
          }
        }
      }

      // Caches the map so we don't have to do this repeatedly.
      cache_set('facetapi:delta_map', $map, 'cache', CACHE_TEMPORARY);
    }
  }

  return $map;
}

/**
 * Build a delta from the searcher, realm name, and facet name.
 *
 * @param $searcher
 *   The machine readable name of the searcher.
 * @param $realm_name
 *   The machine readable name of the realm.
 * @param $facet_name
 *   The machine readable name of the facet.
 *
 * @return
 *   A string containing the raw delta.
 */
function facetapi_build_delta($searcher, $realm_name, $facet_name) {
  return $searcher . ':' . $realm_name . ':' . urlencode($facet_name);
}

/**
 * Hashing code for deltas.
 *
 * @param $delta
 *   A string containing the delta.
 *
 * @return
 *   The hashed delta value.
 */
function facetapi_hash_delta($delta) {
  return substr(drupal_hash_base64($delta), 0, 32);
}

/**
 * Returns facets that are enabled or whose delta mapping is forced.
 *
 * @param $searcher
 *   The machine readable name of the searcher.
 * @param $realm_name
 *   The machine readable name of the realm.
 *
 * @return array
 *   A list of machine readable facet names.
 */
function facetapi_get_delta_map_queue($searcher, $realm_name) {
  static $forced;
  if (NULL === $forced) {
    $forced = module_invoke_all('facetapi_force_delta_mapping');
  }

  $enabled = array_keys(facetapi_get_enabled_facets($searcher, $realm_name));
  if (!isset($forced[$searcher][$realm_name])) {
    $forced[$searcher][$realm_name] = array();
  }

  // Merges enabled facets and facets whose mapping is forced.
  return array_unique(array_merge($enabled, $forced[$searcher][$realm_name]));
}
