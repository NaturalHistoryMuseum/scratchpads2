<?php

class PathRedirect6Migration extends PathRedirectMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'source' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationPathRedirect::getKeySchema());
    $this->destination = new MigrateDestinationPathRedirect();
    // Field mappings
    $this->addSimpleMappings(array(
      'source',
      'redirect'
    ));
    $this->addFieldMapping('status_code', 'type');
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  protected function pathQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('path_redirect', 'p')->fields('p', array(
      'source',
      'redirect',
      'type'
    ))->condition('last_used', time() - 31556926, '>'); // Seconds in one year (Google).
  }
}