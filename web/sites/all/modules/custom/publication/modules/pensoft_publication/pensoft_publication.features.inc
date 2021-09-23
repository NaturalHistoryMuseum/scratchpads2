<?php

/**
 * @file
 * pensoft_publication.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function pensoft_publication_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "field_group" && $api == "field_group"){return array(
      "version" => "1"
    );}
  list($module, $api) = func_get_args();
  if($module == "relation" && $api == "relation_type_default"){return array(
      "version" => "1"
    );}
}

/**
 * Implements hook_views_api().
 */
function pensoft_publication_views_api(){
  list($module, $api) = func_get_args();
  if($module == "views" && $api == "views_default"){return array(
      "version" => "3.0"
    );}
}

/**
 * Implements hook_default_publication_type().
 */
function pensoft_publication_default_publication_type(){
  $items = array();
  $items['pensoft'] = entity_import('publication_type', '{
    "type" : "pensoft",
    "label" : "Taxonomic paper (Pensoft Writing Tool)",
    "weight" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
