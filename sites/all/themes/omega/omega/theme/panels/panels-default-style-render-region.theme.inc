<?php

/**
 * @file
 * Contains the theme function override for 'admin_block'.
 */

/**
 * Returns HTML for a region when rendered with the default style.
 *
 * @ingroup themeable
 */
function omega_panels_default_style_render_region(&$variables) {
  // Remove the ugly .panels-separator.
  return implode('', $variables['panes']);
}
