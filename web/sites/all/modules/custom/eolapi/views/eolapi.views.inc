<?php

/**
 * Implementation of hook_views_plugins().
 */
function eolapi_views_plugins(){
  return array(
    'style' => array(
      'eolapi' => array(
        'title' => t('EOL API List'),
        'help' => t('Displays data from the EOL search API when available, with dynamic magic when not.'),
        'handler' => 'eolapi_plugin_style_eolapi',
        'theme' => 'views_view_unformatted',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal'
      )
    )
  );
}