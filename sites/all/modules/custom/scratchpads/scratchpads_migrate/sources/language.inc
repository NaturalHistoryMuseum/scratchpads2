<?php

class LanguageMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $table_name = 'languages';
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'language' => array(
        'type' => 'varchar',
        'length' => 12,
        'not null' => TRUE
      )
    ), MigrateDestinationTable::getKeySchema($table_name));
    $this->destination = new MigrateDestinationTable($table_name);
    // Field mappings
    $this->addSimpleMappings(array(
      'language',
      'name',
      'native',
      'direction',
      'enabled',
      'plurals',
      'formula',
      'domain',
      'prefix',
      'weight',
      'javascript'
    ));
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  protected function sourceQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('languages', 'l')->fields('l')->condition('enabled', 1)->condition('language', 'en', '!=');
  }

  protected function postImport(){
    parent::postImport();
    variable_set('language_count', $this->processedCount() + 1);
    // Changing the language settings impacts the interface.
    cache_clear_all('*', 'cache_page', TRUE);
    module_invoke_all('multilingual_settings_changed');
    $modules = module_list();
    locale_system_update($modules);
  }
}