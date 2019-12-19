<?php

/**
 * @file
 * Contains a process hook for 'block'.
 */

/**
 * Implements hook_process_block().
 */
function omega_process_block(&$variables) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}
