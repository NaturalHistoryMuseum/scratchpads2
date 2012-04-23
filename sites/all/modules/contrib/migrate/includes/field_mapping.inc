<?php

/**
 * @file
 * The MigrateFieldMapping class - tracking mappings between source and
 * destination.
 */

class MigrateFieldMapping {
  /**
   * Destination field name for the mapping. If empty, the mapping is just a
   * stub for annotating the source field.
   *
   * @var string
   */
  protected $destinationField;
  public function getDestinationField() {
    return $this->destinationField;
  }

  /**
   * Source field name for the mapping. If empty, the defaultValue will be
   * applied.
   *
   * @var string
   */
  protected $sourceField;
  public function getSourceField() {
    return $this->sourceField;
  }

  /**
   * Default value for simple mappings, when there is no source mapping or the
   * source field is empty. If both this and the sourceField are omitted, the
   * mapping is just a stub for annotating the destination field.
   *
   * @var mixed
   */
  protected $defaultValue;
  public function getDefaultValue() {
    return $this->defaultValue;
  }

  /**
   * Separator string. If present, the destination field will be set up as an
   * array of values exploded from the corresponding source field.
   *
   * @var string
   */
  protected $separator;
  public function getSeparator() {
    return $this->separator;
  }

  /**
   * Class name of source migration for a field. If present, the value in the
   * source field is considered to be a source ID in the mapping table of this
   * migration, and the corresponding destination ID will be retrieved.
   *
   * @var mixed
   *   An array of source migrations, or string for a single migration.
   */
  protected $sourceMigration;
  public function getSourceMigration() {
    return $this->sourceMigration;
  }

  /**
   * Array of callbacks to be called on a source value.
   *
   * @var string
   */
  protected $callbacks = array();
  public function getCallbacks() {
    return $this->callbacks;
  }

  /**
   * An associative array with keys:
   *   - table: The table for querying for a duplicate.
   *   - column: The column for querying for a duplicate.
   *
   * @todo: Let fields declare this data and a replacement pattern. Then
   * developers won't have to specify this.
   *
   * @var string
   */
  protected $dedupe;
  public function getDedupe() {
    return $this->dedupe;
  }

  /**
   * Argument overrides. If present this will be an array, keyed by
   * a field API array key, with one or both of these entries:
   * 'source_field' - Name of the source field in the incoming row containing the
   *  value to be assigned
   * 'default_value' - A constant value to be assigned in the absence of source_field
   *
   * @var array
   */
  protected $arguments;
  public function getArguments() {
    return $this->arguments;
  }

  protected $description = '';
  public function getDescription() {
    return $this->description;
  }

  protected $issueGroup;
  public function getIssueGroup() {
    return $this->issueGroup;
  }

  protected $issueNumber;
  public function getIssueNumber() {
    return $this->issueNumber;
  }

  protected $issuePriority = self::ISSUE_PRIORITY_OK;
  public function getIssuePriority() {
    return $this->issuePriority;
  }

  const ISSUE_PRIORITY_OK = 1;
  const ISSUE_PRIORITY_LOW = 2;
  const ISSUE_PRIORITY_MEDIUM = 3;
  const ISSUE_PRIORITY_BLOCKER = 4;

  public static $priorities = array();

  public function __construct($destination_field, $source_field) {
    // Must have one or the other
    if (!$destination_field && !$source_field) {
      throw new Exception('Field mappings must have a destination field or a source field');
    }
    $this->destinationField = $destination_field;
    $this->sourceField = $source_field;
    $this->issueGroup = t('Done');
    if (count(self::$priorities) == 0) {
      self::$priorities[self::ISSUE_PRIORITY_OK] = t('OK');
      self::$priorities[self::ISSUE_PRIORITY_LOW] = t('Low');
      self::$priorities[self::ISSUE_PRIORITY_MEDIUM] = t('Medium');
      self::$priorities[self::ISSUE_PRIORITY_BLOCKER] = t('Blocker');
    }
  }

  public function defaultValue($default_value) {
    $this->defaultValue = $default_value;
    return $this;
  }

  public function separator($separator) {
    $this->separator = $separator;
    return $this;
  }

  public function sourceMigration($source_migration) {
    $this->sourceMigration = $source_migration;
    return $this;
  }

  public function callbacks($callbacks) {
    $this->callbacks = func_get_args();
    return $this;
  }

  public function dedupe($table, $column) {
    $this->dedupe = array('table' => $table, 'column' => $column);
    return $this;
  }

  public function arguments($arguments) {
    $this->arguments = $arguments;
    return $this;
  }

  public function description($text) {
    $this->description = $text;
    return $this;
  }

  public function issueGroup($group) {
    if (!$group) {
      $group = t('Done');
    }
    $this->issueGroup = $group;
    return $this;
  }

  public function issueNumber($number) {
    $this->issueNumber = $number;
    return $this;
  }

  public function issuePriority($priority) {
    $this->issuePriority = $priority;
    return $this;
  }
}
