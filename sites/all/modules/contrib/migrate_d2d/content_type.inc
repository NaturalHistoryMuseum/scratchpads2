<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalContentTypeMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->contentTypeQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Check whether or not this content type already exists.
    $node_info = node_entity_info();
    if(isset($node_info['node']['bundles'][$row->type])){return FALSE;}
    return TRUE;
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function contentTypeQuery();
}

abstract class MigrateDestinationContentType extends MigrateDestination{

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
    return t('Create content types');
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

  public function fields(){
    return array(
      'type' => t('Machine name'),
      'name' => t('Human readable name'),
      'description' => t('Node type description'),
      'title_label' => t('Title label')
    );
  }

  public function rollback(array $ids){
    node_type_delete($ids['destid1']);
  }
}