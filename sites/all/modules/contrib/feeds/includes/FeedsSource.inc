<?php

/**
 * @file
 * Definition of FeedsSourceInterface, FeedsState and FeedsSource class.
 */

/**
 * Distinguish exceptions occurring when handling locks.
 */
class FeedsLockException extends Exception {}

/**
 * Denote a import or clearing stage. Used for multi page processing.
 */
define('FEEDS_START', 'start_time');
define('FEEDS_FETCH', 'fetch');
define('FEEDS_PARSE', 'parse');
define('FEEDS_PROCESS', 'process');
define('FEEDS_PROCESS_CLEAR', 'process_clear');
define('FEEDS_PROCESS_EXPIRE', 'process_expire');

/**
 * Defines an interface for a feed source.
 */
interface FeedsSourceInterface {

  /**
   * Returns if a plugin handles source specific configuration.
   *
   * Crutch: for ease of use, we implement FeedsSourceInterface for every
   * plugin, but then we need to have a handle which plugin actually implements
   * source configuration.
   *
   * @see FeedsPlugin
   *
   * @return bool
   *   TRUE if a plugin handles source specific configuration, FALSE otherwise.
   */
  public function hasSourceConfig();

  /**
   * Return an associative array of default values.
   */
  public function sourceDefaults();

  /**
   * Returns a Form API form array that defines a form configuring values.
   *
   * Keys correspond to the keys of the return value of sourceDefaults().
   */
  public function sourceForm($source_config);

  /**
   * Validate user entered values submitted by sourceForm().
   */
  public function sourceFormValidate(&$source_config);

  /**
   * A source is being saved.
   */
  public function sourceSave(FeedsSource $source);

  /**
   * A source is being deleted.
   */
  public function sourceDelete(FeedsSource $source);

}

/**
 * Status of an import or clearing operation on a source.
 */
class FeedsState {

  /**
   * Floating point number denoting the progress made.
   *
   * 0.0 meaning no progress.
   * 1.0 = FEEDS_BATCH_COMPLETE, meaning finished.
   *
   * @var float
   */
  public $progress;

  /**
   * Used as a pointer to store where left off. Must be serializable.
   *
   * @var mixed
   */
  public $pointer;

  /**
   * Natural numbers denoting more details about the progress being made.
   *
   * @var int
   */
  public $total;
  public $created;
  public $updated;
  public $deleted;
  public $unpublished;
  public $blocked;
  public $skipped;
  public $failed;

  /**
   * IDs of entities to be removed.
   *
   * @var array
   */
  public $removeList;

  /**
   * Constructor, initialize variables.
   */
  public function __construct() {
    $this->progress = FEEDS_BATCH_COMPLETE;
    $this->total
      = $this->created
      = $this->updated
      = $this->deleted
      = $this->unpublished
      = $this->blocked
      = $this->skipped
      = $this->failed
      = 0;
  }

  /**
   * Safely report progress.
   *
   * When $total == $progress, the state of the task tracked by this state is
   * regarded to be complete.
   *
   * Handles the following cases gracefully:
   *
   * - $total is 0
   * - $progress is larger than $total
   * - $progress approximates $total so that $finished rounds to 1.0
   *
   * @param int $total
   *   A natural number that is the total to be worked off.
   * @param int $progress
   *   A natural number that is the progress made on $total.
   */
  public function progress($total, $progress) {
    if ($progress > $total) {
      $this->progress = FEEDS_BATCH_COMPLETE;
    }
    elseif ($total) {
      $this->progress = (float) $progress / $total;
      if ($this->progress == FEEDS_BATCH_COMPLETE && $total != $progress) {
        $this->progress = 0.99;
      }
    }
    else {
      $this->progress = FEEDS_BATCH_COMPLETE;
    }
  }

}

/**
 * Holds the source of a feed to import.
 *
 * This class encapsulates a source of a feed. It stores where the feed can be
 * found and how to import it.
 *
 * Information on how to import a feed is encapsulated in a FeedsImporter object
 * which is identified by the common id of the FeedsSource and the
 * FeedsImporter. More than one FeedsSource can use the same FeedsImporter
 * therefore a FeedsImporter never holds a pointer to a FeedsSource object, nor
 * does it hold any other information for a particular FeedsSource object.
 *
 * Classes extending FeedsPlugin can implement a sourceForm to expose
 * configuration for a FeedsSource object. This is for instance how FeedsFetcher
 * exposes a text field for a feed URL or how FeedsCSVParser exposes a select
 * field for choosing between colon or semicolon delimiters.
 *
 * It is important that a FeedsPlugin does not directly hold information about
 * a source but leave all storage up to FeedsSource. An instance of a
 * FeedsPlugin class only exists once per FeedsImporter configuration, while an
 * instance of a FeedsSource class exists once per feed_nid to be imported.
 *
 * As with FeedsImporter, the idea with FeedsSource is that it can be used
 * without actually saving the object to the database.
 */
