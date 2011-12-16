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
        'theme path' => "$slickgrid_path/theme",
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

class CharacterProjectViewsController extends EntityDefaultViewsController{

  public function views_data(){
    $data = parent::views_data();
    $data['views']['character_field'] = array(
      'title' => t('Character field'),
      'help' => t('Display a character editor field.'),
      'field' => array(
        'handler' => 'character_editor_views_handler_field'
      )
    );
    return $data;
  }
}

