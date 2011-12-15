<?php
/**
 * @file
 */


/**
 * Implementation of hook_views_data_alter().
 */
function cck_select_other_views_data_alter(&$data) {
  $instances = field_read_instances();
  foreach ($instances as $name => $instance) {
    if ($instance['widget']['type'] == 'cck_select_other') {
      $table = 'field_data_' . $instance['field_name'];
      $field = $instance['field_name'] . '_value';
      $data[$table]['entity_id']['field']['handler'] = 'cck_select_other_handler_field';
      $data[$table][$field]['filter']['handler'] = 'cck_select_other_handler_filter';
    }
  }
}