class FeedsSource extends FeedsConfigurable {

  /**
   * Contains the node id of the feed this source info object is attached to.
   *
   * Equals 0 if not attached to any node - for example when used on a
   * standalone import form within Feeds or by other API users.
   *
   * @var int
   */
  protected $feed_nid;

  /**
   * The FeedsImporter object that this source is expected to be used with.
   *
   * @var FeedsImporter
   */
  protected $importer;

  /**
   * Holds the current state of an import, clear or expire task.
   *
   * Array keys can be:
   * - FEEDS_START
   *   Timestamp of when a task has started.
   * - FEEDS_FETCH
   *   A FeedsState object holding the state of the fetch stage, used during
   *   imports.
   * - FEEDS_PARSE
   *   A FeedsState object holding the state of the parse stage, used during
   *   imports.
   * - FEEDS_PROCESS
   *   A FeedsState object holding the state of the process stage, used during
   *   imports.
   * - FEEDS_PROCESS_CLEAR
   *   A FeedsState object holding the state of the clear task.
   * - FEEDS_PROCESS_EXPIRE
   *   A FeedsState object holding the state of the expire task.
   *
   * @var FeedsState[]|array|null
   */
  protected $state;

  /**
   * Fetcher result, used to cache fetcher result when batching.
   *
   * @var FeedsFetcherResult
   */
  protected $fetcher_result;

  /**
   * Timestamp of when this source was imported the last time.
   *
   * @var int
   */
  protected $imported;

  /**
   * Holds an exception object in case an exception occurs during importing.
   *
   * @var Exception|null
   */
  protected $exception;

  /**
   * The account switcher.
   *
   * @var FeedsAccountSwitcherInterface
   */
  protected $accountSwitcher;

  /**
   * Instantiates an unique FeedsSource per class, importer ID and Feed node ID.
   *
   * Don't use this method directly, use feeds_source() instead.
   *
   * @param string $importer_id
   *   The machine name of the importer.
   * @param int $feed_nid
   *   The node id of a feed node if the source is attached to a feed node.
   * @param FeedsAccountSwitcherInterface $account_switcher
   *   The account switcher to use to be able to perform actions as a different
   *   user.
   */
  public static function instance($importer_id, $feed_nid, FeedsAccountSwitcherInterface $account_switcher = NULL) {
    $class = variable_get('feeds_source_class', 'FeedsSource');

    $instances = &drupal_static(__METHOD__, array());

    if (!isset($instances[$class][$importer_id][$feed_nid])) {
      $instances[$class][$importer_id][$feed_nid] = new $class($importer_id, $feed_nid, $account_switcher);
    }
    return $instances[$class][$importer_id][$feed_nid];
  }

  /**
   * Constructor.
   *
   * @param string $importer_id
   *   The machine name of the importer.
   * @param int $feed_nid
   *   The feed node ID for this Feeds source. This should be '0' if the
   *   importer is not attached to a content type.
   * @param FeedsAccountSwitcherInterface $account_switcher
   *   The account switcher to use to be able to perform actions as a different
   *   user.
   */
  protected function __construct($importer_id, $feed_nid, FeedsAccountSwitcherInterface $account_switcher = NULL) {
    $this->feed_nid = $feed_nid;
    $this->importer = feeds_importer($importer_id);
    if (is_null($account_switcher)) {
      $this->accountSwitcher = new FeedsAccountSwitcher();
    }
    else {
      $this->accountSwitcher = $account_switcher;
    }
    parent::__construct($importer_id);
    $this->load();
  }

  /**
   * Returns the FeedsImporter object for this source.
   *
   * @return FeedsImporter
   *   The importer associated with this Feeds source.
   */
  public function importer() {
    return $this->importer;
  }

  /**
   * Preview = fetch and parse a feed.
   *
   * @return FeedsParserResult
   *   A FeedsParserResult instance.
   *
   * @throws Exception
   *   If an error occurs when fetching or parsing.
   */
  public function preview() {
    $result = $this->importer->fetcher->fetch($this);
    $result = $this->importer->parser->parse($this, $result);
    module_invoke_all('feeds_after_parse', $this, $result);
    return $result;
  }

