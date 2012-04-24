<?php

/**
 * Base class for all user migrations - handles commonalities across all
 * supported source Drupal versions.
 */
abstract class DrupalUserMigration extends DrupalMigration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->userQuery(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source user ID',
                       'alias' => 'u',
                      ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    // Most mappings are straight-forward
    // @todo: Anything special to do with theme?
    $this->addSimpleMappings(array('pass', 'mail', 'theme', 'signature',
      'created', 'access', 'login', 'status', 'language', 'init', 'timezone'));
    // user_save() expects the data field to be unpacked
    $this->addFieldMapping('data', 'data')
         ->callbacks('unserialize');
    // Dedupe username collisions by appending _1, _2, etc.
    $this->addFieldMapping('name', 'name')
         ->dedupe('users', 'name');
    $this->addFieldMapping('picture', 'picture')
         ->description(t('TODO: map picture fids'))
         ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_MEDIUM);
    $this->addFieldMapping('roles')
         ->description(t('TODO: handle roles automatically'))
         ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_MEDIUM);
    $this->addFieldMapping('signature_format', 'signature_format')
         ->callbacks(array($this, 'mapFormat'))
         ->description(t('We map D6 format numbers to D7 format machine names'));

    $this->addUnmigratedDestinations(array('is_new'));

    $this->addFieldMapping('path', 'path')
         ->description('Handled in prepareRow');

    if (module_exists('pathauto')) {
      $this->addFieldMapping('pathauto')
           ->description('By default, disable in favor of migrated paths')
           ->defaultValue(0);
    }
  }

  /**
   * Query for the basic user data.
   *
   * @return QueryConditionInterface
   */
  protected function userQuery() {
    // Do not attempt to migrate the anonymous or admin user rows.
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('users', 'u')
             ->fields('u')
             ->condition('uid', 1, '>');
    return $query;
  }

  /**
   * Review a data row after fetch, returning FALSE to skip it.
   *
   * @param $row
   * @return bool
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    /*
    // On initial import, skip accounts with email addresses already existing
    // on the destination side (we assume these are manually-created staff
    // accounts or the like).
    // TODO: Add options to overwrite such accounts, throw errors, etc.
    if (empty($row->migrate_map_sourceid1)) {
      $uid = db_select('users', 'u')
             ->fields('u', array('uid'))
             ->condition('mail', $row->mail)
             ->execute()
             ->fetchField();
      if ($uid) {
        return FALSE;
      }
    }*/

    // Add the path to the source row, if relevant
    // @TODO: Should check source side instead
    if (module_exists('path')) {
      $path = $this->versionSpecific->getPath('user/' . $row->uid);
      if ($path) {
        $row->path = $path;
      }
    }

    return TRUE;
  }

  /**
   * Actions to take after a user account has been saved.
   *
   * @param $account
   * @param $row
   */
  public function complete($account, $row) {
    // Unlike nodes and taxonomy terms, core does not automatically save an
    // alias in a user entity, we must do it ourselves.
    if (module_exists('path')) {
      if (isset($account->path['alias'])) {
        $path = array(
          'source' => 'user/' . $account->uid,
          'alias' => $account->path['alias'],
        );
        migrate_instrument_start('path_save');
        path_save($path);
        migrate_instrument_stop('path_save');
      }
    }
  }
}
