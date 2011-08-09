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
      'name' => t('Import into: !vocabulary_name', array(
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
            array(
              'source' => 'Term name',
              'target' => 'name',
              'unique' => 0
            ),
            array(
              'source' => 'Parent Term Name',
              'target' => 'parent',
              'unique' => 0
            ),
            array(
              'source' => 'Term description',
              'target' => 'description',
              'unique' => FALSE
            ),
            array(
              'source' => 'GUID',
              'target' => 'guid',
              'unique' => 1
            ),
            array(
              'source' => 'Parent GUID',
              'target' => 'parentguid',
              'unique' => 0
            )
          ),
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
    $field_info = field_info_instances('taxonomy_term', $vocabulary->machine_name);
    foreach($field_info as $key => $field){
      $feeds_importer->config['processor']['config']['mappings'][] = array(
        'source' => $field['label'],
        'target' => $key,
        'unique' => 0
      );
    }
    $export['taxonomy_importer_' . $vocabulary->vid] = $feeds_importer;
  }
  return $export;
}