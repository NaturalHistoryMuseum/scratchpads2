<?php

/**
 * Implementation of hook_views_plugins()
 */
function relation_select_views_plugins() {
  $path = drupal_get_path('module', 'relation_select');
  return array(
    'style' => array(
      'relation_select' => array(
        'title' => t('Relation select'),
        'help' => t('Contains rows in contexts.'),
        'handler' => 'relation_select_plugin_style',
        'path' => $path . '/includes',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'theme' => 'views_view_relation_select',
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'type' => 'normal'
      )
    )
  );
}

