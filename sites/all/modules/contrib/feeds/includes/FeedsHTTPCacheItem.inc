<?php

/**
 * @file
 * Contains FeedsHTTPCacheItem class.
 */

/**
 * Class of a cached item.
 */
class FeedsHTTPCacheItem {

  /**
   * The cache bin to save the data to.
   *
   * @var string
   */
  const CACHE_BIN = 'cache_feeds_http';

  /**
   * The cache ID.
   *
   * @var string
   */
  protected $cid;

  /**
   * Headers of the response.
   *
   * @var array
   */
  protected $headers = array();

  /**
   * Path to the cache file.
   *
   * @var string
   */
  protected $file_path;

  /**
   * FeedsHTTPCacheItem object constructor.
   *
   * @param string $cid
   *   The cache ID.
   * @param object $response
   *   The HTTP response object.
   */
  public function __construct($cid, $response) {
    $this->setCid($cid);

    // Copy over other metadata from result, but not the raw data.
    // Value is assigned by reference to save memory.
    foreach ($response as $key => &$value) {
      switch ($key) {
        case 'headers':
          $this->headers = (array) $value;
          break;

        case 'file_path':
          $this->setFilePath($value);
          break;

        case 'data':
          // Data should not be cached in the database, so save data to a file
          // instead. The whole response object is passed to save memory usage.
          // The data could potentially be huge.
          $this->saveResponseData($response);
          break;

        default:
          $this->$key = $response->$key;
      }
    }
  }

  /**
   * Sets cache ID.
   *
   * @param string $cid
   *   The cache ID.
   */
  public function setCid($cid) {
    $this->cid = $cid;
  }

  /**
   * Sets file path.
   *
   * @param string $file_path
   *   The file path to set.
   */
  public function setFilePath($file_path) {
    $this->file_path = $file_path;
  }

  /**
   * Magic isset.
   */
  public function __isset($member) {
    if ($member == 'data') {
      // Data should always be cached to a file, so in that case we check if the
      // file exist where the data should be in.
      return (isset($this->file_path) && file_exists($this->file_path));
    }

    return isset($this->$member);
  }

  /**
   * Magic getter.
   */
  public function __get($member) {
    if ($member == 'data') {
      // Data is cached in a file, so when that member is requested, return the
      // file contents.
      return $this->getFileContents();
    }

    return $this->$member;
  }

  /**
   * Returns cache object.
   *
   * @return FeedsHTTPCache
   *   An instance of FeedsHTTPCache.
   */
  public function getCacheObject() {
    return FeedsHTTPCache::getInstance(self::CACHE_BIN);
  }

  /**
   * Gets the cached file from the file system.
   *
   * @return string|null
   *   The file's contents, if the file exists.
   *   NULL otherwise.
   */
  public function getFileContents() {
    if (file_exists($this->file_path)) {
      return file_get_contents($this->file_path);
    }
  }

  /**
   * Writes data to a file.
   *
   * @param object $response
   *   The HTTP Response object that contains the data.
   */
  public function saveResponseData($response) {
    if (isset($response->data)) {
      // Get cache object, this object knows where to save the file.
      $filename = $this->getCacheObject()->saveFile($this->cid, $response);
      $this->setFilePath($filename);
    }
  }

  /**
   * Saves item to cache.
   */
  public function cacheSet() {
    cache_set($this->cid, $this, self::CACHE_BIN);
  }

}
