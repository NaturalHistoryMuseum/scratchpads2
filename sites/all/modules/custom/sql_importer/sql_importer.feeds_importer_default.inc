<?php
/**
 * @file
 * sql_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function sql_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_into_cyperaceae_sql_';
  $feeds_importer->config = array(
    'name' => 'Import into: Cyperaceae (SQL)',
    'description' => 'Import a classification into a specific vocabulary.',
    'fetcher' => array(
      'plugin_key' => 'FeedsSQLFetcher',
      'config' => array(
        'databases' => array(
          0 => 'default',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSQLParser',
      'config' => array(
        'query' => 'SELECT * FROM import_from;',
        'database' => 'default',
        'mapping' => array(
          'authors' => 'authors',
          'parent_term_name' => 'parent_term_name',
          'rank' => 'rank',
          'term_name' => 'term_name',
          'unacceptability_reason' => 'unacceptability_reason',
          'unit_indicator_1' => 'unit_indicator_1',
          'unit_indicator_2' => 'unit_indicator_2',
          'unit_indicator_3' => 'unit_indicator_3',
          'unit_indicator_4' => 'unit_indicator_4',
          'unit_name_1' => 'unit_name_1',
          'unit_name_2' => 'unit_name_2',
          'unit_name_3' => 'unit_name_3',
          'unit_name_4' => 'unit_name_4',
          'usage_field' => 'usage_field',
          'wcm_id' => 'wcm_id',
          'wcm_protologue_reference' => 'wcm_protologue_reference',
        ),
        'results' => '',
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'cyperaceae',
        'mappings' => array(
          0 => array(
            'source' => 'term_name',
            'target' => 'name',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'parent_term_name',
            'target' => 'parent',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'authors',
            'target' => 'field_authors',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'rank',
            'target' => 'field_rank',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'unacceptability_reason',
            'target' => 'field_unacceptability_reason',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'unit_indicator_1',
            'target' => 'field_unit_indicator1',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'unit_indicator_2',
            'target' => 'field_unit_indicator2',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'unit_indicator_3',
            'target' => 'field_unit_indicator3',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'unit_indicator_4',
            'target' => 'field_unit_indicator4',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'unit_name_1',
            'target' => 'field_unit_name1',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'unit_name_2',
            'target' => 'field_unit_name2',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'unit_name_3',
            'target' => 'field_unit_name3',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'unit_name_4',
            'target' => 'field_unit_name4',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'usage_field',
            'target' => 'field_usage',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'wcm_id',
            'target' => 'field_id',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'wcm_protologue_reference',
            'target' => 'field_itis_em_other_ref',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 1,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_into_cyperaceae_sql_'] = $feeds_importer;

  return $export;
}
