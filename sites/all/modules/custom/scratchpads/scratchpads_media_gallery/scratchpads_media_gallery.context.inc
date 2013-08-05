<?php
/**
* @file
* scratchpads_media_gallery.context.inc
*/

/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_media_gallery_context_default_contexts(){
  $export = array();
  $context = new stdClass();
  $context->disabled = FALSE;
  $context->api_version = 3;
  $context->name = 'media_gallery_page_facets';
  $context->description = t('Media gallery page facets');
  $context->tag = 'solr';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'gallery*' => 'gallery*'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array()
    )
  );
  module_load_include('block.inc', 'facetapi');
  $map = facetapi_get_delta_map();
  foreach(scratchpads_media_gallery_facetapi_default_facet_settings() as $facet){
    $delta = array_search($facet->name, $map);
    if($delta){
      $context->reactions['block']['blocks'][] = array(
        'module' => 'facetapi',
        'delta' => $delta,
        'region' => 'sidebar',
        'weight' => -27
      );
    }
  }
  $context->condition_mode = 0;
  $export['media_gallery_page_facets'] = $context;
  return $export;
}