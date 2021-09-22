<?php

/**
 * @file
 * scratchpads_forum.context.inc
 */
/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_forum_context_default_contexts(){
  $export = array();
  // forum_node context
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forum_node';
  $context->description = 'Context activated on forum node view';
  $context->tag = 'Scratchpads node';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'forum' => 'forum'
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
    )
  );
  $context->condition_mode = 0;
  $export[$context->name] = $context;
  // forum_page context
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forum_page';
  $context->description = 'Activated on main forum pages';
  $context->tag = 'forum';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'forum' => 'forum',
        'forum/*' => 'forum/*'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'scratchpads_forum-scratchpadsforum_tree' => array(
          'module' => 'scratchpads_forum',
          'delta' => 'scratchpadsforum_tree',
          'region' => 'sidebar',
          'weight' => '-10'
        )
      )
    )
  );
  $context->condition_mode = 0;
  $export[$context->name] = $context;
  // forum_page context
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forum_tree';
  $context->description = 'Activated on main forum pages';
  $context->tag = 'forum';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'forum' => 'forum'
      ),
      'options' => array(
        'node_form' => '0'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'scratchpads_forum-scratchpadsforum_tree' => array(
          'module' => 'scratchpads_forum',
          'delta' => 'scratchpadsforum_tree',
          'region' => 'sidebar',
          'weight' => '10'
        )
      )
    )
  );
  $context->condition_mode = 0;
  $export[$context->name] = $context;
  return $export;
}
