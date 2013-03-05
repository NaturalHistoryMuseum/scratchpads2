<?php
function _biblio_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  if ($entity_type == 'node' && $bundle_name == 'biblio') {
    $schema = drupal_get_schema('biblio');
    foreach ($schema['fields'] as $field => $spec) {
      if (strstr($field, 'biblio_')) {
        $type = $spec['type'];
        $length = isset($spec['length']) ? ' ['. $spec['length'] . ']': '';
        $targets[$field] = array(
          'name' => $field . '  (' . $type . $length . ')',
          'description' =>'',
          'callback' => '_biblio_feeds_set__simple_target',
        );
      }
    }
    $targets['biblio_type']['callback'] = '_biblio_feeds_set__type_target';
    $targets['biblio_contributor'] = array(
      'name' => t('biblio_contributor'),
      'description' => t('This is a contributor (author) contained in a biblio entry.'),
      'callback' => '_biblio_feeds_set__contrib_target',
 //     'real_target' => 'my_node_field_two', // Specify real target field on node.
    );
    $targets['biblio_keyword'] = array(
      'name' => t('biblio_keyword'),
      'description' => t('This is a keyword contained in a biblio entry.'),
      'callback' => '_biblio_feeds_set__keyword_target',
 //     'real_target' => 'my_node_field_two', // Specify real target field on node.
    );
  }
}

function _biblio_feeds_set__type_target($source, $entity, $target, $value) {
  static $types = array();

  if (empty($value)) {
    return;
  }

  // Handle non-multiple value fields.
  if (!is_array($value)) {
    $value = array($value);
  }

  if (isset($value[0]) && !empty($value[0])) {
    if (intval($value[0]) > 0) {
      // value[0] is the bibio type ID
      if (empty($types)) {
        $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid > 0');
        foreach ($result as $row) {
          $types[$row->tid] = $row->tid;
        }
      }

      $type_id = $value[0];
      $entity->biblio_type = (isset($types[$type_id])) ? $type_id : 129;
    }
    elseif (is_string($value[0])) {
      // value[0] is the bibio type name
      if (empty($types)) {
        $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid > 0');
        foreach ($result as $row) {
          $types[$row->tid] = str_replace(" ","_", strtolower($row->name));
        }
      }

      $type = array_search($value[0], $types);
      $entity->biblio_type = (!empty($type)) ? $type : 129;
    }
  }
}

function _biblio_feeds_set__simple_target($source, $entity, $target, $value) {
  if (empty($value)) {
    return;
  }

  // Handle non-multiple value fields.
  if (!is_array($value)) {
    $value = array($value);
  }

  if (isset($value[0]) && !empty($value[0])) {
    $entity->$target = $value[0];
    if ($target == 'biblio_abst_e') {
      $entity->biblio_formats[$target] = 'full_html';
    }
  }
}

function _biblio_feeds_set__contrib_target($source, $entity, $target, $value) {
  if (is_string($value)) {
    $value = explode('||', $value);
  }
  foreach ($value as $author) {
    $entity->biblio_contributors[] =  array(
            'name' => $author,
            'auth_category' => 1,
          );
  }
}

function _biblio_feeds_set__keyword_target($source, $entity, $target, $value) {
  if (!empty($value)) {
    $entity->biblio_keywords = $value;
  }
}

function _biblio_feeds_oai_importer_default() {

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'biblio_oai_pmh';
  $feeds_importer->config = array(
    'name' => 'Biblio OAI-PMH',
    'description' => 'Import an OAI-PMH feed into the Biblio node type.',
    'fetcher' => array(
      'plugin_key' => 'FeedsOAIHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'last_fetched_timestamp' => '',
        'earliest_timestamp' => '',
        'use_dates' => FALSE,
        'to' => array(),
        'from' => array(),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsOAIParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'biblio',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'publisher',
            'target' => 'biblio_publisher',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'subject',
            'target' => 'biblio_keyword',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'source',
            'target' => 'biblio_secondary_title',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          5 => array(
            'source' => 'creator',
            'target' => 'biblio_contributor',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'description',
            'target' => 'biblio_abst_e',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'contributor',
            'target' => 'biblio_contributor',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'identifier',
            'target' => 'biblio_url',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'date',
            'target' => 'biblio_year',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'setspec_raw',
            'target' => 'biblio_type',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );

  return array(
    'biblio_oai' => $feeds_importer
  );
}