<?php

/**
 * Callback to display a list of issues with this site.
 */
function scratchpads_data_integrity_overview(){
  return array(
    'messages' => array(
      '#theme' => 'item_list',
      '#items' => str_replace("\n", '<br/>', scratchpads_data_integrity_get_messages(TRUE, TRUE)),
      '#title' => t('All data issues')
    )
  );
}