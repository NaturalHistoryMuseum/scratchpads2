<?php

/**
 * Simplest processor of them all - just return the raw value.
 */
function dwcarchiver_processor_raw($query, $map, $entity_info, $map_id){
  // Add the join
  $query->leftJoin('field_data_' . $map->field, $map_id, 'base.' . $entity_info['entity keys']['id'] . ' = ' . $map_id . '.entity_id');
  $query->addExpression($map_id . '.' . $map->field . '_value');
}