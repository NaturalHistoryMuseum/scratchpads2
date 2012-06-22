<?php

/**
 * 
 */
function silver_feeds_importer_default(){
  if(function_exists('strongarm_init')){
    strongarm_init();
  }
  // Array of Feeds importers.
  $export = array();
  // Get a list of the vocabularies.
  $vocabularies = taxonomy_get_vocabularies();
  foreach($vocabularies as $vocabulary){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'taxonomy_importer_' . $vocabulary->machine_name;
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
      $field_info = field_info_field($field['field_name']);
      switch($field_info['type']){
        case 'file':
          // Do nothing.  We can't import files in an Excel template.
          break;
        case 'node_reference':
          // We add two columns for the node reference field.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'] . ' (NID)',
            'target' => $key . ':nid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'] . ' (' . t('Title') . ')',
            'target' => $key . ':title:duplicates',
            'unique' => 0
          );
          break;
        default:
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'],
            'target' => $key,
            'unique' => 0
          );
      }
    }
    $export['taxonomy_importer_' . $vocabulary->machine_name] = $feeds_importer;
  }
  // Get a list of the content types.
  $entity_info = entity_get_info('node');
  foreach($entity_info['bundles'] as $node_type => $node_type_info){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'node_importer_' . $node_type;
    $feeds_importer->config = array(
      'name' => t('Import into: !node_type_name', array(
        '!node_type_name' => $node_type_info['label']
      )),
      'description' => t('Import a classification into a specific content type.'),
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
        'plugin_key' => 'FeedsNodeProcessor',
        'config' => array(
          'content_type' => $node_type,
          'expire' => '-1',
          'author' => 0,
          'mappings' => array(
            array(
              'source' => 'GUID',
              'target' => 'guid',
              'unique' => TRUE
            )
          ),
          'update_existing' => '2',
          'input_format' => 'filtered_html'
        )
      ),
      'content_type' => '',
      'update' => 0,
      'import_period' => -1,
      'expire_period' => 3600,
      'import_on_create' => TRUE,
      'process_in_background' => FALSE
    );
    if(auto_nodetitle_get_setting($node_type) != AUTO_NODETITLE_ENABLED){
      $feeds_importer->config['processor']['config']['mappings'][] = array(
        'source' => 'Title',
        'target' => 'title',
        'unique' => FALSE
      );
    }
    $field_info = field_info_instances('node', $node_type);
    foreach($field_info as $key => $field){
      $field_info = field_info_field($field['field_name']);
      switch($field_info['type']){
        case 'file':
          // Do nothing.  We can't import files in an Excel template.
          break;
        case 'node_reference':
          // We add two columns for the node reference field.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'] . ' (NID)',
            'target' => $key . ':nid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'] . ' (' . t('Title') . ')',
            'target' => $key . ':title:duplicates',
            'unique' => 0
          );
          break;
        default:
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field['label'],
            'target' => $key,
            'unique' => 0
          );
      }
    }
    $export['node_importer_' . $node_type] = $feeds_importer;
  }
  return $export;
}
