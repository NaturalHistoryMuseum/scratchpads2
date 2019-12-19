<?php

/**
 * @file
 * Contains a pre-process hook for 'comment_wrapper'.
 */

/**
 * Implements hook_preprocess_comment_wrapper().
 */
function omega_preprocess_comment_wrapper(&$variables) {
  // Add a HTML id so it can be used as an anchor target.
  $variables['attributes_array']['id'] = 'comments';
  // Add the component class.
  $variables['attributes_array']['class'][] = 'comments';
  // Add BEM style classes to subcomonents.
  $variables['title_attributes_array']['class'][] = 'comments__title';
  $variables['form_title_attributes_array']['class'][] = 'comments__form-title';
}
