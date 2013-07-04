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
        'scratchpads_statistics-site' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'site',
          'region' => 'sidebar',
          'weight' => '-10'
        ),
        'scratchpads_statistics-user' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'user',
          'region' => 'sidebar',
          'weight' => '-9'
        ),
        'scratchpads_statistics-term' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'term',
          'region' => 'sidebar',
          'weight' => '-8'
        ),
        'scratchpads_statistics-category' => array(
          'module' => 'scratchpads_statistics',
          'delta' => 'category',
          'region' => 'sidebar',
          'weight' => '-7'
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
