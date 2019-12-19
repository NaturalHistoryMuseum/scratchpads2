<?php

/**
 * @file
 * Contains a pre-process hook for 'block_admin_display_form'.
 */

/**
 * Implements hook_preprocess_admin_display_form().
 */
function omega_preprocess_block_admin_display_form(&$variables) {
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'block') . '/block.js');
  foreach ($variables['block_regions'] as $region => $title) {
    drupal_add_tabledrag('blocks', 'match', 'sibling', 'block-region-select', 'block-region-' . $region, NULL, FALSE);
    drupal_add_tabledrag('blocks', 'order', 'sibling', 'block-weight', 'block-weight-' . $region);
  }
}
