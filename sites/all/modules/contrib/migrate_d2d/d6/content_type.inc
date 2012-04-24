<?php

class DrupalContentType6Migration extends DrupalContentTypeMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationContentType6::getKeySchema());
    $this->destination = new MigrateDestinationContentType6();
    $this->addSimpleMappings(array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function contentTypeQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('node_type', 'n')->fields('n', array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ))->condition('module', 'node');
  }
}

class MigrateDestinationContentType6 extends MigrateDestinationContentType{

  public function __construct(){
    parent::__construct();
  }

  public function import(stdClass $object, stdClass $row){
    $type = node_type_set_defaults();
    $type->type = $object->type;
    $type->name = $object->name;
    $type->description = $object->description;
    $type->help = $object->help;
    $type->title_label = $object->title_label;
    $type->has_title = TRUE;
    $type->base = 'node_content';
    $type->custom = 1;
    $type->modified = TRUE;
    $type->locked = 0;
    $type->module = 'node';
    $status = node_type_save($type);
    if($status){
      node_types_rebuild();
      menu_rebuild();
      return array(
        $type->type
      );
    }else{
      return FALSE;
    }
  }

  public function rollback(array $ids){
    node_type_delete($ids['destid1']);
  }
}