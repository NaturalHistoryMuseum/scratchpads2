<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_species_context_default_contexts(){
  global $conf;
  $contexts = array();
  if(isset($conf['biological_vids'])){
    foreach(taxonomy_get_vocabularies() as $vocabulary){
      if(array_key_exists($vocabulary->vid, $conf['biological_vids'])){
        $name = 'species_' . $vocabulary->machine_name;
        $context = new stdClass();
        $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
        $context->api_version = 3;
        $context->name = $name;
        $context->description = t('!name species page.', array(
          '!name' => $vocabulary->name
        ));
        $context->tag = 'Species';
        $context->conditions = array(
          'taxonomy_term' => array(
            'values' => array(
              $vocabulary->machine_name => $vocabulary->machine_name
            ), 
            'options' => array(
              'term_form' => '0'
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
                'weight' => '0'
              )
            )
          ),
          'menu' => 'classification/' . $vocabulary->vid,
        );
        $context->condition_mode = 0;
        $contexts[$name] = $context;
      }
    }
  }
  // TODO - Tabs
  foreach(scratchpads_species_get_tabs() as $tabs){
  }
  return $contexts;
}