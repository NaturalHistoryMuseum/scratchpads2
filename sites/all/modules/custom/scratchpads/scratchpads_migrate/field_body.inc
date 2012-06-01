<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldBodyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->fieldQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Check if this field has a different destination entity type / bundle
    $mapping = scratchpads_migrate_type_mappings($row->type);
    if($mapping && is_array($mapping) && isset($mapping['bundle'])){
      $row->type = $mapping['bundle'];
    }
    // Does an instance of this field already exists?
    if(field_read_instance('node', 'body_field', $row->type)){// Instance already exists, so skip it
return false;}
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function fieldQuery();
}

abstract class MigrateDestinationFieldBody extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create body fields');
  }

  public function import(stdClass $object, stdClass $row){
    if(node_type_load($object->type)){
      $instance = node_add_body_field($object, $object->body_label);
      // Has the field been created?
      if(isset($instance['field_id'])){
        $this->numCreated++;
        return array(
          $object->type
        );
      }
    }else{
      throw new MigrateException(t('Node %type does not exist', array(
        '%type' => $object->type
      )));
    }
    return false;
  }

  public function fields(){
    return array(
      'type' => t('Machine name'),
      'body_label' => t('Title label')
    );
  }

  public function rollback(array $ids){
    $instance = field_read_instance('node', 'body_field', $ids['destid1']);
    field_delete_instance($instance);
  }
}