<?php

/**
 * Implements hook_context_default_contexts().
 */
function scratchpads_simple_taxonomy_page_context_default_contexts(){
  $export = array();
  foreach(variable_get('none_biological_vids', array()) as $vid => $use){
    if($use){
      $vocabulary = taxonomy_vocabulary_load($vid);
      $context = new stdClass();
      $context->disabled = FALSE;
      $context->api_version = 3;
      $context->name = 'solr_search_page_' . $vocabulary->machine_name;
      $context->description = t('Display tinytax on a @vocabulary term page', array(
        '@vocabulary' => $vocabulary->name
      ));
      $context->tag = 'scratchpads';
      $context->conditions = array(
        'scratchpads_simple_taxonomy_page_context_condition' => array(
          'values' => array(
            $vocabulary->machine_name => $vocabulary->machine_name
          )
        )
      );
      $context->reactions = array(
        'block' => array(
          'blocks' => array(
            'tinytax-' . $vocabulary->vid => array(
              'module' => 'tinytax',
              'delta' => $vocabulary->vid,
              'region' => 'sidebar',
              'weight' => '-100'
            )
          )
        )
      );
      $context->condition_mode = 0;
      $export[$context->name] = $context;
    }
  }
  return $export;
}
