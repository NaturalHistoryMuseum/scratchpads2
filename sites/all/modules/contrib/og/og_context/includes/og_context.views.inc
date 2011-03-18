<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function og_context_views_plugins() {
  return array(
    'argument default' => array(
      'og_context' => array(
        'title' => t('Current group from context'),
        'handler' => 'og_context_plugin_argument_default_group_context',
        'path' => drupal_get_path('module', 'og_context') . '/includes/views',
        'parent' => 'views_plugin_argument_default',
      ),
    ),
  );
}