  /**
   * Start importing a source.
   *
   * This method starts an import job. Depending on the configuration of the
   * importer of this source, a Batch API job or a background job with Job
   * Scheduler will be created.
   *
   * @throws Exception
   *   If processing in background is enabled, the first batch chunk of the
   *   import will be executed on the current page request. This means that this
   *   method may throw the same exceptions as FeedsSource::import().
   */
  public function startImport() {
    $config = $this->importer->getConfig();
    if ($config['process_in_background']) {
      $this->startBackgroundJob('import');
    }
    else {
      $this->startBatchAPIJob(t('Importing'), 'import');
    }
  }

  /**
   * Start deleting all imported items of a source.
   *
   * This method starts a clear job. Depending on the configuration of the
   * importer of this source, a Batch API job or a background job with Job
   * Scheduler will be created.
   *
   * @throws Exception
   *   If processing in background is enabled, the first batch chunk of the
   *   clear task will be executed on the current page request. This means that
   *   this method may throw the same exceptions as FeedsSource::clear().
   */
  public function startClear() {
    $config = $this->importer->getConfig();
    if ($config['process_in_background']) {
      $this->startBackgroundJob('clear');
    }
    else {
      $this->startBatchAPIJob(t('Deleting'), 'clear');
    }
  }

  /**
   * Schedule all periodic tasks for this source, even when scheduled before.
   */
  public function schedule() {
    $this->scheduleImport();
    $this->scheduleExpire();
  }

  /**
   * Schedule all periodic tasks for this source if not already scheduled.
   */
  public function ensureSchedule() {
    $this->scheduleImport(FALSE);
    $this->scheduleExpire(FALSE);
  }

  /**
   * Schedule periodic or background import tasks.
   *
   * @param bool $force
   *   (optional) If true, forces the scheduling to happen.
   *   Defaults to true.
   */
  public function scheduleImport($force = TRUE) {
    // Check whether any fetcher is overriding the import period.
    $period = $this->importer->config['import_period'];
    $fetcher_period = $this->importer->fetcher->importPeriod($this);
    if (is_numeric($fetcher_period)) {
      $period = $fetcher_period;
    }
    $job = array(
      'type' => $this->id,
      'id' => $this->feed_nid,
      'period' => $period,
      'periodic' => TRUE,
    );
    if ($period == FEEDS_SCHEDULE_NEVER && $this->progressImporting() === FEEDS_BATCH_COMPLETE) {
      JobScheduler::get('feeds_source_import')->remove($job);
    }
    elseif ($this->progressImporting() === FEEDS_BATCH_COMPLETE) {
      // Check for an existing job first.
      $existing = JobScheduler::get('feeds_source_import')->check($job);
      if (!$existing || $force) {
        // If there is no existing job, schedule a new job.
        JobScheduler::get('feeds_source_import')->set($job);
      }
      elseif ($existing['scheduled']) {
        // If the previous job is still marked as 'running', reschedule it.
        JobScheduler::get('feeds_source_import')->reschedule($existing);
      }
    }
    elseif (!$this->isQueued()) {
      // Feed is not fully imported yet, so we put this job back in the queue
      // immediately for further processing.
      $queue = DrupalQueue::get('feeds_source_import');
      $queue->createItem($job);
    }
  }

  /**
   * Schedule background expire tasks.
   *
   * @param bool $force
   *   (optional) If true, forces the scheduling to happen.
   *   Defaults to true.
   */
  public function scheduleExpire($force = TRUE) {
    // Schedule as soon as possible if a batch is active.
    $period = $this->progressExpiring() === FEEDS_BATCH_COMPLETE ? 3600 : 0;

    $job = array(
      'type' => $this->id,
      'id' => $this->feed_nid,
      'period' => $period,
      'periodic' => TRUE,
    );
    if ($this->importer->processor->expiryTime() == FEEDS_EXPIRE_NEVER) {
      JobScheduler::get('feeds_source_expire')->remove($job);
    }
    else {
      // Check for an existing job first.
      $existing = JobScheduler::get('feeds_source_expire')->check($job);
      if (!$existing || $force) {
        // If there is no existing job, schedule a new job.
        JobScheduler::get('feeds_source_expire')->set($job);
      }
      elseif ($existing['scheduled']) {
        // If the previous job is still marked as 'running', reschedule it.
        JobScheduler::get('feeds_source_expire')->reschedule($existing);
      }
    }
  }

  /**
   * Schedule background clearing tasks.
   */
  public function scheduleClear() {
    $job = array(
      'type' => $this->id,
      'id' => $this->feed_nid,
    );

    if ($this->progressClearing() !== FEEDS_BATCH_COMPLETE) {
      // Feed is not fully cleared yet, so we put this job back in the queue
      // immediately for further processing.
      $queue = DrupalQueue::get('feeds_source_clear');
      $queue->createItem($job);
    }
  }

