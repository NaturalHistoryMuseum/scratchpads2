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
  $order = array(
    'im_field_smg_galleries' => '1',
    'im_field_smg_galleries_global' => '2',
    'ss_field_creator' => '3',
    'ss_field_creator_global' => '4',
    'author' => '5',
    'author_global' => '6',
    'tus_type_name' => '7',
    'tus_type_name_global' => '8',
    'im_field_keywords' => '9',
    'im_field_keywords_global' => '10',
    'im_field_cc_licence' => '11',
    'im_field_cc_licence_global' => '12',
    'im_field_imaging_technique' => '13',
    'im_field_imaging_technique_global' => '14',
    'im_field_preparation_technique' => '15',
    'im_field_preparation_technique_global' => '16'
  );
  foreach(scratchpads_media_gallery_facetapi_default_facet_settings() as $key => $facet){
    if (isset($order[$key])){
      $weight = $order[$key];
    }
    else{
      $weight = 20;
    }
    $delta = array_search($facet->name, $map);
    if($delta){
      $context->reactions['block']['blocks'][] = array(
        'module' => 'facetapi',
        'delta' => $delta,
        'region' => 'sidebar',
        'weight' => $weight
      );
    }
  }
  $context->condition_mode = 0;
  $export['media_gallery_page_facets'] = $context;
  return $export;
}