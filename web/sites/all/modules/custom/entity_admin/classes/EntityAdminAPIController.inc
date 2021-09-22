<?php

/**
 * Clone of EntityAPIController but with the load hooks removed from attachLoad() for faster use in views
 */
class EntityAdminAPIController extends EntityAPIController{

  protected function attachLoad(&$queried_entities, $revision_id = FALSE){
    // Attach fields.
    if($this->entityInfo['fieldable']){
      if($revision_id){
        field_attach_load_revision($this->entityType, $queried_entities);
      }else{
        field_attach_load($this->entityType, $queried_entities);
      }
    }
    // All hook_entity_load alls have been removed from here
  }
}