<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_pages_context_default_contexts(){
  $display_settings = scratchpads_pages_variable_get('scratchpads_pages_display');
  foreach(node_type_get_types() as $type => $node_info){
    if(!isset($display_settings[$type]) || $display_settings[$type] == 'view' || $display_settings[$type] == 'solr'){
      $context = new stdClass();
      $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
      $context->api_version = 3;
      $context->name = $type . '_node';
      $context->description = 'Context activated on ' . $type . ' node view';
      $context->tag = 'Scratchpads node';
      $context->conditions = array(
        'node' => array(
          'values' => array(
            $type => $type
          ),
          'options' => array(
            'node_form' => '0'
          )
        )
      );
      $context->reactions = array(
        'block' => array(
          'blocks' => array(
            'scratchpads_pages-signpost' => array(
              'module' => 'scratchpads_pages',
              'delta' => 'signpost',
              'region' => 'sidebar',
              'weight' => '-10'
            )
          )
        ),
        'menu' => $type
      );
      $context->condition_mode = 0;
      $contexts[$context->name] = $context;
    }
  }
  ;
  return $contexts;
}