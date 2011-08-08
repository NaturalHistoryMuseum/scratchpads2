<?php

/**
 * 
 */
function silver_feeds_importer_default(){
  // Array of Feeds importers.
  $export = array();
  // Get a list of the vocabularies.
  $vocabularies = taxonomy_get_vocabularies();
  foreach($vocabularies as $vocabulary){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'taxonomy_importer_' . $vocabulary->vid;
    $feeds_importer->config = array(
      'name' => t('Import into "!vocabulary_name"', array(
        '!vocabulary_name' => $vocabulary->name
      )),
      'description' => t('Import a classification into a specific vocabulary.'),
      'fetcher' => array(
        'plugin_key' => 'FeedsFileFetcher',
        'config' => array(
          'allowed_extensions' => 'xls xlsx',
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
          'vocabulary' => $vocabulary->machine_name,
          'mappings' => array(
            0 => array(
              'source' => 'Term name',
              'target' => 'name',
              'unique' => 0
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
            ),
            3 => array(
              'source' => 'GUID',
              'target' => 'guid',
              'unique' => 1
            ),
            4 => array(
              'source' => 'Parent GUID',
              'target' => 'parentguid',
              'unique' => 0
            )
          ), // FIXME: NEED TO ADD ADDITIONAL FIELDS HERE
          'update_existing' => '2',
          'input_format' => 'plain_text'
        )
      ),
      'content_type' => '',
      'update' => 1,
      'import_period' => '-1',
      'expire_period' => 3600,
      'import_on_create' => 1,
      'process_in_background' => 0
    );
    $export['taxonomy_importer_' . $vocabulary->vid] = $feeds_importer;
  }
  return $export;
}