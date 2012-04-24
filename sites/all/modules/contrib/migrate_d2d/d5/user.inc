<?php

/**
 * Handling specific to a Drupal 5 source for users.
 */
class DrupalUser5Migration extends DrupalUserMigration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Indicate that our incoming MD5 passwords should be rehashed for Drupal 7.
    $this->destination = new MigrateDestinationUser(array('md5_passwords' => TRUE));

    // Per-user comment settings dropped in Drupal 7.
    $this->addUnmigratedSources(array('mode', 'threshold', 'sort'));

    $this->addFieldMapping(NULL, 'timezone_name')
         ->description('If present, assigned to D7 timezone');
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    /**
     * Timezones need some runtime massaging when going from D5 to D7. Note that
     * the date module adds a timezone_name column to the users table, so if
     * present we can use that directly. Otherwise, we do as the D5->D7 upgrade
     * does and just clear it - let users reset their timezones in the D7 site.
     */
    if (!empty($row->timezone_name)) {
      $row->timezone = $row->timezone_name;
    }
    else {
      $row->timezone = NULL;
    }
  }
}
