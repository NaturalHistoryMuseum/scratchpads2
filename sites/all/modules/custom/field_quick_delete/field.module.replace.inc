<?php

/**
 * Replacement cron function
 */
function field_cron(){
  field_cron_original();
  if(db_table_exists('field_quick_delete')){
    $result = db_query_range('SELECT * FROM {field_quick_delete} ORDER BY id ASC', 0, 100);
    $last_id = FALSE;
    foreach($result as $item){
      $last_id = $item->id;
      field_attach_delete_original($item->entity_type, unserialize($item->entity));
    }
    if($last_id){
      // Delete the records from field_quick_delete that we have just deleted.
      db_delete('field_quick_delete')->condition('id', $last_id, '<=')->execute();
    }else{
      // We can now remove the field_quick_delete table.
      db_drop_table('field_quick_delete');
    }
  }
}