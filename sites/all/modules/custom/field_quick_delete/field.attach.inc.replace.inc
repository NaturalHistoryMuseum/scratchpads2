<?php

/**
 * Save the entity_type and entity to a table for deleting later.
 */
function field_attach_delete($entity_type, $entity){
  $record = array(
    'entity' => $entity,
    'entity_type' => $entity_type
  );
  drupal_write_record('field_quick_delete', $record);
}