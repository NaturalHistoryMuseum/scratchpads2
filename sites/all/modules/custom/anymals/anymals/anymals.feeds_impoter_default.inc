<?php

/**
 * @file
 * anymals.feeds_importer_default.inc
 */
/**
 * Implements hook_feeds_importer_default().
 */
function anymals_feeds_importer_default(){
  $export = array();
  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'anymals_plants';
  $feeds_importer->config = array(
    'name' => 'anymals+plants',
    'description' => 'Periodically import records from the anymals+plants server',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL
      )
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'UnitExtension/SightingForms/form/select1[label="Basis of record"]/inputValue',
          'xpathparser:1' => 'UnitExtension/SightingForms/form/input[label="Catalogue number"]/inputValue',
          'xpathparser:2' => 'UnitExtension/SightingForms/form/input[label="Collection code"]/inputValue',
          'xpathparser:3' => 'UnitExtension/SightingForms/form/input[label="Institution code"]/inputValue'
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0
        ),
        'context' => '//DataSets/DataSet[DatasetGUID="87901689-8C9F-44AA-843F-4A05DA55F1C5"]/Units/Unit',
        'exp' => array(
          'errors' => 0,
          'tidy' => 0,
          'tidy_encoding' => 'UTF8',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0
          )
        ),
        'allow_override' => 1
      )
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'field_basis_of_record',
            'unique' => FALSE
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'field_catalogue_number',
            'unique' => FALSE
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_collection_code',
            'unique' => FALSE
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_institution_code',
            'unique' => FALSE
          )
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'specimen_observation'
      )
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '86400',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0
  );
  $export['anymals_plants'] = $feeds_importer;
  return $export;
}
