<?php

/**
 * UI controller.
 */
class EntityAdminController extends EntityDefaultUIController{

  // Defines this as a ui controller
  protected static $entityAdmin;

  public function __construct($entity_type, $entity_info){
    parent::__construct($entity_type, $entity_info);
  }

  public function get_entity_info(){
    return $this->entityInfo;
  }
}
