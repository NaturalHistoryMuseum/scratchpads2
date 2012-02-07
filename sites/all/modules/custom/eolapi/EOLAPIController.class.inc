<?php

class EOLAPIController extends EntityAPIController{

  public function load($ids = array(), $conditions = array()){
    $entities = parent::load($ids, $conditions);
    field_attach_load('eolapi', $entities);
    return $entities;
  }
}