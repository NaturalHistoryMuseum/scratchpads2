<?php

/**
 * @file
 * Defines the base class for migration processes.
 */

/**
 * The base class for all objects representing distinct steps in a migration
 * process. Most commonly these will be Migration objects which actually import
 * data from a source into a Drupal destination, but by deriving classes
 * directly from MigrationBase one can have other sorts of tasks (e.g.,
 * enabling/disabling of modules) occur during the migration process.
 */
abstract class MigrationBase {

  /**
   * Track the migration currently running, so handlers can easily determine it
   * without having to pass a Migration object everywhere.
   *
   * @var Migration
   */
  protected static $currentMigration;

  public static function currentMigration() {
    return self::$currentMigration;
  }

  /**
   * The machine name of this Migration object, derived by removing the
   * 'Migration' suffix from the class name. Used to construct default
   * map/message table names, displayed in drush migrate-status, key to
   * migrate_status table...
   *
   * @var string
   */
  protected $machineName;

  public function getMachineName() {
    return $this->machineName;
  }

  /**
   * A migration group object, used to collect related migrations.
   *
   * @var MigrateGroup
   */
  protected $group;

  public function getGroup() {
    return $this->group;
  }

  /**
   * Detailed information describing the migration.
   *
   * @var string
   */
  protected $description;

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Save options passed to current operation
   *
   * @var array
   */
  protected $options;

  public function getOption($option_name) {
    if (isset($this->options[$option_name])) {
      return $this->options[$option_name];
    }
    else {
      return NULL;
    }
  }

  public function getItemLimit() {
    if (isset($this->options['limit']) &&
      ($this->options['limit']['unit'] == 'items' || $this->options['limit']['unit'] == 'item')) {
      return $this->options['limit']['value'];
    }
    else {
      return NULL;
    }
  }

  public function getTimeLimit() {
    if (isset($this->options['limit']) &&
      ($this->options['limit']['unit'] == 'seconds' || $this->options['limit']['unit'] == 'second')) {
      return $this->options['limit']['value'];
    }
    else {
      return NULL;
    }
  }

  /**
   * Indicates that we are processing a rollback or import - used to avoid
   * excess writes in endProcess()
   *
   * @var boolean
   */
  protected $processing = FALSE;

  /**
   * Are we importing, rolling back, or doing nothing?
   *
   * @var enum
   */
  protected $status = MigrationBase::STATUS_IDLE;

  /**
   * When the current operation started.
   *
   * @var int
   */
  protected $starttime;

  /**
   * Whether to maintain a history of migration processes in migrate_log
   *
   * @var boolean
   */
  protected $logHistory = TRUE;

  /**
   * Primary key of the current history record (inserted at the beginning of
   * a process, to be updated at the end)
   *
   * @var int
   */
  protected $logID;

  /**
   * Number of "items" processed in the current migration process (whatever that
   * means for the type of process)
   *
   * @var int
   */
  protected $total_processed = 0;

  /**
   * List of other Migration classes which should be imported before this one.
   * E.g., a comment migration class would typically have node and user
   * migrations as dependencies.
   *
   * @var array
   */
  protected $dependencies = array(), $softDependencies = array();

  public function getHardDependencies() {
    return $this->dependencies;
  }

  public function setHardDependencies(array $dependencies) {
    $this->dependencies = $dependencies;
  }

  public function addHardDependencies(array $dependencies) {
    $this->dependencies = array_merge($this->dependencies, $dependencies);
  }

  public function getSoftDependencies() {
    return $this->softDependencies;
  }

  public function setSoftDependencies(array $dependencies) {
    $this->softDependencies = $dependencies;
  }

  public function addSoftDependencies(array $dependencies) {
    $this->softDependencies = array_merge($this->softDependencies, $dependencies);
  }

  public function getDependencies() {
    return array_merge($this->dependencies, $this->softDependencies);
  }

  /**
   * Name of a function for displaying feedback. It must take the message to
   * display as its first argument, and a (string) message type as its second
   * argument
   * (see drush_log()).
   *
   * @var string
   */
  protected static $displayFunction;

  public static function setDisplayFunction($display_function) {
    self::$displayFunction = $display_function;
  }

  /**
   * Track whether or not we've already displayed an encryption warning
   *
   * @var bool
   */
  protected static $showEncryptionWarning = TRUE;

  /**
   * The fraction of the memory limit at which an operation will be interrupted.
   * Can be overridden by a Migration subclass if one would like to push the
   * envelope. Defaults to 85%.
   *
   * @var float
   */
  protected $memoryThreshold = 0.85;

  /**
   * The PHP memory_limit expressed in bytes.
   *
   * @var int
   */
  protected $memoryLimit;

  /**
   * The fraction of the time limit at which an operation will be interrupted.
   * Can be overridden by a Migration subclass if one would like to push the
   * envelope. Defaults to 90%.
   *
   * @var float
   */
  protected $timeThreshold = 0.90;

