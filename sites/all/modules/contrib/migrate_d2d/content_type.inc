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
   *
   * @param $row
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    return TRUE;
  }

  /**
   * @abstract
   * Version-specific classes must implement menuQuery, returning a query that
   * provides all the basic fields for each menu.
   */
  abstract protected function contentTypeQuery();
}

abstract class MigrateDestinationContentType extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __construct(){
    // No options.
  }

  public function __toString(){
    return t('Create content types');
  }

  public function fields(){
    return array(
      'type' => t('Machine name'),
      'name' => t('Human readable name'),
      'description' => t('Node type description'),
      'title_label' => t('Title label')
    );
  }
}