<?php

/**
 * @file
 * Contains a pre-process hook for 'comment'.
 */

/**
 * Implements hook_preprocess_comment().
 */
function omega_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  $variables['user_picture'] = theme_get_setting('toggle_comment_user_picture') ? theme('user_picture', array('account' => $comment)) : '';
  // Add a sub component class for the comment content.
  $variables['content_attributes_array']['class'][] = 'comment__content';
  // Add a sub component class to the comment links.
  $variables['content']['links']['#attributes']['class'][] = 'comment__links';
  // Change modifier classes to use BEM syntax.
  $variables['classes_array'] = preg_replace('/^comment-new$/', 'comment--new', $variables['classes_array']);
  $variables['classes_array'] = preg_replace('/^comment-by-viewer$/', 'comment--by-viewer', $variables['classes_array']);
  $variables['classes_array'] = preg_replace('/^comment-by-node-author$/', 'comment--by-node-author', $variables['classes_array']);
}
