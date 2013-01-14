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
      'site_slogan' => 'site_slogan',
      'site_frontpage' => 'site_frontpage',
      'site_mission' => 'front_page_welcome_message',
      'language_default' => 'language_default',
      'user_registration_help' => 'user_registration_help'
    );
    $default_theme = Database::getConnection('default', 'scratchpad_1')->select('variable', 'v')->fields('v', array(
      'value'
    ))->condition('name', 'theme_default')->execute()->fetchField();
    if($default_theme){
      $theme_var = 'theme_' . unserialize($default_theme) . '_settings';
      $variable_names[$theme_var] = $theme_var;
    }
    $result = Database::getConnection('default', 'scratchpad_1')->select('filters', 'f')->fields('f', array(
      'format'
    ))->distinct()->execute();
    foreach($result as $record){
      $variable_names['glossary_vids_' . $record->format] = 'lexicon_vids';
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
    switch($row->name){
      case 'site_frontpage':
        // Do we want to use this front page?
        if(!scratchpads_migrate_variable_get('keep_homepage')){return false;}
        break;
      case 'user_registration_help':
        // We're not actually going to migrate this variable
        // Just create a block - see block.install line 326
        if(isset($row->value) && !empty($row->value)){
          $user_help = unserialize($row->value);
          if($user_help){
            // Create a block
            $bid = db_insert('block_custom')->fields(array(
              'body' => $user_help,
              'info' => 'User registration guidelines',
              'format' => 'filtered_html'
            ))->execute();
            // Collect a list of themes with blocks.
            $result = db_query("SELECT s.name FROM {system} s INNER JOIN {block} b ON s.name = b.theme WHERE s.type = 'theme' GROUP by s.name");
            $themes_with_blocks = $result->fetchCol();
            // Add contact help block
            $insert = db_insert('block')->fields(array(
              'module',
              'delta',
              'theme',
              'status',
              'weight',
              'region',
              'visibility',
              'pages',
              'cache'
            ));
            foreach($themes_with_blocks as $theme){
              // Add user registration help block for themes, which had blocks.
              $insert->values(array(
                'module' => 'block',
                'delta' => $bid,
                'theme' => $theme,
                'status' => 1,
                'weight' => -1,
                'region' => 'content',
                'visibility' => BLOCK_VISIBILITY_LISTED,
                'pages' => 'user/register',
                'cache' => DRUPAL_NO_CACHE
              ));
              $insert->execute();
            }
          }
        }
        // We don't want to actually migrate this variable
        return false;
        break;
    }
  }
}