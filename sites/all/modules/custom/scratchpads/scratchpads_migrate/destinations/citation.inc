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
    // Firstly, we ensure that the citation module is enabled.
    if(!module_exists('citation')){
      module_enable(array(
        'citation'
      ));
    }
    // We also copy accross the citation folder if we haven't already done so
    if(!file_exists('public://citations')){
      global $s1_domain;
      copy_directory('/var/lib/aegir/quartz.nhm.ac.uk_scratchpads-1/' . $s1_domain . '/files/citations', drupal_realpath('public://citations/'));
    }
    // Write to the citation table.
    $record = (array)$object;
    drupal_write_record('citation', $record);
    $this->numCreated++;
    return array(
      $object->cid
    );
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
    db_delete('citation')->condition('cid', $ids['destid1'])->execute();
  }
}

function copy_directory($source, $destination){
  if(is_dir($source)){
    @mkdir($destination);
    $directory = dir($source);
    while(FALSE !== ($readdirectory = $directory->read())){
      if($readdirectory == '.' || $readdirectory == '..'){
        continue;
      }
      $PathDir = $source . '/' . $readdirectory;
      if(is_dir($PathDir)){
        copy_directory($PathDir, $destination . '/' . $readdirectory);
        continue;
      }
      copy($PathDir, $destination . '/' . $readdirectory);
    }
    $directory->close();
  }else{
    copy($source, $destination);
  }
}