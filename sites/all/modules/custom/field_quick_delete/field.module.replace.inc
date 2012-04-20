<?php

/**
 * Replacement cron function
 */
function field_cron(){
  field_cron_original();
  $result = db_query_range('SELECT * FROM {field_quick_delete} ORDER BY id ASC', 0, 100);
  foreach($result as $item){
    $last_id = $item->id;
    field_attach_delete_original($item->entity_type, unserialize($item->entity));
  }
  // Delete the records from field_quick_delete that we have just deleted.
  db_delete('field_quick_delete')->condition('id', $last_id, '<=')->execute();
}