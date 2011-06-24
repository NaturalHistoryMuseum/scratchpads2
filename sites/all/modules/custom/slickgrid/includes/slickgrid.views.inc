<?php

/**
 * Implementation of hook_views_plugins()
 */
function slickgrid_views_plugins(){
  $path = drupal_get_path('module', 'slickgrid');
  $views_path = drupal_get_path('module', 'views');
  return array(
    'style' => array(
      'parent' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'handler' => 'views_plugin_style',
        'path' => "$views_path/plugins",
        'theme file' => 'theme.inc',
        'theme path' => "$views_path/theme",
        'parent' => ''
      ),
      'slickgrid' => array(
        'title' => t('Slickgrid'),
        'help' => t('Displays data in an editable spreadsheet-style grid.'),
        'path' => "$path/includes",
        'parent' => 'parent',
        'handler' => 'slickgrid_views_plugin',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'theme' => 'views_view_slickgrid',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'use ajax' => FALSE,
        'type' => 'normal'
      )
    )
  );
} 