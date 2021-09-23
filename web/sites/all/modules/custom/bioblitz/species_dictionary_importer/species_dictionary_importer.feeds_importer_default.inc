<?php
/**
 * @file
 * species_dictionary_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function species_dictionary_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'species_dictionary_importer';
  $feeds_importer->config = array(
    'name' => 'Species Dictionary Importer',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'csv',
        'direct' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'species_dictionary',
        'mappings' => array(
          0 => array(
            'source' => 'Parent Term Name',
            'target' => 'parent',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'Term description',
            'target' => 'description',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'GUID',
            'target' => 'guid',
            'unique' => 1,
          ),
          3 => array(
            'source' => 'Parent GUID',
            'target' => 'parentguid',
            'unique' => 0,
          ),
          4 => array(
            'source' => 'Associated accepted name',
            'target' => 'field_aan_4',
            'unique' => 0,
          ),
          5 => array(
            'source' => 'Authors',
            'target' => 'field_authors',
            'unique' => 0,
          ),
          6 => array(
            'source' => 'Page number',
            'target' => 'field_page_number',
            'unique' => 0,
          ),
          7 => array(
            'source' => 'Rank',
            'target' => 'field_rank',
            'unique' => 0,
          ),
          8 => array(
            'source' => 'Reference',
            'target' => 'field_reference',
            'unique' => 0,
          ),
          9 => array(
            'source' => 'Unacceptability Reason',
            'target' => 'field_unacceptability_reason',
            'unique' => 0,
          ),
          10 => array(
            'source' => 'Unit indicator 1',
            'target' => 'field_unit_indicator1',
            'unique' => 0,
          ),
          11 => array(
            'source' => 'Unit indicator 2',
            'target' => 'field_unit_indicator2',
            'unique' => 0,
          ),
          12 => array(
            'source' => 'Unit indicator 3',
            'target' => 'field_unit_indicator3',
            'unique' => 0,
          ),
          13 => array(
            'source' => 'Unit indicator 4',
            'target' => 'field_unit_indicator4',
            'unique' => 0,
          ),
          14 => array(
            'source' => 'Unit name 1',
            'target' => 'field_unit_name1',
            'unique' => 0,
          ),
          15 => array(
            'source' => 'Unit name 2',
            'target' => 'field_unit_name2',
            'unique' => 0,
          ),
          16 => array(
            'source' => 'Unit name 3',
            'target' => 'field_unit_name3',
            'unique' => 0,
          ),
          17 => array(
            'source' => 'Unit name 4',
            'target' => 'field_unit_name4',
            'unique' => 0,
          ),
          18 => array(
            'source' => 'Usage',
            'target' => 'field_usage',
            'unique' => 0,
          ),
          19 => array(
            'source' => 'Vernacular names',
            'target' => 'field_vernacular_names',
            'unique' => 0,
          ),
          20 => array(
            'source' => 'Unit name 1',
            'target' => 'name',
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
  $export['species_dictionary_importer'] = $feeds_importer;

  return $export;
}
