<?php

/**
 * Field schema
 */
function field_schema(){
  // Extend the schema to include one additional table.
  $schema = field_schema_original();
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
  return $schema;
}