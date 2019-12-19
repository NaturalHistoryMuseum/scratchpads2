<?php

/**
 * @file
 * Contains the theme function override for 'more_help_link'.
 */

/**
 * Returns HTML for a "more help" link.
 *
 * @ingroup themeable
 */
function omega_more_help_link($variables) {
  return l(t('More help'), $variables['url'], array('attributes' => array('class' => array('more-link'))));
}
