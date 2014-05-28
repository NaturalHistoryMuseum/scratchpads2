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
    $feeds_importer->id = 'taxonomy_importer_' . $vocabulary->machine_name . '_xls';
    $feeds_importer->config = array(
      'name' => t('Import into: !vocabulary_name (!type)', array(
        '!vocabulary_name' => $vocabulary->name,
        '!type' => 'XLS'
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
          'bundle' => $vocabulary->machine_name,
          'update_existing' => '2',
          'input_format' => 'plain_text',
          'skip_hash_check' => 0
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
    _silver_feeds_importer_default_helper($field_info, $feeds_importer, 'taxonomy_term:' . $vocabulary->machine_name . ':');
    $export['taxonomy_importer_' . $vocabulary->machine_name . '_xls'] = $feeds_importer;
    if(module_exists('silvercsv')){
      $csv_importer = clone $feeds_importer;
      $csv_importer->id = 'taxonomy_importer_' . $vocabulary->machine_name . '_csv';
      $csv_importer->config['name'] = t('Import into: !vocabulary_name (!type)', array(
        '!vocabulary_name' => $vocabulary->name,
        '!type' => 'CSV'
      ));
      $csv_importer->config['parser'] = array(
        'plugin_key' => 'FeedsCSVParser',
        'config' => array(
          'delimiter' => ',',
          'no_headers' => 0
        )
      );
      $export['taxonomy_importer_' . $vocabulary->machine_name . '_csv'] = $csv_importer;
    }
  }
  // Get a list of the content types.
  $entity_info = entity_get_info('node');
  foreach($entity_info['bundles'] as $node_type => $node_type_info){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'node_importer_' . $node_type . '_xls';
    $feeds_importer->config = array(
      'name' => t('Import into: !node_type_name (!type)', array(
        '!node_type_name' => $node_type_info['label'],
        '!type' => 'XLS'
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
        'plugin_key' => 'FeedsNodeProcessorValidate',
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
          'bundle' => $node_type,
          'update_existing' => '2',
          'input_format' => 'filtered_html',
          'skip_hash_check' => 0
        )
      ),
      'content_type' => '',
      'update' => 0,
      'import_period' => -1,
      'expire_period' => 3600,
      'import_on_create' => TRUE,
      'process_in_background' => FALSE
    );
    // The title field may not have been enabled.
    if(function_exists('title_field_replacement_enabled') && title_field_replacement_enabled('node', $node_type, 'title')){
      if(function_exists('auto_entitylabel_get_setting') && auto_entitylabel_get_setting($node_type) != AUTO_ENTITYLABEL_ENABLED){
        $feeds_importer->config['processor']['config']['mappings'][] = array(
          'source' => 'Title',
          'target' => 'title_field',
          'unique' => FALSE
        );
      }
    }else if(function_exists('auto_entitylabel_get_setting') && auto_entitylabel_get_setting($node_type) != AUTO_ENTITYLABEL_ENABLED){
      $feeds_importer->config['processor']['config']['mappings'][] = array(
        'source' => 'Title',
        'target' => 'title',
        'unique' => FALSE
      );
    }
    $field_info = field_info_instances('node', $node_type);
    $field_info = _silver_feeds_sort_field_info('node', $node_type, $field_info);
    _silver_feeds_importer_default_helper($field_info, $feeds_importer, "node:$node_type:");
    $export['node_importer_' . $node_type . '_xls'] = $feeds_importer;
    if(module_exists('silvercsv')){
      $csv_importer = clone $feeds_importer;
      $csv_importer->id = 'node_importer_' . $node_type . '_csv';
      $csv_importer->config['name'] = t('Import into: !node_type_name (!type)', array(
        '!node_type_name' => $node_type_info['label'],
        '!type' => 'CSV'
      ));
      $csv_importer->config['parser'] = array(
        'plugin_key' => 'FeedsCSVParser',
        'config' => array(
          'delimiter' => ',',
          'no_headers' => 0
        )
      );
      $export['node_importer_' . $node_type . '_csv'] = $csv_importer;
    }
  }
  // Get a list of the file types.
  $entity_info = entity_get_info('file');
  foreach($entity_info['bundles'] as $file_type => $file_type_info){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'file_importer_' . $file_type . '_xls';
    $feeds_importer->config = array(
      'name' => t('Import into: @file_type_name (!type)', array(
        '@file_type_name' => $file_type_info['label'],
        '!type' => 'XLS'
      )),
      'description' => t('Update the metadata for a specific file type.'),
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
        'plugin_key' => 'FeedsFileProcessor',
        'config' => array(
          'bundle' => $file_type,
          'expire' => '-1',
          'author' => 0,
          'input_format' => 'filtered_html',
          'mappings' => array(
            array(
              'source' => 'GUID',
              'target' => 'guid',
              'unique' => TRUE
            ),
            array(
              'source' => 'Filename',
              'target' => 'filename',
              'unique' => FALSE
            )
          )
        )
      ),
      'update' => 0,
      'import_period' => -1,
      'expire_period' => 3600,
      'import_on_create' => TRUE,
      'process_in_background' => FALSE
    );
    $field_info = field_info_instances('file', $file_type);
    _silver_feeds_importer_default_helper($field_info, $feeds_importer, "file:$file_type:");
    $export['file_importer_' . $file_type . '_xls'] = $feeds_importer;
    if(module_exists('silvercsv')){
      $csv_importer = clone $feeds_importer;
      $csv_importer->id = 'file_importer_' . $file_type . '_csv';
      $csv_importer->config['name'] = t('Import into: @file_type_name (!type)', array(
        '@file_type_name' => $file_type_info['label'],
        '!type' => 'CSV'
      ));
      $csv_importer->config['parser'] = array(
        'plugin_key' => 'FeedsCSVParser',
        'config' => array(
          'delimiter' => ',',
          'no_headers' => 0
        )
      );
      $export['file_importer_' . $file_type . '_csv'] = $csv_importer;
    }
  }
  // Users ---------------------------------------------------------------------
  if(user_access('administer users')){
    // Add user fields 
    // We use the form to ensure that the fields are in the same order as the form
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE;
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'user_importer_user_xls';
    $feeds_importer->config = array(
      'name' => t('Import into: Users (!type)', array(
        '!type' => 'XLS'
      )),
      'description' => t('Add and Update users.'),
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
        'plugin_key' => 'FeedsUserProcessor',
        'config' => array(
          'status' => 1,
          'mappings' => array(),
          'update_existing' => '1'
        )
      ),
      'update' => 0,
      'import_period' => -1,
      'expire_period' => 3600,
      'import_on_create' => TRUE,
      'process_in_background' => FALSE
    );
    $form = drupal_get_form('user_register_form');
    $field_info = field_info_instances('user', 'user');
    $ordered_field_info = array();
    $order = array();
    $group_number = 0;
    // If we are using fieldgroups, as in biography
    if(!empty($form['#groups'])){
      foreach($form['#groups'] as $group_name => $group){
        if($group_name != 'group_tabs'){
          foreach($group->children as $field_name){
            // GroupNumber - Weight - Fieldname
            $order[$group_number . $form[$field_name]['#weight'] . $field_name] = $field_name;
          }
        }
        $group_number++;
      }
    }else{
      // No fieldgroups, as in default scratchpads
      foreach($field_info as $field_name => $info){
        $order[$form[$field_name]['#weight'] . $field_name] = $field_name;
      }
    }
    ksort($order);
    foreach($order as $field_name){
      $ordered_field_info[$field_name] = $field_info[$field_name];
      unset($field_info[$field_name]);
    }
    // Add any fields that are not in fieldgroups to the end
    foreach($field_info as $field_name => $info){
      $ordered_field_info[$field_name] = $info;
    }
    $field_info = $ordered_field_info;
    _silver_feeds_importer_default_helper($field_info, $feeds_importer, 'user:user:');
    $feeds_importer->config['processor']['config']['mappings'][] = array(
      'source' => 'E-Mail',
      'target' => 'mail',
      'unique' => TRUE
    );
    $feeds_importer->config['processor']['config']['mappings'][] = array(
      'source' => 'Username',
      'target' => 'name',
      'unique' => TRUE
    );
    $feeds_importer->config['processor']['config']['mappings'][] = array(
      'source' => 'Password',
      'target' => 'pass',
      'unique' => 0
    );
    $feeds_importer->config['processor']['config']['mappings'][] = array(
      'source' => 'GUID',
      'target' => 'guid',
      'unique' => 1
    );
    $export['user_importer_user_xls'] = $feeds_importer;
    if(module_exists('silvercsv')){
      $csv_importer = clone $feeds_importer;
      $csv_importer->id = 'user_importer_user_csv';
      $csv_importer->config['name'] = t('Import into: Users (!type)', array(
        '!type' => 'CSV'
      ));
      $csv_importer->config['parser'] = array(
        'plugin_key' => 'FeedsCSVParser',
        'config' => array(
          'delimiter' => ',',
          'no_headers' => 0
        )
      );
      $export['user_importer_user_csv'] = $csv_importer;
    }
  }
  return $export;
}

