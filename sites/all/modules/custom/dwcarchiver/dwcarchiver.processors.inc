<?php

/**
 * Simplest processor of them all - just return the raw value.
 */
function dwcarchiver_processor_dwcarchiver_raw_text(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $field_info = field_info_field($field);
  if($field_info){
    $entity_info = entity_get_info($entity_type);
    _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
    $query->addExpression("REPLACE(REPLACE(table_{$map_id}.{$field}_value, '\r', ''), '\n', ' ')");
  }else{
    $query->addExpression("REPLACE(REPLACE({$table}.{$field}, '\r', ''), '\n', ' ')");
  }
}

/**
 * Second simplest processor of them all - groups together values
 */
function dwcarchiver_processor_dwcarchiver_raw_text_group_concat(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $field_info = field_info_field($field);
  $query->groupBy('base.uuid');
  if($field_info){
    $entity_info = entity_get_info($entity_type);
    _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
    $query->addExpression("GROUP_CONCAT(REPLACE(REPLACE(table_{$map_id}.{$field}_value, '\r', ''), '\n', ' ') SEPARATOR ';')");
  }else{
    $query->addExpression("GROUP_CONCAT(REPLACE(REPLACE({$table}.{$field}, '\r', ''), '\n', ' ') SEPARATOR ';')");
  }
}

/**
 * Similar to the above processor, but this concatenates a value to it.
 * 
 */
function dwcarchiver_processor_dwcarchiver_raw_text_concatenate_value_after(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $concat_items = array();
  $field_info = field_info_field($field);
  if($field_info){
    $entity_info = entity_get_info($entity_type);
    _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
    $concat_items[] = "table_{$map_id}.{$field}_value";
  }else{
    $concat_items[] = "{$table}.{$field}";
  }
  if(isset($map->value)){
    $concat_items[] = "'{$map->value}'";
  }else{
    $value = db_select('dwcarchiver_value', 'd')->fields('d', array(
      'value'
    ))->condition('eid', $map->eid)->condition('did', $map->did)->condition('delta', $map->delta)->condition('field', $map->field)->execute()->fetchField();
    $concat_items[] = "'$value'";
  }
  $query->addExpression('CONCAT(' . implode(',', $concat_items) . ')');
}

/**
 * Special processor (takes different args) for mapping back to the core 
 * entity.
 */
function dwcarchiver_processor_core_map($archive_or_extension, $dwcarchiver, SelectQuery $query){
  if(!isset($archive_or_extension->extensions)){
    $entity_info = entity_get_info($archive_or_extension->entity);
    $field_info = field_info_field($archive_or_extension->field_name);
    switch($field_info['type']){
      case 'field_collection':
        $query->innerJoin('field_data_' . $archive_or_extension->field_name, 'table_core_link', "table_core_link.{$archive_or_extension->field_name}_value = base.{$entity_info['entity keys']['id']}");
        foreach(array_keys($field_info['bundles']) as $entity_type){
          switch($entity_type){
            default:
              $join_entity_info = entity_get_info($entity_type);
              $query->innerJoin($join_entity_info['base table'], 'table_core', "table_core_link.entity_id = table_core.{$join_entity_info['entity keys']['id']}");
              break;
          }
          break;
        }
        $query->addExpression('table_core.uuid');
        break;
      case 'taxonomy_term_reference':
        $query->innerJoin('field_data_' . $archive_or_extension->field_name, 'table_core_link', "table_core_link.entity_id = base.{$entity_info['entity keys']['id']}");
        $query->innerJoin('taxonomy_term_data', 'table_core', "table_core_link.{$archive_or_extension->field_name}_tid = table_core.tid");
        $query->addExpression('table_core.uuid');
        break;
      default:
        // FIXME.
        drupal_set_message(t('You have attempted to create a link to an entity that is not currently supported, please contact your site administrator.'), 'error');
        break;
    }
  }else{
    $query->addExpression('base.uuid');
  }
}

/**
 * Convert a Mime type to DCMI Type
 */
