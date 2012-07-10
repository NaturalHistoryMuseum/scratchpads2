<?php
/*
 * Class for path redirects migration
 */
abstract class PathRedirectMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->pathQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function pathQuery();
}

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