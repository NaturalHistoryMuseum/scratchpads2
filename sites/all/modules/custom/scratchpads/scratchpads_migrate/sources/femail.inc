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
  function preImport() {
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
  public function prepareRow($row) {
    parent::prepareRow($row);
    $query = db_select('migrate_map_forumcomments', 'f')->fields('f', array(
     'destid1'
    ))->condition('sourceid1', $row->cid);
    $cid = $query->execute()->fetchField();
    if ($cid) {
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

  /**
   * Migrate settings
   */
  protected function postImport(){
    parent::postImport();
    // Copy femail settings accross
    $tables = array(
      'femail_user_emails' => array(
        'uid',
        'email',
        'status'
      ),
      'femail_user_subscriptions' => array(
        'uid',
        'tid'
      )
    );
    foreach ($tables as $table => $fields) {
      $result = Database::getConnection('default', $this->sourceConnection)->select($table, 'f')->fields('f', $fields)->execute();
      foreach ($result as $row) {
        $data = array();
        foreach ($fields as $f) {
          $data[] = $row->$f;
        }
        db_insert($table)->fields($fields, $data)->execute();
      }
    }
  }
}