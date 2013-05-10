<?php

/**
 * Callback to display a list of issues with this site.
 */
function scratchpads_data_integrity_overview(){
  $messages = scratchpads_data_integrity_get_messages(TRUE);
  return array(
    'messages' => array(
      '#theme' => 'item_list',
      '#items' => $messages,
      '#title' => t('All data issues')
    )
  );
}