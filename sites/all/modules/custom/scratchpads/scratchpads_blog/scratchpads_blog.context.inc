<?php

/**
 * @file
 * scratchpads_blog.context.inc
 */
/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_blog_context_default_contexts(){
  $export = array();
  // forum_node context
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'add_scratchpads_blog_block_to_blog_pages';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'blog' => 'blog'
      ),
      'options' => array(
        'node_form' => '0'
      )
    ),
    'path' => array(
      'values' => array(
        'blog' => 'blog',
        'blog/*' => 'blog/*'
      )
    )
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'scratchpads_blog-blog_filters' => array(
          'module' => 'scratchpads_blog',
          'delta' => 'blog_filters',
          'region' => 'sidebar',
          'weight' => '20'
        )
      )
    )
  );
  $context->condition_mode = 0;
  $export[$context->name] = $context;
  return $export;
}
