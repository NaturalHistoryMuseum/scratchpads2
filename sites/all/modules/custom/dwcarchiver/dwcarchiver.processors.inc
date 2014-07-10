<?php

/**
 * Simplest processor of them all - just return the raw value.
 */
function dwcarchiver_processor_dwcarchiver_raw_text($query, $field, $entity_info, $map_id, $table = 'base'){
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
function dwcarchiver_processor_dwcarchiver_list($query, $field, $entity_info, $map_id, $table = 'base'){}

/**
 * Return the latitude
 */
function dwcarchiver_processor_gm3_latitude($query, $field, $entity_info, $map_id, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_latitude");
}

/**
 * Return the longitude
 */
function dwcarchiver_processor_gm3_longitude($query, $field, $entity_info, $map_id, $table = 'base'){
  $query->leftJoin('field_data_' . $field, "table_{$map_id}", "{$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id");
  $query->addExpression("table_{$map_id}.{$field}_longitude");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_from($query, $field, $entity_info, $map_id, $table = 'base'){}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_to($query, $field, $entity_info, $map_id, $table = 'base'){}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_path($query, $field, $entity_info, $map_id, $table = 'base'){}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_creative_commons($query, $field, $entity_info, $map_id, $table = 'base'){}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_biblio($query, $field, $entity_info, $map_id, $table = 'base'){}