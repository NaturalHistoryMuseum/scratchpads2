<?php

/**
 * @file
 * Defines the base class for import/rollback processes.
 */

/**
 * The base class for all import objects. This is where most of the smarts
 * of the migrate module resides. Migrations are created by deriving from this
 * class, and in the constructor (after calling parent::__construct())
 * initializing at a minimum the name, description, source, and destination
 * properties. The constructor will also usually make several calls to
 * addFieldMapping().
 */
abstract class Migration extends MigrationBase {

  /**
   * Source object for the migration, derived from MigrateSource.
   *
   * @var MigrateSource
   */
  protected $source;

  public function getSource() {
    return $this->source;
  }

  public function setSource(MigrateSource $source) {
    $this->source = $source;
  }

  /**
   * Destination object for the migration, derived from MigrateDestination.
   *
   * @var MigrateDestination
   */
  protected $destination;

  public function getDestination() {
    return $this->destination;
  }

  public function setDestination(MigrateDestination $destination) {
    $this->destination = $destination;
  }

  /**
   * Map object tracking relationships between source and destination data
   *
   * @var MigrateMap
   */
  protected $map;

  public function getMap() {
    return $this->map;
  }

  public function setMap(MigrateMap $map) {
    $this->map = $map;
  }

  /**
   * Indicate whether the primary system of record for this migration is the
   * source, or the destination (Drupal). In the source case, migration of
   * an existing object will completely replace the Drupal object with data from
   * the source side. In the destination case, the existing Drupal object will
   * be loaded, then changes from the source applied; also, rollback will not be
   * supported.
   *
   * @var int
   */
  const SOURCE = 1;

  const DESTINATION = 2;

  protected $systemOfRecord = Migration::SOURCE;

  public function getSystemOfRecord() {
    return $this->systemOfRecord;
  }

  public function setSystemOfRecord($system_of_record) {
    $this->systemOfRecord = $system_of_record;
  }

  /**
   * Specify value of needs_update for current map row. Usually set by
   * MigrateFieldHandler implementations.
   *
   * @var int
   */
  public $needsUpdate = MigrateMap::STATUS_IMPORTED;

  /**
   * The default rollback action for this migration. Can be overridden on
   * a per-row basis by setting $row->rollbackAction in prepareRow().
   *
   * @var int
   */
  protected $defaultRollbackAction = MigrateMap::ROLLBACK_DELETE;

  public function getDefaultRollbackAction() {
    return $this->defaultRollbackAction;
  }

  public function setDefaultRollbackAction($rollback_action) {
    $this->defaultRollbackAction = $rollback_action;
  }

  /**
   * The rollback action to be saved for the current row.
   *
   * @var int
   */
  public $rollbackAction;

  /**
   * Field mappings defined in code.
   *
   * @var array
   */
  protected $storedFieldMappings = array();

  protected $storedFieldMappingsRetrieved = FALSE;

  public function getStoredFieldMappings() {
    if (!$this->storedFieldMappingsRetrieved) {
      $this->loadFieldMappings();
      $this->storedFieldMappingsRetrieved = TRUE;
    }
    return $this->storedFieldMappings;
  }

  /**
   * Field mappings retrieved from storage.
   *
   * @var array
   */
  protected $codedFieldMappings = array();

  public function getCodedFieldMappings() {
    return $this->codedFieldMappings;
  }

  /**
   * All field mappings, with those retrieved from the database overriding those
   * defined in code.
   *
   * @var array
   */
  protected $allFieldMappings = array();

  public function getFieldMappings() {
    if (empty($allFieldMappings)) {
      $this->allFieldMappings = array_merge($this->getCodedFieldMappings(),
        $this->getStoredFieldMappings());
      // If there are multiple mappings of a given source field to no
      // destination field, keep only the last (so the UI can override a source
      // field DNM that was defined in code).
      $no_destination = array();
      // But also remove a mapping of a source field to nothing, if there is
      // a mapping to something.
      $mapped_source_fields = array();
      /** @var MigrateFieldMapping $mapping */
      foreach ($this->allFieldMappings as $destination_field => $mapping) {
        $source_field = $mapping->getSourceField();
        // If the source field is not mapped to a destination field, the
        // array index is integer.
        if (is_int($destination_field)) {
          if (isset($no_destination[$source_field])) {
            unset($this->allFieldMappings[$no_destination[$source_field]]);
            unset($no_destination[$source_field]);
          }
          $no_destination[$source_field] = $destination_field;
          if (isset($mapped_source_fields[$source_field])) {
            unset($this->allFieldMappings[$destination_field]);
          }
        }
        else {
          $mapped_source_fields[$source_field] = $source_field;
        }
      }

      // Make sure primary fields come before their subfields
      ksort($this->allFieldMappings);
    }
    return $this->allFieldMappings;
  }

  /**
   * An array of counts. Initially used for cache hit/miss tracking.
   *
   * @var array
   */
  protected $counts = array();

  /**
   * When performing a bulkRollback(), the maximum number of items to pass in
   * a single call. Can be overridden in derived class constructor.
   *
   * @var int
   */
  protected $rollbackBatchSize = 50;

  /**
   * If present, an array with keys name and alias (optional). Name refers to
   * the source columns used for tracking highwater marks. alias is an
   * optional table alias.
   *
   * @var array
   */
  protected $highwaterField = array();

  public function getHighwaterField() {
    return $this->highwaterField;
  }

  public function setHighwaterField(array $highwater_field) {
    $this->highwaterField = $highwater_field;
  }

  /**
   * The object currently being constructed
   *
   * @var stdClass
   */
  protected $destinationValues;

  /**
   * The current data row retrieved from the source.
   *
   * @var stdClass
   */
  protected $sourceValues;

  /**
   * Queue up messages that can't be safely saved (in particular, if they're
   * generated in prepareRow().
   *
   * @var array
   */
  protected $queuedMessages = array();

  /**
   * General initialization of a Migration object.
   */
  public function __construct($arguments = array()) {
    parent::__construct($arguments);
  }

