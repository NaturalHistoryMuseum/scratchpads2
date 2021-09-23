<?php

/**
 * @file
 * Implementation of cache.inc with memcache logic included.
 */

require_once dirname(__FILE__) . '/dmemcache.inc';

/**
 * Defines the period after which wildcard clears are not considered valid.
 */
define('MEMCACHE_WILDCARD_INVALIDATE', 86400 * 28);
define('MEMCACHE_CONTENT_CLEAR', 'MEMCACHE_CONTENT_CLEAR');

/**
 * Implementation of cache.inc with memcache logic included
 */
class MemCacheDrupal implements DrupalCacheInterface {
  protected $memcache;

  /**
   * Constructs a MemCacheDrupal object.
   *
   * @param string $bin
   *   The cache bin for which the object is created.
   */
  public function __construct($bin) {
    $this->memcache = dmemcache_object($bin);
    $this->bin = $bin;

    // If page_cache_without_database is enabled, we have to manually load the
    // $conf array out of cache_bootstrap.
    static $variables_loaded = FALSE;
    // NOTE: We don't call drupal_get_bootstrap_phase() because this would
    // break all 7.x Drupal installations prior to 7.33. For more information
    // see https://www.drupal.org/node/667098.
    if (drupal_bootstrap(NULL, FALSE) < DRUPAL_BOOTSTRAP_VARIABLES && !$variables_loaded) {
      global $conf;
      $variables_loaded = TRUE;
      // Try loading variables from cache. If that fails, we have to bootstrap
      // further in order to fetch them.
      if ($cached = cache_get('variables', 'cache_bootstrap')) {
        $variables = $cached->data;
        // Make sure variable overrides are applied, see variable_initialize().
        foreach ($conf as $name => $value) {
          $variables[$name] = $value;
        }
        $conf = $variables;
      }
      else {
        drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
      }
    }

    $this->reloadVariables();
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  public function get($cid) {
    $cache = dmemcache_get($cid, $this->bin, $this->memcache);
    return $this->valid($cid, $cache) ? $cache : FALSE;
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
    $results = dmemcache_get_multi($cids, $this->bin, $this->memcache);
    foreach ($results as $cid => $result) {
      if (!$this->valid($cid, $result)) {
        // This object has expired, so don't return it.
        unset($results[$cid]);
      }
    }
    // Remove items from the referenced $cids array that we are returning,
    // per the comment in cache_get_multiple() in includes/cache.inc.
    $cids = array_diff($cids, array_keys($results));
    return $results;
  }

  /**
   * Checks if a retrieved cache item is valid.
   *
   * @param string $cid
   *   The cache id of the item
   * @param mixed $cache
   *   The cache item.
   *
   * @return bool
   *   Whether the item is valid.
   */
  protected function valid($cid, $cache) {
    if ($cache) {
      $cache_tables = isset($_SESSION['cache_flush']) ? $_SESSION['cache_flush'] : NULL;
      // Items that have expired are invalid.
      if (isset($cache->expire) && $cache->expire !== CACHE_PERMANENT && $cache->expire <= $_SERVER['REQUEST_TIME']) {
        // If the memcache_stampede_protection variable is set, allow one
        // process to rebuild the cache entry while serving expired content to
        // the rest. Note that core happily returns expired cache items as valid
        // and relies on cron to expire them, but this is mostly reliant on its
        // use of CACHE_TEMPORARY which does not map well to memcache.
        // @see http://drupal.org/node/534092
        if (variable_get('memcache_stampede_protection', FALSE)) {
          // The process that acquires the lock will get a cache miss, all
          // others will get a cache hit.
          if ($this->lockInit() && $this->stampedeProtected($cid) && lock_acquire("memcache_$cid:$this->bin", variable_get('memcache_stampede_semaphore', 15))) {
            $cache = FALSE;
          }
        }
        else {
          $cache = FALSE;
        }
      }
      // Items created before the last full wildcard flush against this bin are
      // invalid.
      elseif (!isset($cache->created) || $cache->created <= $this->cache_flush) {
        $cache = FALSE;
      }
      // Items created before the last content flush on this bin i.e.
      // cache_clear_all() are invalid.
      elseif ($cache->expire != CACHE_PERMANENT && $cache->created + $this->cache_lifetime <= $this->cache_content_flush) {
        $cache = FALSE;
      }
      // Items cached before the cache was last flushed by the current user are
      // invalid.
      elseif ($cache->expire != CACHE_PERMANENT && is_array($cache_tables) && isset($cache_tables[$this->bin]) && $cache_tables[$this->bin] >= $cache->created) {
        // Cache item expired, return FALSE.
        $cache = FALSE;
      }
      // Temporary items created before the cache was last flushed by
      // cache_clear_all(NULL, $bin) are invalid.
      elseif (!empty($cache->temporary) && $cache->created + $this->cache_lifetime <= $this->cache_temporary_flush) {
        // CACHE_TEMPORARY item expired, return FALSE.
        $cache = FALSE;
      }
      // Finally, check for wildcard clears against this cid.
      else {
        if (!$this->wildcardValid($cid, $cache)) {
          $cache = FALSE;
        }
      }
    }

    // On cache misses, attempt to avoid stampedes when the
    // memcache_stampede_protection variable is enabled.
    if (!$cache) {
      if (variable_get('memcache_stampede_protection', FALSE) && $this->lockInit() && $this->stampedeProtected($cid) && !lock_acquire("memcache_$cid:$this->bin", variable_get('memcache_stampede_semaphore', 15))) {
        // Prevent any single request from waiting more than three times due to
        // stampede protection. By default this is a maximum total wait of 15
        // seconds. This accounts for two possibilities - a cache and lock miss
        // more than once for the same item. Or a cache and lock miss for
        // different items during the same request.
        // @todo: it would be better to base this on time waited rather than
        // number of waits, but the lock API does not currently provide this
        // information. Currently the limit will kick in for three waits of 25ms
        // or three waits of 5000ms.
        static $lock_count = 0;
        $lock_count++;
        if ($lock_count <= variable_get('memcache_stampede_wait_limit', 3)) {
          // The memcache_stampede_semaphore variable was used in previous
          // releases of memcache, but the max_wait variable was not, so by
          // default divide the semaphore value by 3 (5 seconds).
          lock_wait("memcache_$cid:$this->bin", variable_get('memcache_stampede_wait_time', 5));
          $cache = $this->get($cid);
        }
      }
    }

    return (bool) $cache;
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $created_microtime = round(microtime(TRUE), 3);

    // Create new cache object.
    $cache = new stdClass();
    $cache->cid = $cid;
    $cache->data = is_object($data) ? clone $data : $data;
    $cache->created = REQUEST_TIME;
    $cache->created_microtime = $created_microtime;
    // Record the previous number of wildcard flushes affecting our cid.
    $cache->flushes = $this->wildcardFlushes($cid);
    if ($expire == CACHE_TEMPORARY) {
      // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
      // until the next flush.
      $cache->expire = REQUEST_TIME + 2591999;
      // This is a temporary cache item.
      $cache->temporary = TRUE;
    }
    // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
    elseif ($expire != CACHE_PERMANENT && $expire < 2592000) {
      // Expire is expressed in seconds, convert to the proper future timestamp
      // as expected in dmemcache_get().
      $cache->expire = REQUEST_TIME + $expire;
      $cache->temporary = FALSE;
    }
    else {
      $cache->expire = $expire;
      $cache->temporary = FALSE;
    }

    // Manually track the expire time in $cache->expire.  When the object
    // expires, if stampede protection is enabled, it may be served while one
    // process rebuilds it. The ttl sent to memcache is set to the expire twice
    // as long into the future, this allows old items to be expired by memcache
    // rather than evicted along with a sufficient period for stampede
    // protection to continue to work.
    if ($cache->expire == CACHE_PERMANENT) {
      $memcache_expire = $cache->expire;
    }
    else {
      $memcache_expire = $cache->expire + (($cache->expire - REQUEST_TIME) * 2);
    }
    dmemcache_set($cid, $cache, $memcache_expire, $this->bin, $this->memcache);

    // Release lock if acquired in $this->valid().
    $lock = "memcache_$cid:$this->bin";
    if (variable_get('memcache_stampede_protection', FALSE) && isset($GLOBALS['locks'][$lock])) {
      lock_release("$lock");
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    if ($this->memcache === FALSE) {
      // No memcache connection.
      return;
    }

    // It is not possible to detect a cache_clear_all() call other than looking
    // at the backtrace unless http://drupal.org/node/81461 is added.
    $backtrace = debug_backtrace();
    if ($cid == MEMCACHE_CONTENT_CLEAR || (isset($backtrace[2]) && $backtrace[2]['function'] == 'cache_clear_all' && empty($backtrace[2]['args']))) {
      // Update the timestamp of the last global flushing of this bin.  When
      // retrieving data from this bin, we will compare the cache creation
      // time minus the cache_flush time to the cache_lifetime to determine
      // whether or not the cached item is still valid.
      $this->cache_content_flush = time();
      $this->variable_set('cache_content_flush_' . $this->bin, $this->cache_content_flush);
      if (variable_get('cache_lifetime', 0)) {
        // We store the time in the current user's session. We then simulate
        // that the cache was flushed for this user by not returning cached
        // data to this user that was cached before the timestamp.
        if (isset($_SESSION['cache_flush']) && is_array($_SESSION['cache_flush'])) {
          $cache_bins = $_SESSION['cache_flush'];
        }
        else {
          $cache_bins = array();
        }
        // Use time() rather than request time here for correctness.
        $cache_tables[$this->bin] = $this->cache_content_flush;
        $_SESSION['cache_flush'] = $cache_tables;
      }
    }
    if (empty($cid) || $wildcard === TRUE) {
      // system_cron() flushes all cache bins returned by hook_flush_caches()
      // with cache_clear_all(NULL, $bin); The expected behaviour in this case
      // is to perform garbage collection on expired cache items (which is
      // irrelevant to memcache) but also to remove all CACHE_TEMPORARY items.
      // @see https://api.drupal.org/api/drupal/includes!cache.inc/function/cache_clear_all/7
      if (!isset($cid)) {
        // Update the timestamp of the last CACHE_TEMPORARY clear. All
        // temporary cache items created before this will be invalidated.
        $this->cache_temporary_flush = time();
        $this->variable_set("cache_temporary_flush_$this->bin", $this->cache_temporary_flush);
        // Return early here as we do not want to register a wildcard flush.
        return;
      }
      elseif ($cid == '*') {
        $cid = '';
      }
      if (empty($cid)) {
        // Update the timestamp of the last global flushing of this bin.  When
        // retrieving data from this bin, we will compare the cache creation
        // time minus the cache_flush time to the cache_lifetime to determine
        // whether or not the cached item is still valid.
        $this->cache_flush = time();
        $this->variable_set("cache_flush_$this->bin", $this->cache_flush);
        $this->flushed = min($this->cache_flush, time() - $this->cache_lifetime);

        if ($this->cache_lifetime) {
          // We store the time in the current user's session which is saved into
          // the sessions table by sess_write().  We then simulate that the
          // cache was flushed for this user by not returning cached data to
          // this user that was cached before the timestamp.
          if (isset($_SESSION['cache_flush']) && is_array($_SESSION['cache_flush'])) {
            $cache_bins = $_SESSION['cache_flush'];
          }
          else {
            $cache_bins = array();
          }
          $cache_bins[$this->bin] = $this->cache_flush;
          $_SESSION['cache_flush'] = $cache_bins;
        }
      }
      else {
        // Register a wildcard flush for current cid.
        $this->wildcards($cid, TRUE);
      }
    }
    else {
      $cids = is_array($cid) ? $cid : array($cid);
      foreach ($cids as $cid) {
        dmemcache_delete($cid, $this->bin, $this->memcache);
      }
    }
  }

  /**
   * Sum of all matching wildcards.
   *
   * Checking any single cache item's flush value against this single-value sum
   * tells us whether or not a new wildcard flush has affected the cached item.
   *
   * @param string $cid
   *   The cache id to check.
   *
   * @return int
   *   Sum of all matching wildcards for the given cache id.
   */
  protected function wildcardFlushes($cid) {
    return array_sum($this->wildcards($cid));
  }

  /**
   * Retrieves all matching wildcards for the given cache id.
   *
   * Utilize multiget to retrieve all possible wildcard matches, storing
   * statically so multiple cache requests for the same item on the same page
   * load doesn't add overhead.
   */
  protected function wildcards($cid, $flush = FALSE) {
    static $wildcards = array();
    $matching = array();

    $length = strlen($cid);

    if (isset($this->wildcard_flushes[$this->bin]) && is_array($this->wildcard_flushes[$this->bin])) {
      // Wildcard flushes per table are keyed by a substring equal to the
      // shortest wildcard clear on the table so far. So if the shortest
      // wildcard was "links:foo:", and the cid we're checking for is
      // "links:bar:bar", then the key will be "links:bar:".
      $keys = array_keys($this->wildcard_flushes[$this->bin]);
      $wildcard_length = strlen(reset($keys));
      $wildcard_key = substr($cid, 0, $wildcard_length);

      // Determine which lookups we need to perform to determine whether or not
      // our cid was impacted by a wildcard flush.
      $lookup = array();

      // Find statically cached wildcards, and determine possibly matching
      // wildcards for this cid based on a history of the lengths of past
      // valid wildcard flushes in this bin.
      if (isset($this->wildcard_flushes[$this->bin][$wildcard_key])) {
        foreach ($this->wildcard_flushes[$this->bin][$wildcard_key] as $flush_length => $timestamp) {
          if ($length >= $flush_length && $timestamp >= ($_SERVER['REQUEST_TIME'] - $this->invalidate)) {
            $wildcard = '.wildcard-' . substr($cid, 0, $flush_length);
            if (isset($wildcards[$this->bin][$wildcard])) {
              $matching[$wildcard] = $wildcards[$this->bin][$wildcard];
            }
            else {
              $lookup[$wildcard] = $wildcard;
            }
          }
        }
      }

      // Do a multi-get to retrieve all possibly matching wildcard flushes.
      if (!empty($lookup)) {
        $values = dmemcache_get_multi($lookup, $this->bin, $this->memcache);
        if (is_array($values)) {
          // Prepare an array of matching wildcards.
          $matching = array_merge($matching, $values);
          // Store matches in the static cache.
          if (isset($wildcards[$this->bin])) {
            $wildcards[$this->bin] = array_merge($wildcards[$this->bin], $values);
          }
          else {
            $wildcards[$this->bin] = $values;
          }
          $lookup = array_diff_key($lookup, $values);
        }

        // Also store failed lookups in our static cache, so we don't have to
        // do repeat lookups on single page loads.
        foreach ($lookup as $key => $key) {
          $wildcards[$this->bin][$key] = 0;
        }
      }
    }

    if ($flush) {
      $key_length = $length;
      if (isset($this->wildcard_flushes[$this->bin])) {
        $keys = array_keys($this->wildcard_flushes[$this->bin]);
        $key_length = strlen(reset($keys));
      }
      $key = substr($cid, 0, $key_length);
      // Avoid too many calls to variable_set() by only recording a flush for
      // a fraction of the wildcard invalidation variable, per cid length.
      // Defaults to 28 / 4, or one week.
      if (!isset($this->wildcard_flushes[$this->bin][$key][$length]) || ($_SERVER['REQUEST_TIME'] - $this->wildcard_flushes[$this->bin][$key][$length] > $this->invalidate / 4)) {

        // If there are more than 50 different wildcard keys for this bin
        // shorten the key by one, this should reduce variability by
        // an order of magnitude and ensure we don't use too much memory.
        if (isset($this->wildcard_flushes[$this->bin]) && count($this->wildcard_flushes[$this->bin]) > 50) {
          $key = substr($cid, 0, $key_length - 1);
          $length = strlen($key);
        }

        // If this is the shortest key length so far, we need to remove all
        // other wildcards lengths recorded so far for this bin and start
        // again. This is equivalent to a full cache flush for this table, but
        // it ensures the minimum possible number of wildcards are requested
        // along with cache consistency.
        if ($length < $key_length) {
          $this->wildcard_flushes[$this->bin] = array();
          $this->variable_set("cache_flush_$this->bin", time());
          $this->cache_flush = time();
        }
        $key = substr($cid, 0, $key_length);
        $this->wildcard_flushes[$this->bin][$key][$length] = $_SERVER['REQUEST_TIME'];

        variable_set('memcache_wildcard_flushes', $this->wildcard_flushes);
      }
      $key = '.wildcard-' . $cid;
      if (isset($wildcards[$this->bin][$key])) {
        $wildcards[$this->bin][$key]++;
      }
      else {
        $wildcards[$this->bin][$key] = 1;
      }
      dmemcache_set($key, $wildcards[$this->bin][$key], 0, $this->bin);
    }
    return $matching;
  }

  /**
   * Check if a wildcard flush has invalidated the current cached copy.
   */
  protected function wildcardValid($cid, $cache) {
    // Previously cached content won't have ->flushes defined.  We could
    // force flush, but instead leave this up to the site admin.
    $flushes = isset($cache->flushes) ? (int) $cache->flushes : 0;
    if ($flushes < (int) $this->wildcardFlushes($cid)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    // We do not know so err on the safe side?
    return FALSE;
  }

  /**
   * Helper function to load locking framework if not already loaded.
   *
   * @return bool
   *   Whether the locking system was initialized successfully. This must always
   *   return TRUE or throw an exception.
   */
  public function lockInit() {
    // On a cache miss when page_cache_without_database is enabled, we can end
    // up here without the lock system being initialized. Bootstrap drupal far
    // enough to load the lock system.
    if (!function_exists('lock_acquire')) {
      drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
    }

    if (!function_exists('lock_acquire')) {
      // Bootstrap failed, log error.
      register_shutdown_function('watchdog', 'memcache', 'Bootstrap failed in lockInit(), lock_acquire() is not available. (phase:!phase)', array('!phase' => drupal_get_bootstrap_phase()), WATCHDOG_ERROR);
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Determines whether stampede protection is enabled for a given bin/cid.
   *
   * Memcache stampede protection is primarily designed to benefit the following
   * caching pattern: a miss on a cache_get for a specific cid is immediately
   * followed by a cache_set for that cid. In cases where this pattern is not
   * followed, stampede protection can be disabled to avoid long hanging locks.
   * For example, a cache miss in Drupal core's module_implements() won't
   * execute a cache_set until drupal_page_footer() calls
   * module_implements_write_cache() which can occur much later in page
   * generation.
   *
   * @param string $cid
   *   The cache id of the data to retrieve.
   *
   * @return bool
   *   Returns TRUE if stampede protection is enabled for that particular cache
   *   bin/cid, otherwise FALSE.
   */
  protected function stampedeProtected($cid) {
    $ignore_settings = variable_get('memcache_stampede_protection_ignore', array(
      // Disable stampede protection for specific cids in 'cache_bootstrap'.
      'cache_bootstrap' => array(
        // The module_implements cache is written after finishing the request.
        'module_implements',
        // Variables have their own lock protection.
        'variables',
        // Both schema and the theme_registry uses DrupalCacheArray, which sets
        // the cache entry with a class destructor.
        'schema:runtime:*',
        'theme_registry:runtime:*',
      ),
      // Disable stampede protection for cid prefix in 'cache'.
      'cache' => array(
        // I18n uses a class destructor to write the cache.
        'i18n:string:*',
      ),
      // Disable stampede protection for the contrib cache_rules bin as recent
      // versions of the rules module provides its own stampede protection.
      'cache_rules',
    ));

    // Support ignoring an entire bin.
    if (in_array($this->bin, $ignore_settings)) {
      return FALSE;
    }

    // Support ignoring by cids.
    if (isset($ignore_settings[$this->bin])) {
      // Support ignoring specific cids.
      if (in_array($cid, $ignore_settings[$this->bin])) {
        return FALSE;
      }
      // Support ignoring cids starting with a suffix.
      foreach ($ignore_settings[$this->bin] as $ignore) {
        $split = explode('*', $ignore);
        if (count($split) > 1 && strpos($cid, $split[0]) === 0) {
          return FALSE;
        }
      }
    }

    return TRUE;
  }

  /**
   * Helper function to reload variables.
   *
   * This is used by the tests to verify that the cache object used the correct
   * settings.
   */
  public function reloadVariables() {
    $this->wildcard_flushes = variable_get('memcache_wildcard_flushes', array());
    $this->invalidate = variable_get('memcache_wildcard_invalidate', MEMCACHE_WILDCARD_INVALIDATE);
    $this->cache_lifetime = variable_get('cache_lifetime', 0);
    $this->cache_flush = variable_get('cache_flush_' . $this->bin);
    $this->cache_content_flush = variable_get('cache_content_flush_' . $this->bin, 0);
    $this->cache_temporary_flush = variable_get('cache_temporary_flush_' . $this->bin, 0);
    $this->flushed = min($this->cache_flush, REQUEST_TIME - $this->cache_lifetime);
  }

  /**
   * Re-implementation of variable_set() that writes through instead of clearing.
   */
  public function variable_set($name, $value) {
    global $conf;

    // When lots of writes happen in a short period of time db_merge can throw
    // errors. This should only happen if another request has written the variable
    // first, so we catch the error to prevent a fatal error.
    try {
      db_merge('variable')
        ->key(array('name' => $name))
        ->fields(array('value' => serialize($value)))
        ->execute();
    }
    catch (Exception $e) {
      // We can safely ignore the error, since it's likely a cache flush timestamp
      // which should still be accurate.
    }

    // If the variables are cached, get a fresh copy, update with the new value
    // and set it again.
    if ($cached = cache_get('variables', 'cache_bootstrap')) {
      $variables = $cached->data;
      $variables[$name] = $value;
      cache_set('variables', $variables, 'cache_bootstrap');
    }
    // If the variables aren't cached, there's no need to do anything.
    $conf[$name] = $value;
  }
}
