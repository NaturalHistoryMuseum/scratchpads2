<?php
/**
 * Implementation of hook_views_plugins()
 */
function character_editor_views_plugins(){
  $path = drupal_get_path('module', 'character_editor');
  $slickgrid_path = drupal_get_path('module', 'slickgrid');
  return array(
    'style' => array(
      'character_editor' => array(
        'title' => t('Character editor'),
        'help' => t('Edit characters in a grid editor.'),
        'path' => "$path/includes",
        'parent' => 'slickgrid',
        'handler' => 'character_editor_views_plugin',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'theme' => 'views_view_character_editor',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'use ajax' => FALSE,
        'type' => 'normal'
      )
    )
  );
}