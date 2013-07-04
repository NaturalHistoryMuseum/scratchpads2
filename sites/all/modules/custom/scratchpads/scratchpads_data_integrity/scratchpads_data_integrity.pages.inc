<?php

/**
 * Callback to display a list of issues with this site.
 */
function scratchpads_data_integrity_overview(){
  return array(
    'messages' => array(
      '#theme' => 'item_list',
      '#items' => scratchpads_data_integrity_get_messages(TRUE),
      '#title' => t('All data issues')
    )
  );
}