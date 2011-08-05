<?php 

/**
 * 
 */
function silver_feeds_importer_default(){
  $export = array();
  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'taxonomy';
  $feeds_importer->config = array(
    'name' => 'Taxonomy',
    'description' => 'Import a taxonomy',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml xls xlsx',
        'direct' => 0
      )
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExcelParser',
      'config' => array(
        'no_headers' => 0,
        'all_worksheets' => 1
      )
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'tags',
        'mappings' => array(
          0 => array(
            'source' => 'Term name',
            'target' => 'name',
            'unique' => 1
          ),
          1 => array(
            'source' => 'Parent Term Name',
            'target' => 'parent',
            'unique' => 0
          ),
          2 => array(
            'source' => 'Term description',
            'target' => 'description',
            'unique' => FALSE
          )
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text'
      )
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0
  );
  $export['taxonomy'] = $feeds_importer;
  return $export;
}