  /**
   * Register a new migration process in the migrate_status table. This will
   * generally be used in two contexts - by the class detection code for
   * static (one instance per class) migrations, and by the module implementing
   * dynamic (parameterized class) migrations.
   *
   * @param string $class_name
   * @param string $machine_name
   * @param array $arguments
   */
  static public function registerMigration($class_name, $machine_name = NULL,
                                           array $arguments = array()) {
    // Record any field mappings provided via arguments.
    if (isset($arguments['field_mappings'])) {
      self::saveFieldMappings($machine_name, $arguments['field_mappings']);
      unset($arguments['field_mappings']);
    }

    parent::registerMigration($class_name, $machine_name, $arguments);
  }

  /**
   * Deregister a migration - remove all traces of it from the database (without
   * touching any content which was created by this migration).
   *
   * We'd like to do this at uninstall time, but the implementing module is
   * already disabled, so we can't instantiate it to get at the map. This can
   * be done in hook_disable(), however.
   *
   * @param string $machine_name
   */
  static public function deregisterMigration($machine_name) {
    try {
      // Remove map and message tables
      $migration = self::getInstance($machine_name);
      if ($migration && method_exists($migration, 'getMap')) {
        $migration->getMap()->destroy();
      }

      // @todo: Clear log entries? Or keep for historical purposes?

      // Remove stored field mappings for this migration
      $rows_deleted = db_delete('migrate_field_mapping')
        ->condition('machine_name', $machine_name)
        ->execute();

      // Call the parent deregistration (which clears migrate_status) last, the
      // above will reference it.
      parent::deregisterMigration($machine_name);
    } catch (Exception $e) {
      // Fail silently if it's already gone
    }
  }

  /**
   * Record an array of field mappings to the database.
   *
   * @param $machine_name
   * @param array $field_mappings
   */
  static public function saveFieldMappings($machine_name, array $field_mappings) {
    // Clear existing field mappings
    db_delete('migrate_field_mapping')
      ->condition('machine_name', $machine_name)
      ->execute();
    foreach ($field_mappings as $field_mapping) {
      $destination_field = $field_mapping->getDestinationField();
      $source_field = $field_mapping->getSourceField();
      db_insert('migrate_field_mapping')
        ->fields(array(
          'machine_name' => $machine_name,
          'destination_field' => is_null($destination_field) ? '' : $destination_field,
          'source_field' => is_null($source_field) ? '' : $source_field,
          'options' => serialize($field_mapping),
        ))
        ->execute();
    }
  }

  /**
   * Load any stored field mappings from the database.
   */
  public function loadFieldMappings() {
    $result = db_select('migrate_field_mapping', 'mfm')
      ->fields('mfm', array('destination_field', 'source_field', 'options'))
      ->condition('machine_name', $this->machineName)
      ->execute();
    foreach ($result as $row) {
      $field_mapping = unserialize($row->options);
      $field_mapping->setMappingSource(MigrateFieldMapping::MAPPING_SOURCE_DB);
      if (empty($row->destination_field)) {
        $this->storedFieldMappings[] = $field_mapping;
      }
      else {
        $this->storedFieldMappings[$row->destination_field] = $field_mapping;
      }
    }
  }

  ////////////////////////////////////////////////////////////////////
  // Processing

  /**
   * Add a mapping for a destination field, specifying a source field and/or
   * a default value.
   *
   * @param string $destinationField
   *  Name of the destination field.
   * @param string $sourceField
   *  Name of the source field (optional).
   * @param boolean $warn_on_override
   *  Set to FALSE to prevent warnings when there's an existing mapping
   *  for this destination field.
   */
  public function addFieldMapping($destination_field, $source_field = NULL,
                                  $warn_on_override = TRUE) {
    // Warn of duplicate mappings
    if ($warn_on_override && !is_null($destination_field) &&
      isset($this->codedFieldMappings[$destination_field])) {
      self::displayMessage(
        t('!name addFieldMapping: !dest was previously mapped from !source, overridden',
          array(
            '!name' => $this->machineName,
            '!dest' => $destination_field,
            '!source' => $this->codedFieldMappings[$destination_field]->getSourceField(),
          )),
        'warning');
    }
    $mapping = new MigrateFieldMapping($destination_field, $source_field);
    if (is_null($destination_field)) {
      $this->codedFieldMappings[] = $mapping;
    }
    else {
      $this->codedFieldMappings[$destination_field] = $mapping;
    }
    return $mapping;
  }

  /**
   * Remove any existing coded mappings for a given destination or source field.
   *
   * @param string $destination_field
   *  Name of the destination field.
   * @param string $source_field
   *  Name of the source field.
   */
  public function removeFieldMapping($destination_field, $source_field = NULL) {
    if (isset($destination_field)) {
      unset($this->codedFieldMappings[$destination_field]);
    }
    if (isset($source_field)) {
      foreach ($this->codedFieldMappings as $key => $mapping) {
        if ($mapping->getSourceField() == $source_field) {
          unset($this->codedFieldMappings[$key]);
        }
      }
    }
  }

  /**
   * Shortcut for adding several fields which have the same name on both source
   * and destination sides.
   *
   * @param array $fields
   *  List of field names to map.
   */
  public function addSimpleMappings(array $fields) {
    foreach ($fields as $field) {
      $this->addFieldMapping($field, $field);
    }
  }

  /**
   * Shortcut for adding several destination fields which are to be explicitly
   * not migrated.
   *
   * @param array $fields
   *  List of fields to mark as not for migration.
   *
   * @param string $issue_group
   *  Issue group name to apply to the generated mappings (defaults to 'DNM').
   */
  public function addUnmigratedDestinations(array $fields, $issue_group = NULL, $warn_on_override = TRUE) {
    if (!$issue_group) {
      $issue_group = t('DNM');
    }
    foreach ($fields as $field) {
      $this->addFieldMapping($field, NULL, $warn_on_override)
        ->issueGroup($issue_group);
    }
  }

  /**
   * Shortcut for adding several source fields which are to be explicitly
   * not migrated.
   *
   * @param array $fields
   *  List of fields to mark as not for migration.
   *
   * @param string $issue_group
   *  Issue group name to apply to the generated mappings (defaults to 'DNM').
   */
  public function addUnmigratedSources(array $fields, $issue_group = NULL, $warn_on_override = TRUE) {
    if (!$issue_group) {
      $issue_group = t('DNM');
    }
    foreach ($fields as $field) {
      $this->addFieldMapping(NULL, $field, $warn_on_override)
        ->issueGroup($issue_group);
    }
  }

