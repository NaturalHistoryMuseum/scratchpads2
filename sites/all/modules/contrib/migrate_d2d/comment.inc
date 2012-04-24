<?php

/**
 * Base class for all comment migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 entity machine name (@todo: set automatically)
 * node_migration - The node migration machine name for source_type
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 *   dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 *   the default.
 */
abstract class DrupalCommentMigration extends DrupalMigration {

  protected $sourceType;
  protected $destinationType;
  protected $nodeMigration;

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    $this->sourceType = $arguments['source_type'];
    $this->destinationType = $arguments['destination_type'];
    $this->nodeMigration = $arguments['node_migration'];
    $this->dependencies[] = $arguments['node_migration'];

    if (!empty($arguments['user_migration'])) {
      $user_migration = $arguments['user_migration'];
      $this->dependencies[] = $user_migration;
    }
    parent::__construct($arguments);

    // Create our three main objects - source, destination, and map

    $this->source = new MigrateSourceSQL($this->commentQuery($this->sourceType),
      $this->sourceFields, NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationComment('comment_node_' . $this->destinationType);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'cid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source comment ID',
                       'alias' => 'c',
                      ),
      ),
      MigrateDestinationComment::getKeySchema()
    );

    /**
     * Setup common mappings.
     * @todo: These mappings are common to D6 and D7, but I'm not sure for D5.
     * The following mappings are version-specific:
     *   -'created'
     *   -'changed'
     *   -'comment_body'
     */
    $this->addSimpleMappings(array('pid', 'subject',
      'hostname', 'thread', 'name', 'mail', 'homepage'));

    $this->addFieldMapping('nid', 'nid')
         ->sourceMigration($this->nodeMigration);

    if (!empty($arguments['default_uid'])) {
      $default_uid = $arguments['default_uid'];
    }
    else {
      $default_uid = 1;
    }
    if (isset($user_migration)) {
      $this->addFieldMapping('uid', 'uid')
           ->sourceMigration($user_migration)
           ->defaultValue($default_uid);
    }
    else {
      $this->addFieldMapping('uid')
           ->defaultValue($default_uid);
    }
  }

  /**
   * In D7, comments are entities, and need to be handled as in
   * migrate_d2d/node.inc::prepareRow()
   *
   * But because comments are more boring in D6, nothing needs to be done here.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }

  /**
   * Implementation of Migration::createStub().
   * @todo: if possible, implement this :)
   *
   * @param $migration
   * @return array|bool
   */
/*
  protected function createStub($migration) {
      return FALSE;
  }
*/
}
