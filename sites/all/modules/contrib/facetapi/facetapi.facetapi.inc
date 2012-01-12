<?php

/**
 * @file
 * Facet API hook implementations.
 */

/**
 * Implements hook_facetapi_realm_info().
 */
function facetapi_facetapi_realm_info() {
  $realms = array();

  $realms['block'] = array(
    'label' => t('Blocks'),
    'weight' => -10,
    'sortable' => FALSE,
    'default widget' => 'facetapi_links',
    'element type' => 'links',
    'description' => t(
      'The <em>Blocks</em> realm displays each facet in a separate <a href="@block-page">block</a>. Users are able to refine their searches in a drill-down fashion.',
      array('@block-page' => url('admin/structure/block', array('query' => array('destination' => current_path()))))
    ),
  );

  return $realms;
}

/**
 * Implements hook_facetapi_sort_info().
 */
function facetapi_facetapi_sort_info() {
  $sorts = array();

  $sorts['active'] = array(
    'label' => t('Facet active'),
    'callback' => 'facetapi_sort_active',
    'description' => t('Sort by whether the facet is active or not.'),
    'weight' => -50,
  );

  $sorts['count'] = array(
    'label' => t('Count'),
    'callback' => 'facetapi_sort_count',
    'description' => t('Sort by the facet count.'),
    'weight' => -49,
  );

  $sorts['display'] = array(
    'label' => t('Display value'),
    'callback' => 'facetapi_sort_display',
    'description' => t('Sort by the value displayed to the user.'),
    'weight' => -48,
  );

  $sorts['indexed'] = array(
    'label' => t('Indexed value'),
    'callback' => 'facetapi_sort_indexed',
    'description' => t('Sort by the raw value stored in the index.'),
    'weight' => -47,
  );

  return $sorts;
}

/**
 * Implements hook_facetapi_widgets().
 */
function facetapi_facetapi_widgets() {
  return array(
    'facetapi_links' => array(
      'handler' => array(
        'label' => t('Links'),
        'class' => 'FacetapiWidgetLinks',
        'query types' => array('term', 'date'),
      ),
    ),
    'facetapi_checkbox_links' => array(
      'handler' => array(
        'label' => t('Links with checkboxes'),
        'class' => 'FacetapiWidgetCheckboxLinks',
        'query types' => array('term', 'date'),
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_filters().
 */
function facetapi_facetapi_filters() {
  return array(
    'active_items' => array(
      'handler' => array(
        'label' => t('Do not display active items'),
        'class' => 'FacetapiFilterActiveItems',
      ),
    ),
    'current_depth' => array(
      'handler' => array(
        'label' => t('Only show items in the current level of the hierarchy'),
        'class' => 'FacetapiFilterCurrentDepth',
        'requirements' => array('facetapi_requirement_facet_hierarchical' => TRUE),
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_dependencies().
 */
function facetapi_facetapi_dependencies() {
  return array(
    'bundle' => array(
      'handler' => array(
        'label' => t('Bundles'),
        'class' => 'FacetapiDependencyBundle',
      ),
    ),
    'role' => array(
      'handler' => array(
        'label' => t('Roles'),
        'class' => 'FacetapiDependencyRole',
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_empty_behaviors().
 */
function facetapi_facetapi_empty_behaviors() {
  return array(
    'none' => array(
      'handler' => array(
        'label' => t('Do not display facet'),
        'class' => 'FacetapiEmptyBehaviorNone',
      ),
    ),
    'text' => array(
      'handler' => array(
        'label' => t('Display text'),
        'class' => 'FacetapiEmptyBehaviorText',
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_url_processors().
 */
function facetapi_facetapi_url_processors() {
  return array(
    'standard' => array(
      'handler' => array(
        'label' => t('Standard URL processors'),
        'class' => 'FacetapiUrlProcessorStandard',
      ),
    ),
  );
}
