<?php
/**
 * @file
 * Views module hooks for the jPlayer module.
 */

/**
 * Implements hook_views_plugins().
 */
function jplayer_views_plugins() {
  $path = drupal_get_path('module', 'jplayer');

  return array(
    'style' => array(
      'jplayer' => array(
        'title' => t('jPlayer'),
        'help' => t('Display audio files from different nodes as a jPlayer playlist.'),
        'handler' => 'jplayer_style_plugin',
        'path' => "$path/includes",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}