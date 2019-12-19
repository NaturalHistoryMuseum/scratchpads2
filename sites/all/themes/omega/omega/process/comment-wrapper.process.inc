<?php

/**
 * @file
 * Contains a process hook for 'comment_wrapper'.
 */

/**
 * Implements hook_process_comment_wrapper().
 */
function omega_process_comment_wrapper(&$variables) {
  // Flatten the attributes array.
  $variables['form_title_attributes'] = drupal_attributes($variables['form_title_attributes_array']);
}
