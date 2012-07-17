<?php

class MigrateDestinationPathRedirect extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'source' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create path redirects');
  }

  public function import(stdClass $object, stdClass $row){
    // Install the module if it is not already.
    if(!function_exists('redirect_save')){
      module_enable(array(
        'redirect'
      ));
      module_load_include('module', 'redirect');
    }
    redirect_object_prepare($object);
    redirect_save($object);
    if($object->rid){
      $this->numCreated++;
      return array(
        $object->source
      );
    }else{
      return FALSE;
    }
  }

  public function fields(){
    return array(
      'source' => t('Source'),
      'redirect' => t('Redirect'),
      'status_code' => t('Type')
    );
  }

  public function rollback(array $ids){
    $redirect = redirect_load_by_source($ids['destid1']);
    if($redirect){
      redirect_delete($redirect->rid);
    }
  }
}