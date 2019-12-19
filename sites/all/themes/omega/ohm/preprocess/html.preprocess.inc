<?php

/**
 * @file
 * Contains a pre-process hook for 'html'.
 */

/**
 * Implements hook_preprocess_TEMPLATE().
 */
function ohm_preprocess_html(&$variables) {
  // Ohm should only be enabled as an example theme, and not used in production.
  if (omega_theme_get_setting('ohm_enable_warning', TRUE)) {
    drupal_set_message(t('Ohm is a demonstration subtheme and will therefore be constantly evolving with latest best practices. Explore, break it and learn but don\'t use it in production directly, or as a base theme. You can disable this message on the <a href="!settings">theme settings page</a>.', array(
      '!settings' => url("admin/appearance/settings/{$GLOBALS['theme']}"),
    )));
  }
}
