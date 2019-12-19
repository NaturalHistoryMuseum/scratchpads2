<?php

/**
 * @file
 * Contains a pre-process hook for 'panels-pane'.
 */

/**
 * Implements hook_preprocess_panels_pane().
 */
function omega_preprocess_panels_pane(&$variables) {
  if (!empty($variables['id'])) {
    // Extract the actual id from the id string.
    $variables['attributes_array']['id'] = substr($variables['id'], 5, -1);
  }
}
