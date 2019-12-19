<?php

/**
 * @file
 * Contains the theme function override for 'admin_block'.
 */

/**
 * Returns HTML for an administrative block for display.
 *
 * @ingroup themeable
 */
function omega_admin_block($variables) {
  $block = $variables['block'];
  $output = '';

  // Don't display the block if it has no content to display.
  if (empty($block['show'])) {
    return $output;
  }

  $output .= '<div class="admin-panel">';
  if (!empty($block['title'])) {
    $output .= '<h3>' . $block['title'] . '</h3>';
  }
  if (!empty($block['content'])) {
    $output .= '<div class="admin-panel__body">' . $block['content'] . '</div>';
  }
  else {
    $output .= '<div class="admin-panel__body">' . $block['description'] . '</div>';
  }
  $output .= '</div>';

  return $output;
}
