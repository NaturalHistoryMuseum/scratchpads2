<?php

/**
 * @file
 * Contains the theme function override for 'mark'.
 */

/**
 * Returns HTML for a marker for new or updated content.
 *
 * @ingroup themeable
 */
function omega_mark($variables) {
  $type = $variables['type'];

  if ($GLOBALS['user']->uid) {
    if ($type == MARK_NEW) {
      return ' <mark class="marker">' . t('new') . '</mark>';
    }
    elseif ($type == MARK_UPDATED) {
      return ' <mark class="marker">' . t('updated') . '</mark>';
    }
  }
}
