<?php

/**
 * @file
 */
/**
 * Implements hook_context_default_contexts().
 */
function scratchpads_statistics_context_default_contexts(){
  $export = array();
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'scratchpads_statistics';
  $context->description = 'Display the Scratchpads Statistics facets blocks on the stats pages';
  $context->tag = 'scratchpads_statistics';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'stats' => 'stats',
        'stats/*' => 'stats/*'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'scratchpads_statistics-ss_facet_user' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'ss_facet_user',
          'region' => 'sidebar',
          'weight' => '-10'
        ),
        'scratchpads_statistics-ss_facet_site' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'ss_facet_site',
          'region' => 'sidebar',
          'weight' => '-10'
        ),
        'scratchpads_statistics-ss_facet_site_category' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'ss_facet_site_category',
          'region' => 'sidebar',
          'weight' => '-10'
        ),
        'scratchpads_statistics-ss_facet_taxon' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'ss_facet_taxon',
          'region' => 'sidebar',
          'weight' => '-10'
        )
      )
    )
  );
  $context->condition_mode = 1;
  // Translatables
  // Included for use with string extractors like potx.
  t('Display the Scratchpads Statistics facets blocks on the stats pages');
  t('scratchpads_statistics');
  $export['scratchpads_statistics'] = $context;
  return $export;
}
