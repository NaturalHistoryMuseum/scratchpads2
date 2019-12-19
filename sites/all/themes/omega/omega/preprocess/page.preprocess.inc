<?php

/**
 * @file
 * Contains a pre-process hook for 'page'.
 */

/**
 * Implements hook_preprocess_page().
 */
function omega_preprocess_page(&$variables, $hook) {
  // Remove the 'page' class from the attributes array.
  $variables['attributes_array']['class'] = array_diff($variables['attributes_array']['class'], array(drupal_html_class($hook)));

  if ($layout = omega_layout()) {
    foreach ($layout['info']['regions'] as $region => $name) {
      if (!isset($variables['page'][$region])) {
        $variables['page'][$region] = array();
      }
    }

    $original = system_region_list($GLOBALS['theme_key'], REGIONS_VISIBLE);
    foreach (array_diff_key($original, $layout['info']['regions']) as $region => $name) {
      unset($variables['page'][$region]);
    }
  }
}
