<?php

/**
 * @file
 *   A baseball game migration example.
 */


/*
 * You must implement hook_migrate_api(), setting the API level to 2, for
 * your migration classes to be recognized by the Migrate module.
 */
function migrate_example_baseball_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}

/**
 * A dynamic migration that is reused for each source CSV file.
 */
class GameBaseball extends DynamicMigration {
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    $this->description = t('Import box scores from CSV file.');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'start_date' => array('type' => 'varchar',
                           'length' => 8,
                           'not null' => TRUE,
                           'description' => 'Start date',
                          ),
          'home_team' => array('type' => 'varchar',
                           'length' => 255,
                           'not null' => TRUE,
                           'description' => 'Home team',
                          ),
          'home_game_number' => array('type' => 'int',
                           'not null' => TRUE,
                           'description' => 'Home team game number',
                          ),
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($arguments['source_file'], $this->csvcolumns(), array(), $this->fields());

    $this->destination = new MigrateDestinationNode('migrate_example_baseball');

    $this->addFieldMapping('title', 'title')
         ->description('See prepareRow().');
    $this->addFieldMapping('field_start_date', 'start_date');
    $this->addFieldMapping('field_park', 'park_id');
    $this->addFieldMapping('field_visiting_team', 'visiting_team');
    $this->addFieldMapping('field_home_team', 'home_team');
    $this->addFieldMapping('field_home_game_number', 'home_game_number');
    $this->addFieldMapping('field_home_score', 'home_score');
    $this->addFieldMapping('field_visiting_score', 'visiting_score');
    $this->addFieldMapping('field_outs', 'outs');
    $this->addFieldMapping('field_attendance', 'attendance');
    $this->addFieldMapping('field_duration', 'duration')
         ->defaultValue(NULL);
    $this->addFieldMapping('field_home_pitcher', 'home_pitcher');
    $this->addFieldMapping('field_visiting_pitcher', 'visiting_pitcher');
    $this->addFieldMapping('field_home_batters', 'home_batters')
         ->separator(',')
         ->description('See prepareRow().');
    $this->addFieldMapping('field_visiting_batters', 'visiting_batters')
         ->separator(',')
         ->description('See prepareRow().');
    for ($i=1; $i <= 9; $i++ ) {
      $this->addFieldMapping(NULL, "visiting_batter_$i")
           ->description('Not needed since we use the multi-value field: visiting_batters.');
      $this->addFieldMapping(NULL, "home_batter_$i")
           ->description('Not needed since we use the multi-value field: home_batters.');
    }
  }

  function csvcolumns() {
    // Note: Remember to subtract 1 from column number at http://www.retrosheet.org/gamelogs/glfields.txt
    $columns[0] = array('start_date', 'Date of game');
    $columns[3] = array('visiting_team', 'Visiting team');
    $columns[6] = array('home_team', 'Home team');
    $columns[8] = array('home_game_number', 'Home team game number');
    $columns[9] = array('home_score', 'Home score');
    $columns[10] = array('visiting_score', 'Visiting score');
    $columns[11] = array('outs', 'Length of game in outs');
    $columns[16] = array('park_id', 'Ballpark ID');
    $columns[17] = array('attendance', 'Attendance');
    $columns[18] = array('duration', 'Duration in minutes');
    for ($i=1; $i <= 9; $i++ ) {
      $columns[103+3*$i] = array("visiting_batter_$i", "Visiting batter $i");
      $columns[130+3*$i] = array("home_batter_$i", "Home batter $i");
    }
    $columns[102] = array('visiting_pitcher', 'Visiting starting pitcher');
    $columns[104] = array('home_pitcher', 'Home starting pitcher');
    return $columns;
  }

  function prepareRow($row) {
    // Collect all the batters into one multi-value field.
    for ($i=1; $i <= 9; $i++ ) {
      $key = "visiting_batter_$i";
      $visiting_batters[] = $row->$key;
      $key = "home_batter_$i";
      $home_batters[] = $row->$key;
    }
    $row->visiting_batters = implode(',', $visiting_batters);
    $row->home_batters = implode(',', $home_batters);
    $row->title = "$row->home_team vs. $row->visiting_team. " . gmdate('M d, Y', strtotime($row->start_date));
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    return drupal_strtolower(pathinfo($this->arguments['source_file'],
                                      PATHINFO_FILENAME));
  }

  function fields() {
    return array(
      'title' => 'A composite field made during prepareRow().',
      'home_batters' => 'An array of batters, populated during prepareRow().',
      'visiting_batters' => 'An array of batters, populated during prepareRow().',
    );
  }
}