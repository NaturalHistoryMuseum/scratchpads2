<?php

class MigrateDestinationCitation extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'cid' => array(
        'type' => 'int'
      )
    );
  }

  public function __toString(){
    return t('Create citations.');
  }

  public function import(stdClass $object, stdClass $row){
    // Write to the citation table.
  }

  public function fields(){
    return array(
      'cid' => t('Primary key'),
      'created' => t('Created'),
      'reference' => t('Reference'),
      'url' => t('URL')
    );
  }

  public function rollback(array $ids){
    // Simply delete the CID from the citation table.
  }
}