  /**
   * Import a source: execute fetching, parsing and processing stage.
   *
   * This method only executes the current batch chunk, then returns. If you are
   * looking to import an entire source, use FeedsSource::startImport() instead.
   *
   * @return float
   *   FEEDS_BATCH_COMPLETE if the import process finished. A decimal between
   *   0.0 and 0.9 periodic if import is still in progress.
   *
   * @throws Exception
   *   In case an error occurs when importing.
   */
  public function import() {
    $this->acquireLock();
    try {
      // If fetcher result is empty, we are starting a new import, log.
      if (empty($this->fetcher_result)) {
        module_invoke_all('feeds_before_import', $this);
        if (module_exists('rules')) {
          rules_invoke_event('feeds_before_import', $this);
        }
        $this->state[FEEDS_START] = time();
      }

      // Fetch.
      if (empty($this->fetcher_result) || FEEDS_BATCH_COMPLETE == $this->progressParsing()) {
        $this->fetcher_result = $this->importer->fetcher->fetch($this);
        // Clean the parser's state, we are parsing an entirely new file.
        unset($this->state[FEEDS_PARSE]);
      }

      // Parse.
      $parser_result = $this->importer->parser->parse($this, $this->fetcher_result);
      module_invoke_all('feeds_after_parse', $this, $parser_result);

      // Process.
      $this->importer->processor->process($this, $parser_result);

      // Import finished without exceptions, so unset any potentially previously
      // recorded exceptions.
      unset($this->exception);
    }
    catch (Exception $e) {
      // $e is stored and re-thrown once we've had a chance to log our progress.
      // Set the exception so that other modules can check if an exception
      // occurred in hook_feeds_after_import().
      $this->exception = $e;
    }

    // Clean up.
    $result = $this->progressImporting();
    if ($result == FEEDS_BATCH_COMPLETE || isset($e)) {
      $this->finishImport();
    }

    $this->save();
    $this->releaseLock();

    if (isset($e)) {
      throw $e;
    }

    return $result;
  }

  /**
   * Imports a fetcher result all at once in memory.
   *
   * @param FeedsFetcherResult $fetcher_result
   *   The fetcher result to process.
   *
   * @throws Exception
   *   Thrown if an error occurs when importing.
   */
  public function pushImport(FeedsFetcherResult $fetcher_result) {
    // Since locks only work during a request, check if an import is active.
    if (!empty($this->fetcher_result) || !empty($this->state)) {
      throw new RuntimeException('The feed is currently importing.');
    }

    $this->acquireLock();
    $this->state[FEEDS_START] = time();

    try {
      module_invoke_all('feeds_before_import', $this);

      // Parse.
      do {
        $parser_result = $this->importer->parser->parse($this, $fetcher_result);
        module_invoke_all('feeds_after_parse', $this, $parser_result);

        // Process.
        $this->importer->processor->process($this, $parser_result);

      } while ($this->progressParsing() !== FEEDS_BATCH_COMPLETE);
    }
    catch (Exception $e) {
      // $e is stored and re-thrown once we've had a chance to log our progress.
      // Set the exception so that other modules can check if an exception
      // occurred in hook_feeds_after_import().
      $this->exception = $e;
    }

    $this->finishImport();

    $this->save();
    $this->releaseLock();

    if (isset($e)) {
      throw $e;
    }
  }

  /**
   * Cleans up after an import.
   */
  protected function finishImport() {
    $this->imported = time();
    $this->log('import', 'Imported in @s seconds.', array('@s' => $this->imported - $this->state[FEEDS_START]), WATCHDOG_INFO);

    // Allow fetcher to react on finishing importing.
    $this->importer->fetcher->afterImport($this);

    // Allow other modules to react upon finishing importing.
    module_invoke_all('feeds_after_import', $this);
    if (module_exists('rules')) {
      rules_invoke_event('feeds_after_import', $this);
    }

    $this->clearStates();
  }

