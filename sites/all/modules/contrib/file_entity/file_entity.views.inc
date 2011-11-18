<?php

/**
 * @file
 * Views integration for the file_entity module.
 */

/**
 * Implements hook_views_plugins().
 */
function file_entity_views_plugins() {
  return array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'row' => array(
      'file' => array(
        'title' => t('File'),
        'help' => t('Display the file with standard file view.'),
        'handler' => 'views_plugin_row_file_view',
        'base' => array('file_managed'), // only works with 'file' as base.
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-file',
      ),
    ),
  );
}
