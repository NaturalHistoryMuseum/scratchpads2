<?php

class CitationMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->citationQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationCitation::getKeySchema());
    $this->destination = new MigrateDestinationCitation();
    // Field mappings
    $this->addSimpleMappings(array(
      'cid',
      'created',
      'reference',
      'url'
    ));
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  protected function citationQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('citation', 'c')->fields('c');
  }
}