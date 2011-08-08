<?php 

/**
 * 
 */
function silver_feeds_importer_default(){
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'taxonomy_importer';
  $feeds_importer->config = array(
    'name' => 'Taxonomy importer',
    'description' => 'Taxonomy importer',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml xls xlsx',
        'direct' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExcelParser',
      'config' => array(
        'no_headers' => 0,
        'all_worksheets' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 'tags',
        'mappings' => array(
          0 => array(
            'source' => 'Term name',
            'target' => 'name',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'Parent Term Name',
            'target' => 'parent',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'Term description',
            'target' => 'description',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'GUID',
            'target' => 'guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'Parent GUID',
            'target' => 'parentguid',
            'unique' => 0,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['taxonomy_importer'] = $feeds_importer;

  return $export;
}