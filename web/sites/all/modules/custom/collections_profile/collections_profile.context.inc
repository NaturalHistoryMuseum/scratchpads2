<?php
/**
 * @file
 * collection_statistic_blocks.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function collections_profile_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'collection_statistics';
  $context->description = 'Specimen statistics for collection profile pages';
  $context->tag = 'Collection profiles';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'collections_profile/*/*' => 'collections_profile/*/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-a52da6d2c0e3fd6daa0fc908dbf5df45' => array(
          'module' => 'views',
          'delta' => 'a52da6d2c0e3fd6daa0fc908dbf5df45',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
        'views-b0697ed4b2aae661e1580a5933f158fb' => array(
          'module' => 'views',
          'delta' => 'b0697ed4b2aae661e1580a5933f158fb',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
      ),
    ),
    'breadcrumb' => 'collections_profile',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Collection profiles');
  t('Specimen statistics for collection profile pages');
  $export['collection_statistics'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'collection_statistics_all';
  $context->description = 'Block of statistics for all collections to show on the collections_profiles index page';
  $context->tag = 'Collection profiles';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'collections_profile' => 'collections_profile',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-b51f10b4574a1cd5dec6648188e79e32' => array(
          'module' => 'views',
          'delta' => 'b51f10b4574a1cd5dec6648188e79e32',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Block of statistics for all collections to show on the collections_profiles index page');
  t('Collection profiles');
  $export['collection_statistics_all'] = $context;

  return $export;
}
