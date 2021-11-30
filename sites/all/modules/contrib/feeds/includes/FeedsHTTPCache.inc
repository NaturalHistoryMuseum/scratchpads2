<?php

/**
 * @file
 * Contains FeedsHTTPCache class.
 */

/**
 * Cache implementation for the Feeds HTTP cache.
 */
class FeedsHTTPCache extends DrupalDatabaseCache {

  /**
   * Returns cache object.
   *
   * @param string $bin
   *   The cache bin.
   *
   * @return FeedsHTTPCache
   *   An instance of FeedsHTTPCache.
   */
  public static function getInstance($bin) {
    $cache_object = _cache_get_object($bin);
    if (!($cache_object instanceof self)) {
      // A different cache class could be used for the cache_feeds_http bin that
      // does not extend FeedsHTTPCache. In this case, just instantiate the
      // FeedsHTTPCache class.
      $cache_object = new self($bin);
    }
    return $cache_object;
  }

  /**
   * Returns cache dir.
   *
   * @return string
   *   The cache dir to use.
   */
  public function getCacheDir() {
    $dir = variable_get('feeds_http_file_cache_dir', NULL);
    if ($dir) {
      return $dir;
    }
    else {
      $schemes = file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE);
      $scheme = isset($schemes['private']) ? 'private' : 'public';
      return $scheme . '://feeds/cache';
    }
  }

  /**
   * Returns a list of file names in the cache directory.
   *
   * @return array
   *   A list of files.
   */
  public function getFileList() {
    $files = array();

    $file_cache_dir = $this->getCacheDir();
    if (is_dir($file_cache_dir)) {
      $dir = dir($file_cache_dir);
      while (($entry = $dir->read()) !== FALSE) {
        if ($entry == '.' || $entry == '..') {
          continue;
        }
        $files[] = $entry;
      }
      $dir->close();
    }

    return $files;
  }

  /**
   * Constructs a file path for a certain cache ID.
   *
   * @param string $cid
   *   The cache ID to construct a file path for.
   *
   * @return string
   *   The constructed file path.
   */
  public function constructFilePath($cid) {
    return $this->getCacheDir() . '/' . $cid;
  }

  /**
   * Saves raw contents to a file in the cache directory.
   *
   * @param string $cid
   *   The cache ID.
   * @param object $response
   *   The HTTP Response object.
   *
   * @return string|null
   *   The name of the file that was created or NULL if no file was created.
   *
   * @throws Exception
   *   In case the cache dir is not writable.
   */
  public function saveFile($cid, $response) {
    if (isset($response->data)) {
      $file_cache_dir = $this->getCacheDir();
      if (!file_prepare_directory($file_cache_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        // Cache directory is not writeable.
        if (user_access('administer feeds')) {
          $message = t("The feeds cache directory (@dir) either cannot be created or is not writable. You can change the cache directory by setting the '@variable' variable.", array(
            '@dir' => $file_cache_dir,
            '@variable' => 'feeds_http_file_cache_dir',
          ));
        }
        else {
          $message = t('The feeds cache directory either cannot be created or is not writable. Please contact your site administrator.');
        }
        throw new Exception($message);
      }
      $filename = $this->constructFilePath($cid);
      file_put_contents($filename, $response->data);

      return $filename;
    }
  }

  /**
   * Deletes a file from the cache directory.
   *
   * @param string $cid
   *   The file to delete.
   */
  protected function deleteFile($cid) {
    $filename = $this->constructFilePath($cid);
    if (is_file($filename)) {
      drupal_unlink($filename);
    }
  }

  /**
   * Deletes multiple files from the cache directory.
   *
   * @param array $cids
   *   The files to delete.
   */
  protected function deleteMultipleFiles(array $cids) {
    foreach ($cids as $cid) {
      $this->deleteFile($cid);
    }
  }

  /**
   * Deletes all files from the cache directory.
   */
  protected function deleteAllFiles() {
    $file_cache_dir = $this->getCacheDir();
    if (drupal_realpath($file_cache_dir) && file_exists($file_cache_dir)) {
      @file_unmanaged_delete_recursive($file_cache_dir);
    }
  }

  /**
   * Deletes files from the cache directory starting with a certain string.
   *
   * @param string $string
   *   The string with which the file name should start.
   */
  protected function deleteFilesStartingWith($string) {
    $mask = '/^' . preg_quote($string) . '/';
    $files_to_delete = file_scan_directory($this->getCacheDir(), $mask);
    foreach ($files_to_delete as $file) {
      file_unmanaged_delete($file->uri);
    }
  }

  /**
   * {@inheritdoc}
   *
   * Converts data to a FeedsHTTPCacheItem object, if needed.
   */
  protected function prepareItem($cache) {
    $cache = parent::prepareItem($cache);

    if (isset($cache->data) && is_object($cache->data) && !($cache->data instanceof FeedsHTTPCacheItem)) {
      // Convert to a FeedsHTTPCacheItem object.
      $cache->data = new FeedsHTTPCacheItem($cache->cid, $cache->data);
    }

    return $cache;
  }

  /**
   * {@inheritdoc}
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    if (empty($cid)) {
      // Clean up expired cached files.
      $cache_lifetime = variable_get('cache_lifetime', 0);

      // Check if expired cached files should be cleaned up now.
      if ($cache_lifetime) {
        $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
        $flush_expired = $cache_flush && REQUEST_TIME > ($cache_flush + $cache_lifetime);
      }
      else {
        $flush_expired = TRUE;
      }

      if ($flush_expired) {
        // Clear files for which the cache entries are expired.
        $result = db_select($this->bin)
          ->fields($this->bin, array('cid'))
          ->condition('expire', CACHE_PERMANENT, '<>')
          ->condition('expire', REQUEST_TIME, '<')
          ->execute()
          ->fetchAllAssoc('cid');
        $cids = array_keys($result);

        $this->deleteMultipleFiles($cids);
      }
    }
    else {
      if ($wildcard) {
        if ($cid == '*') {
          $this->deleteAllFiles();
        }
        else {
          $this->deleteFilesStartingWith($cid);
        }
      }
      elseif (is_array($cid)) {
        $this->deleteMultipleFiles($cid);
      }
      else {
        $this->deleteFile($cid);
      }
    }

    parent::clear($cid, $wildcard);
  }

  /**
   * {@inheritdoc}
   */
  public function set($cid, $item, $expire = CACHE_PERMANENT) {
    if ($item instanceof FeedsHTTPCacheItem) {
      // Given item already got rid of the raw data.
      $item->setCid($cid);
      return parent::set($cid, $item, $expire);
    }

    // Create a cache item to only cache what is needed.
    // The cache item will take care of saving the raw data to a file.
    $item = new FeedsHTTPCacheItem($cid, $item);

    parent::set($cid, $item, $expire);
  }

  /**
   * {@inheritdoc}
   */
  public function isEmpty() {
    // Check database first.
    if (!parent::isEmpty()) {
      return FALSE;
    }

    // Check if the cache directory is empty.
    $file_cache_dir = $this->getCacheDir();

    if (is_dir($file_cache_dir)) {
      $dir = dir($file_cache_dir);
      while (($entry = $dir->read()) !== FALSE) {
        if ($entry == '.' || $entry == '..') {
          continue;
        }
        // At least one file is found, so the cache directory is not empty.
        $dir->close();
        return FALSE;
      }
      // No files found. The cache directory is empty.
      $dir->close();
      return TRUE;
    }

    // No cache dir found.
    return TRUE;
  }

  /**
   * Queues all files to be synced with the cache list.
   */
  public function startSync() {
    // Get all files currently in the cache directory.
    $files = $this->getFileList();

    $queue = DrupalQueue::get('feeds_sync_cache_feeds_http');
    $queue->createItem(array(
      'files' => $files,
    ));
  }

  /**
   * Removes files for which an entry no longer appears in the cache.
   *
   * @param array $files
   *   The files to check.
   */
  public function sync(array $files) {
    if (count($files) > 50) {
      // There are more than 50 files in the cache directory to check. Take the
      // first 50 and put the rest on the queue.
      $queue = DrupalQueue::get('feeds_sync_cache_feeds_http');
      $queue->createItem(array(
        'files' => array_slice($files, 50),
      ));

      // Pick the first 50.
      $files = array_slice($files, 0, 50);
    }

    // Try to load cache entries for each file. Since the file names are passed
    // by reference and the file names for which a cache entry is found are
    // removed from the array, we end up with a list of file names for which NO
    // cache entry is found.
    cache_get_multiple($files, $this->bin);

    // The files for which no cache entry is found, can be removed.
    $this->deleteMultipleFiles($files);
  }

}