  /**
   * Reports whether this migration process is complete (i.e., all available
   * source rows have been processed).
   */
  public function isComplete() {
    $total = $this->sourceCount(TRUE);
    // If the source is uncountable, we have no way of knowing if it's
    // complete, so stipulate that it is.
    if ($total < 0) {
      return TRUE;
    }
    $processed = $this->processedCount();
    return $total <= $processed;
  }

  /**
   * Override MigrationBase::beginProcess, to make sure the map/message tables
   * are present.
   *
   * @param int $newStatus
   *  Migration::STATUS_IMPORTING or Migration::STATUS_ROLLING_BACK
   */
  protected function beginProcess($newStatus) {
    parent::beginProcess($newStatus);

    // Do some standard setup
    if (isset($this->options['feedback']) && isset($this->options['feedback']['value']) &&
      isset($this->options['feedback']['unit'])) {
      $this->feedback = $this->options['feedback']['value'];
      $this->feedback_unit = $this->options['feedback']['unit'];
      if ($this->feedback_unit == 'item') {
        $this->feedback_unit = 'items';
      }
      elseif ($this->feedback_unit == 'second') {
        $this->feedback_unit = 'seconds';
      }
    }
    $this->lastfeedback = $this->starttime;

    $this->total_processed = $this->total_successes =
    $this->processed_since_feedback = $this->successes_since_feedback = 0;

    // Call pre-process methods
    if ($this->status == Migration::STATUS_IMPORTING) {
      $this->preImport();
    }
    elseif ($this->status == Migration::STATUS_ROLLING_BACK) {
      $this->preRollback();
    }
  }

  /**
   * Override MigrationBase::endProcess, to call post hooks. Note that it must
   * be public to be callable as the shutdown function.
   */
  public function endProcess() {
    // Call post-process methods
    if ($this->status == Migration::STATUS_IMPORTING) {
      $this->postImport();
    }
    elseif ($this->status == Migration::STATUS_ROLLING_BACK) {
      $this->postRollback();
    }

    parent::endProcess();
  }

  /**
   * Default implementations of pre/post import/rollback methods. These call
   * the destination methods (if they exist) - when overriding, always
   * call parent::preImport() etc.
   */
  protected function preImport() {
    if (method_exists($this->destination, 'preImport')) {
      $this->destination->preImport();
    }
  }

  protected function preRollback() {
    if (method_exists($this->destination, 'preRollback')) {
      $this->destination->preRollback();
    }
  }

  protected function postImport() {
    if (method_exists($this->destination, 'postImport')) {
      $this->destination->postImport();
    }
  }

  protected function postRollback() {
    if (method_exists($this->destination, 'postRollback')) {
      $this->destination->postRollback();
    }
  }

  /**
   * Perform a rollback operation - remove migrated items from the destination.
   */
  protected function rollback() {
    $return = MigrationBase::RESULT_COMPLETED;
    $itemlimit = $this->getItemLimit();
    $idlist = $this->getOption('idlist');
    if ($idlist) {
      // Make the IDs keys, to more easily identify them
      $idlist = array_flip(explode(',', $idlist));
    }

    if (method_exists($this->destination, 'bulkRollback')) {
      // Too many at once can lead to memory issues, so batch 'em up
      $destids = array();
      $sourceids = array();
      $batch_count = 0;
      foreach ($this->map as $destination_key) {
        if ($this->timeOptionExceeded()) {
          break;
        }
        if (($return = $this->checkStatus()) != MigrationBase::RESULT_COMPLETED) {
          break;
        }

        if ($itemlimit && ($this->total_processed + $batch_count >= $itemlimit)) {
          break;
        }

        $current_source_key = $this->map->getCurrentKey();

        // If there's an idlist, skip anything not in the list
        if ($idlist && !isset($idlist[$current_source_key['sourceid1']])) {
          continue;
        }

        // Note that bulk rollback is only supported for single-column keys
        $sourceids[] = $current_source_key;
        if (!empty($destination_key->destid1)) {
          $map_row = $this->map->getRowByDestination((array) $destination_key);
          if ($map_row['rollback_action'] == MigrateMap::ROLLBACK_DELETE) {
            $destids[] = $destination_key->destid1;
          }
        }

        $batch_count++;
        if ($batch_count >= $this->rollbackBatchSize) {
          try {
            if ($this->systemOfRecord == Migration::SOURCE) {
              if (!empty($destids)) {
                migrate_instrument_start('destination bulkRollback');
                $this->destination->bulkRollback($destids);
                migrate_instrument_stop('destination bulkRollback');
              }
            }
            // Keep track in case of interruption
            migrate_instrument_start('rollback map/message update');
            $this->map->deleteBulk($sourceids);
            migrate_instrument_stop('rollback map/message update');
            $this->total_successes += $batch_count;
            $this->successes_since_feedback += $batch_count;
          } catch (Exception $e) {
            $this->handleException($e, FALSE);
            migrate_instrument_stop('bulkRollback');
            migrate_instrument_stop('rollback map/message update');
          }
          $destids = array();
          $sourceids = array();
          // Will increment even if there was an exception... But we don't
          // really have a way to know how many really were successfully rolled back
          $this->total_processed += $batch_count;
          $this->processed_since_feedback += $batch_count;
          $batch_count = 0;
        }
      }
      if ($batch_count > 0) {
        if ($this->systemOfRecord == Migration::SOURCE) {
          if (!empty($destids)) {
            migrate_instrument_start('destination bulkRollback');
            $this->destination->bulkRollback($destids);
            migrate_instrument_stop('destination bulkRollback');
          }
          $this->total_processed += $batch_count;
          $this->total_successes += $batch_count;
          $this->processed_since_feedback += $batch_count;
          $this->successes_since_feedback += $batch_count;
        }
        migrate_instrument_start('rollback map/message update');
        $this->map->deleteBulk($sourceids);
        migrate_instrument_stop('rollback map/message update');
      }
    }
    else {
      foreach ($this->map as $destination_key) {
        if ($this->timeOptionExceeded()) {
          break;
        }
        if (($return = $this->checkStatus()) != MigrationBase::RESULT_COMPLETED) {
          break;
        }
        if ($this->itemOptionExceeded()) {
          break;
        }
        $current_source_key = $this->map->getCurrentKey();

        // If there's an idlist, skip anything not in the list
        if ($idlist && !isset($idlist[$current_source_key['sourceid1']])) {
          continue;
        }

        // Rollback one record
        try {
          if ($this->systemOfRecord == Migration::SOURCE) {
            // Skip when the destination key is null
            $skip = FALSE;
            foreach ($destination_key as $key_value) {
              if (is_null($key_value)) {
                $skip = TRUE;
                break;
              }
            }
            if (!$skip) {
              $map_row = $this->map->getRowByDestination((array) $destination_key);
              if ($map_row['rollback_action'] == MigrateMap::ROLLBACK_DELETE) {
                migrate_instrument_start('destination rollback');
                $this->destination->rollback((array) $destination_key);
                migrate_instrument_stop('destination rollback');
              }
            }
          }

          migrate_instrument_start('rollback map/message update');
          $this->map->delete($current_source_key);
          migrate_instrument_stop('rollback map/message update');
          $this->total_successes++;
          $this->successes_since_feedback++;
        } catch (Exception $e) {
          // TODO: At least count failures
          continue;
        }
        $this->total_processed++;
        $this->processed_since_feedback++;
      }
    }

    $this->map->clearMessages();

    $this->progressMessage($return);

    // If we're using highwater marks, reset at completion of a full rollback
    // TODO: What about partial rollbacks? Probably little we can do to make
    // that work cleanly...
    if ($this->highwaterField) {
      $this->saveHighwater('', TRUE);
    }
    return $return;
  }

