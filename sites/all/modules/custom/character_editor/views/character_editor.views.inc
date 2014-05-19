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
      )
    )
  );
}

class CharacterProjectViewsController extends EntityDefaultViewsController{

  public function views_data(){
    $data = parent::views_data();
    // Add a field for characters
    $data['views']['character_field'] = array(
      'title' => t('Character field'),
      'help' => t('Display a character editor field.'),
      'field' => array(
        'handler' => 'character_editor_views_handler_field'
      )
    );
    // Add a field used to display both taxonomic terms and specimens
    $data['character_editor']['table']['group'] = t('Character Editor');
    $data['character_editor']['table']['join'] = array(
      '#global' => array()
    );
    $data['character_editor']['character_entity_field'] = array(
      'title' => t('Taxon and Specimen'),
      'help' => t('This field returns both taxonomic terms and specimens referenced by the character editor in the first argument. Using this field causes the query to be re-written, so it cannot be used with other fields or filters'),
      'field' => array(
        'handler' => 'character_editor_views_handler_entity_field'
      )
    );
    return $data;
  }
}

