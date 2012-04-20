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

/**
 * Additional hook_schema_alter function.
 */
function field_schema_alter(&$schema){
  $schema['field_quick_delete'] = array(
    'description' => 'Allows for quick deletion of fields',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Primary Key: Unique term ID.'
      ),
      'entity_type' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
        'description' => 'The entity type of the entity to delete fields for.'
      ),
      'entity' => array(
        'type' => 'text',
        'not null' => TRUE,
        'serialize' => TRUE
      )
    ),
    'primary key' => array(
      'id'
    )
  );
}