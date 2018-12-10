<?php

/**
 * @file
 * Define base for migration sources.
 */

/**
 * Abstract base class for source handling.
 *
 * Derived classes are expected to define __toString(), returning a string
 * describing the source and significant options. See
 * MigrateSourceSQL for an example.
 */
abstract class MigrateSource implements Iterator {

  /**
   * The current row from the quey
   *
   * @var stdClass
   */
  protected $currentRow;

  /**
   * The primary key of the current row
   *
   * @var array
   */
  protected $currentKey;

  public function getCurrentKey() {
    return $this->currentKey;
  }

  /**
   * The Migration class currently invoking us, during rewind() and next().
   *
   * @var Migration
   */
  protected $activeMigration;

  /**
   * The MigrateMap class for the current migration.
   *
   * @var MigrateMap
   */
  protected $activeMap;

  /**
   * Number of rows intentionally ignored (prepareRow() returned FALSE)
   *
   * @var int
   */
  protected $numIgnored = 0;

  public function getIgnored() {
    return $this->numIgnored;
  }

  /**
   * Number of rows we've at least looked at.
   *
   * @var int
   */
  protected $numProcessed = 0;

  public function getProcessed() {
    return $this->numProcessed;
  }

  /**
   * Reset numIgnored back to 0.
   */
  public function resetStats() {
    $this->numIgnored = 0;
  }

  /**
   * Information on the highwater mark for the current migration, if any.
   *
   * @var array
   */
  protected $highwaterField;

  /**
   * The highwater mark at the beginning of the import operation.
   *
   * @var
   */
  protected $originalHighwater = '';

  /**
   * Used in the case of multiple key sources that need to use idlist.
   *
   * @var string
   */
  protected $multikeySeparator = ':';

  /**
   * List of source IDs to process.
   *
   * @var array
   */
  protected $idList = array();

  /**
   * Derived classes must implement fields(), returning a list of available
   * source fields.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  abstract public function fields();

  /**
   * Whether this instance should cache the source count.
   *
   * @var boolean
   */
  protected $cacheCounts = FALSE;

  /**
   * Key to use for caching counts.
   *
   * @var string
   */
  protected $cacheKey;

  /**
   * Whether this instance should not attempt to count the source.
   *
   * @var boolean
   */
  protected $skipCount = FALSE;

  /**
   * If TRUE, we will maintain hashed source rows to determine whether incoming
   * data has changed.
   *
   * @var bool
   */
  protected $trackChanges = FALSE;

  /**
   * By default, next() will directly read the map row and add it to the data
   * row. A source plugin implementation may do this itself (in particular, the
   * SQL source can incorporate the map table into the query) - if so, it should
   * set this TRUE so we don't duplicate the effort.
   *
   * @var bool
   */
  protected $mapRowAdded = FALSE;

  /**
   * Return a count of available source records, from the cache if appropriate.
   * Returns -1 if the source is not countable.
   *
   * @param boolean $refresh
   */
  public function count($refresh = FALSE) {
    if ($this->skipCount) {
      return -1;
    }

    if (!isset($this->cacheKey)) {
      $this->cacheKey = md5((string) $this);
    }

    // If a refresh is requested, or we're not caching counts, ask the derived
    // class to get the count from the source.
    if ($refresh || !$this->cacheCounts) {
      $count = $this->computeCount();
      cache_set($this->cacheKey, $count, 'cache');
    }
    else {
      // Caching is in play, first try to retrieve a cached count.
      $cache_object = cache_get($this->cacheKey, 'cache');
      if (is_object($cache_object)) {
        // Success
        $count = $cache_object->data;
      }
      else {
        // No cached count, ask the derived class to count 'em up, and cache
        // the result
        $count = $this->computeCount();
        cache_set($this->cacheKey, $count, 'cache');
      }
    }
    return $count;
  }

  /**
   * Derived classes must implement computeCount(), to retrieve a fresh count of
   * source records.
   */
  //abstract public function computeCount();

