<?php

// $Id$
/**
 * Implements hook_views_data().
 */
function efq_views_views_data(){
  $data['efq']['table']['group'] = t('EntityFieldQuery');
  // Add a EFQ base table for each defined entity
  foreach(entity_get_info() as $type => $info){
    $data['efq_' . $type]['table']['base'] = array(
      'title' => 'EntityFieldQuery: ' . $info['label'],
      'help' => t('Uses EntityFieldQuery for querying the !type entity type', array(
        '!type' => $type
      )),
      'query class' => 'efq_query',
      'field' => $info['entity keys']['id']
    );
    $data['efq_' . $type] += _efq_views_get_entity_data($type, $info);
    $data['efq_' . $type] += _efq_views_get_field_data($type, $info);
    $data['efq_' . $type] += _efq_views_get_property_data($type, $info);
    // Entity: Bundle is not supported for these two entity types.
    if($type == 'comment' || $type == 'taxonomy_term'){
      unset($data['efq_' . $type]['bundle']);
    }
    // Base tables with 'entity type' defined get the entity row plugin added
    // by the Entity API. Nodes and comments already have their own row plugins.
    if(!in_array($type, array(
      'node',
      'comment'
    ))){
      $data['efq_' . $type]['table']['base']['entity type'] = $type;
    }
  }
  // Now add the EFQ: Multiple base table, for querying across
  // multiple entity types.
  $data['efq_multiple']['table']['base'] = array(
    'title' => t('EntityFieldQuery: Multiple'),
    'help' => t('Uses EntityFieldQuery for querying multiple entity types.'),
    'query class' => 'efq_query',
    'field' => ''
  );
  $data['efq_multiple'] += _efq_views_get_entity_data();
  $data['efq_multiple'] += _efq_views_get_field_data();
  $data['efq_multiple'] += _efq_views_get_property_data();
  return $data;
}

/**
 * Helper for efq_views_views_data()
 * Returns all possible entity metadata variants
 * (entity_id, revision_id, entity_type, bundle).
 *
 * @param $entity_type The selected entity type, if any.
 * @param $info Entity info array.
 */
function _efq_views_get_entity_data($entity_type = NULL, $info = NULL){
  $data['entity_id'] = array(
    'group' => t('Entity'),
    'title' => t('Entity ID'),
    'help' => t('The entity ID of the entity.'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'efq_views_handler_field_entity'
    ),
    'filter' => array(
      'handler' => 'efq_views_handler_filter_entity_numeric'
    ),
    'sort' => array(
      'handler' => 'efq_views_handler_sort_entity'
    ),
    'argument' => array(
      'handler' => 'efq_views_handler_argument_entity_numeric'
    )
  );
  $data['revision_id'] = array(
    'group' => t('Entity'),
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the entity revision.'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'efq_views_handler_field_entity'
    ),
    'filter' => array(
      'handler' => 'efq_views_handler_filter_entity_numeric'
    ),
    'sort' => array(
      'handler' => 'efq_views_handler_sort_entity'
    ),
    'argument' => array(
      'handler' => 'efq_views_handler_argument_entity_numeric'
    )
  );
  if(!isset($entity_type)){
    $data['entity_type'] = array(
      'group' => t('Entity'),
      'title' => t('Entity Type'),
      'help' => t('The type of an entity (for example, "node", "comment", etc)..'),
      'field' => array(
        'click sortable' => TRUE,
        'handler' => 'efq_views_handler_field_entity'
      ),
      'filter' => array(
        'handler' => 'efq_views_handler_filter_entity_type'
      ),
      'sort' => array(
        'handler' => 'efq_views_handler_sort_entity'
      ),
      'argument' => array(
        'handler' => 'efq_views_handler_argument_entity_type'
      )
    );
  }
  $data['bundle'] = array(
    'group' => t('Entity'),
    'title' => t('Bundle'),
    'help' => t('The entity bundle (for example, "article", "page", etc for nodes).'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'efq_views_handler_field_entity'
    ),
    'filter' => array(
      'entity_type' => $entity_type,
      'handler' => 'efq_views_handler_filter_entity_bundle'
    ),
    'sort' => array(
      'handler' => 'efq_views_handler_sort_entity'
    ),
    'argument' => array(
      'handler' => 'efq_views_handler_argument_entity_bundle'
    )
  );
  $data['label'] = array(
    'group' => t('Entity'),
    'title' => t('Label'),
    'help' => t('The entity label (for example, node title for nodes).'),
    'field' => array(
      'click sortable' => FALSE,
      'handler' => 'efq_views_handler_field_entity_label'
    ),
  );

  
  // We can only do click sorting and filters/sorts/arguments when the label is
  // stored in the database, instead of being generated in a callback.
  if(isset($info['entity keys']['label']) && !isset($info['label callback'])){
    $data['label']['field'] = array(
      'click sortable' => TRUE,
      'handler' => 'efq_views_handler_field_entity_label',
      'label column' => $info['entity keys']['label']
    );
    $data['label']['filter'] = array(
      'handler' => 'efq_views_handler_filter_entity_label',
      'label column' => $info['entity keys']['label']
    );
    $data['label']['sort'] = array(
      'handler' => 'efq_views_handler_sort_entity_label',
      'label column' => $info['entity keys']['label']
    );
    $data['label']['argument'] = array(
      'handler' => 'efq_views_handler_argument_entity_label',
      'label column' => $info['entity keys']['label']
    );
  }
  return $data;
}

