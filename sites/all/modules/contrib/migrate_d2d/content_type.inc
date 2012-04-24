<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalContentTypeMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->menuQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->destination = new MigrateDestinationMenu();
    $this->map = new MigrateSQLMap($this->machineName, array(
      'menu_name' => array(
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
        'description' => 'Primary Key: Unique key for menu. This is used as a block delta so length is 32.',
        'alias' => 'm'
      )
    ), MigrateDestinationMenu::getKeySchema());
    // The mappings are straightforward
    // @todo: Does this for D5 as well, or should we move this to d6/menu.inc?
    $this->addSimpleMappings(array(
      'menu_name',
      'title',
      'description'
    ));
  }

  /**
   * @abstract
   * Version-specific classes must implement menuQuery, returning a query that
   * provides all the basic fields for each menu.
   */
  abstract protected function menuQuery();
}
