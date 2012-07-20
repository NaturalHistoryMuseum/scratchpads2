<?php

class MigrateDestinationVariable extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create variables/settings.');
  }

  public function import(stdClass $object, stdClass $row){
    $map = VariableMigration::variableNames();
    variable_set($map[$row->name], unserialize($row->value));
    $this->numCreated++;
  }

  public function fields(){
    return array(
      'name' => t('Name'),
      'value' => t('Value')
    );
  }

  public function rollback(array $ids){
    // We don't do anything here.  Once variables are set, they're set and have
    // overridden the previous value that may or may not have been set.  We can
    // not simply delete the variable.
  }
}