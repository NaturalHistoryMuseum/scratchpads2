<?php

class BlockMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->description = t('Migrate blocks.');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'bid' => array(
        'type' => 'int',
        'description' => 'Block ID.'
      )
    ), MigrateDestinationBlock::getKeySchema());
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->destination = new MigrateDestinationBlock();
    $this->addSimpleMappings(array(
      'pages',
      'title',
      'body',
      'info',
      'visibility',
      'status',
      'module',
      'delta'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('blocks', 'b');
    $query->fields('b', array(
      'bid',
      'pages',
      'title',
      'visibility',
      'status',
      'module',
      'delta'
    ));
    $query->leftJoin('boxes', 'bx', 'bx.bid=b.delta');
    $query->fields('bx', array(
      'body',
      'info'
    ));
    $query->condition('status', 1);
    $query->condition('module', array(
      'block',
      'menu'
    ), 'IN');
    $query->condition('delta', array(
      'primary-links',
      'secondary-links'
    ), 'NOT IN');
    $query->condition('theme', 'garland');
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    // We don't know who created the block and added images, so just use UID=2
    $row->uid = 2;
    // Extract and save the images
    $row->body = ScratchpadsNodeMigration::parseImages($row->body, $row);
  }
}