  /**
   * The PHP max_execution_time.
   *
   * @var int
   */
  protected $timeLimit;

  /**
   * A time limit in seconds appropriate to be used in a batch
   * import. Defaults to 240.
   *
   * @var int
   */
  protected $batchTimeLimit = 240;

  /**
   * MigrateTeamMember objects representing people involved with this
   * migration.
   *
   * @var array
   */
  protected $team = array();

  public function getTeam() {
    return $this->team;
  }

  public function setTeam(array $team) {
    $this->team = $team;
  }

  /**
   * If provided, an URL for an issue tracking system containing :id where
   * the issue number will go (e.g., 'http://example.com/project/ticket/:id').
   *
   * @var string
   */
  protected $issuePattern;

  public function getIssuePattern() {
    return $this->issuePattern;
  }

  public function setIssuePattern($issue_pattern) {
    $this->issuePattern = $issue_pattern;
  }

  /**
   * If we set an error handler (during import), remember the previous one so
   * it can be restored.
   *
   * @var callback
   */
  protected $previousErrorHandler = NULL;

  /**
   * Arguments configuring a migration.
   *
   * @var array
   */
  protected $arguments = array();

  public function getArguments() {
    return $this->arguments;
  }

  public function setArguments(array $arguments) {
    $this->arguments = $arguments;
  }

  public function addArguments(array $arguments) {
    $this->arguments = array_merge($this->arguments, $arguments);
  }

  /**
   * Disabling a migration prevents it from running with --all, or individually
   * without --force
   *
   * @var boolean
   */
  protected $enabled = TRUE;

  public function getEnabled() {
    return $this->enabled;
  }

  public function setEnabled($enabled) {
    $this->enabled = $enabled;
  }

  /**
   * Any module hooks which should be disabled during migration processes.
   *
   * @var array
   *  Key: Hook name (e.g., 'node_insert')
   *  Value: Array of modules for which to disable this hook (e.g.,
   *   array('pathauto')).
   */
  protected $disableHooks = array();

  public function getDisableHooks() {
    return $this->disableHooks;
  }

  /**
   * An array to track 'mail_system' variable if disabled.
   */
  protected $mailSystem;

  /**
   * Have we already warned about obsolete constructor argumentss on this
   * request?
   *
   * @var bool
   */
  static protected $groupArgumentWarning = FALSE;

  static protected $emptyArgumentsWarning = FALSE;

  /**
   * Codes representing the result of a rollback or import process.
   */
  const RESULT_COMPLETED = 1;   // All records have been processed

  const RESULT_INCOMPLETE = 2;  // The process has interrupted itself (e.g., the

  // memory limit is approaching)
  const RESULT_STOPPED = 3;     // The process was stopped externally (e.g., via

  // drush migrate-stop)
  const RESULT_FAILED = 4;      // The process had a fatal error

  const RESULT_SKIPPED = 5;     // Dependencies are unfulfilled - skip the process

  const RESULT_DISABLED = 6;    // This migration is disabled, skipping

  /**
   * Codes representing the current status of a migration, and stored in the
   * migrate_status table.
   */
  const STATUS_IDLE = 0;

  const STATUS_IMPORTING = 1;

  const STATUS_ROLLING_BACK = 2;

  const STATUS_STOPPING = 3;

  const STATUS_DISABLED = 4;

  /**
   * Message types to be passed to saveMessage() and saved in message tables.
   * MESSAGE_INFORMATIONAL represents a condition that did not prevent the
   * operation from succeeding - all others represent different severities of
   * conditions resulting in a source record not being imported.
   */
  const MESSAGE_ERROR = 1;

  const MESSAGE_WARNING = 2;

  const MESSAGE_NOTICE = 3;

  const MESSAGE_INFORMATIONAL = 4;

  /**
   * Get human readable name for a message constant.
   *
   * @return string
   *  Name.
   */
  public function getMessageLevelName($constant) {
    $map = array(
      MigrationBase::MESSAGE_ERROR => t('Error'),
      MigrationBase::MESSAGE_WARNING => t('Warning'),
      MigrationBase::MESSAGE_NOTICE => t('Notice'),
      MigrationBase::MESSAGE_INFORMATIONAL => t('Informational'),
    );
    return $map[$constant];
  }

