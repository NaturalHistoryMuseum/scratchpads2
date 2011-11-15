<?php

/**
 * Implementation of hook_views_plugins().
 */
function gm3_views_plugins(){
  return array(
    'style' => array(
      'gmap' => array(
        'title' => t('Google Map'),
        'help' => t('Displays rows as a map.'),
        'handler' => 'gm3_plugin_style_gm3',
        'theme' => 'gm3_view_gm3',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal'
      )
    )
  );
}