  /**
   * Class constructor.
   *
   * @param array $options
   *  Optional array of options.
   */
  public function __construct($options = array()) {
    if (!empty($options['cache_counts'])) {
      $this->cacheCounts = TRUE;
    }
    if (!empty($options['skip_count'])) {
      $this->skipCount = TRUE;
    }
    if (!empty($options['cache_key'])) {
      $this->cacheKey = $options['cache_key'];
    }
    if (!empty($options['track_changes'])) {
      $this->trackChanges = $options['track_changes'];
    }
  }

  /**
   * Default implementations of Iterator methods - many derivations will find
   * these adequate and will only need to implement rewind() and next()
   */

  /**
   * Implementation of Iterator::current() - called when entering a loop
   * iteration, returning the current row
   */
  public function current() {
    return $this->currentRow;
  }

  /**
   * Implementation of Iterator::key - called when entering a loop iteration,
   * returning the key of the current row. It must be a scalar - we will
   * serialize to fulfill the requirement, but using getCurrentKey() is
   * preferable.
   */
  public function key() {
    return serialize($this->currentKey);
  }

  /**
   * Implementation of Iterator::valid() - called at the top of the loop,
   * returning TRUE to process the loop and FALSE to terminate it
   */
  public function valid() {
    return !is_null($this->currentRow);
  }

  /**
   * Implementation of Iterator::rewind() - subclasses of MigrateSource should
   * implement performRewind() to do any class-specific setup for iterating
   * source records.
   */
  public function rewind() {
    $this->activeMigration = Migration::currentMigration();
    $this->activeMap = $this->activeMigration->getMap();
    $this->numProcessed = 0;
    $this->numIgnored = 0;
    $this->highwaterField = $this->activeMigration->getHighwaterField();
    if (!empty($this->highwaterField)) {
      $this->originalHighwater = $this->activeMigration->getHighwater();
    }
    if ($this->activeMigration->getOption('idlist')) {
      $this->idList = explode(',', $this->activeMigration->getOption('idlist'));
    }
    else {
      $this->idList = array();
    }
    migrate_instrument_start(get_class($this) . ' performRewind');
    $this->performRewind();
    migrate_instrument_stop(get_class($this) . ' performRewind');
    $this->next();
  }

  /**
   * Implementation of Iterator::next() - subclasses of MigrateSource should
   * implement getNextRow() to retrieve the next valid source rocord to process.
   */
  public function next() {
    $this->currentKey = NULL;
    $this->currentRow = NULL;

    migrate_instrument_start(get_class($this) . ' getNextRow');
    while ($row = $this->getNextRow()) {
      migrate_instrument_stop(get_class($this) . ' getNextRow');

      // Populate the source key for this row
      $this->currentKey = $this->activeMigration->prepareKey(
        $this->activeMap->getSourceKey(), $row);

      // Pick up the existing map row, if any, unless getNextRow() did it.
      if (!$this->mapRowAdded) {
        $map_row = $this->activeMap->getRowBySource($this->currentKey);
        // Add map info to the row, if present
        if ($map_row) {
          foreach ($map_row as $field => $value) {
            $field = 'migrate_map_' . $field;
            $row->{$field} = $value;
          }
        }
      }

      // First, determine if this row should be passed to prepareRow(), or
      // skipped entirely. The rules are:
      // 1. If there's an explicit idlist, that's all we care about (ignore
      //    highwaters and map rows).
      $prepared = FALSE;
      if (!empty($this->idList)) {
        // Check first source key.
        if (!in_array(reset($this->currentKey), $this->idList)) {
          // If this is a compound source key, check the full key.
          $compoundKey = implode($this->multikeySeparator, $this->currentKey);
          if (count($this->currentKey) == 1 || !in_array($compoundKey, $this->idList)) {
            // Could not find the key, skip.
            continue;
          }
        }
      }
      // 2. If the row is not in the map (we have never tried to import it
      //    before), we always want to try it.
      elseif (!isset($row->migrate_map_sourceid1)) {
        // Fall through
      }
      // 3. If the row is marked as needing update, pass it.
      elseif ($row->migrate_map_needs_update == MigrateMap::STATUS_NEEDS_UPDATE) {
        // Fall through
      }
      // 4. At this point, we have a row which has previously been imported and
      //    not marked for update. If we're not using highwater marks, then we
      //    will not take this row. Except, if we're looking for changes in the
      //    data, we need to go through prepareRow() before we can decide to
      //    skip it.
      elseif (empty($this->highwaterField)) {
        if ($this->trackChanges) {
          if ($this->prepareRow($row) !== FALSE) {
            if ($this->dataChanged($row)) {
              // This is a keeper
              $this->currentRow = $row;
              break;
            }
            else {
              // No change, skip it.
              continue;
            }
          }
          else {
            // prepareRow() told us to skip it.
            continue;
          }
        }
        else {
          // No highwater and not tracking changes, skip.
          continue;
        }
      }
      // 5. The initial highwater mark, before anything is migrated, is ''. We
      //    want to make sure we don't mistakenly skip rows with a highwater
      //    field value of 0, so explicitly handle '' here.
      elseif ($this->originalHighwater === '') {
        // Fall through
      }
      // 6. So, we are using highwater marks. Take the row if its highwater
      //    field value is greater than the saved mark, otherwise skip it.
      else {
        // Call prepareRow() here, in case the highwaterField needs preparation
        if ($this->prepareRow($row) !== FALSE) {
          if ($row->{$this->highwaterField['name']} > $this->originalHighwater) {
            $this->currentRow = $row;
            break;
          }
          else {
            // Skip
            continue;
          }
        }
        $prepared = TRUE;
      }

      // Allow the Migration to prepare this row. prepareRow() can return boolean
      // FALSE to ignore this row.
      if (!$prepared) {
        if ($this->prepareRow($row) !== FALSE) {
          // Finally, we've got a keeper.
          $this->currentRow = $row;
          break;
        }
        else {
          $this->currentRow = NULL;
        }
      }
    }
    migrate_instrument_stop(get_class($this) . ' getNextRow');
    if (!$this->currentRow) {
      $this->currentKey = NULL;
    }
  }