  /**
   * Remove all items from a feed.
   *
   * This method only executes the current batch chunk, then returns. If you are
   * looking to delete all items of a source, use FeedsSource::startClear()
   * instead.
   *
   * @return float
   *   FEEDS_BATCH_COMPLETE if the clearing process finished. A decimal between
   *   0.0 and 0.9 periodic if clearing is still in progress.
   *
   * @throws Exception
   *   In case an error occurs when clearing.
   */
  public function clear() {
    $this->acquireLock();
    try {
      $this->importer->fetcher->clear($this);
      $this->importer->parser->clear($this);
      $this->importer->processor->clear($this);
    }
    catch (Exception $e) {
      // $e is stored and re-thrown once we've had a chance to log our progress.
    }
    $this->releaseLock();

    // Clean up.
    $result = $this->progressClearing();
    if ($result == FEEDS_BATCH_COMPLETE || isset($e)) {
      module_invoke_all('feeds_after_clear', $this);
      $this->clearStates();
    }
    $this->save();
    if (isset($e)) {
      throw $e;
    }
    return $result;
  }

  /**
   * Removes all expired items from a feed.
   */
  public function expire() {
    $this->acquireLock();
    try {
      $result = $this->importer->processor->expire($this);
    }
    catch (Exception $e) {
      // Will throw after the lock is released.
    }
    $this->releaseLock();

    if (isset($e)) {
      throw $e;
    }

    return $result;
  }

  /**
   * Report progress as float between 0 and 1. 1 = FEEDS_BATCH_COMPLETE.
   */
  public function progressParsing() {
    return $this->state(FEEDS_PARSE)->progress;
  }

  /**
   * Report progress as float between 0 and 1. 1 = FEEDS_BATCH_COMPLETE.
   */
  public function progressImporting() {
    $fetcher = $this->state(FEEDS_FETCH);
    $parser = $this->state(FEEDS_PARSE);
    if ($fetcher->progress == FEEDS_BATCH_COMPLETE && $parser->progress == FEEDS_BATCH_COMPLETE) {
      return FEEDS_BATCH_COMPLETE;
    }
    // Fetching envelops parsing.
    // @todo: this assumes all fetchers neatly use total. May not be the case.
    $fetcher_fraction = $fetcher->total ? 1.0 / $fetcher->total : 1.0;
    $parser_progress = $parser->progress * $fetcher_fraction;
    $result = $fetcher->progress - $fetcher_fraction + $parser_progress;
    if ($result == FEEDS_BATCH_COMPLETE) {
      return 0.99;
    }
    return $result;
  }

  /**
   * Report progress on clearing.
   */
  public function progressClearing() {
    return $this->state(FEEDS_PROCESS_CLEAR)->progress;
  }

  /**
   * Report progress on expiry.
   */
  public function progressExpiring() {
    return $this->state(FEEDS_PROCESS_EXPIRE)->progress;
  }

  /**
   * Return a state object for a given stage. Lazy instantiates new states.
   *
   * @param string $stage
   *   One of FEEDS_FETCH, FEEDS_PARSE, FEEDS_PROCESS or FEEDS_PROCESS_CLEAR.
   *
   * @return FeedsState|mixed
   *   The FeedsState object for the given stage.
   *   In theory, this could return something else, if $this->state has been
   *   polluted with e.g. integer timestamps.
   *
   * @see FeedsSource::$state
   */
  public function state($stage) {
    if (!is_array($this->state)) {
      $this->state = array();
    }
    if (!isset($this->state[$stage])) {
      $this->state[$stage] = new FeedsState();
    }
    return $this->state[$stage];
  }

  /**
   * Clears states.
   */
  protected function clearStates() {
    $this->state = array();
    $this->fetcher_result = NULL;
  }

  /**
   * Count items imported by this source.
   */
  public function itemCount() {
    return $this->importer->processor->itemCount($this);
  }

  /**
   * Returns the next time that the feed will be imported.
   *
   * @return int|null
   *   The next time the feed will be imported as a UNIX timestamp.
   *   NULL if not known.
   */
  public function getNextImportTime() {
    $details = $this->getNextImportTimeDetails();
    if (isset($details['time'])) {
      return $details['time'];
    }
  }

