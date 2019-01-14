<?php

/**
 * @file
 * Definition for a migration group.
 */

class MigrateGroup {

  /**
   * The machine name of the group - used to identify it in drush commands.
   *
   * @var string
   */
  protected $name;

  public function getName() {
    return $this->name;
  }

  /**
   * The user-visible title of the group.
   *
   * @var string
   */
  protected $title;

  public function getTitle() {
    return $this->title;
  }

  /**
   * Domain-specific arguments for the group (to be applied to all migrations
   * in the group).
   *
   * @var array
   */
  protected $arguments = array();

  public function getArguments() {
    return $this->arguments;
  }

  /**
   * List of groups this group is dependent on.
   *
   * @var array
   */
  protected $dependencies = array();

  public function getDependencies() {
    return $this->dependencies;
  }

  /**
   * The central list of all known groups, keyed by group name.
   *
   * @var array
   */
  static protected $groupList = array();

  static public function groups() {
    $groups = array();
    $dependent_groups = array();
    $dependencies_list = array();
    $required_groups = array();
    foreach (self::$groupList as $name => $group) {
      $dependencies = $group->getDependencies();
      $dependencies_list[$name] = $dependencies;
      if (count($dependencies) > 0) {
        // Set groups with dependencies aside for reordering
        $dependent_groups[$name] = $group;
        $required_groups += $dependencies;
      }
      else {
        // No dependencies, just add
        $groups[$name] = $group;
      }
    }

    $ordered_groups = migrate_order_dependencies($dependencies_list);
    foreach ($ordered_groups as $name) {
      if (!isset($groups[$name])) {
        $groups[$name] = $dependent_groups[$name];
      }
    }

    return $groups;
  }

  /**
   * Basic constructor.
   *
   * @param string $name
   *  Group name.
   *
   * @param array $dependencies
   *  List of dependent groups.
   */
  public function __construct($name, $dependencies = array(), $title = '', $arguments = array()) {
    $this->name = $name;
    $this->title = $title;
    $this->arguments = $arguments;
    $this->dependencies = $dependencies;
  }

  /**
   * Retrieve (creating if necessary) an instance of the named group.
   *
   * @param string $name
   *  Group name.
   *
   * @param array $dependencies
   *  List of dependent groups.
   */
  static public function getInstance($name, $dependencies = array()) {
    if (empty(self::$groupList[$name])) {
      $row = db_select('migrate_group', 'mg')
        ->fields('mg')
        ->condition('name', $name)
        ->execute()
        ->fetchObject();
      if ($row) {
        $arguments = unserialize($row->arguments);
        $arguments = MigrationBase::decryptArguments($arguments);
        if (empty($dependencies) && isset($arguments['dependencies'])) {
          $dependencies = $arguments['dependencies'];
        }
        self::$groupList[$name] = new MigrateGroup($name, $dependencies,
          $row->title, $arguments);
      }
      else {
        self::register($name);
        self::$groupList[$name] = new MigrateGroup($name, $dependencies);
      }
    }
    return self::$groupList[$name];
  }

  /**
   * Register a new migration group in the migrate_group table.
   *
   * @param string $name
   *  The machine name (unique identifier) for the group.
   *
   * @param string $title
   *  A user-visible title for the group. Defaults to the machine name.
   *
   * @param array $arguments
   *  An array of group arguments - generally data that applies to all
   *   migrations in the group.
   */
  static public function register($name, $title = NULL, array $arguments = array()) {
    if (!$title) {
      $title = $name;
    }

    $arguments = MigrationBase::encryptArguments($arguments);

    // Register the migration if it's not already there; if it is,
    // update the class and arguments in case they've changed.
    db_merge('migrate_group')
      ->key(array('name' => $name))
      ->fields(array(
        'title' => $title,
        'arguments' => serialize($arguments),
      ))
      ->execute();
  }


  /**
   * Deregister a migration group - remove it from the database, and also
   * remove migrations attached to it.
   *
   * @param string $name
   *  (Machine) name of the group to deregister.
   */
  static public function deregister($name) {
    $result = db_select('migrate_status', 'ms')
      ->fields('ms', array('machine_name'))
      ->condition('group_name', $name)
      ->execute();
    foreach ($result as $row) {
      Migration::deregisterMigration($row->machine_name);
    }

    db_delete('migrate_group')
      ->condition('name', $name)
      ->execute();
  }

  /**
   * Remove any groups which no longer contain any migrations.
   */
  static public function deleteOrphans() {
    $query = db_select('migrate_group', 'mg');
    $query->addField('mg', 'name');
    $query->leftJoin('migrate_status', 'ms', 'mg.name=ms.group_name');
    $query->isNull('ms.machine_name');
    $result = $query->execute();
    foreach ($result as $row) {
      db_delete('migrate_group')
        ->condition('name', $row->name)
        ->execute();
    }
  }
}
