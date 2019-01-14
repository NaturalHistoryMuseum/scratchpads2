<?php

/**
 * @file
 * Defines base for migration destinations.
 */

/**
 * Abstract base class for destination handling.
 *
 * Derived classes are expected to define __toString(), returning a string
 * describing the type of destination and significant options. See
 * MigrateDestinationEntity for an example.
 */
abstract class MigrateDestination {

  /**
   * To support MigrateSQLMap maps, derived destination classes should return
   * schema field definition(s) corresponding to the primary key of the
   * destination being implemented. These are used to construct the destination
   * key fields of the map table for a migration using this destination.
   *
   * abstract static public function getKeySchema()
   */

  /**
   * Derived classes must implement __toString().
   *
   * @return string
   *  Description of the destination being migrated into
   */
  abstract public function __toString();

  /**
   * Derived classes must implement fields(), returning a list of available
   * destination fields.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  abstract public function fields();

  /**
   * Derived classes must implement either bulkRollback or rollback() according
   * to the signatures below, to rollback (usually by deletion)
   * previously-migrated items.
   *
   * $ids is an array of single-field keys to be deleted
   * abstract public function bulkRollback(array $ids);
   *
   * $key is an array of fields keying a single entry to delete
   * abstract public function rollback(array $key);
   */

  /**
   * Derived classes must implement import(), to construct one new object
   * (pre-pppulated using field mappings in the Migration). It is expected to
   * call prepare and complete handlers, passing them $row (the raw data from
   * the source).
   */
  abstract public function import(stdClass $object, stdClass $row);

  /**
   * Derived classes may implement preImport() and/or postImport(), to do any
   * processing they need done before or after looping over all source rows.
   * Similarly, preRollback() or postRollback() may be implemented.
   *
   * abstract public function preImport();
   * abstract public function postImport();
   * abstract public function preRollback();
   * abstract public function postRollback();
   */

  /**
   * Maintain stats on the number of destination objects created or updated.
   *
   * @var int
   */
  protected $numCreated = 0;

  public function getCreated() {
    return $this->numCreated;
  }

  protected $numUpdated = 0;

  public function getUpdated() {
    return $this->numUpdated;
  }

  /**
   * Reset numCreated and numUpdated back to 0.
   */
  public function resetStats() {
    $this->numCreated = 0;
    $this->numUpdated = 0;
  }

  /**
   * Null constructor
   */
  public function __construct() {
  }
}

/**
 * All destination handlers should be derived from MigrateDestinationHandler
 */
abstract class MigrateDestinationHandler extends MigrateHandler {

  // Any one or more of these methods may be implemented

  /**
   * Documentation of any fields added to the destination by this handler.
   *
   * @param $entity_type
   *  The entity type (node, user, etc.) for which to list fields.
   * @param $bundle
   *  The bundle (article, blog, etc.), if any, for which to list fields.
   * @param Migration $migration
   *  Optionally, the migration providing the context.
   *
   * @return array
   *  An array keyed by field name, with field descriptions as values.
   */
  //abstract public function fields($entity_type, $bundle, $migration = NULL);

  //abstract public function prepare($entity, stdClass $row);
  //abstract public function complete($entity, stdClass $row);
}
