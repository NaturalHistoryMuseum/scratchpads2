<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_front_context_default_contexts(){
  $contexts = array();
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'scratchpads_front_page';
  $context->description = 'Scratchpads front page';
  $context->tag = 'scratchpads';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'scratchpads-front' => 'scratchpads-front'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-front_page_blocks-block_1' => array(
          'module' => 'views',
          'delta' => 'front_page_blocks-block_1',
          'region' => 'content',
          'weight' => '1'
        ),
        'views-front_page_blocks-block_2' => array(
          'module' => 'views',
          'delta' => 'front_page_blocks-block_2',
          'region' => 'content',
          'weight' => '2'
        ),
        'views-front_page_blocks-block_3' => array(
          'module' => 'views',
          'delta' => 'front_page_blocks-block_3',
          'region' => 'content',
          'weight' => '3'
        )
      )
    ),
    'region' => array(
      'scratchpads' => array(
        'disable' => array(
          'sidebar' => 'sidebar',
          'page_top' => 0,
          'page_bottom' => 0,
          'slide_top' => 0,
          'content' => 0,
          'footer' => 0,
          'slide_bottom' => 0,
          'branding' => 0,
          'menu' => 0,
          'secondary_menu' => 0,
          'header' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0
        )
      )
    )
  );
  $context->condition_mode = 0;
  $contexts['scratchpads_front_page'] = $context;
  return $contexts;
}