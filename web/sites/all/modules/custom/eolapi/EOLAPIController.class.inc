<?php

class EOLAPIController extends EntityAPIController{

  public function load($ids = array(), $conditions = array()){
    $entities = parent::load($ids, $conditions);
    field_attach_load('eolapi', $entities);
    return $entities;
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL){
    // Delete the files associated with the entity.
    if(db_table_exists('field_data_eolapi_image')){
      $fids = db_select('field_data_eolapi_image', 'f')->fields('f', array(
        'eolapi_image_fid'
      ))->condition('entity_id', $ids)->execute()->fetchCol();
      if(count($fids)){
        entity_delete_multiple('file', $fids);
      }
      // Delete this entity
      parent::delete($ids, $transaction);
    }
  }
}