<?php

/**
 * @file
 * Installation code for MySQL embedded database engine.
 */

/**
 * Specifies installation tasks for MySQL and equivalent databases.
 */
class DatabaseTasks_mysql extends DatabaseTasks {
  /**
   * The PDO driver name for MySQL and equivalent databases.
   *
   * @var string
   */
  protected $pdoDriver = 'mysql';

  /**
   * Returns a human-readable name string for MySQL and equivalent databases.
   */
  public function name() {
    return st('MySQL, MariaDB, or equivalent');
  }

  /**
   * Returns the minimum version for MySQL.
   */
  public function minimumVersion() {
    return '5.0.15';
  }

  /**
   * Validates settings, specifically the character set.
   */
  public function validateDatabaseSettings($database) {
    // Perform standard validation.
    $errors = parent::validateDatabaseSettings($database);

    // If we are using utf8mb4 charset, make sure the database supports it.
    if (isset($database['charset']) && $database['charset'] == 'utf8mb4') {
      if (!db_query("SHOW CHARACTER SET WHERE Charset = 'utf8mb4'")->rowCount()) {
        $errors['mysql_charset'] = st('Your database does not support the utf8mb4 character set');
      }
    }

    return $errors;
  }
}

