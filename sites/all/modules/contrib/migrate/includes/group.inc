<?php

/**
 * @file
 * Definition for a migration group.
 */

class MigrateGroup {
  /**
   * The name of the group - used to identify it in drush commands.
   *
   * @var string
   */
  protected $name;
  public function getName() {
    return $this->name;
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
    $required_groups = array();
    foreach (self::$groupList as $name => $group) {
      $dependencies = $group->getDependencies();
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
    $iterations = 0;
    while (count($dependent_groups) > 0) {
      if ($iterations++ > 20) {
        $group_names = implode(',', array_keys($dependent_groups));
        throw new MigrateException(t('Failure to sort migration list - most likely due ' .
              'to circular dependencies involving groups !group_names',
        array('!group_names' => $group_names)));
      }
      foreach ($dependent_groups as $name => $group) {
        $ready = TRUE;
        // Scan all the dependencies for this group and make sure they're all
        // in the final list
        foreach ($group->getDependencies() as $dependency) {
          if (!isset($groups[$dependency])) {
            $ready = FALSE;
            break;
          }
        }
        if ($ready) {
          // Yes they are! Move this group to the final list
          $groups[$name] = $group;
          unset($dependent_groups[$name]);
        }
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
  public function __construct($name, $dependencies = array()) {
    $this->name = $name;
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
      self::$groupList[$name] = new MigrateGroup($name, $dependencies);
    }
    return self::$groupList[$name];
  }
}
