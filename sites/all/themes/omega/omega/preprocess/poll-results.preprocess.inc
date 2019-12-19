<?php

/**
 * @file
 * Contains a pre-process hook for 'poll_results'.
 */

/**
 * Implements hook_preprocess_poll_results__block().
 */
function omega_preprocess_poll_results(&$variables) {
  // Change the links to a render array with sensible, BEM based classes instead
  // of a string.
  $variables['links'] = array(
    '#theme' => 'links',
    '#links' => $variables['raw_links'],
    '#attributes' => array('class' => array('links', 'poll-results__links')),
  );
}
