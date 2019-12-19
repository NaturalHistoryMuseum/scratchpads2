<?php

/**
 * @file
 * Contains a pre-process hook for 'user_profile_category'.
 */

/**
 * Implements hook_preprocess_user_profile_category().
 *
 * Backports the following changes made to Drupal 8:
 * - #1190218: Convert user-profile-category.tpl.php to HTML5.
 */
function omega_preprocess_user_profile_category(&$variables) {
  $variables['attributes_array']['class'][] = 'user-profile-category-' . drupal_html_class($variables['title']);
}
