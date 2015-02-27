<?php

/**
 * @file
 * Defines the View Style Plugins for Views Slideshow module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_slideshow_views_plugins() {
  return array(
    'style' => array(
      'slideshow' => array(
        'title' => t('Slideshow'),
        'help' => t('Display the results as a slideshow.'),
        'handler' => 'views_slideshow_plugin_style_slideshow',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'views_slideshow'),
        'theme' => 'views_slideshow',
        'theme path' => drupal_get_path('module', 'views_slideshow') . '/theme',
        'theme file' => 'views_slideshow.theme.inc',
      ),
    ),
  );
}