/**
 * Helper function to sort the field_info array
 */
function _silver_feeds_sort_field_info($entity_type, $bundle, $field_info){
  if(variable_get('install_task', 'notdone') == 'done'){
    $groups = field_group_info_groups($entity_type, $bundle, 'form');
    foreach($groups as $key => $group){
      foreach($group->children as $child_key => $child){
        if($child && is_string($child) && isset($groups[$child])){
          $groups[$key]->children[$child_key] = $groups[$child];
          unset($groups[$child]);
        }
      }
    }
    foreach($groups as $key => $group){
      uasort($groups[$key]->children, '_silver_sort_groups');
      foreach($group->children as $child_key => $child){
        if(is_object($child) && isset($child->children) && is_array($child->children)){
          foreach($child->children as $gchild_key => $gchild){
            if(is_string($gchild) && isset($field_info[$gchild])){
              $groups[$key]->children[$child_key]->children[$gchild_key] = $field_info[$gchild];
              unset($field_info[$gchild]);
            }else{
              unset($groups[$key]->children[$child_key]->children[$gchild_key]);
            }
          }
        }
        if(@is_object($groups[$key]->children[$child_key]->children)){
          uasort($groups[$key]->children[$child_key]->children, '_silver_sort_groups');
        }
      }
    }
    uasort($groups, '_silver_sort_groups');
    foreach(array_keys($groups) as $key){
      foreach(array_keys($groups[$key]->children) as $child_key){
        if(@is_array($groups[$key]->children[$child_key]->children)){
          foreach(array_keys($groups[$key]->children[$child_key]->children) as $gchild_key){
            $field_info[$groups[$key]->children[$child_key]->children[$gchild_key]['field_name']] = $groups[$key]->children[$child_key]->children[$gchild_key];
          }
        }
      }
    }
    return $field_info;
  }
}

