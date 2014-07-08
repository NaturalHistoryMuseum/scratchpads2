<?php

/**
 * Simplest processor of them all - just return the raw value.
 */
function dwcarchiver_processor_dwcarchiver_raw_text($query, $map, $entity_info, $map_id){
  // Add the join
  $query->leftJoin('field_data_' . $map->field, $map_id, 'base.' . $entity_info['entity keys']['id'] . ' = ' . $map_id . '.entity_id');
  $query->addExpression($map_id . '.' . $map->field . '_value');
}

/**
 * A little trickier this, as the labels are only available in PHP (they're
 * serialized in the database).  We therefore get all possible values, and craft
 * the query accordingly.
 */
function dwcarchiver_processor_dwcarchiver_list($query, $map, $entity_info, $map_id){}

/**
 * Return the latitude
 */
function dwcarchiver_processor_gm3_latitude($query, $map, $entity_info, $map_id){}

/**
 * Return the longitude
 */
function dwcarchiver_processor_gm3_longitude($query, $map, $entity_info, $map_id){}