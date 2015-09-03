<?php

/**
 * @file
 * Holds methods directly related to the Views module.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function views_charts_views_plugins() {
  $plugins = array();

  $plugins['style'] = array(
    'charts' => array(
      'title' => t('Charts & Graphs'),
      'help' => t('Displays rows in a chart.'),
      'handler' => 'views_plugin_style_chart',
      'path' => drupal_get_path('module', 'views_charts'),
      'uses row plugin' => TRUE,
      'uses options' => TRUE,
      'uses fields' => TRUE,
      'type' => 'normal',
    ),
  );

  return $plugins;
}