  /**
   * Perform an import operation - migrate items from source to destination.
   */
  protected function import() {
    $return = MigrationBase::RESULT_COMPLETED;

    try {
      $this->source->rewind();
    } catch (Exception $e) {
      self::displayMessage(
        t('Migration for %class failed with source plugin exception: %e, in %file:%line',
          array(
            '%class' => get_class($this),
            '%e' => $e->getMessage(),
            '%file' => $e->getFile(),
            '%line' => $e->getLine(),
          )));
      return MigrationBase::RESULT_FAILED;
    }
    while ($this->source->valid()) {
      $data_row = $this->source->current();

      // Wipe old messages, and save any new messages.
      $this->map->delete($this->currentSourceKey(), TRUE);
      $this->saveQueuedMessages();

      $this->sourceValues = $data_row;
      $this->applyMappings();

      try {
        migrate_instrument_start('destination import', TRUE);
        $ids = $this->destination->import($this->destinationValues, $this->sourceValues);
        migrate_instrument_stop('destination import');
        if ($ids) {
          $this->map->saveIDMapping($this->sourceValues, $ids,
            $this->needsUpdate, $this->rollbackAction,
            $data_row->migrate_map_hash);
          $this->successes_since_feedback++;
          $this->total_successes++;
        }
        else {
          $this->map->saveIDMapping($this->sourceValues, array(),
            MigrateMap::STATUS_FAILED, $this->rollbackAction,
            NULL);
          if ($this->map->messageCount() == 0) {
            $message = t('New object was not saved, no error provided');
            $this->saveMessage($message);
            self::displayMessage($message);
          }
        }
      } catch (MigrateException $e) {
        $this->map->saveIDMapping($this->sourceValues, array(),
          $e->getStatus(), $this->rollbackAction, $data_row->migrate_map_hash);
        $this->saveMessage($e->getMessage(), $e->getLevel());
        self::displayMessage($e->getMessage());
      } catch (Exception $e) {
        $this->map->saveIDMapping($this->sourceValues, array(),
          MigrateMap::STATUS_FAILED, $this->rollbackAction,
          NULL);
        $this->handleException($e);
      }
      $this->total_processed++;
      $this->processed_since_feedback++;
      if ($this->highwaterField) {
        $this->saveHighwater($this->sourceValues->{$this->highwaterField['name']});
      }

      // Reset row properties.
      unset($this->sourceValues, $this->destinationValues);
      $this->needsUpdate = MigrateMap::STATUS_IMPORTED;

      // TODO: Temporary. Remove when http://drupal.org/node/375494 is committed.
      // TODO: Should be done in MigrateDestinationEntity
      if (!empty($this->destination->entityType)) {
        entity_get_controller($this->destination->entityType)->resetCache();
      }

      if ($this->timeOptionExceeded()) {
        break;
      }
      if (($return = $this->checkStatus()) != MigrationBase::RESULT_COMPLETED) {
        break;
      }
      if ($this->itemOptionExceeded()) {
        break;
      }
      try {
        $this->source->next();
      } catch (Exception $e) {
        self::displayMessage(
          t('Migration for %class failed with source plugin exception: %e, in %file:%line',
            array(
              '%class' => get_class($this),
              '%e' => $e->getMessage(),
              '%file' => $e->getFile(),
              '%line' => $e->getLine(),
            )));
        return MigrationBase::RESULT_FAILED;
      }
    }

    $this->progressMessage($return);

    return $return;
  }

