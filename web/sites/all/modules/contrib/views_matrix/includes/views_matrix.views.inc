<?php
/**
 * @file
 *   Views Matrix views definitions.
 */


/**
 * Implements hook_views_plugins().
 */
function views_matrix_views_plugins() {
  $plugins = array();

  $plugins['style'] = array(
    'matrix' => array(
      'title' => t('Matrix'),
      'help' => t('Displays a table sorted by row and column.'),
      'handler' => 'views_matrix_plugin_style_matrix',
      'theme' => 'views_view_matrix',
      'theme file' => 'views_matrix.theme.inc',
      'theme path' => drupal_get_path('module', 'views_matrix') . '/includes',
      'uses row plugin' => FALSE,
      'uses fields' => TRUE,
      'uses row class' => TRUE,
      'uses grouping' => TRUE,
      'uses options' => TRUE,
      'type' => 'normal',
    ),
    'matrix_csv' => array(
      'title' => t('Matrix CSV'),
      'help' => t('Displays a table sorted by row and column and output in CSV.'),
      'handler' => 'views_matrix_plugin_style_matrix_csv',
      'theme' => 'views_view_matrix_csv',
      'theme file' => 'views_matrix.theme.inc',
      'theme path' => drupal_get_path('module', 'views_matrix') . '/includes',
      'uses row plugin' => FALSE,
      'uses fields' => TRUE,
      'uses row class' => TRUE,
      'uses grouping' => TRUE,
      'uses options' => TRUE,
      'type' => 'normal',
    ),
  );

  return $plugins;
}
