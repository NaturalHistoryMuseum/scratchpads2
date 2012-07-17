<?php

class MigrateDestinationScratchpadsUser extends MigrateDestinationUser{

  public function fields($migration = NULL){
    $fields = parent::fields($migration);
    // Add some extra fields
    $fields['data'] = t("Data");
    $fields['legal_accept'] = t("Legal accept");
    return $fields;
  }

  public function import(stdClass $account, stdClass $row){
    // Map some of the duplicate fields
    // Content profile fields always take precedence so ovverwite existing content 
    foreach(DrupalFieldContentProfileMigration::getFieldNameMappings() as $d6_field => $d7_field){
      if(!empty($row->{$d6_field})){
        $account->{$d7_field} = $row->{$d6_field};
      }
    }
    return parent::import($account, $row);
  }
}