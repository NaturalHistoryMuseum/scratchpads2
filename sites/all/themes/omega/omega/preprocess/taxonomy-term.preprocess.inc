<?php

/**
 * @file
 * Contains a pre-process hook for 'taxonomy_term'.
 */

/**
 * Implements hook_preprocess_taxonomy_term().
 */
function omega_preprocess_taxonomy_term(&$variables) {
  $view_mode = drupal_html_class($variables['view_mode']);
  $machine_name = drupal_html_class($variables['vocabulary_machine_name']);

  $variables['attributes_array']['class'] = array(
    'taxonomy-term--' . $machine_name,
    'taxonomy-term--' . $view_mode,
    'taxonomy-term--' . $machine_name . '--' . $view_mode,
  );

  $variables['content_attributes_array']['class'][] = 'taxonomy-term__content';
}
