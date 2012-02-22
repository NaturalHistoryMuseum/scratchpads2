<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function scratchpads_backend_context_default_contexts(){
  global $conf;
  $contexts = array();
  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'admin_content';
  $context->description = '';
  $context->tag = 'admin';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'article' => 'article',
        'biblio' => 'biblio',
        'forum' => 'forum',
        'location' => 'location',
        'media_gallery' => 'media_gallery',
        'page' => 'page',
        'specimen_observation' => 'specimen_observation',
        'spm' => 'spm',
        'test' => 'test'
      ),
      'options' => array(
        'node_form' => '2'
      )
    ),
    'path' => array(
      'values' => array(
        'file/add' => 'file/add',
        'admin/content/file/import' => 'admin/content/file/import',
        'admin/content/slickgrid/*' => 'admin/content/slickgrid/*',
        'admin/file/slickgrid/*' => 'admin/file/slickgrid/*'
      )
    ),
    'views' => array(
      'values' => array(
        'admin_file' => 'admin_file',
        'admin_node' => 'admin_node'
      )
    )
  );
  $context->reactions = array(
    'breadcrumb' => 'admin/content'
  );
  $context->condition_mode = 0;
  // Translatables
  // Included for use with string extractors like potx.
  t('admin');
  $contexts[$context->name] = $context;
  return $contexts;
}