<?php

/**
 * @file
 * Support for user destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys
// Speed up password generation a ton: $conf['password_count_log2'] = 1;

/**
 * Destination class implementing migration into users.
 */
class MigrateDestinationUser extends MigrateDestinationEntity {

  /**
   * Indicates whether incoming passwords are md5-encrypted - if so, we will
   * rehash them similarly to the D6->D7 upgrade path.
   *
   * @var boolean
   */
  protected $md5Passwords = FALSE;

  static public function getKeySchema() {
    return array(
      'uid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination user',
      ),
    );
  }

  /**
   * Return an options array for user destinations.
   *
   * @param string $language
   *  Default language for usrs created via this destination class.
   * @param string $text_format
   *  Default text format for users created via this destination class.
   * @param boolean $md5_passwords
   *  Set TRUE to indicate incoming passwords are md5-encrypted.
   */
  static public function options($language, $text_format, $md5_passwords) {
    return compact('language', 'text_format', 'md5_passwords');
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to comments.
   */
  public function __construct(array $options = array()) {
    parent::__construct('user', 'user', $options);
    if (!empty($options['md5_passwords'])) {
      $this->md5Passwords = $options['md5_passwords'];
    }

    // Reduce hash count so import runs in a reasonable time (use same value as
    // the standard Drupal 6=>Drupal 7 upgrade path).
    global $conf;
    $conf['password_count_log2'] = 11;
  }

  /**
   * Returns a list of fields available to be mapped for users
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (users table) properties
    $fields['uid'] = t('<a href="@doc">Existing user ID</a>',
      array('@doc' => 'http://drupal.org/node/1349632#uid'));
    $fields['mail'] = t('<a href="@doc">Email address</a>',
      array('@doc' => 'http://drupal.org/node/1349632#mail'));
    $fields['name'] = t('<a href="@doc">Username</a>',
      array('@doc' => 'http://drupal.org/node/1349632#name'));
    $fields['pass'] = t('<a href="@doc">Password</a>',
      array('@doc' => 'http://drupal.org/node/1349632#pass'));
    $fields['status'] = t('<a href="@doc">Status</a>',
      array('@doc' => 'http://drupal.org/node/1349632#status'));
    $fields['created'] = t('<a href="@doc">Registered timestamp</a>',
      array('@doc' => 'http://drupal.org/node/1349632#created'));
    $fields['access'] = t('<a href="@doc">Last access timestamp</a>',
      array('@doc' => 'http://drupal.org/node/1349632#access'));
    $fields['login'] = t('<a href="@doc">Last login timestamp</a>',
      array('@doc' => 'http://drupal.org/node/1349632#login'));
    $fields['roles'] = t('<a href="@doc">Role IDs</a>',
      array('@doc' => 'http://drupal.org/node/1349632#roles'));
    $fields['role_names'] = t('<a href="@doc">Role Names</a>',
      array('@doc' => 'http://drupal.org/node/1349632#role_names'));
    $fields['picture'] = t('<a href="@doc">Picture</a>',
      array('@doc' => 'http://drupal.org/node/1349632#picture'));
    $fields['signature'] = t('<a href="@doc">Signature</a>',
      array('@doc' => 'http://drupal.org/node/1349632#signature'));
    $fields['signature_format'] = t('<a href="@doc">Signature format</a>',
      array('@doc' => 'http://drupal.org/node/1349632#signature_format'));
    $fields['timezone'] = t('<a href="@doc">Timezone</a>',
      array('@doc' => 'http://drupal.org/node/1349632#timezone'));
    $fields['language'] = t('<a href="@doc">Language</a>',
      array('@doc' => 'http://drupal.org/node/1349632#language'));
    $fields['theme'] = t('<a href="@doc">Default theme</a>',
      array('@doc' => 'http://drupal.org/node/1349632#theme'));
    $fields['init'] = t('<a href="@doc">Init</a>',
      array('@doc' => 'http://drupal.org/node/1349632#init'));
    $fields['data'] = t('<a href="@doc">Data</a>',
      array('@doc' => 'http://drupal.org/node/1349632#init'));
    $fields['is_new'] = t('Option: <a href="@doc">Indicates a new user with the specified uid should be created</a>',
      array('@doc' => 'http://drupal.org/node/1349632#is_new'));

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('User', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
  }

  /**
   * Delete a batch of users at once.
   *
   * @param $uids
   *  Array of user IDs to be deleted.
   */
  public function bulkRollback(array $uids) {
    migrate_instrument_start('user_delete_multiple');
    $this->prepareRollback($uids);
    user_delete_multiple($uids);
    $this->completeRollback($uids);
    migrate_instrument_stop('user_delete_multiple');
  }

  /**
   * Import a single user.
   *
   * @param $account
   *  Account object to build. Prefilled with any fields mapped in the
   *   Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields (uid only in this case) of the user that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $account, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      // Make sure is_new is off
      $account->is_new = FALSE;
      if (isset($account->uid)) {
        if ($account->uid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming uid !uid and map destination uid !destid1 don't match",
            array(
              '!uid' => $account->uid,
              '!destid1' => $row->migrate_map_destid1,
            )));
        }
      }
      else {
        $account->uid = $row->migrate_map_destid1;
      }
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($account->uid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination uid provided'));
      }
      $old_account = user_load($account->uid, TRUE);
      if (empty($old_account)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but user !uid does not exist',
          array('!uid' => $account->uid)));
      }
    }
    else {
      $old_account = $account;
    }

    // Roles must be arrays keyed by the role id, which isn't how the data
    // naturally comes in. Fix them up.

    // First, if names instead of IDs are presented, translate them
    if (!empty($account->role_names)) {
      $role_names = is_array($account->role_names) ? $account->role_names : array($account->role_names);
      foreach ($role_names as $role_name) {
        $role = user_role_load_by_name($role_name);
        if ($role) {
          $account->roles[] = $role->rid;
        }
      }
    }
    if (!empty($account->roles)) {
      if (!is_array($account->roles)) {
        $account->roles = array($account->roles);
      }
      $account->roles = drupal_map_assoc($account->roles);
    }
    if (empty($account->roles) && empty($old_account->roles)) {
      $account->roles = array();
    }

    $this->prepare($account, $row);

    if (isset($account->uid) && !(isset($account->is_new) && $account->is_new)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    // While user_save is happy to see a fid in $account->picture on insert,
    // when updating an existing account it wants a file object.
    if ($updating && isset($account->picture) && ($fid = $account->picture)) {
      $account->picture = file_load($fid);
    }

    // Normalize MD5 passwords to lowercase, as generated by Drupal 6 and previous
    if ($this->md5Passwords) {
      $account->pass = drupal_strtolower($account->pass);
    }

    // If any datetime values were included, ensure that they're in timestamp format.
    if (isset($account->created)) {
      $account->created = MigrationBase::timestamp($account->created);
    }
    if (isset($account->access)) {
      $account->access = MigrationBase::timestamp($account->access);
    }
    if (isset($account->login)) {
      $account->login = MigrationBase::timestamp($account->login);
    }

    // Validate field data prior to saving.
    MigrateDestinationEntity::fieldAttachValidate('user', $account);

    migrate_instrument_start('user_save');
    $newaccount = user_save($old_account, (array) $account);
    migrate_instrument_stop('user_save');
    if ($newaccount) {
      if ($this->md5Passwords && !empty($account->pass)) {
        // Ape the Drupal 6 -> Drupal 7 upgrade, which encrypts the MD5 text in the
        // modern way, and marks it with a prepended U so it recognizes and fixes it
        // up at login time.
        $password = 'U' . $newaccount->pass;
        db_update('users')
          ->fields(array('pass' => $password))
          ->condition('uid', $newaccount->uid)
          ->execute();
      }
      // Unlike nodes and taxonomy terms, core does not automatically save an
      // alias in a user entity, we must do it ourselves.
      if (module_exists('path')) {
        if (!empty($account->path['alias'])) {
          $path = array(
            'source' => 'user/' . $account->uid,
            'alias' => $account->path['alias'],
          );
          migrate_instrument_start('path_save');
          path_save($path);
          migrate_instrument_stop('path_save');
        }
      }
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
        // user_save() doesn't update file_usage on account creation, we have
        // to do it ourselves.
        if (!empty($newaccount->picture)) {
          $file = file_load($newaccount->picture);
          if (is_object($file)) {
            file_usage_add($file, 'user', 'user', $newaccount->uid);
          }
        }
      }
      $this->complete($newaccount, $row);
      $return = array($newaccount->uid);
    }
    else {
      $return = FALSE;
    }
    return $return;
  }
}

class MigrateDestinationRole extends MigrateDestinationTable {

  public function __construct() {
    parent::__construct('role');
  }

  /**
   * Get the key definition for the role table.
   *
   * @param $dummy
   *  PHP is picky - it throws E_STRICT notices if we don't have a parameter
   *  because MigrateDestinationTable has one.
   */
  static public function getKeySchema($dummy = NULL) {
    return MigrateDestinationTable::getKeySchema('role');
  }

  /**
   * Delete a single row.
   *
   * @param $id
   *  Primary key values.
   */
  public function rollback(array $id) {
    migrate_instrument_start('role rollback');
    $rid = reset($id);
    user_role_delete((int) $rid);
    migrate_instrument_stop('role rollback');
  }

  /**
   * Import a single row.
   *
   * @param $entity
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    $migration = Migration::currentMigration();
    $updating = FALSE;
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      $updating = TRUE;
      if (isset($entity->rid)) {
        if ($entity->rid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming id !id and map destination id !destid don't match",
            array(
              '!id' => $entity->rid,
              '!destid' => $row->migrate_map_destid1,
            )));
        }
      }
      else {
        $entity->rid = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      $updating = TRUE;
      if (!isset($entity->rid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination id provided'));
      }

      $old_entity = user_role_load($entity->rid);

      foreach ($entity as $field => $value) {
        $old_entity->{$field} = $entity->{$field};
      }
      $entity = $old_entity;
    }

    $this->prepare($entity, $row);
    user_role_save($entity);
    $this->complete($entity, $row);

    if (!empty($entity->rid)) {
      $id = array($entity->rid);
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
    }
    else {
      $id = FALSE;
    }
    return $id;
  }
}
