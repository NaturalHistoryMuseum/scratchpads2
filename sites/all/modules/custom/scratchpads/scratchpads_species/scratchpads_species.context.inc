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
          'menu' => 'classification/' . $vocabulary->vid
        );
        $context->condition_mode = 0;
        $contexts[$name] = $context;
      }
    }
  }
  // Get tabs - we need to add in the overview tab here too
  $tabs = array_merge(array(
    'overview' => t("Overview")
  ), scratchpads_species_get_tabs());
  foreach($tabs as $name => $label){
    $context = new stdClass();
    $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
    $context->api_version = 3;
    $context->name = 'species_' . $name;
    $context->description = $label . ' species page';
    $context->tag = 'Species';
    $context->conditions = array(
      'species' => array(
        'values' => array(
          $name => $name
        )
      )
    );
    if(count($views = scratchpads_species_get_context_views($context->name))){
      $weight = 0;
      foreach($views as $view){
      	$delta = $view . '-block';
      	if(strlen($delta)>32){
      	  $delta = md5($delta);
      	}
        $context->reactions['block']['blocks']['views-' . $delta] = array(
          'module' => 'views',
          'delta' => $delta,
          'region' => 'content',
          'weight' => $weight
        );
        $weight++;
      }
    }
    $context->condition_mode = 0;
    $contexts[$context->name] = $context;
  }
  return $contexts;
}