/**
 * Helper function which creates the correct mapping for certain field types.
 */
function _silver_feeds_importer_default_helper($field_info, &$feeds_importer, $additional = '', $map_field_collection = FALSE, $field_collection_label_addition = ''){
  if(is_array($field_info)){
    foreach($field_info as $key => $field){
      // Don't add the title_field, as it has already been added (or not depending
      // on settings).
      if($key == 'title_field'){
        continue;
      }
      $field_info = field_info_field($field['field_name']);
      switch($field_info['type']){
        case 'file':
          // We allow importing from a file, or a URL.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . trim($field['label']) . ' (Filename)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':MediaFeedsLibraryProvider' : $key . ':MediaFeedsLibraryProvider',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . trim($field['label']) . ' (URL)',
            'target' => $map_field_collection ? $map_field_collection . $key : $key,
            'unique' => 0
          );
          break;
        case 'field_collection':
          // We get the info about the field collection, and add a mapping for
          // each of the fields in it.
          if(!$map_field_collection){
            _silver_feeds_importer_default_helper(field_info_instances('field_collection_item', $field_info['field_name']), $feeds_importer, $field_info['field_name'] . ':field_collection_item:' . $field_info['field_name'] . ':', $field_info['field_name'] . ':', $field['label'] . ': ');
          }
          break;
        case 'country':
          // We use iso3 country codes
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('ISO alpha-2') . ')',
            'target' => $additional . $key . ':iso2',
            'unique' => 0
          );
          break;
        case 'date':
        case 'datetime':
          // Add the from date, and also the to date if required.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('Start') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':start' : $key . ':start',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('End') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':end' : $key . ':end',
            'unique' => 0
          );
          break;
        case 'link_field':
          // We add two columns: 'title' and 'url'
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('TITLE') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':title' : $key . ':title',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('URL') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':url' : $key . ':url',
            'unique' => 0
          );
          break;
        case 'user_reference':
          // We add three columns for the user reference field.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (GUID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':guid:duplicates' : $key . ':guid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (UID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':uid:duplicates' : $key . ':uid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('Name on site') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':name:duplicates' : $key . ':name:duplicates',
            'unique' => 0
          );
          break;
        case 'node_reference':
          // We add three columns for the node reference field.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (GUID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':guid:duplicates' : $key . ':guid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (NID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':nid:duplicates' : $key . ':nid:duplicates',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('Title') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key . ':title:duplicates' : $key . ':title:duplicates',
            'unique' => 0
          );
          break;
        case 'taxonomy_term_reference':
          // We add three columns for the term reference field.
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (' . t('Name') . ')',
            'target' => $map_field_collection ? $map_field_collection . $key : $key,
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (TID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':tid' : $key . ':tid',
            'unique' => 0
          );
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . $field['label'] . ' (GUID)',
            'target' => $map_field_collection ? $map_field_collection . $key . ':guid' : $key . ':guid',
            'unique' => 0
          );
          break;
        default:
          $feeds_importer->config['processor']['config']['mappings'][] = array(
            'source' => $field_collection_label_addition . trim($field['label']),
            'target' => $map_field_collection ? $map_field_collection . $key : $key,
            'unique' => 0
          );
      }
    }
  }
}

function _silver_sort_groups($a, $b){
  $a_weight = (is_object($a) && isset($a->weight)) ? $a->weight : 0;
  $b_weight = (is_object($b) && isset($b->weight)) ? $b->weight : 0;
  if($a_weight == $b_weight){return 0;}
  return ($a_weight < $b_weight) ? -1 : 1;
}