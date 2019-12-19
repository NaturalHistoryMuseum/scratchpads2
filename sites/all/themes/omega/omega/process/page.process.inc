<?php

/**
 * @file
 * Contains a process hook for 'page'.
 */

/**
 * Implements hook_process_page().
 */
function omega_process_page(&$variables) {
  if ($layout = omega_layout()) {
    $variables['theme_hook_suggestions'][] = 'omega_page_layout';
    $variables['omega_layout'] = $layout;
  }
}
