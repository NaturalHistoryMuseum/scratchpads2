<?php

/**
 * Implements hook_context_default_contexts().
 */
function scratchpads_simple_taxonomy_page_context_default_contexts(){
  $export = array();
  foreach(variable_get('none_biological_vids', array()) as $vid => $use){
    if($use){
      $vocabulary = taxonomy_vocabulary_load($vid);
      if($vocabulary){
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
      }else{
        // This vocabulary must have been deleted.  We remove it from the list
        // of vocabularies.
        $values = variable_get('none_biological_vids', array());
        unset($values[$vid]);
        variable_set('none_biological_vids', $values);
      }
    }
  }
  // Taxonomy images context
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'taxonomy_files';
  $context->description = '';
  $context->tag = 'taxonomy';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'simpletaxonomy/*' => 'simpletaxonomy/*'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-aab5a3c124fdc5f353a0194e2041f1d9' => array(
          'module' => 'views',
          'delta' => 'aab5a3c124fdc5f353a0194e2041f1d9',
          'region' => 'content',
          'weight' => '-10'
        )
      )
    )
  );
  $context->condition_mode = 0;
  $export[$context->name] = $context;
  // Translatables
  // Included for use with string extractors like potx.
  t('Taxonomy');
  return $export;
}
