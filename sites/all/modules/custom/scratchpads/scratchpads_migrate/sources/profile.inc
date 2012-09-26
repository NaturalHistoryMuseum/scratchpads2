<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsProfileMigration extends ScratchpadsUserMigration{

  public function __construct(array $arguments){
    // Add user source fields
    $this->sourceFields = array(
      "uid" => t("uid"),
      "name" => t("name"),
      "pass" => t("pass"),
      "mail" => t("mail"),
      "mode" => t("mode"),
      "sort" => t("sort"),
      "threshold" => t("threshold"),
      "theme" => t("theme"),
      "signature" => t("signature"),
      "signature_format" => t("signature_format"),
      "created" => t("created"),
      "access" => t("access"),
      "login" => t("login"),
      "status" => t("status"),
      "timezone" => t("timezone"),
      "language" => t("language"),
      "picture" => t("picture"),
      "init" => t("init"),
      "data" => t("data"),
      "timezone_name" => t("timezone_name")
    );
    parent::__construct($arguments);
    // Add legal accept
    $this->removeFieldMapping('legal_accept');
    $this->addFieldMapping('legal_accept')->defaultValue(0);
    $this->addUnmigratedSources(array(
      'uid',
      'title'
    ));
    $this->map = new MigrateSQLMap($this->machineName, array(
      'nid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Source profile node ID',
        'alias' => 'n'
      )
    ), MigrateDestinationUser::getKeySchema());
  }

  protected function userQuery(){
    // Do not attempt to migrate the anonymous or admin user rows.
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n')->fields('n', array(
      'nid',
      'title'
    ))->condition('type', 'profile')->condition('language', 'en');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    $row->uid = 0;
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    $content_profile = ScratchpadsUserMigration::getProfileNode($row, array(
      'n.nid' => $row->nid
    ));
    $row->name = $row->title;
    unset($row->location_country);
  }
}