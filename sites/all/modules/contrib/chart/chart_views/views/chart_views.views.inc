<?php
/**
 * @file
 * Chart views integration.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_views_plugins().
 */
function chart_views_views_plugins() {
  return array(
    'module' => 'chart_views',
    'style'  => array(
      'chart_views' => array(
        'handler' => 'chart_views_plugin_style_chart',
        'help' => t('Displays the content in several chart styles.'),
        'path' => drupal_get_path('module', 'chart_views') . '/views',
        'parent' => 'parent',
        'title' => t('Chart'),
        'type' => 'normal',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses row plugin' => FALSE,
      ),
    )
  );
}
