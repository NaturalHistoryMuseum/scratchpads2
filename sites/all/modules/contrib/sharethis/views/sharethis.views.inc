<?php

/**
 * @file
 * Views integration for ShareThis.
 */

/**
 * Implements hook_views_data().
 */
function sharethis_views_data() {
  $data = array();

  // ShareThis link.
  $data['node']['sharethis'] = array(
    'title' => t('ShareThis Link'),
    'help' => t('Link provided by the ShareThis service.'),
    'field' => array(
      'handler' => 'sharethis_handler_field_link',
      'click sortable' => FALSE,
    ),
  );

  return $data;
}
