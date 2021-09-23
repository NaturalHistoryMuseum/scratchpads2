<?php

/**
 * Save the entity_type and entity to a table for deleting later.
 */
function field_attach_delete($entity_type, $entity){
  drupal_register_shutdown_function('field_attach_delete_original', $entity_type, $entity);
}