  /**
   * Construction of a MigrationBase instance.
   *
   * @param array $arguments
   */
  public function __construct($arguments = array()) {
    // Support for legacy code passing a group object as the first parameter.
    if (is_object($arguments) && is_a($arguments, 'MigrateGroup')) {
      $this->group = $arguments;
      $this->arguments['group_name'] = $arguments->getName();
      if (!self::$groupArgumentWarning &&
        variable_get('migrate_deprecation_warnings', 1)) {
        self::displayMessage(t('Passing a group object to a migration constructor is now deprecated - pass through the arguments array passed to the leaf class instead.'));
        self::$groupArgumentWarning = TRUE;
      }
    }
    else {
      if (empty($arguments)) {
        $this->arguments = array();
        if (!self::$emptyArgumentsWarning &&
          variable_get('migrate_deprecation_warnings', 1)) {
          self::displayMessage(t('Passing an empty first parameter to a migration constructor is now deprecated - pass through the arguments array passed to the leaf class instead.'));
          self::$emptyArgumentsWarning = TRUE;
        }
      }
      else {
        $this->arguments = $arguments;
      }
      if (empty($this->arguments['group_name'])) {
        $this->arguments['group_name'] = 'default';
      }
      $this->group = MigrateGroup::getInstance($this->arguments['group_name']);
    }
    if (isset($this->arguments['machine_name'])) {
      $this->machineName = $this->arguments['machine_name'];
    }
    else {
      // Deprecated - this supports old code which does not pass the arguments
      // array through to the base constructor. Remove in the next version.
      $this->machineName = $this->machineFromClass(get_class($this));
    }
    // Make any group arguments directly accessible to the specific migration,
    // other than group dependencies.
    $group_arguments = $this->group->getArguments();
    unset($group_arguments['dependencies']);
    $this->arguments += $group_arguments;

    // Record the memory limit in bytes
    $limit = trim(ini_get('memory_limit'));
    if ($limit == '-1') {
      $this->memoryLimit = PHP_INT_MAX;
    }
    else {
      if (!is_numeric($limit)) {
        $last = drupal_strtolower($limit[strlen($limit) - 1]);
        $limit = substr($limit, 0, -1);
        switch ($last) {
          case 'g':
            $limit *= 1024;
          case 'm':
            $limit *= 1024;
          case 'k':
            $limit *= 1024;
            break;
          default:
            throw new Exception(t('Invalid PHP memory_limit !limit',
              array('!limit' => $limit)));
        }
      }
      $this->memoryLimit = $limit;
    }

    // Record the time limit
    $this->timeLimit = ini_get('max_execution_time');

    // Make sure we clear our semaphores in case of abrupt exit
    drupal_register_shutdown_function(array($this, 'endProcess'));

    // Save any hook disablement information.
    if (isset($this->arguments['disable_hooks']) &&
      is_array($this->arguments['disable_hooks'])) {
      $this->disableHooks = $this->arguments['disable_hooks'];
    }
  }

  /**
   * Initialize static members, before any class instances are created.
   */
  static public function staticInitialize() {
    // Default the displayFunction outputFunction based on context
    if (function_exists('drush_log')) {
      self::$displayFunction = 'drush_log';
    }
    else {
      self::$displayFunction = 'drupal_set_message';
    }
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
    // Support for legacy migration code - in later releases, the machine_name
    // should always be explicit.
    if (!$machine_name) {
      $machine_name = self::machineFromClass($class_name);
    }

    if (!preg_match('|^[a-z0-9_]+$|i', $machine_name)) {
      throw new Exception(t('!name is not a valid Migration machine name. Use only alphanumeric or underscore characters.',
        array('!name' => $machine_name)));
    }

    // We no longer have any need to store the machine_name in the arguments.
    if (isset($arguments['machine_name'])) {
      unset($arguments['machine_name']);
    }

    if (isset($arguments['group_name'])) {
      $group_name = $arguments['group_name'];
      unset($arguments['group_name']);
    }
    else {
      $group_name = 'default';
    }

    $arguments = self::encryptArguments($arguments);

    // Register the migration if it's not already there; if it is,
    // update the class and arguments in case they've changed.
    db_merge('migrate_status')
      ->key(array('machine_name' => $machine_name))
      ->fields(array(
        'class_name' => $class_name,
        'group_name' => $group_name,
        'arguments' => serialize($arguments),
      ))
      ->execute();
  }

  /**
   * Deregister a migration - remove all traces of it from the database (without
   * touching any content which was created by this migration).
   *
   * @param string $machine_name
   */
  static public function deregisterMigration($machine_name) {
    $rows_deleted = db_delete('migrate_status')
      ->condition('machine_name', $machine_name)
      ->execute();
    // Make sure the group gets deleted if we were the only member.
    MigrateGroup::deleteOrphans();
  }

  /**
   * The migration machine name is stored in the arguments.
   *
   * @return string
   */
  protected function generateMachineName() {
    return $this->arguments['machine_name'];
  }

  /**
   * Given only a class name, derive a machine name (the class name with the
   * "Migration" suffix, if any, removed).
   *
   * @param $class_name
   *
   * @return string
   */
  protected static function machineFromClass($class_name) {
    if (preg_match('/Migration$/', $class_name)) {
      $machine_name = drupal_substr($class_name, 0,
        strlen($class_name) - strlen('Migration'));
    }
    else {
      $machine_name = $class_name;
    }
    return $machine_name;
  }

  /**
   * Return the single instance of the given migration.
   *
   * @param string $machine_name
   */