  /**
   * Perform an analysis operation - report on field values in the source.
   *
   * @return array
   *  Array of analysis details - each element is keyed by field name and
   *  contains an array describing the field values.
   */
  public function analyze() {
    // The source needs this to find the map table.
    self::$currentMigration = $this;
    try {
      $this->source->rewind();
    } catch (Exception $e) {
      self::displayMessage(
        t('Migration analysis failed with source plugin exception: !e',
          array('!e' => $e->getMessage())));
      self::$currentMigration = NULL;
      return array();
    }
    // Get the documented fields first
    $source_fields = $this->source->fields();
    $analysis = array();
    $field_init = array(
      'is_numeric' => TRUE,
      'min_numeric' => NULL,
      'max_numeric' => NULL,
      'min_strlen' => 0,
      'max_strlen' => 0,
      'distinct_values' => array(),
    );
    foreach ($source_fields as $field_name => $description) {
      // Ignore fields from the map table
      if (substr($field_name, 0, strlen('migrate_map_')) == 'migrate_map_') {
        continue;
      }
      $analysis[$field_name] = $field_init +
        array('description' => $description);
    }
    // For each data row...
    while ($this->source->valid()) {
      $row = $this->source->current();
      // Cheat for XML migrations, which don't pick up the source values
      // until applyMappings() applies the xpath()
      if (is_a($this, 'XMLMigration') && isset($row->xml)) {
        $this->sourceValues = $row;
        $this->applyMappings();
        $row = $this->sourceValues;
        unset($row->xml);
      }
      // For each field in this row...
      foreach ($row as $field_name => $raw_value) {
        // Ignore fields from the map table
        if (substr($field_name, 0, strlen('migrate_map_')) == 'migrate_map_') {
          continue;
        }
        // It might be an array of values, check each value
        if (!is_array($raw_value)) {
          $raw_value = array($raw_value);
        }
        foreach ($raw_value as $field_value) {
          // If this is an undocumented field, initialize it
          if (!isset($analysis[$field_name])) {
            $analysis[$field_name] = $field_init +
              array('description' => '');
          }
          // Ignore leading/trailing spaces in determing numerics
          $trimmed_value = trim($field_value);
          if (is_numeric($trimmed_value)) {
            $trimmed_value = floatval($trimmed_value);
            // First numeric value, initialize the min/max
            if (is_null($analysis[$field_name]['min_numeric'])) {
              $analysis[$field_name]['min_numeric'] = $trimmed_value;
              $analysis[$field_name]['max_numeric'] = $trimmed_value;
            }
            else {
              $analysis[$field_name]['min_numeric'] = min($trimmed_value,
                $analysis[$field_name]['min_numeric']);
              $analysis[$field_name]['max_numeric'] = max($trimmed_value,
                $analysis[$field_name]['max_numeric']);
            }
          }
          elseif ($trimmed_value !== '') {
            // Empty strings are !is_numeric(), but treat as empty rather than
            // assuming we don't have a numeric field
            $analysis[$field_name]['is_numeric'] = FALSE;
          }
          $strlen = strlen($field_value);
          // First string value, initialize both min and max
          if ($analysis[$field_name]['max_strlen'] == 0) {
            $analysis[$field_name]['min_strlen'] = $strlen;
            $analysis[$field_name]['max_strlen'] = $strlen;
          }
          else {
            $analysis[$field_name]['min_strlen'] = min($strlen,
              $analysis[$field_name]['min_strlen']);
            $analysis[$field_name]['max_strlen'] = max($strlen,
              $analysis[$field_name]['max_strlen']);
          }
          // Track up to 10 distinct values
          if (count($analysis[$field_name]['distinct_values']) <= 10) {
            $analysis[$field_name]['distinct_values'][$trimmed_value]++;
          }
        }
      }

      try {
        $this->source->next();
      } catch (Exception $e) {
        self::displayMessage(
          t('Migration analysis failed with source plugin exception: !e. Partial results follow:',
            array('!e' => $e->getMessage())));
        self::$currentMigration = NULL;
        return $analysis;
      }
    }

    self::$currentMigration = NULL;
    return $analysis;
  }

  /**
   * Fetch the key array for the current source record.
   *
   * @return array
   */
  protected function currentSourceKey() {
    return $this->source->getCurrentKey();
  }

  /**
   * Default implementation of prepareKey. This method is called from the source
   * plugin immediately after retrieving the raw data from the source - by
   * default, it simply assigns the key values based on the field names passed
   * to MigrateSQLMap(). Override this if you need to generate your own key
   * (e.g., the source doesn't have a natural unique key). Be sure to also
   * set any values you generate in $row.
   *
   * @param array $source_key
   * @param object $row
   *
   * @return array
   */
  public function prepareKey($source_key, $row) {
    $key = array();
    foreach ($source_key as $field_name => $field_schema) {
      $key[$field_name] = $row->{$field_name};
    }
    return $key;
  }

  /**
   * Default implementation of prepareRow(). This method is called from the
   * source plugin upon first pulling the raw data from the source.
   *
   * @param $row
   *  Object containing raw source data.
   *
   * @return bool
   *  TRUE to process this row, FALSE to have the source skip it.
   */
  public function prepareRow($row) {
    $this->rollbackAction = $this->defaultRollbackAction;
    return TRUE;
  }

  ////////////////////////////////////////////////////////////////////
  // Utility methods

  /**
   * Convenience function to return count of total source records
   *
   * @param boolean $refresh
   *  Pass TRUE to refresh the cached count.
   */
  public function sourceCount($refresh = FALSE) {
    try {
      $count = $this->source->count($refresh);
    } catch (Exception $e) {
      $count = t('N/A');
      self::displayMessage($e->getMessage());
    }
    return $count;
  }

  /**
   * Get the number of source records processed.
   *
   * @return int
   *  Number of processed records.
   */
  public function processedCount() {
    try {
      $count = $this->map->processedCount();
    } catch (Exception $e) {
      $count = t('N/A');
      self::displayMessage($e->getMessage());
    }
    return $count;
  }

  /**
   * Get the number of records successfully imported.
   *
   * @return int
   *  Number of imported records.
   */
  public function importedCount() {
    try {
      $count = $this->map->importedCount();
    } catch (Exception $e) {
      $count = t('N/A');
      self::displayMessage($e->getMessage());
    }
    return $count;
  }

  /**
   * Get the number of records marked as needing update.
   *
   * @return int
   */
  public function updateCount() {
    try {
      $count = $this->map->updateCount();
    } catch (Exception $e) {
      $count = t('N/A');
      self::displayMessage($e->getMessage());
    }
    return $count;
  }