  /**
   * Returns the next time that the feed will be imported.
   *
   * @param array $methods
   *   (optional) Methods to check.
   *
   * @return array|null
   *   Information about when the next time the feed will be imported:
   *   - time: the next time the feed will be imported as a UNIX timestamp.
   *   - method: via which scheduler the job will ran.
   *   - message: If set, time and method should be ignored.
   *   Null if no information is available.
   */
  public function getNextImportTimeDetails(array $methods = array()) {
    if (empty($methods)) {
      $methods = array(
        'queue',
        'feeds_reschedule',
        'job_scheduler',
      );
    }

    if (in_array('queue', $methods)) {
      // Check queue.
      $serialized_job_type = db_like(strtr('s:4:"type";s:!length:"!type";', array(
        '!length' => strlen($this->id),
        '!type' => $this->id,
      )));
      $serialized_job_id_as_string = db_like(strtr('s:2:"id";s:!length:"!id";', array(
        '!length' => strlen($this->feed_nid),
        '!id' => $this->feed_nid,
      )));
      $serialized_job_id_as_integer = db_like(strtr('s:2:"id";i:!id;', array(
        '!id' => $this->feed_nid,
      )));

      $queue_created = db_select('queue')
        ->fields('queue', array('created'))
        ->condition('name', 'feeds_source_import')
        ->condition('data', '%' . $serialized_job_type . '%', 'LIKE')
        ->condition(db_or()
          ->condition('data', '%' . $serialized_job_id_as_string . '%', 'LIKE')
          ->condition('data', '%' . $serialized_job_id_as_integer . '%', 'LIKE')
        )
        ->condition('expire', 0)
        ->execute()
        ->fetchField();

      if ($queue_created) {
        return array(
          'time' => $queue_created,
          'method' => t('Queue'),
        );
      }

      // Special case for PostgreSQL: if using that database type, we cannot
      // search in the data column of the queue table, because the Drupal
      // database layer adds '::text' to bytea columns, which results into the
      // data column becoming unreadable in conditions. So instead, we check for
      // the first 10 records in the queue to see if the given importer ID +
      // feed NID is amongst them.
      if (Database::getConnection()->databaseType() == 'pgsql') {
        $items = db_query("SELECT data, created FROM {queue} WHERE name = :name AND expire = 0 LIMIT 10", array(
          ':name' => 'feeds_source_import',
        ));
        foreach ($items as $item) {
          if (is_string($item->data)) {
            $item->data = unserialize($item->data);
          }
          if ($item->data['type'] == $this->id && $item->data['id'] == $this->feed_nid) {
            return array(
              'time' => $item->created,
              'method' => t('Queue'),
            );
          }
        }

        // If not found by now, count how many items there are in the
        // feeds_source_import queue. We use this number later to indicate that
        // the job *could* be in the queue.
        $number_of_queue_items = db_query('SELECT COUNT(name) FROM {queue} WHERE name = :name AND expire = 0', array(
          ':name' => 'feeds_source_import',
        ))->fetchField();
      }
    }

    if (in_array('feeds_reschedule', $methods)) {
      if (!$this->doesExist()) {
        if ($this->importer->config['import_period'] == FEEDS_SCHEDULE_NEVER) {
          // Just not scheduled.
          return NULL;
        }
        // Scheduling information cannot exist yet.
        return array(
          'time' => NULL,
          'method' => NULL,
          'message' => t('not scheduled yet, because there is no source'),
        );
      }

      // Check if the importer is in the process of being rescheduled.
      $importers = feeds_reschedule();
      if (isset($importers[$this->id])) {
        return array(
          'time' => NULL,
          'method' => NULL,
          'message' => t('to be rescheduled'),
        );
      }
    }

    if (in_array('job_scheduler', $methods)) {
      // Check job scheduler.
      $job = db_select('job_schedule')
        ->fields('job_schedule', array('next', 'scheduled'))
        ->condition('name', 'feeds_source_import')
        ->condition('type', $this->id)
        ->condition('id', $this->feed_nid)
        ->execute()
        ->fetch();

      if (isset($job->next)) {
        $details = array(
          'time' => $job->next,
          'method' => t('Job scheduler'),
        );
        if (!empty($job->scheduled)) {
          if (isset($number_of_queue_items) && $number_of_queue_items > 10) {
            // When using PostgreSQL we were not able to efficiently search the
            // queue table, so it could still be in that table.
            $details['message'] = t('unknown, could still be in the queue');
          }
          else {
            $details['message'] = t('possibly stuck');
          }
        }
        return $details;
      }
    }
  }

