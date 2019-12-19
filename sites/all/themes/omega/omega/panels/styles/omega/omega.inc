<?php

/**
 * @file
 * Contains a style plugin that registers Omega layouts in the theme registry.
 */

/**
 * Implementation of hook_STYLE_panels_styles().
 */
function omega_omega_panels_styles() {
  return array(
    'title' => t('Omega'),
    'hidden' => TRUE,
    'hook theme' => 'omega_panels_theme_registry_hack',
  );
}

/**
 * Slight hack to ensure that the Omega layouts are always properly registered.
 *
 * This is required so that they can be used in backend UIs that are not served
 * with Omega based themes.
 */
function omega_panels_theme_registry_hack(&$theme, $data) {
  // The theme files might not be loaded yet.
  if (!function_exists('_omega_theme_layouts')) {
    require_once drupal_get_path('theme', 'omega') . '/template.php';
  }

  // Register each layout.
  $theme = array_merge($theme, _omega_theme_layouts());
}