  /**
   * Give the calling migration a shot at manipulating, and possibly rejecting,
   * the source row.
   *
   * @return bool
   *  FALSE if the row is to be skipped.
   */
  protected function prepareRow($row) {
    migrate_instrument_start(get_class($this->activeMigration) . ' prepareRow');
    $return = $this->activeMigration->prepareRow($row);
    migrate_instrument_stop(get_class($this->activeMigration) . ' prepareRow');
    // We're explicitly skipping this row - keep track in the map table
    if ($return === FALSE) {
      // Make sure we replace any previous messages for this item with any
      // new ones.
      $this->activeMigration->getMap()->delete($this->currentKey, TRUE);
      $this->activeMigration->saveQueuedMessages();
      $this->activeMigration->getMap()->saveIDMapping($row, array(),
        MigrateMap::STATUS_IGNORED, $this->activeMigration->rollbackAction);
      $this->numIgnored++;
      $this->currentRow = NULL;
      $this->currentKey = NULL;
    }
    else {
      $return = TRUE;
      // When tracking changed data, We want to quietly skip (rather than
      // "ignore") rows with changes. The caller needs to make that decision,
      // so we need to provide them with the necessary information (before and
      // after hashes).
      if ($this->trackChanges) {
        $unhashed_row = clone $row;
        // Remove all map data, otherwise we'll have a false positive on the
        // second import (attempt) on a row.
        foreach ($unhashed_row as $field => $data) {
          if (strpos($field, 'migrate_map_') === 0) {
            unset($unhashed_row->{$field});
          }
        }
        $row->migrate_map_original_hash = isset($row->migrate_map_hash) ?
          $row->migrate_map_hash : '';
        $row->migrate_map_hash = $this->hash($unhashed_row);
      }
      else {
        $row->migrate_map_hash = '';
      }
    }

    $this->numProcessed++;
    return $return;
  }

  /**
   * Determine whether this row has changed, and therefore whether it should
   * be processed.
   *
   * @param $row
   *
   * @return bool
   */
  protected function dataChanged($row) {
    if ($row->migrate_map_original_hash != $row->migrate_map_hash) {
      $return = TRUE;
    }
    else {
      $return = FALSE;
    }

    return $return;
  }

  /**
   * Generate a hash of the source row.
   *
   * @param $row
   *
   * @return string
   */
  protected function hash($row) {
    migrate_instrument_start('MigrateSource::hash');
    $hash = md5(serialize($row));
    migrate_instrument_stop('MigrateSource::hash');
    return $hash;
  }
}
