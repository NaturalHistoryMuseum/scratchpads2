<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsUserMigration extends DrupalUser6Migration{

  public $unmigratedDesinations = array();

  public $profileFields = array();

  public function __construct(array $arguments){
    // Add role names
    $this->sourceFields['role_names'] = t('User role');
    $this->addFieldMapping('role_names', 'role_names');
    parent::__construct($arguments);
    //     // Change the destination to our modified user destination
    $this->destination = new MigrateDestinationScratchpadsUser(array(
      'md5_passwords' => TRUE
    ));
    // Path isn't used
    $this->removeFieldMapping('path');
    // Add legal accept
    $this->addFieldMapping('legal_accept')->defaultValue(1);
    //Keep UID
    $this->addFieldMapping('uid', 'uid');
    // Handle picture
    $this->removeFieldMapping('picture');
    $this->addFieldMapping('picture', 'picture')->sourceMigration('UserPicture');
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Get the user roles
    $query = Database::getConnection('default', $this->sourceConnection)->select('users_roles', 'ur');
    $query->innerJoin('role', 'r', 'r.rid = ur.rid');
    $query->condition('ur.uid', $row->uid);
    $query->fields('r', array(
      'rid',
      'name'
    ));
    $row->role_names = $query->execute()->fetchAllKeyed();
  }
}