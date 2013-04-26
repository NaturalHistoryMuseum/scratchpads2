<?php

class FemailMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->femailQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'msgid' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFemail::getKeySchema());
    $this->destination = new MigrateDestinationFemail();
    // Field mappings
    $this->addSimpleMappings(array(
      'nid',
      'cid',
      'msgid'
    ));
  }

  /**
   * preImport - ensure femail module is enabled
   */
  function preImport(){
    if(!module_exists('femail')){
      module_enable(array(
        'femail'
      ));
    }
  }

  /**
   * prepareRow
   * 
   * Ensure we get the mapped CID
   */
  public function prepareRow($row){
    parent::prepareRow($row);
    $query = db_select('migrate_map_forumcomments', 'f')->fields('f', array(
      'destid1'
    ))->condition('sourceid1', $row->cid);
    $cid = $query->execute()->fetchField();
    if($cid){
      $row->cid = $cid;
    }
  }

  /**
   * Query for this migratiojn type
   */
  protected function femailQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('femail_msgs', 'f')->fields('f', array(
      'nid',
      'cid',
      'msgid'
    ));
  }
}

class FemailEmailMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->femailEmailQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'uid' => array(
        'type' => 'int'
      )
    ), MigrateDestinationFemailEmail::getKeySchema());
    $this->destination = new MigrateDestinationFemailEmail();
    // Field mappings
    $this->addSimpleMappings(array(
      'uid',
      'email',
      'status'
    ));
  }

  /**
   * preImport - ensure femail module is enabled
   */
  function preImport(){
    if(!module_exists('femail')){
      module_enable(array(
        'femail'
      ));
    }
  }

  /**
   * Query for this migratiojn type
   */
  protected function femailEmailQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('femail_user_emails', 'f')->fields('f', array(
      'uid',
      'email',
      'status'
    ));
  }
}

class FemailSubscriptionMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->femailSubscriptionQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'uid' => array(
        'type' => 'int'
      ),
      'tid' => array(
        'type' => 'int'
      )
    ), MigrateDestinationFemailSubscription::getKeySchema());
    $this->destination = new MigrateDestinationFemailSubscription();
    // Field mappings
    $this->addSimpleMappings(array(
      'uid',
      'tid'
    ));
  }

  /**
   * preImport - ensure femail module is enabled
   */
  function preImport(){
    if(!module_exists('femail')){
      module_enable(array(
        'femail'
      ));
    }
  }

  /**
   * Query for this migration type
   */
  protected function femailSubscriptionQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('femail_user_subscriptions', 'f')->fields('f', array(
      'uid',
      'tid'
    ));
  }
}