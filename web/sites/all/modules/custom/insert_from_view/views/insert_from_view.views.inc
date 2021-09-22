<?php

/**
 * Implement hook_views_data
 */
function insert_from_view_views_data(){
  $data['insert_from_view']['table']['group'] = t('Insert from view');
  $data['insert_from_view']['table']['join'] = array(
    '#global' => array()
  );
  $data['insert_from_view']['insert_from_view_checkbox'] = array(
    'title' => 'Insert from view checkbox',
    'help' => t('Add this field to allow Insert From View to select and insert multiple rows at once.'),
    'field' => array(
      'handler' => 'insert_from_view_handler_field_checkbox'
    )
  );
  return $data;
}
