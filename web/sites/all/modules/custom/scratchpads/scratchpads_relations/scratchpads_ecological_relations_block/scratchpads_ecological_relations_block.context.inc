<?php

/**
 * @file
 * scratchpads_ecological_relations_block.context.inc
 */
/**
 * Implements hook_context_default_contexts().
 */
function scratchpads_ecological_relations_block_context_default_contexts(){
  $export = array();
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'species_relationships';
  $context->description = 'Ecological relations species page';
  $context->tag = 'Species';
  $context->conditions = array(
    'species' => array(
      'values' => array(
        'overview' => 'descriptions'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-species_relationships-block' => array(
          'module' => 'views',
          'delta' => 'species_relationships-block',
          'region' => 'content',
          'weight' => '9'
        )
      )
    )
  );
  $context->condition_mode = 0;
  // Translatables
  // Included for use with string extractors like potx.
  t('Ecological relations species page');
  t('Species');
  $export['species_relationships'] = $context;
  return $export;
}