  /**
   * Return the single instance of the given migration.
   *
   * @param $machine_name
   *  The unique machine name of the migration to retrieve.
   * @param string $class_name
   *  Deprecated - no longer used, class name is retrieved from migrate_status.
   * @param array $arguments
   *  Deprecated - no longer used, arguments are retrieved from migrate_status.
   *
   * @return MigrationBase
   */
  static public function getInstance($machine_name, $class_name = NULL, array $arguments = array()) {
    $migrations = &drupal_static(__FUNCTION__, array());
    // Otherwise might miss cache hit on case difference
    $machine_name_key = drupal_strtolower($machine_name);
    if (!isset($migrations[$machine_name_key])) {
      // See if we know about this migration
      $row = db_select('migrate_status', 'ms')
        ->fields('ms', array('class_name', 'group_name', 'arguments'))
        ->condition('machine_name', $machine_name)
        ->execute()
        ->fetchObject();
      if ($row) {
        $class_name = $row->class_name;
        $arguments = unserialize($row->arguments);
        $arguments = self::decryptArguments($arguments);
        $arguments['group_name'] = $row->group_name;
      }
      else {
        // Can't find a migration with this name
        self::displayMessage(t('No migration found with machine name !machine',
          array('!machine' => $machine_name)));
        return NULL;
      }
      $arguments['machine_name'] = $machine_name;
      if (class_exists($class_name)) {
        try {
          $migrations[$machine_name_key] = new $class_name($arguments);
        } catch (Exception $e) {
          self::displayMessage(t('Migration !machine could not be constructed.',
            array('!machine' => $machine_name)));
          self::displayMessage($e->getMessage());
          return NULL;
        }
      }
      else {
        self::displayMessage(t('No migration class !class found',
          array('!class' => $class_name)));
        return NULL;
      }
      if (isset($arguments['dependencies'])) {
        $migrations[$machine_name_key]->setHardDependencies(
          $arguments['dependencies']);
      }
      if (isset($arguments['soft_dependencies'])) {
        $migrations[$machine_name_key]->setSoftDependencies(
          $arguments['soft_dependencies']);
      }
    }
    return $migrations[$machine_name_key];
  }

  /**
   * @deprecated - No longer a useful distinction between "status" and "dynamic"
   *  migrations.
   */
  static public function isDynamic() {
    return FALSE;
  }

  /**
   * Default to printing messages, but derived classes are expected to save
   * messages indexed by current source ID.
   *
   * @param string $message
   *  The message to record.
   * @param int $level
   *  Optional message severity (defaults to MESSAGE_ERROR).
   */
  public function saveMessage($message, $level = MigrationBase::MESSAGE_ERROR) {
    switch ($level) {
      case MigrationBase::MESSAGE_ERROR:
        $level = 'error';
        break;
      case MigrationBase::MESSAGE_WARNING:
        $level = 'warning';
        break;
      case MigrationBase::MESSAGE_NOTICE:
        $level = 'notice';
        break;
      case MigrationBase::MESSAGE_INFORMATIONAL:
        $level = 'status';
        break;
    }

    self::displayMessage($message, $level);
  }

  /**
   * Output the given message appropriately
   * (drush_print/drupal_set_message/etc.)
   *
   * @param string $message
   *  The message to output.
   * @param int $level
   *  Optional message severity as understood by drupal_set_message and
   *   drush_log
   *  (defaults to 'error').
   */
  static public function displayMessage($message, $level = 'error') {
    call_user_func(self::$displayFunction, $message, $level);
  }

  /**
   * Custom PHP error handler.
   * TODO: Redundant with hook_watchdog?
   *
   * @param $error_level
   *   The level of the error raised.
   * @param $message
   *   The error message.
   * @param $filename
   *   The filename that the error was raised in.
   * @param $line
   *   The line number the error was raised at.
   * @param $context
   *   An array that points to the active symbol table at the point the error
   *   occurred.
   */
  public function errorHandler($error_level, $message, $filename, $line, $context) {
    if ($error_level & error_reporting()) {
      $message .= "\n" . t('File !file, line !line',
          array('!line' => $line, '!file' => $filename));
      // Record notices and continue
      if ($error_level == E_NOTICE || $error_level == E_USER_NOTICE) {
        $this->saveMessage($message . "(file: $filename, line $line)", MigrationBase::MESSAGE_INFORMATIONAL);
      }
      // Simply ignore strict and deprecated errors
      // Note DEPRECATED constants introduced in PHP 5.3
      elseif (!($error_level == E_STRICT || $error_level == 8192 ||
        $error_level == 16384)) {
        throw new MigrateException($message, MigrationBase::MESSAGE_ERROR);
      }
    }
  }

