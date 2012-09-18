<?php

class VariableMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->variableQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationVariable::getKeySchema());
    $this->destination = new MigrateDestinationVariable();
    // Field mappings
    $this->addSimpleMappings(array(
      'name',
      'value'
    ));
  }

  /**
   * Returns an array of variables to transfer.
   * 
   * The key is the name of the variable in D6, with the value being the name in
   * D7 (just in case the name of the variable has changed).
   */
  public function variableNames(){
    $variable_names = array(
    	'site_type' => 'site_type',
      'site_name' => 'site_name',
      'site_mail' => 'site_mail',
      'site_frontpage' => 'site_frontpage',
      'site_mission' => 'front_page_welcome_message'
    );
    $default_theme = Database::getConnection('default', 'scratchpad_1')->select('variable', 'v')->fields('v', array(
      'value'
    ))->condition('name', 'theme_default')->execute()->fetchField();
    if($default_theme){
      $theme_var = 'theme_' . unserialize($default_theme) . '_settings';
      $variable_names[$theme_var] = $theme_var;
    }
    return $variable_names;
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  protected function variableQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('variable', 'v')->fields('v', array(
      'name',
      'value'
    ))->condition('name', array_keys($this->variableNames()));
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    if($row->name == 'site_frontpage'){
      // Do we want to use this front page?
      if(!scratchpads_migrate_variable_get('keep_homepage')){return false;}
    }
  }
}