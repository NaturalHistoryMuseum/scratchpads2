<?php

/**
 * UI controller.
 */
class EntityUIController extends EntityDefaultUIController{

  // Defines this as a ui controller
  protected static $ui;

  public function __construct($entity_type, $entity_info){
    parent::__construct($entity_type, $entity_info);
  }

  public function get_entity_info(){
    return $this->entityInfo;
  }
}
