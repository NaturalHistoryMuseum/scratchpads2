<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 *   dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 *   the default.
 */
abstract class DrupalNodeMigration extends DrupalMigration {
  /**
   * The source and destination content (node) types we're dealing with and the
   * full mapping of all source and destination content types.
   */
  protected $sourceType;
  protected $destinationType;

  /**
   * Mapping of each source CCK field to its type.
   *
   * @var array
   */
  protected $sourceFieldTypes = array();

  /**
   * Mapping of database column name suffixes for each field type. Derived
   * classes may add custom fields to this array.
   *
   * @var array
   */
  protected $fieldSuffixes = array();

  /**
   * @abstract
   * Version-specific classes must implement nodeQuery, returning a query that
   * provides all the basic fields for each node.
   */
  abstract protected function nodeQuery();

  /**
   * @abstract
   * Version-specific classes must implement sourceFieldList, returning an array
   * of field information retrieved from CCK (or whatever), keyed by field
   * machine name with the field label as the value.
   */
  abstract protected function sourceFieldList();

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    $this->sourceType = $arguments['source_type'];
    $this->destinationType = $arguments['destination_type'];
    if (!empty($arguments['user_migration'])) {
      $user_migration = $arguments['user_migration'];
      $this->dependencies[] = $user_migration;
    }
    parent::__construct($arguments);

    $fields = $this->sourceFieldList();
    $fields['path'] = t('Node: Path alias');

    // Create our three main objects - source, destination, and map

    $this->source = new MigrateSourceSQL($this->nodeQuery(), $fields, NULL,
      $this->sourceOptions);

    $this->destination = new MigrateDestinationNode($this->destinationType);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source node ID',
                       'alias' => 'n',
                      ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Setup common mappings
    $this->addSimpleMappings(array('title', 'status', 'created', 'changed',
      'comment', 'promote', 'sticky'));

    $this->addFieldMapping('language', 'language')
         ->defaultValue('und');

    $this->addFieldMapping(NULL, 'format')
         ->description(t('Translated in prepareRow()'));
    if (!empty($arguments['default_uid'])) {
      $default_uid = $arguments['default_uid'];
    }
    else {
      $default_uid = 0;
    }
         
    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'teaser'),
                                                    array('source_field' => 'format'));
    $this->addFieldMapping('body', 'body')
         ->arguments($arguments);

    $this->addUnmigratedSources(array('vid', 'moderate', 'teaser'));
    $this->addUnmigratedDestinations(array('is_new', 'revision',
      'revision_uid', 'log', 'tnid'));
    
    if (isset($user_migration)) {
      $this->addFieldMapping('uid', 'uid')
           ->sourceMigration($user_migration)
           ->defaultValue($default_uid);
    }
    else {
      $this->addFieldMapping('uid')
           ->defaultValue($default_uid);
    }

    $this->addFieldMapping('path', 'path')
         ->description('Handled in prepareRow');
  }

  /**
   * Called after the query data is fetched - we'll use this to populate the
   * source row with the CCK fields.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Translate the format
    if (!empty($row->format)) {
      $row->format = $this->mapFormat($row->format);
    }
    if (empty($row->format)) {
      $row->format = 'filtered_html';
    }

    // Add the path to the source row, if relevant
    // @TODO: Should check source side instead
    if (module_exists('path')) {
      $path = $this->versionSpecific->getPath('node/' . $row->nid);
      if ($path) {
        $row->path = $path;
      }
    }
  }

  /**
   * Implementation of Migration::createStub().
   *
   * @param $migration
   * @return array|bool
   */
  protected function createStub($migration) {
    migrate_instrument_start('DrupalNodeMigration::createStub');
    $node = new stdClass;
    $node->title = t('Stub');
    $node->body = t('Stub body');
    $node->type = $this->destination->getBundle();
    $node->uid = 1;
    node_save($node);
    migrate_instrument_stop('DrupalNodeMigration::createStub');
    if (isset($node->nid)) {
      return array($node->nid);
    }
    else {
      return FALSE;
    }
  }
}
