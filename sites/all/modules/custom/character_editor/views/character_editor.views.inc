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
      // SDD export style
      'views_data_export_sdd' => array(
        'title' => t('SDD file'),
        'help' => t('Display the view as a txt file.'),
        'handler' => 'views_data_export_plugin_style_export_xml',
        'export headers' => array(
          'Content-Type' => 'text/xml'
        ),
        'export feed type' => 'sdd',
        'export feed text' => 'SDD',
        'export feed file' => '%view.sdd',
        'export feed icon' => $path . '/images/sdd.png',
        'additional themes' => array(
          'views_data_export_sdd_header' => 'style',
          'views_data_export_sdd_body' => 'style',
          'views_data_export_sdd_footer' => 'style'
        ),
        'additional themes base' => 'views_data_export_sdd',
        'path' => drupal_get_path('module', 'views_data_export') . '/plugins',
        'parent' => 'views_data_export',
        // FIXME - This needs reinstating to allow the SDD export function to
        // work.  This has been removed to allow the dwca_export drush/cron
        // command to work when this module is enabled.
        //'theme' => 'views_data_export',
        'theme path' => $path . '/theme',
        'theme file' => 'theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'data_export'
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
      '#global' => array(),
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