  /**
   * Test whether we've exceeded the designated item limit.
   *
   * @return boolean
   *  TRUE if the threshold is exceeded, FALSE if not.
   */
  protected function itemOptionExceeded() {
    $itemlimit = $this->getItemLimit();
    if ($itemlimit && $this->total_processed >= $itemlimit) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Get the number of source records which failed to import.
   * TODO: Doesn't yet account for informationals, or multiple errors for
   * a source record.
   *
   * @return int
   *  Number of records errored out.
   */
  public function errorCount() {
    return $this->map->errorCount();
  }

  /**
   * Get the number of messages associated with this migration
   *
   * @return int
   *  Number of messages.
   */
  public function messageCount() {
    return $this->map->messageCount();
  }

  /**
   * Prepares this migration to run as an update - that is, in addition to
   * unmigrated content (source records not in the map table) being imported,
   * previously-migrated content will also be updated in place.
   */
  public function prepareUpdate() {
    $this->map->prepareUpdate();
  }

  /**
   * Outputs a progress message, reflecting the current status of a migration
   * process.
   *
   * @param int $result
   *  Status of the process, represented by one of the Migration::RESULT_*
   *   constants.
   */
  protected function progressMessage($result) {
    $time = microtime(TRUE) - $this->lastfeedback;
    if ($time > 0) {
      $perminute = round(60 * $this->processed_since_feedback / $time);
      $time = round($time, 1);
    }
    else {
      $perminute = '?';
    }

    if ($this->status == Migration::STATUS_IMPORTING) {
      switch ($result) {
        case Migration::RESULT_COMPLETED:
          $basetext = "Processed !numitems (!created created, !updated updated, !failed failed, !ignored ignored) in !time sec (!perminute/min) - done with '!name'";
          $type = 'completed';
          break;
        case Migration::RESULT_FAILED:
          $basetext = "Processed !numitems (!created created, !updated updated, !failed failed, !ignored ignored) in !time sec (!perminute/min) - failure with '!name'";
          $type = 'failed';
          break;
        case Migration::RESULT_INCOMPLETE:
          $basetext = "Processed !numitems (!created created, !updated updated, !failed failed, !ignored ignored) in !time sec (!perminute/min) - continuing with '!name'";
          $type = 'status';
          break;
        case Migration::RESULT_STOPPED:
          $basetext = "Processed !numitems (!created created, !updated updated, !failed failed, !ignored ignored) in !time sec (!perminute/min) - stopped '!name'";
          $type = 'warning';
          break;
      }
    }
    else {
      switch ($result) {
        case Migration::RESULT_COMPLETED:
          $basetext = "Rolled back !numitems in !time sec (!perminute/min) - done with '!name'";
          $type = 'completed';
          break;
        case Migration::RESULT_FAILED:
          $basetext = "Rolled back !numitems in !time sec (!perminute/min) - failure with '!name'";
          $type = 'failed';
          break;
        case Migration::RESULT_INCOMPLETE:
          $basetext = "Rolled back !numitems in !time sec (!perminute/min) - continuing with '!name'";
          $type = 'status';
          break;
        case Migration::RESULT_STOPPED:
          $basetext = "Rolled back !numitems in !time sec (!perminute/min) - stopped '!name'";
          $type = 'warning';
          break;
      }
    }
    $numitems = $this->processed_since_feedback + $this->source->getIgnored();
    $message = t($basetext,
      array(
        '!numitems' => $numitems,
        '!successes' => $this->successes_since_feedback,
        '!failed' => $this->processed_since_feedback - $this->successes_since_feedback,
        '!created' => $this->destination->getCreated(),
        '!updated' => $this->destination->getUpdated(),
        '!ignored' => $this->source->getIgnored(),
        '!time' => $time,
        '!perminute' => $perminute,
        '!name' => $this->machineName,
      ));
    self::displayMessage($message, $type);

    // Report on lookup_cache hit rate. Only visible at 'debug' level.
    if ($result != Migration::RESULT_INCOMPLETE && !empty($this->counts['lookup_cache'])) {
      foreach ($this->counts['lookup_cache'] as $name => $tallies) {
        $tallies += array(
          'hit' => 0,
          'miss_hit' => 0,
          'miss_miss' => 0,
        ); // Set defaults to avoid NOTICE.
        $sum = $tallies['hit'] + $tallies['miss_hit'] + $tallies['miss_miss'];
        self::displayMessage(
          t('Lookup cache: !mn SM=!name !hit hit, !miss_hit miss_hit, !miss_miss miss_miss (!total total).', array(
            '!mn' => $this->machineName,
            '!name' => $name,
            '!hit' => round((100 * $tallies['hit']) / $sum) . '%',
            '!miss_hit' => round((100 * $tallies['miss_hit']) / $sum) . '%',
            '!miss_miss' => round((100 * $tallies['miss_miss']) / $sum) . '%',
            '!total' => $sum,
          )), 'debug');
      }
      $this->counts['lookup_cache'] = array();
    }
    if ($result == Migration::RESULT_INCOMPLETE) {
      $this->lastfeedback = time();
      $this->processed_since_feedback = $this->successes_since_feedback = 0;
      $this->source->resetStats();
      $this->destination->resetStats();
    }
  }

  /**
   * Standard top-of-loop stuff, common between rollback and import - check
   * for exceptional conditions, and display feedback.
   */
  protected function checkStatus() {
    if ($this->memoryExceeded()) {
      return MigrationBase::RESULT_INCOMPLETE;
    }
    if ($this->timeExceeded()) {
      return MigrationBase::RESULT_INCOMPLETE;
    }
    if ($this->getStatus() == Migration::STATUS_STOPPING) {
      return MigrationBase::RESULT_STOPPED;
    }
    // If feedback is requested, produce a progress message at the proper time
    if (isset($this->feedback)) {
      if (($this->feedback_unit == 'seconds' && time() - $this->lastfeedback >= $this->feedback) ||
        ($this->feedback_unit == 'items' && $this->processed_since_feedback >= $this->feedback)) {
        $this->progressMessage(MigrationBase::RESULT_INCOMPLETE);
      }
    }

    return MigrationBase::RESULT_COMPLETED;
  }

  /**
   * Apply field mappings to a data row received from the source, returning
   * a populated destination object.
   */
  protected function applyMappings() {
    $this->destinationValues = new stdClass;
    foreach ($this->getFieldMappings() as $mapping) {
      $destination = $mapping->getDestinationField();
      // Skip mappings with no destination (source fields marked DNM)
      if ($destination) {
        $source = $mapping->getSourceField();
        $default = $mapping->getDefaultValue();

        // When updating existing items, make sure we don't create a destination
        // field that is not mapped to anything (a source field or a default value)
        if (!$source && !isset($default)) {
          continue;
        }

        $destination_values = NULL;

        // If there's a source mapping, and a source value in the data row, copy
        // to the destination
        if ($source && isset($this->sourceValues->{$source})) {
          $destination_values = $this->sourceValues->{$source};
        }
        // Otherwise, apply the default value (if any)
        elseif (!is_null($default)) {
          $destination_values = $default;
        }

        // If there's a separator specified for this destination, then it
        // will be populated as an array exploded from the source value
        $separator = $mapping->getSeparator();
        if ($separator && isset($destination_values)) {
          if (is_array($separator)) {
            if (isset($separator['group separator'])) {
              $destination_values = explode($separator['group separator'], $destination_values);
            }
            else {
              $destination_values = array($destination_values);
            }
            foreach ($destination_values as $group => $value) {
              if (isset($separator['key separator'])) {
                $destination_values[$group] = explode($separator['key separator'], $value);
              }
              else {
                $destination_values[$group] = array($value);
              }
            }
          }
          else {
            $destination_values = explode($separator, $destination_values);
          }
        }

        // If a source migration is supplied, use the current value for this field
        // to look up a destination ID from the provided migration
        $source_migration = $mapping->getSourceMigration();
        if ($source_migration && isset($destination_values)) {
          $destination_values = $this->handleSourceMigration($source_migration, $destination_values, $default, $this);
        }

        // Call any designated callbacks
        $callbacks = $mapping->getCallbacks();
        foreach ($callbacks as $callback) {
          if (isset($destination_values)) {
            $destination_values = call_user_func_array($callback['callback'], array_merge(array($destination_values), $callback['params']));
          }
        }

        // If specified, assure a unique value for this property.
        $dedupe = $mapping->getDedupe();
        if ($dedupe && isset($destination_values)) {
          $destination_values = $this->handleDedupe($dedupe, $destination_values);
        }

        // Assign any arguments
        if (isset($destination_values)) {
          $arguments = $mapping->getArguments();
          if ($arguments) {
            if (!is_array($destination_values)) {
              $destination_values = array($destination_values);
            }
            // TODO: Stuffing arguments into the destination field is gross - can
            // we come up with a better way to communicate them to the field
            // handlers?
            $destination_values['arguments'] = array();
            foreach ($arguments as $argname => $destarg) {
              if (is_array($destarg) && isset($destarg['source_field']) && property_exists($this->sourceValues, $destarg['source_field'])) {
                $destination_values['arguments'][$argname] = $this->sourceValues->{$destarg['source_field']};
              }
              elseif (is_array($destarg) && isset($destarg['default_value'])) {
                $destination_values['arguments'][$argname] = $destarg['default_value'];
              }
              else {
                $destination_values['arguments'][$argname] = $destarg;
              }
            }
          }
        }

        // Are we dealing with the primary value of the destination field, or a
        // subfield?
        $destination = explode(':', $destination);
        // Count how many levels of fields are in the mapping. We'll use the
        // last one.
        $destination_count = count($destination);
        $destination_field = $destination[0];
        if ($destination_count == 2) {
          $subfield = $destination[1];
          // We're processing the subfield before the primary value, initialize it
          if (!property_exists($this->destinationValues, $destination_field)) {
            $this->destinationValues->{$destination_field} = array();
          }
          // We have a value, and need to convert to an array so we can add
          // arguments.
          elseif (!is_array($this->destinationValues->{$destination_field})) {
            $this->destinationValues->{$destination_field} = array($this->destinationValues->{$destination_field});
          }
          // Add the subfield value to the arguments array.
          $this->destinationValues->{$destination_field}['arguments'][$subfield] = $destination_values;
        }
        elseif ($destination_count == 3) {
          $subfield2 = $destination[2];
          // We're processing the subfield before the primary value, initialize it
          if (!property_exists($this->destinationValues, $destination_field)) {
            $this->destinationValues->{$destination_field} = array();
          }
          // We have a value, and need to convert to an array so we can add
          // arguments.
          elseif (!is_array($this->destinationValues->{$destination_field})) {
            $this->destinationValues->{$destination_field} = array($this->destinationValues->{$destination_field});
          }
          if (!is_array($this->destinationValues->{$destination_field}['arguments'][$destination[1]])) {
            // Convert first subfield level to an array so we can add to it.
            $this->destinationValues->{$destination_field}['arguments'][$destination[1]] = array($this->destinationValues->{$destination_field}['arguments'][$destination[1]]);
          }
          // Add the subfield value to the arguments array.
          $this->destinationValues->{$destination_field}['arguments'][$destination[1]]['arguments'][$subfield2] = $destination_values;
        }
        // Just the primary value, the first time through for this field, simply
        // set it.
        elseif (!property_exists($this->destinationValues, $destination_field)) {
          $this->destinationValues->{$destination_field} = $destination_values;
        }
        // We've seen a subfield, so add as an array value.
        else {
          $this->destinationValues->{$destination_field} = array_merge(
            (array) $destination_values, $this->destinationValues->{$destination_field});
        }
      }
    }
  }

  /**
   * Look up a value migrated in another migration.
   *
   * @param mixed $source_migrations
   *   An array of source migrations, or string for a single migration.
   * @param mixed $source_keys
   *   Key(s) to be looked up against the source migration(s). This may be a
   *   simple value (one single-field key), an array of values (multiple
   *   single-field keys to each be looked up), or an array of arrays (multiple
   *   multi-field keys to each be looked up).
   * @param mixed $default
   *   The default value, if no ID was found.
   * @param $migration
   *   The implementing migration.
   *
   * @return
   *   Destination value(s) from the source migration(s), as a single value if
   *   a single key was passed in, or an array of values if there were multiple
   *   keys to look up.
   */
  protected function handleSourceMigration($source_migrations, $source_keys, $default = NULL, $migration = NULL) {
    // Handle the source migration(s) as an array.
    $source_migrations = (array) $source_migrations;

    // We want to treat source keys consistently as an array of arrays (each
    // representing one key).
    if (is_array($source_keys)) {
      if (empty($source_keys)) {
        // Empty value should return empty results.
        return NULL;
      }
      elseif (is_array(reset($source_keys))) {
        // Already an array of key arrays, fall through
      }
      else {
        // An array of single-key values - make each one an array
        $new_source_keys = array();
        foreach ($source_keys as $source_key) {
          $new_source_keys[] = array($source_key);
        }
        $source_keys = $new_source_keys;
      }
    }
    else {
      // A simple value - make it an array within an array
      $source_keys = array(array($source_keys));
    }

    // Instantiate each migration, and store back in the array.
    foreach ($source_migrations as $key => $source_migration) {
      $source_migrations[$key] = Migration::getInstance($source_migration);
      if (!isset($source_migrations[$key])) {
        MigrationBase::displayMessage(t('The @source cannot be resolved to a migration instance.',
          array('@source' => $source_migration)));
        unset($source_migrations[$key]);
      }
    }

    $results = array();
    // Each $source_key will be an array of key values
    foreach ($source_keys as $source_key) {
      // If any source keys are NULL, skip this set
      $continue = FALSE;
      foreach ($source_key as $value) {
        if (!isset($value)) {
          $continue = TRUE;
          break;
        }
      }
      // Occasionally $source_key comes through with an empty string.
      $sanity_check = array_filter($source_key);
      if ($continue || empty($source_key) || empty($sanity_check)) {
        continue;
      }
      // Loop through each source migration, checking for an existing dest ID.
      foreach ($source_migrations as $source_migration) {
        // Break out of the loop as soon as a destination ID is found.
        if ($destids = $source_migration->getMap()
          ->lookupDestinationID($source_key)) {
          if (!empty($destids['destid1'])) {
            break;
          }
        }
      }
      // If no destination ID was found, give each source migration a chance to
      // create a stub.
      if (!$destids) {
        foreach ($source_migrations as $source_migration) {
          // Is this a self reference?
          if ($source_migration->machineName == $this->machineName) {
            if (!array_diff($source_key, $this->currentSourceKey())) {
              $destids = array();
              $this->needsUpdate = MigrateMap::STATUS_NEEDS_UPDATE;
              break;
            }
          }
          // Break out of the loop if a stub was successfully created.
          if ($destids = $source_migration->createStubWrapper($source_key, $migration)) {
            break;
          }
        }
      }
      if ($destids) {
        // Assume that if the destination key is a single value, it
        // should be passed as such
        if (count($destids) == 1) {
          $results[] = reset($destids);
        }
        else {
          $results[] = $destids;
        }
      }
      // If no match found, apply the default value (if any)
      elseif (!is_null($default)) {
        $results[] = $default;
      }
    }
    // Return a single result if we had a single key
    if (count($source_keys) > 1) {
      return $results;
    }
    else {
      $value = reset($results);
      return empty($value) && $value !== 0 && $value !== '0' ? NULL : $value;
    }
  }

  /**
   * For fields which require uniqueness, assign a new unique value if
   * necessary.
   *
   * @param array $dedupe
   *  An array with two keys, 'table' the name of the Drupal table and 'column'
   *  the column within that table where uniqueness must be maintained.
   * @param $original
   *  The value coming in, which must be checked for uniqueness.
   *
   * @return string
   *  The value to use - either the original, or a variation created by
   *   appending a sequence number.
   */
  protected function handleDedupe($dedupe, $original) {
    // If we're remigrating a previously-existing value, simply running through
    // our normal process will re-dedupe it - we must be sure to preserve the
    // previously-written value. Note that this means that you cannot migrate
    // changes to this field - the originally-migrated value will always
    // remain, because we can't tell what the original was.
    if (isset($this->sourceValues->migrate_map_destid1)) {
      $key_field = key($this->destination->getKeySchema());
      $existing_value = db_select($dedupe['table'], 't')
        ->fields('t', array($dedupe['column']))
        ->range(0, 1)
        ->condition($key_field, $this->sourceValues->migrate_map_destid1)
        ->execute()
        ->fetchField();
      // Note that if, for some reason, we don't find a value, fall through
      // to the normal deduping process
      if ($existing_value) {
        return $existing_value;
      }
    }
    $i = 1;
    $candidate = $original;
    while (db_select($dedupe['table'], 't')
      ->fields('t', array($dedupe['column']))
      ->range(0, 1)
      ->condition('t.' . $dedupe['column'], $candidate)
      ->execute()
      ->rowCount() > 0) {
      // We already have the candidate value. Find a non-existing value.
      $i++;
      // @TODO: support custom replacement pattern instead of just append.
      $candidate = $original . '_' . $i;
    }
    if ($i > 1) {
      $message = t('Replacing !column !original with !candidate',
        array(
          '!column' => $dedupe['column'],
          '!original' => $original,
          '!candidate' => $candidate,
        ));
      $migration = Migration::currentMigration();
      $migration->saveMessage($message, Migration::MESSAGE_INFORMATIONAL);
    }
    return $candidate;
  }

  /**
   * If stub creation is enabled, try to create a stub and save the mapping.
   */
  protected function createStubWrapper(array $source_key, $migration = NULL) {
    if (method_exists($this, 'createStub')) {
      $destids = $this->createStub($migration, $source_key);
      if ($destids) {
        // Fake a data row with the source key in it
        $map_source_key = $this->map->getSourceKey();
        $data_row = new stdClass;
        $i = 0;
        foreach ($map_source_key as $key => $definition) {
          $data_row->{$key} = $source_key[$i++];
        }
        $this->map->saveIDMapping($data_row, $destids,
          MigrateMap::STATUS_NEEDS_UPDATE, $this->defaultRollbackAction);
      }
    }
    else {
      $destids = NULL;
    }
    return $destids;
  }

  /**
   * Pass messages through to the map class.
   *
   * @param string $message
   *  The message to record.
   * @param int $level
   *  Optional message severity (defaults to MESSAGE_ERROR).
   */
  public function saveMessage($message, $level = MigrationBase::MESSAGE_ERROR) {
    $this->map->saveMessage($this->currentSourceKey(), $message, $level);
  }

  /**
   * Queue messages to be later saved through the map class.
   *
   * @param string $message
   *  The message to record.
   * @param int $level
   *  Optional message severity (defaults to MESSAGE_ERROR).
   */
  public function queueMessage($message, $level = MigrationBase::MESSAGE_ERROR) {
    $this->queuedMessages[] = array('message' => $message, 'level' => $level);
  }

  /**
   * Save any messages we've queued up to the message table.
   */
  public function saveQueuedMessages() {
    foreach ($this->queuedMessages as $queued_message) {
      $this->saveMessage($queued_message['message'], $queued_message['level']);
    }
    $this->queuedMessages = array();
  }

  /**
   * Set the specified row to be updated, if it exists.
   */
  public function setUpdate(array $source_key = NULL) {
    if (!$source_key) {
      $source_key = $this->currentSourceKey();
    }
    $this->map->setUpdate($source_key);
  }
}

/**
 * @deprecated - This class is no longer necessary, inherit directly from
 *  Migration instead.
 */
abstract class DynamicMigration extends Migration {

  static $deprecationWarning = FALSE;

  public function __construct($arguments) {
    parent::__construct($arguments);
    if (variable_get('migrate_deprecation_warnings', 1) &&
      !self::$deprecationWarning) {
      self::displayMessage(t('The DynamicMigration class is no longer necessary and is now deprecated - please derive your migration classes directly from Migration.'));
      self::$deprecationWarning = TRUE;
    }
  }

  /**
   * Overrides default of FALSE
   */
  static public function isDynamic() {
    return TRUE;
  }
}