  /**
   * Checks if a source is queued for import.
   *
   * @return bool
   *   True if the source is queued to be imported.
   *   False otherwise.
   */
  public function isQueued() {
    $details = $this->getNextImportTimeDetails(array('queue'));
    if ($details) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Unlocks a feed.
   */
  public function unlock() {
    $this->clearStates();
    $this->save();
    try {
      $this->releaseLock();
    }
    catch (FeedsAccountSwitcherException $exception) {
      // Ignore switch back exceptions.
    }
  }

  /**
   * Save configuration.
   */
  public function save() {
    // Alert implementers of FeedsSourceInterface to the fact that we're saving.
    foreach ($this->importer->plugin_types as $type) {
      $this->importer->$type->sourceSave($this);
    }
    $config = $this->getConfig();

    // Store the source property of the fetcher in a separate column so that we
    // can do fast lookups on it.
    $source = '';
    if (isset($config[get_class($this->importer->fetcher)]['source'])) {
      $source = $config[get_class($this->importer->fetcher)]['source'];
    }
    $object = array(
      'id' => $this->id,
      'feed_nid' => $this->feed_nid,
      'imported' => $this->imported,
      'config' => $config,
      'source' => $source,
      'state' => isset($this->state) ? $this->state : FALSE,
      'fetcher_result' => isset($this->fetcher_result) ? $this->fetcher_result : FALSE,
    );
    if (db_query_range("SELECT 1 FROM {feeds_source} WHERE id = :id AND feed_nid = :nid", 0, 1, array(':id' => $this->id, ':nid' => $this->feed_nid))->fetchField()) {
      drupal_write_record('feeds_source', $object, array('id', 'feed_nid'));
    }
    else {
      drupal_write_record('feeds_source', $object);
    }
  }

  /**
   * Load configuration and unpack.
   *
   * @todo Patch CTools to move constants from export.inc to ctools.module.
   */
  public function load() {
    $record = db_query("SELECT imported, config, state, fetcher_result FROM {feeds_source} WHERE id = :id AND feed_nid = :nid", array(
      ':id' => $this->id,
      ':nid' => $this->feed_nid,
    ))->fetchObject();
    if ($record) {
      // While FeedsSource cannot be exported, we still use CTool's export.inc
      // export definitions.
      ctools_include('export');
      $this->export_type = EXPORT_IN_DATABASE;
      $this->imported = $record->imported;
      $this->config = unserialize($record->config);
      if (!empty($record->state)) {
        $this->state = unserialize($record->state);
      }
      if (!is_array($this->state)) {
        $this->state = array();
      }
      if (!empty($record->fetcher_result)) {
        $this->fetcher_result = unserialize($record->fetcher_result);
      }
    }
  }

  /**
   * Removes the feed source from the database.
   */
  public function delete() {
    // Alert implementers of FeedsSourceInterface to the fact that we're
    // deleting.
    foreach ($this->importer->plugin_types as $type) {
      $this->importer->$type->sourceDelete($this);
    }
    db_delete('feeds_source')
      ->condition('id', $this->id)
      ->condition('feed_nid', $this->feed_nid)
      ->execute();
    // Remove from schedule.
    $job = array(
      'type' => $this->id,
      'id' => $this->feed_nid,
    );
    JobScheduler::get('feeds_source_import')->remove($job);
    JobScheduler::get('feeds_source_expire')->remove($job);
  }

  /**
   * Checks whether or not the source configuration is valid.
   *
   * @return bool
   *   True if it is valid.
   *   False otherwise.
   */
  public function hasValidConfiguration() {
    // If there is no feed nid given, there must be no content type specified.
    $standalone = empty($this->feed_nid) && empty($this->importer->config['content_type']);

    // If there is a feed nid given, there must be a content type specified.
    $attached = !empty($this->feed_nid) && !empty($this->importer->config['content_type']);

    if ($standalone || $attached) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Overrides FeedsConfigurable::doesExist().
   *
   * Checks the following:
   * - If the importer is persistent (= defined in code or DB).
   * - If the source is persistent (= defined in DB).
   */
  public function doesExist() {
    return $this->importer->doesExist() && parent::doesExist();
  }

  /**
   * Only return source if configuration is persistent and valid.
   *
   * @see FeedsConfigurable::existing()
   */
  public function existing() {
    // Ensure that the source configuration is valid.
    if (!$this->hasValidConfiguration()) {
      throw new FeedsNotExistingException(t('Source configuration not valid.'));
    }

    // Ensure that the importer is persistent (= defined in code or DB).
    $this->importer->existing();

    // Ensure that the source is persistent (= defined in DB).
    return parent::existing();
  }

  /**
   * Returns the configuration for a specific client class.
   *
   * @param FeedsSourceInterface $client
   *   An object that is an implementer of FeedsSourceInterface.
   *
   * @return array
   *   An array stored for $client.
   */
  public function getConfigFor(FeedsSourceInterface $client) {
    $class = get_class($client);
    return isset($this->config[$class]) ? $this->config[$class] : $client->sourceDefaults();
  }

  /**
   * Sets the configuration for a specific client class.
   *
   * @param FeedsSourceInterface $client
   *   An object that is an implementer of FeedsSourceInterface.
   * @param array $config
   *   The configuration for $client.
   */
  public function setConfigFor(FeedsSourceInterface $client, array $config) {
    $this->config[get_class($client)] = $config;
  }

  /**
   * Return defaults for feed configuration.
   *
   * @return array
   *   The default feed configuration, keyed per Feeds plugin.
   */
  public function configDefaults() {
    // Collect information from plugins.
    $defaults = array();
    foreach ($this->importer->plugin_types as $type) {
      if ($this->importer->$type->hasSourceConfig()) {
        $defaults[get_class($this->importer->$type)] = $this->importer->$type->sourceDefaults();
      }
    }
    return $defaults;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    // Collect information from plugins.
    $form = array();
    foreach ($this->importer->plugin_types as $type) {
      if ($this->importer->$type->hasSourceConfig()) {
        $class = get_class($this->importer->$type);
        $config = isset($this->config[$class]) ? $this->config[$class] : array();
        $form[$class] = $this->importer->$type->sourceForm($config);
        $form[$class]['#tree'] = TRUE;
      }
    }
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    foreach ($this->importer->plugin_types as $type) {
      $class = get_class($this->importer->$type);
      if (isset($values[$class]) && $this->importer->$type->hasSourceConfig()) {
        $this->importer->$type->sourceFormValidate($values[$class]);
      }
    }
  }

  /**
   * Writes to feeds log.
   */
  public function log($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE) {
    feeds_log($this->id, $this->feed_nid, $type, $message, $variables, $severity);
  }

  /**
   * Background job helper. Starts a background job using the Drupal queue.
   *
   * @param string $method
   *   Method to execute on importer; one of 'import' or 'clear'.
   *
   * @see FeedsSource::startImport()
   * @see FeedsSource::startClear()
   */
  protected function startBackgroundJob($method) {
    $job = array(
      'type' => $this->id,
      'id' => $this->feed_nid,
    );
    $queue = DrupalQueue::get('feeds_source_' . $method);
    $queue->createItem($job);

    switch ($method) {
      case 'import':
        $state = $this->state(FEEDS_PARSE);
        break;

      case 'clear':
        $state = $this->state(FEEDS_PROCESS_CLEAR);
        break;

      case 'expire':
        $state = $this->state(FEEDS_PROCESS_EXPIRE);
        break;
    }

    if (isset($state)) {
      $state->progress = 0;
      $this->save();
    }
  }

  /**
   * Batch API helper. Starts a Batch API job.
   *
   * @param string $title
   *   Title to show to user when executing batch.
   * @param string $method
   *   Method to execute on importer; one of 'import' or 'clear'.
   *
   * @see FeedsSource::startImport()
   * @see FeedsSource::startClear()
   * @see feeds_batch()
   */
  protected function startBatchAPIJob($title, $method) {
    $batch = array(
      'title' => $title,
      'operations' => array(
        array('feeds_batch', array($method, $this->id, $this->feed_nid)),
      ),
      'progress_message' => '',
    );
    batch_set($batch);
  }

  /**
   * Acquires a lock for this source.
   *
   * @throws FeedsLockException
   *   If a lock for the requested job could not be acquired.
   */
  protected function acquireLock() {
    if (!lock_acquire("feeds_source_{$this->id}_{$this->feed_nid}", 60.0)) {
      throw new FeedsLockException(t('Cannot acquire lock for source @id / @feed_nid.', array('@id' => $this->id, '@feed_nid' => $this->feed_nid)));
    }

    // Switch account.
    $this->switchAccount();
  }

  /**
   * Releases a lock for this source.
   */
  protected function releaseLock() {
    lock_release("feeds_source_{$this->id}_{$this->feed_nid}");

    // Switch back to original account.
    $this->switchBack();
  }

  /**
   * Switches account to the feed owner or user 1.
   *
   * To the feed owner is switched if the importer is attached to a content
   * type. When using the standalone form, there is no feed owner, so then a
   * switch to user 1 happens instead.
   */
  protected function switchAccount() {
    // Use author of feed node.
    if ($this->feed_nid) {
      $node = node_load($this->feed_nid);
      if (!empty($node->uid)) {
        $account = user_load($node->uid);
      }
    }

    // If the owner of the feed node is anonymous or if the importer is not
    // attached to a content type, pick user 1 instead.
    if (empty($account)) {
      $account = user_load(1);
    }

    $this->accountSwitcher->switchTo($account);
  }

  /**
   * Switches back to the original user.
   */
  protected function switchBack() {
    $this->accountSwitcher->switchBack();
  }

  /**
   * Implements FeedsConfigurable::dependencies().
   */
  public function dependencies() {
    $dependencies = parent::dependencies();
    return array_merge($dependencies, $this->importer()->dependencies());
  }

}
