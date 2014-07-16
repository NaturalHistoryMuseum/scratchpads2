<?php

/**
 * Simplest processor of them all - just return the raw value.
 */
function dwcarchiver_processor_dwcarchiver_raw_text(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $field_info = field_info_field($field);
  if($field_info){
    $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
    $query->addExpression("table_{$map_id}.{$field}_value");
  }else{
    $query->addExpression("{$table}.{$field}");
  }
}

/**
 * A little trickier this, as the labels are only available in PHP (they're
 * serialized in the database).  We therefore get all possible values, and craft
 * the query accordingly.
 */
function dwcarchiver_processor_dwcarchiver_list(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){}

/**
 * Return the latitude
 */
function dwcarchiver_processor_gm3_latitude(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_latitude");
}

/**
 * Return the longitude
 */
function dwcarchiver_processor_gm3_longitude(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_longitude");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_from(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_value");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_to(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_value2");
}

/**
 * Return a URL for an entity.  Note, I'm not sure if this will work for all 
 * entity types, so possibly needs testing.
 */
function dwcarchiver_processor_dwcarchiver_content_url(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $url = url('', array(
    'absolute' => TRUE
  ));
  if(isset($entity_info['uri callback']) && function_exists($entity_info['uri callback'])){
    $entity = new stdClass();
    $entity->{$entity_info['entity keys']['id']} = '';
    $uri = call_user_func($entity_info['uri callback'], $entity);
  }
  $query->addExpression("CONCAT('{$url}{$uri['path']}', {$table}.{$entity_info['entity keys']['id']})");
}

/**
 * Add a simple expression for a "value".
 */
function dwcarchiver_processor_dwcarchiver_value(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $value = db_select('dwcarchiver_value', 'd')->fields('d', array(
    'value'
  ))->condition('eid', $map->eid)->condition('did', $map->did)->condition('delta', $map->delta)->condition('field', $map->field)->execute()->fetchField();
  $query->addExpression("'{$value}'");
}

/**
 * 
function dwcarchiver_processor_dwcarchiver_path(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){}
 */
/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_creative_commons(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $expression = '(CASE ';
  foreach(creative_commons_get_licence_types() as $id => $text){
    if(is_numeric($id)){
      // Clean up text to ensure we can include it in the query.
      $text = Database::getConnection('default')->quote($text);
      $expression .= " WHEN table_{$map_id}.{$field}_licence = {$id} THEN {$text}";
    }
  }
  $expression .= ' END)';
  $query->addExpression($expression);
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_biblio(SelectQuery $query, $field, $entity_info, $map_id, $map, $table = 'base'){
  // Note, biblio is always associated with a node, so we can link via nid.
  // FIXME - Need to fix this so that ALL biblio fields are handled.  Need to
  // check which ones don't currently work.
  switch($field){
    default:
      static $joined_biblio = FALSE;
      if(!$joined_biblio){
        $joined_biblio = $query->leftJoin('biblio', "table_{$map_id}", "{$table}.vid = table_{$map_id}.vid");
      }
      $query->addExpression("{$joined_biblio}.{$field}");
      break;
    case 'biblio_authors_field':
      // FIXME - May have to add a table that concatenates this for us - this is
      // far too slow.
      $query->leftJoin('biblio_contributor', "table_{$map_id}", "{$table}.vid = table_{$map_id}.vid");
      $query->leftJoin('biblio_contributor_data', "table_{$map_id}_data", "table_{$map_id}.cid = table_{$map_id}_data.cid");
      $query->addExpression("GROUP_CONCAT(DISTINCT table_{$map_id}_data.name)");
      $query->groupBy("table_{$map_id}.vid");
      //$query->addExpression("table_{$map_id}_data.name");
      break;
  }
}