  /**
   * Takes an Exception object and both saves and displays it, pulling
   * additional information on the location triggering the exception.
   *
   * @param Exception $exception
   *  Object representing the exception.
   * @param boolean $save
   *  Whether to save the message in the migration's mapping table. Set to
   *   FALSE
   *  in contexts where this doesn't make sense.
   */
  public function handleException($exception, $save = TRUE) {
    $result = _drupal_decode_exception($exception);
    $message = $result['!message'] . ' (' . $result['%file'] . ':' . $result['%line'] . ')';
    if ($save) {
      $this->saveMessage($message);
    }
    self::displayMessage($message);
  }

  /**
   * Check the current status of a migration.
   *
   * @return int
   *  One of the MigrationBase::STATUS_* constants
   */
  public function getStatus() {
    if (!$this->enabled) {
      return MigrationBase::STATUS_DISABLED;
    }
    $status = db_select('migrate_status', 'ms')
      ->fields('ms', array('status'))
      ->condition('machine_name', $this->machineName)
      ->execute()
      ->fetchField();
    if (!isset($status)) {
      $status = MigrationBase::STATUS_IDLE;
    }
    return $status;
  }

  /**
   * Retrieve the last time an import operation completed successfully.
   *
   * @return string
   *  Date/time string, formatted... How? Default DB server format?
   */
  public function getLastImported() {
    $last_imported = db_select('migrate_log', 'ml')
      ->fields('ml', array('endtime'))
      ->condition('machine_name', $this->machineName)
      ->isNotNull('endtime')
      ->orderBy('endtime', 'DESC')
      ->execute()
      ->fetchField();
    if ($last_imported) {
      $last_imported = date('Y-m-d H:i:s', $last_imported / 1000);
    }
    else {
      $last_imported = '';
    }
    return $last_imported;
  }

  /**
   * Fetch the current highwater mark for updated content.
   *
   * @return string
   *  The highwater mark.
   */
  public function getHighwater() {
    $highwater = db_select('migrate_status', 'ms')
      ->fields('ms', array('highwater'))
      ->condition('machine_name', $this->machineName)
      ->execute()
      ->fetchField();
    return $highwater;
  }

  /**
   * Save the highwater mark for this migration (but not when using an idlist).
   *
   * @param mixed $highwater
   *  Highwater mark to save
   * @param boolean $force
   *  If TRUE, save even if it's lower than the previous value.
   */
  protected function saveHighwater($highwater, $force = FALSE) {
    if (!isset($this->options['idlist'])) {
      $query = db_update('migrate_status')
        ->fields(array('highwater' => $highwater))
        ->condition('machine_name', $this->machineName);
      if (!$force) {
        if (!empty($this->highwaterField['type']) && $this->highwaterField['type'] == 'int') {
          // If the highwater is an integer type, we need to force the DB server
          // to treat the varchar highwater field as an integer (otherwise it will
          // think '5' > '10').
          switch (Database::getConnection()->databaseType()) {
            case 'pgsql':
              $query->where('(CASE WHEN highwater=\'\' THEN 0 ELSE CAST(highwater AS INTEGER) END) < :highwater', array(':highwater' => intval($highwater)));
              break;
            default:
              // MySQL casts as integers as SIGNED or UNSIGNED.
              $query->where('(CASE WHEN highwater=\'\' THEN 0 ELSE CAST(highwater AS SIGNED) END) < :highwater', array(':highwater' => intval($highwater)));
          }
        }
        else {
          $query->condition('highwater', $highwater, '<');
        }
      }
      $query->execute();
    }
  }

  /**
   * Retrieve the last throughput for current Migration (items / minute).
   *
   * @return integer
   */
  public function getLastThroughput() {
    $last_throughput = 0;
    $row = db_select('migrate_log', 'ml')
      ->fields('ml', array('starttime', 'endtime', 'numprocessed'))
      ->condition('machine_name', $this->machineName)
      ->condition('process_type', 1)
      ->isNotNull('endtime')
      ->orderBy('starttime', 'DESC')
      ->execute()
      ->fetchObject();
    if ($row) {
      $elapsed = ($row->endtime - $row->starttime) / 1000;
      if ($elapsed > 0) {
        $last_throughput = round(($row->numprocessed / $elapsed) * 60);
      }
    }
    return $last_throughput;
  }

  /**
   * Reports whether this migration process is complete. For a Migration, for
   * example, this would be whether all available source rows have been
   * processed. Other MigrationBase classes will need to return TRUE/FALSE
   * appropriately.
   */
  abstract public function isComplete();