function dwcarchiver_processor_dwcarchiver_mime_to_dcmitype(SelectQuery $query, $field, $entity_type, $map_id, $map, $table){
  $field_info = field_info_field($field);
  if($field_info){
    _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
    $column = "table_{$map_id}.{$field}_value";
  }else{
    $column = "{$table}.{$field}";
  }
  // FIXME - Add additional DCMI types.
  // http://purl.org/dc/dcmitype/Collection
  // http://purl.org/dc/dcmitype/Dataset
  // http://purl.org/dc/dcmitype/Event
  // http://purl.org/dc/dcmitype/InteractiveResource
  // http://purl.org/dc/dcmitype/PhysicalObject
  // http://purl.org/dc/dcmitype/Service
  // http://purl.org/dc/dcmitype/Software
  // http://purl.org/dc/dcmitype/StillImage
  $mimes = array(
    'application/%' => '
  http://purl.org/dc/dcmitype/Dataset',
    'audio/%' => 'http://purl.org/dc/dcmitype/Sound',
    'image/%' => 'http://purl.org/dc/dcmitype/StillImage',
    'text/%' => 'http://purl.org/dc/dcmitype/Text',
    'video/%' => 'http://purl.org/dc/dcmitype/MovingImage'
  );
  $expressions = array();
  foreach($mimes as $mime => $dcmi){
    $expressions[] = "WHEN $column LIKE '$mime' THEN '$dcmi'";
  }
  $expressions[] = "ELSE 'http://purl.org/dc/dcmitype/Dataset'";
  $query->addExpression('(CASE ' . implode(' ', $expressions) . ' END)');
}

/**
 * Helper function to join to a field table.
 */
function _dwcarchiver_processor_join_field_table(SelectQuery $query, $field, $entity_type, $map_id, $map, $table){
  $entity_info = entity_get_info($entity_type);
  if($map->required){
    $query->innerJoin('field_data_' . $field, "table_{$map_id}", "({$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id AND table_{$map_id}.entity_type = '{$entity_type}')");
  }else{
    $query->leftJoin('field_data_' . $field, "table_{$map_id}", "({$table}.{$entity_info['entity keys']['id']} = table_{$map_id}.entity_id AND table_{$map_id}.entity_type = '{$entity_type}')");
  }
}

/**
 * A little trickier this, as the labels are only available in PHP (they're
 * serialized in the database).  We therefore get all possible values, and craft
 * the query accordingly.
function dwcarchiver_processor_dwcarchiver_list(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){}
 */
/**
 * Return the latitude
 */
function dwcarchiver_processor_gm3_latitude(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_latitude");
}

/**
 * Return the longitude
 */
function dwcarchiver_processor_gm3_longitude(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_longitude");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_from(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_value");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_link(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_url");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_date_to(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_value2");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_timestamp(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $query->addExpression("FROM_UNIXTIME({$table}.$field)");
}

/**
 * Return a URL for an entity.  Note, I'm not sure if this will work for all 
 * entity types, so possibly needs testing.
 */