/**
 * Helper for efq_views_views_data()
 * Returns defined fields & their columns for the given entity type
 * (or, if absent, all entity types).
 *
 * Based on field_views_field_default_views_data() from field.views.inc
 *
 * @param $entity_type The selected entity type, if any.
 * @param $info Entity info array.
 */
function _efq_views_get_field_data($entity_type = NULL, $info = NULL){
  $data = array();
  foreach(field_info_fields() as $field){
    // This field is not attached to any bundle of our content type.
    if(isset($entity_type) && !isset($field['bundles'][$entity_type])){
      continue;
    }
    $bundles_names = array();
    foreach($field['bundles'] as $type => $bundles){
      foreach($bundles as $bundle){
        $bundles_names[] = t('@entity:@bundle', array(
          '@entity' => $type,
          '@bundle' => $bundle
        ));
      }
    }
    // Note: we don't have a label available here, because we are at the field
    // level, not at the instance level. So we just go through all instances
    // and take the one which is used the most frequently.
    $field_name = $field['field_name'];
    list($label, $all_labels) = field_views_field_label($field_name);
    $group_name = isset($info) ? $info['label'] : t('Fields');
    // Override Node to Content.
    if($group_name == t('Node')){
      $group_name = t('Content');
    }
    // This provides a Views field.
    // Arguments/filters/sorts are provided per column, so there are none here.
    $data[$field_name] = array(
      'group' => $group_name,
      'title' => $label,
      'title short' => $label,
      'help' => t('Appears in: @bundles', array(
        '@bundles' => implode(', ', $bundles_names)
      )),
      'field' => array(
        'click sortable' => TRUE,
        'handler' => 'efq_views_handler_field_field',
        'field_name' => $field_name,
        'entity_type' => $entity_type
      )
    );
    // Add an alias for each alternative field label.
    $aliases = array();
    $also_known = array();
    foreach($all_labels as $entity_name => $labels){
      foreach($labels as $label_name => $true){
        if($label != $label_name){
          $aliases[] = array(
            'group' => $group_name,
            'title' => $label_name,
            'help' => t('This is an alias of @group: @field.', array(
              '@group' => $group_name,
              '@field' => $label
            ))
          );
        }
        $also_known[] = t('@group: @field', array(
          '@group' => $group_name,
          '@field' => $label_name
        ));
      }
    }
    if($aliases){
      $data[$field_name]['aliases'] = $aliases;
      $data[$field_name]['help'] .= ' ' . t('Also known as: !also.', array(
        '!also' => implode(', ', $also_known)
      ));
    }
    foreach($field['columns'] as $column => $attributes){
      $allow_sort = TRUE;
      // Identify likely filters and arguments for each column based on field type.
      switch($attributes['type']){
        case 'int':
        case 'mediumint':
        case 'tinyint':
        case 'bigint':
        case 'serial':
        case 'numeric':
        case 'float':
          $filter = 'efq_views_handler_filter_field_numeric';
          $argument = 'efq_views_handler_argument_field_numeric';
          break;
        case 'text':
        case 'blob':
          // It does not make sense to sort by blob or text.
          $allow_sort = FALSE;
        default:
          $filter = 'efq_views_handler_filter_field_string';
          $argument = 'efq_views_handler_argument_field_string';
          break;
      }
      if(count($field['columns']) == 1 && $column == 'value'){
        $title = t('@label (!name)', array(
          '@label' => $label,
          '!name' => $field_name
        ));
        // CCK used the first 10 characters of $label. Probably doesn't matter.
        $title_short = $field_name;
      }else{
        $title = t('@label (!name) - !column', array(
          '@label' => $label,
          '!name' => $field_name,
          '!column' => $column
        ));
        $title_short = t('@label-truncated - !column', array(
          '@label-truncated' => $field_name,
          '!column' => $column
        ));
      }
      // $data[$field_name][$column] doesn't work because $field_name is not a base table
      // hence, this ugly thing.
      $data[$field_name . '-' . $column] = array(
        'group' => $group_name,
        'title' => $title,
        'title short' => $title_short,
        'help' => t('Appears in: @bundles', array(
          '@bundles' => implode(', ', $bundles_names)
        )),
        'argument' => array(
          'field' => $column,
          'handler' => $argument,
          'field_name' => $field['field_name'],
          'empty field name' => t('<No value>')
        ),
        'filter' => array(
          'field' => $column,
          'handler' => $filter,
          'field_name' => $field_name,
          'allow empty' => TRUE
        )
      );
      if(!empty($allow_sort)){
        $data[$field_name . '-' . $column]['sort'] = array(
          'field' => $column,
          'handler' => 'efq_views_handler_sort_field',
          'field_name' => $field_name
        );
      }
    }
  }
  return $data;
}