  /**
   * Reports whether all (hard) dependencies have completed migration
   */
  protected function dependenciesComplete($rollback = FALSE) {
    if ($rollback) {
      foreach (migrate_migrations() as $migration) {
        $dependencies = $migration->getHardDependencies();
        if (array_search($this->machineName, $dependencies) !== FALSE) {
          if (method_exists($migration, 'importedCount') && $migration->importedCount() > 0) {
            return FALSE;
          }
        }
      }
    }
    else {
      foreach ($this->dependencies as $dependency) {
        $migration = MigrationBase::getInstance($dependency);
        if (!$migration || !$migration->isComplete()) {
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Returns an array of the migration's dependencies that are incomplete.
   */
  public function incompleteDependencies() {
    $incomplete = array();
    foreach ($this->getDependencies() as $dependency) {
      $migration = MigrationBase::getInstance($dependency);
      if (!$migration || !$migration->isComplete()) {
        $incomplete[] = $dependency;
      }
    }
    return $incomplete;
  }

  /**
   * Begin a process, ensuring only one process can be active
   * at once on a given migration.
   *
   * @param int $newStatus
   *  MigrationBase::STATUS_IMPORTING or MigrationBase::STATUS_ROLLING_BACK
   */
  protected function beginProcess($newStatus) {
    // So hook_watchdog() knows what migration (if any) is running
    self::$currentMigration = $this;

    // Try to make the semaphore handling atomic (depends on DB support)
    $transaction = db_transaction();

    // Save the current mail system, prior to disabling emails.
    $this->saveMailSystem();

    // Prevent emails from being sent out during migrations.
    $this->disableMailSystem();

    $this->starttime = microtime(TRUE);

    // Check to make sure there's no process already running for this migration
    $status = $this->getStatus();
    if ($status != MigrationBase::STATUS_IDLE) {
      throw new MigrateException(t('There is already an active process on !machine_name',
        array('!machine_name' => $this->machineName)));
    }

    $this->processing = TRUE;
    $this->status = $newStatus;
    db_merge('migrate_status')
      ->key(array('machine_name' => $this->machineName))
      ->fields(array('class_name' => get_class($this), 'status' => $newStatus))
      ->execute();

    // Set an error handler for imports
    if ($newStatus == MigrationBase::STATUS_IMPORTING) {
      $this->previousErrorHandler = set_error_handler(array(
        $this,
        'errorHandler',
      ));
    }

    // Save the initial history record
    if ($this->logHistory) {
      $this->logID = db_insert('migrate_log')
        ->fields(array(
          'machine_name' => $this->machineName,
          'process_type' => $newStatus,
          'starttime' => round(microtime(TRUE) * 1000),
          'initialHighwater' => $this->getHighwater(),
        ))
        ->execute();
    }

    // If we're disabling any hooks, reset the static module_implements cache so
    // it is rebuilt with the specified hooks removed by our
    // hook_module_implements_alter(). By setting #write_cache to FALSE, we
    // ensure that our munged version of the hooks array does not get written
    // to the persistent cache and interfere with other Drupal processes.
    if (!empty($this->disableHooks)) {
      $implementations = &drupal_static('module_implements');
      $implementations = array();
      $implementations['#write_cache'] = FALSE;
    }
  }

  /**
   * End a rollback or import process, releasing the semaphore. Note that it
   * must be public to be callable as the shutdown function.
   */
  public function endProcess() {
    if ($this->previousErrorHandler) {
      set_error_handler($this->previousErrorHandler);
      $this->previousErrorHandler = NULL;
    }
    if ($this->processing) {
      $this->status = MigrationBase::STATUS_IDLE;

      // Restore the previous mail handler.
      $this->restoreMailSystem();

      $fields = array(
        'class_name' => get_class($this),
        'status' => MigrationBase::STATUS_IDLE,
      );
      db_merge('migrate_status')
        ->key(array('machine_name' => $this->machineName))
        ->fields($fields)
        ->execute();

      // Complete the log record
      if ($this->logHistory) {
        try {
          db_merge('migrate_log')
            ->key(array('mlid' => $this->logID))
            ->fields(array(
              'endtime' => round(microtime(TRUE) * 1000),
              'finalhighwater' => $this->getHighwater(),
              'numprocessed' => $this->total_processed,
            ))
            ->execute();
        } catch (PDOException $e) {
          Migration::displayMessage(t('Could not log operation on migration !name - possibly MigrationBase::beginProcess() was not called',
            array('!name' => $this->machineName)));
        }
      }

      $this->processing = FALSE;
    }
    self::$currentMigration = NULL;
  }

  /**
   * Signal that any current import or rollback process should end itself at
   * the earliest opportunity
   */
  public function stopProcess() {
    // Do not change the status of an idle migration
    db_update('migrate_status')
      ->fields(array('status' => MigrationBase::STATUS_STOPPING))
      ->condition('machine_name', $this->machineName)
      ->condition('status', MigrationBase::STATUS_IDLE, '<>')
      ->execute();
  }

  /**
   * Reset the status of the migration to IDLE (to be used when the status
   * gets stuck, e.g. if a process core-dumped)
   */
  public function resetStatus() {
    // Do not change the status of an already-idle migration
    db_update('migrate_status')
      ->fields(array('status' => MigrationBase::STATUS_IDLE))
      ->condition('machine_name', $this->machineName)
      ->condition('status', MigrationBase::STATUS_IDLE, '<>')
      ->execute();
  }

  /**
   * Perform an operation during the rollback phase.
   *
   * @param array $options
   *  List of options provided (usually from a drush command). Specific to
   *  the derived class.
   */
  public function processRollback(array $options = array()) {
    if ($this->enabled) {
      $return = MigrationBase::RESULT_COMPLETED;
      if (method_exists($this, 'rollback')) {
        $this->options = $options;
        if (!isset($options['force'])) {
          if (!$this->dependenciesComplete(TRUE)) {
            return MigrationBase::RESULT_SKIPPED;
          }
        }
        $this->beginProcess(MigrationBase::STATUS_ROLLING_BACK);
        try {
          $return = $this->rollback();
        } catch (Exception $exception) {
          // If something bad happened, make sure we clear the semaphore
          $this->endProcess();
          throw $exception;
        }
        $this->endProcess();
      }
    }
    else {
      $return = MigrationBase::RESULT_DISABLED;
    }
    return $return;
  }

  /**
   * Perform an operation during the import phase
   *
   * @param array $options
   *  List of options provided (usually from a drush command). Specific to
   *  the derived class.
   */
  public function processImport(array $options = array()) {
    if ($this->enabled) {
      $return = MigrationBase::RESULT_COMPLETED;
      if (method_exists($this, 'import')) {
        $this->options = $options;

        if (!isset($options['force']) || !$options['force']) {
          if (!$this->dependenciesComplete()) {
            return MigrationBase::RESULT_SKIPPED;
          }
        }
        $this->beginProcess(MigrationBase::STATUS_IMPORTING);
        try {
          $return = $this->import();
        } catch (Exception $exception) {
          // If something bad happened, make sure we clear the semaphore
          $this->endProcess();
          throw $exception;
        }

        if ($return == MigrationBase::RESULT_COMPLETED && isset($this->total_successes)) {
          $time = microtime(TRUE) - $this->starttime;
          if ($time > 0) {
            $overallThroughput = round(60 * $this->total_successes / $time);
          }
          else {
            $overallThroughput = 9999;
          }
        }
        else {
          $overallThroughput = 0;
        }
        $this->endProcess($overallThroughput);
      }
    }
    else {
      $return = MigrationBase::RESULT_DISABLED;
    }
    return $return;
  }

  /**
   * Set the PHP time limit. This method may be called from batch callbacks
   * before calling the processImport method.
   */
  public function setBatchTimeLimit() {
    drupal_set_time_limit($this->batchTimeLimit);
  }

  /**
   * A derived migration class does the actual rollback or import work in these
   * methods - we cannot declare them abstract because some classes may define
   * only one.
   *
   * abstract protected function rollback();
   * abstract protected function import();
   */

  /**
   * Test whether we've exceeded the desired memory threshold. If so, output a
   * message.
   *
   * @return boolean
   *  TRUE if the threshold is exceeded, FALSE if not.
   */
  protected function memoryExceeded() {
    $usage = memory_get_usage();
    $pct_memory = $usage / $this->memoryLimit;
    if ($pct_memory > $this->memoryThreshold) {
      self::displayMessage(
        t('Memory usage is !usage (!pct% of limit !limit), resetting statics',
          array(
            '!pct' => round($pct_memory * 100),
            '!usage' => format_size($usage),
            '!limit' => format_size($this->memoryLimit),
          )),
        'warning');
      // First, try resetting Drupal's static storage - this frequently releases
      // plenty of memory to continue
      drupal_static_reset();
      $usage = memory_get_usage();
      $pct_memory = $usage / $this->memoryLimit;
      // Use a lower threshold - we don't want to be in a situation where we keep
      // coming back here and trimming a tiny amount
      if ($pct_memory > (.90 * $this->memoryThreshold)) {
        self::displayMessage(
          t('Memory usage is now !usage (!pct% of limit !limit), not enough reclaimed, starting new batch',
            array(
              '!pct' => round($pct_memory * 100),
              '!usage' => format_size($usage),
              '!limit' => format_size($this->memoryLimit),
            )),
          'warning');
        return TRUE;
      }
      else {
        self::displayMessage(
          t('Memory usage is now !usage (!pct% of limit !limit), reclaimed enough, continuing',
            array(
              '!pct' => round($pct_memory * 100),
              '!usage' => format_size($usage),
              '!limit' => format_size($this->memoryLimit),
            )),
          'warning');
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Test whether we're approaching the PHP time limit.
   *
   * @return boolean
   *  TRUE if the threshold is exceeded, FALSE if not.
   */
  protected function timeExceeded() {
    if ($this->timeLimit == 0) {
      return FALSE;
    }
    $time_elapsed = time() - REQUEST_TIME;
    $pct_time = $time_elapsed / $this->timeLimit;
    if ($pct_time > $this->timeThreshold) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Test whether we've exceeded the designated time limit.
   *
   * @return boolean
   *  TRUE if the threshold is exceeded, FALSE if not.
   */
  protected function timeOptionExceeded() {
    if (!$timelimit = $this->getTimeLimit()) {
      return FALSE;
    }
    $time_elapsed = time() - REQUEST_TIME;
    if ($time_elapsed >= $timelimit) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Encrypt an incoming value. Detects for existence of the Drupal 'Encrypt'
   *  module.
   *
   * @param string $value
   *
   * @return string The encrypted value.
   */
  static public function encrypt($value) {
    if (module_exists('encrypt')) {
      $value = encrypt($value);
    }
    else {
      if (self::$showEncryptionWarning) {
        MigrationBase::displayMessage(t('Encryption of secure migration information is not supported. Ensure the <a href="@encrypt">Encrypt module</a> is installed for this functionality.',
          array(
            '@encrypt' => 'http://drupal.org/project/encrypt',
          )
        ),
          'warning');
        self::$showEncryptionWarning = FALSE;
      }
    }
    return $value;
  }

  /**
   * Decrypt an incoming value.
   *
   * @param string $value
   *
   * @return string The encrypted value
   */
  static public function decrypt($value) {
    if (module_exists('encrypt')) {
      $value = decrypt($value);
    }
    else {
      if (self::$showEncryptionWarning) {
        MigrationBase::displayMessage(t('Encryption of secure migration information is not supported. Ensure the <a href="@encrypt">Encrypt module</a> is installed for this functionality.',
          array(
            '@encrypt' => 'http://drupal.org/project/encrypt',
          )
        ),
          'warning');
        self::$showEncryptionWarning = FALSE;
      }
    }
    return $value;
  }

  /**
   * Make sure any arguments we want to be encrypted get encrypted.
   *
   * @param array $arguments
   *
   * @return array
   */
  static public function encryptArguments(array $arguments) {
    if (isset($arguments['encrypted_arguments'])) {
      foreach ($arguments['encrypted_arguments'] as $argument_name) {
        if (isset($arguments[$argument_name])) {
          $arguments[$argument_name] = self::encrypt(
            serialize($arguments[$argument_name]));
        }
      }
    }
    return $arguments;
  }

  /**
   * Make sure any arguments we want to be decrypted get decrypted.
   *
   * @param array $arguments
   *
   * @return array
   */
  static public function decryptArguments(array $arguments) {
    if (isset($arguments['encrypted_arguments'])) {
      foreach ($arguments['encrypted_arguments'] as $argument_name) {
        if (isset($arguments[$argument_name])) {
          $decrypted_string = self::decrypt($arguments[$argument_name]);
          // A decryption failure will return FALSE and issue a notice. We need
          // to distinguish a failure from a serialized FALSE.
          $unserialized_value = @unserialize($decrypted_string);
          if ($unserialized_value === FALSE && $decrypted_string != serialize(FALSE)) {
            self::displayMessage(t('Failed to decrypt argument %argument_name',
              array('%argument_name' => $argument_name)));
            unset($arguments[$argument_name]);
          }
          else {
            $arguments[$argument_name] = $unserialized_value;
          }
        }
      }
    }
    return $arguments;
  }

  /**
   * Convert an incoming string (which may be a UNIX timestamp, or an
   * arbitrarily-formatted date/time string) to a UNIX timestamp.
   *
   * @param string $value
   *   The time string to convert.
   * @param string $timezone
   *   Optional timezone for the time string. NULL to leave the timezone unset.
   *
   * @return string
   *   The UNIX timestamp.
   */
  static public function timestamp($value, $timezone = NULL) {
    // Does it look like it's already a timestamp? Just return it
    if (is_numeric($value)) {
      return $value;
    }

    // Default empty values to now
    if (empty($value)) {
      return time();
    }

    if (isset($timezone)) {
      $timezone = new DateTimeZone($timezone);
    }
    $date = new DateTime($value, $timezone);
    $time = $date->format('U');
    if ($time == FALSE) {
      // Handles form YYYY-MM-DD HH:MM:SS.garbage
      if (drupal_strlen($value) > 19) {
        $time = strtotime(drupal_substr($value, 0, 19));
      }
    }
    return $time;
  }

  /**
   * Saves the current mail system, or set a system default if there is none.
   */
  public function saveMailSystem() {
    global $conf;
    $this->mailSystem = empty($conf['mail_system']) ? NULL : $conf['mail_system'];
  }

  /**
   * Disables mail system to prevent emails from being sent during migrations.
   */
  public function disableMailSystem() {
    global $conf;
    if (!empty($conf['mail_system'])) {
      foreach ($conf['mail_system'] as $system => $class) {
        $conf['mail_system'][$system] = 'MigrateMailIgnore';
      }
    }
    else {
      $conf['mail_system'] = array('default-system' => 'MigrateMailIgnore');
    }
  }

  /**
   * Restores the original saved mail system for migrations that require it.
   */
  public function restoreMailSystem() {
    global $conf;
    $conf['mail_system'] = $this->mailSystem;
  }
}

// Make sure static members (in particular, $displayFunction) get
// initialized even if there are no class instances.
MigrationBase::staticInitialize();