function dwcarchiver_processor_dwcarchiver_content_url(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $url = url('', array(
    'absolute' => TRUE
  ));
  $entity_info = entity_get_info($entity_type);
  if($table != 'base'){
    $field_info = field_info_field($table);
    if($field_info){
      $referencable_type = str_replace('_reference', '', $field_info['type']);
      $entity_info = entity_get_info($referencable_type);
    }
  }
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
function dwcarchiver_processor_dwcarchiver_value(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  if(isset($map->value)){
    $query->addExpression("'{$map->value}'");
  }else{
    $value = db_select('dwcarchiver_value', 'd')->fields('d', array(
      'value'
    ))->condition('eid', $map->eid)->condition('did', $map->did)->condition('delta', $map->delta)->condition('field', $map->field)->execute()->fetchField();
    $query->addExpression("'$value'");
  }
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_path(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $public_url = file_create_url('public://');
  $private_url = file_create_url('private://');
  $query->addExpression("(CASE WHEN {$table}.uri LIKE 'public://%' THEN CONCAT('{$public_url}', SUBSTRING({$table}.uri, 10)) WHEN {$table}.uri LIKE 'private://%' THEN CONCAT('{$private_url}', SUBSTRING({$table}.uri, 11)) END)");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_creative_commons(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
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
function dwcarchiver_processor_dwcarchiver_creative_commons_url(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $jurisdiction = variable_get('creative_commons_block_jurisdiction', '');
  if($jurisdiction && strpos($jurisdiction, '_')){
    list($jurisdiction, $version) = explode('_', $jurisdiction);
    $version = substr($version, 0, 1) . '.' . substr($version, 1, 1);
  }else{
    $version = CC_DEFAULT_VERSION;
  }
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $expression = '(CASE ';
  foreach(creative_commons_get_licence_types() as $id => $text){
    switch($id){
      // These licence types are text only
      case CC_NONE:
        $url = '';
        break;
      case CC_0:
        $url = 'http://creativecommons.org/about/cc0';
        break;
      case CC_PD:
        $url = 'http://creativecommons.org/about/pdm';
        break;
      case CC_BY:
        $url = CC_LICENCE_URI . '/by/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
      case CC_BY_SA:
        $url = CC_LICENCE_URI . '/by-sa/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
      case CC_BY_ND:
        $url = CC_LICENCE_URI . '/by-nd/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
      case CC_BY_NC:
        $url = CC_LICENCE_URI . '/by-nc/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
      case CC_BY_NC_SA:
        $url = CC_LICENCE_URI . '/by-nc-sa/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
      case CC_BY_NC_ND:
        $url = CC_LICENCE_URI . '/by-nc-nd/' . $version . '/' . ($jurisdiction ? $jurisdiction : '');
        break;
    }
    $url = Database::getConnection('default')->quote($url);
    $expression .= " WHEN table_{$map_id}.{$field}_licence = {$id} THEN {$url}";
  }
  $expression .= ' END)';
  $query->addExpression($expression);
}

/**
 * dwcarchiver_processor_dwcarchiver_content_language
 * 
 * This may need a little more processing, perhaps converting "und" or "en" into
 * "undefined" and "English" respectively.
 */
function dwcarchiver_processor_dwcarchiver_content_language(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $query->addExpression("{$table}.language");
}

/**
 * Processor for the language field.
 */
function dwcarchiver_processor_dwcarchiver_language_field(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  $entity_info = entity_get_info($entity_type);
  _dwcarchiver_processor_join_field_table($query, $field, $entity_type, $map_id, $map, $table);
  $query->addExpression("table_{$map_id}.{$field}_value");
}

/**
 * 
 */
function dwcarchiver_processor_dwcarchiver_biblio(SelectQuery $query, $field, $entity_type, $map_id, $map, $table = 'base'){
  // Note, biblio is always associated with a node, so we can link via nid.
  // FIXME - Need to fix this so that ALL biblio fields are handled.  Need to
  // check which ones don't currently work.
  switch($field){
    default:
      $joined_biblio = dwcarchiver_processor_dwcarchiver_biblio_join_biblio($query, $map_id, $table);
      $query->addExpression("{$joined_biblio}.{$field}");
      break;
    case 'citation':
      $query->leftJoin('dwcarchiver_biblio_citation', "table_{$map_id}", "{$table}.nid = table_{$map_id}.nid");
      $query->addExpression("table_{$map_id}.citation");
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
    case 'biblio_date':
    case 'biblio_year':
      $joined_biblio = dwcarchiver_processor_dwcarchiver_biblio_join_biblio($query, $map_id, $table);
      $query->addExpression("COALESCE({$joined_biblio}.{$field}, {$joined_biblio}.biblio_date, {$joined_biblio}.biblio_year)");
      break;
    case 'biblio_type':
      //return;
      $joined_biblio = dwcarchiver_processor_dwcarchiver_biblio_join_biblio($query, $map_id, $table);
      $query->leftJoin('biblio_types', "biblio_types_{$map_id}", "{$joined_biblio}.biblio_type = biblio_types_{$map_id}.tid");
      $query->addExpression("biblio_types_{$map_id}.name");
      break;
  }
}

/**
 * Simple helper function to join to the biblio table.
 */
function dwcarchiver_processor_dwcarchiver_biblio_join_biblio($query, $map_id, $table){
  static $joined_biblio = FALSE;
  if(!$query->hasTag('biblio_joined')){
    $query->addTag('biblio_joined');
    $joined_biblio = $query->leftJoin('biblio', "table_{$map_id}", "{$table}.vid = table_{$map_id}.vid");
  }
  return $joined_biblio;
}