/**
 * Helper for efq_views_views_data()
 * Returns the property data.
 *
 * @param $entity_type Entity type for which the properties are being loaded.
 * @param $info Entity info array for the given entity type.
 */
function _efq_views_get_property_data($entity_type = NULL, $info = NULL){
  $data['property'] = array(
    'group' => t('Property'),
    'title' => t('Property'),
    'help' => t('Entity-specific property.'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'efq_views_handler_field_property'
    ),
    'filter' => array(
      'handler' => 'efq_views_handler_filter_property'
    ),
    'sort' => array(
      'handler' => 'efq_views_handler_sort_property'
    ),
    'argument' => array(
      'handler' => 'efq_views_handler_argument_property'
    )
  );
  // No single entity_type specified, we can't find more info
  // through the entity definition. So, return what we have so far.
  if(!isset($entity_type)){return $data;}
  $metadata = entity_get_property_info($entity_type);
  // No metadata available for this entity type.
  if(!isset($metadata['properties'])){return $data;}
  // Provide additional help.
  if(isset($info['schema_fields_sql']['base table'])){
    $types = array();
    $available_properties = $info['schema_fields_sql']['base table'];
    foreach($available_properties as $key => $property){
      if(isset($metadata['properties'][$property])){
        $properties[$property] = $metadata['properties'][$property]['label'];
      }else{
        $properties[$property] = $property;
      }
      if(isset($metadata['properties'][$property]['type'])){
        $types[$property] = $metadata['properties'][$property]['type'];
      }
    }
    $data['property']['field']['available_properties'] = $properties;
    $data['property']['filter']['available_properties'] = $properties;
    $data['property']['sort']['available_properties'] = $properties;
    $data['property']['argument']['available_properties'] = $properties;
    // Store the datatype so that it can be used for casting values in handlers.
    $data['property']['field']['types'] = $types;
    $data['property']['filter']['types'] = $types;
    $data['property']['sort']['types'] = $types;
    $data['property']['argument']['types'] = $types;
  }
  // Get individual properties using Entity Metadata
  $supported_types = array(
    'integer',
    'decimal',
    'duration',
    'date',
    'text',
    'token',
    'boolean',
    'uri'
  );
  foreach($metadata['properties'] as $property => $property_info){
    // Only allow the properties whose type we know how to handle
    if(isset($property_info['type']) && !in_array($property_info['type'], $supported_types)){
      continue;
    }
    // Avoid generating a million notices.
    if(!isset($property_info['type'])){
      $property_info['type'] = '';
    }
    // Identify likely handlers for each property, based on property type.
    switch($property_info['type']){
      case 'integer':
      case 'decimal':
      case 'duration':
        $field = 'efq_views_handler_field_property_numeric';
        $filter = 'efq_views_handler_filter_property_numeric';
        $argument = 'efq_views_handler_argument_property_numeric';
        break;
      case 'boolean':
        // Views proper doesn't let boolean fields have argument handlers,
        // so neither do we.
        $field = 'efq_views_handler_field_property_boolean';
        $filter = 'efq_views_handler_filter_property_boolean';
        $argument = NULL;
        break;
      case 'date':
        $field = 'efq_views_handler_field_property_date';
        $filter = 'efq_views_handler_filter_property_date';
        $argument = NULL;
        break;
      case 'uri':
        // URIs aren't stored in the database, no need for additional handlers.
        $field = 'efq_views_handler_field_property_uri';
        break;
      case 'token':
      case 'text':
      default:
        if(isset($property_info['options list'])){
          $field = 'efq_views_handler_field_property_string';
          $filter = 'efq_views_handler_filter_property_in_operator';
          $argument = 'efq_views_handler_argument_property_string';
          break;
        }
        $field = 'efq_views_handler_field_property_string';
        $filter = 'efq_views_handler_filter_property_string';
        $argument = 'efq_views_handler_argument_property_string';
        break;
    }
    $group_name = $info['label'];
    // Override Node to Content.
    if($group_name == t('Node')){
      $group_name = t('Content');
    }
    $data[$property] = array(
      'group' => $group_name,
      'title' => $property_info['label'],
      'help' => $property_info['description'],
      'field' => array(
        'click sortable' => FALSE,
        'handler' => $field
      )
    );
    // This property is a db column, we can do fun stuff with it.
    if(in_array($property, $info['schema_fields_sql']['base table'])){
      $data[$property]['field'] = array(
        'click sortable' => TRUE,
        'handler' => $field
      );
      $data[$property]['filter'] = array(
        'handler' => $filter,
        'options callback' => isset($property_info['options list']) ? $property_info['options list'] : NULL
      );
      $data[$property]['sort'] = array(
        'handler' => 'efq_views_handler_sort_property'
      );
      if(isset($argument)){
        $data[$property]['argument'] = array(
          'handler' => $argument
        );
      }
    }
  }
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function efq_views_views_plugins(){
  $plugins = array(
    'query' => array(
      'efq_query' => array(
        'title' => t('EntityFieldQuery'),
        'help' => t('Uses EntityFieldQuery for querying entities and fields.'),
        'handler' => 'efq_views_plugin_query'
      )
    )
  );
  return $plugins;
}

/**
 * Implements hook_views_plugins_alter().
 *
 * This allows us to use the node view style with EntityFieldQuery: Node
 * and comment view style with EntityFieldQuery: Comment
 * (just like with regular nodes and comments).
 *
 * Note that these style plugins do an entity_load() even though they already
 * get fully loaded entities from our query engine.
 * However, entity_load has internal caching, so the only real overhead is an
 * extra function call.
 */
function efq_views_views_plugins_alter(&$plugins){
  $plugins['row']['node']['base'][] = "efq_node";
  $plugins['row']['comment']['base'][] = "efq_comment";
}
