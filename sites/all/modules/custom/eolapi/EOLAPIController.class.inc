<?php

class EOLAPIController extends EntityAPIController{

  public function load($ids = array(), $conditions = array()){
    $entities = parent::load($ids, $conditions);
    field_attach_load('eolapi', $entities);
    return $entities;
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL){
    // Delete the files associated with the entity.
    $fids = db_select('field_data_eolapi_image', 'f')->fields('f', array(
      'eolapi_image_fid'
    ))->condition('entity_id', $ids)->execute()->fetchCol();
    if(count($fids)){
      entity_delete('file', $fids);
    }
  }
}