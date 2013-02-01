<?php

/**
 * Implementation of hook_views_plugins()
 */
function character_editor_views_plugins(){
  $path = drupal_get_path('module', 'character_editor');
  $slickgrid_path = drupal_get_path('module', 'slickgrid');
  $style_defaults = array();
  return array(
    'style' => array(
      // Add character editor style
      'character_editor' => array(
        'title' => t('Character editor'),
        'help' => t('Edit characters in a grid editor.'),
        'path' => $path . "/views",
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
      ),
      'sdd' => array(
        'title' => t('SDD'),
        'help' => t('Export a Character Project as SDD.'),
        'handler' => 'character_editor_plugin_style_sdd',
        'theme' => 'character_editor_create_sdd',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'theme path' => $path . '/theme',
        'theme file' =>  'theme.inc'
      )
    )
  )
  ;
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

