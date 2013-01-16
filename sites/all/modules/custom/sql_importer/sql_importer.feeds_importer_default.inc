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
          'associated_accepted_name' => 'associated_accepted_name',
          'authors' => 'authors',
          'etymology' => 'etymology',
          'guid' => 'guid',
          'hybrid_parents' => 'hybrid_parents',
          'page_number' => 'page_number',
          'parent_guid' => 'parent_guid',
          'parent_term_name' => 'parent_term_name',
          'protologue_link' => 'protologue_link',
          'rank' => 'rank',
          'reference_nid' => 'reference_nid',
          'reference_title' => 'reference_title',
          'term_description' => 'term_description',
          'term_name' => 'term_name',
          'type_references_nid' => 'type_references_nid',
          'type_references_title' => 'type_references_title',
          'type_taxon' => 'type_taxon',
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
          'vernacular_names' => 'vernacular_names',
          'wcm_id' => 'wcm_id',
          'wcm_protologue_reference' => 'wcm_protologue_reference',
        ),
        'results' => '',
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'cyperaceae_ii',
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
