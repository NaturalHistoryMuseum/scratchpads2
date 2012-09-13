<?php

class PathRedirectMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->pathQuery(), $this->sourceFields, NULL, $this->sourceOptions);
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

  /**
   * Rebuild the node access permissions
   */
  protected function preImport(){
		parent::preImport();
		node_access_rebuild(TRUE);
  }
}