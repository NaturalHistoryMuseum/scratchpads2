<?php

/**
 * @file
 * Contains the FeedsFetcher and related classes.
 */

/**
 * Base class for all fetcher results.
 */
class FeedsFetcherResult extends FeedsResult {
  /**
   * The raw fetched data.
   *
   * @var string
   */
  protected $raw;

  /**
   * The path to a file where the raw data is stored.
   *
   * @var string
   */
  protected $file_path;

  /**
   * Constructor.
   */
  public function __construct($raw) {
    $this->raw = $raw;
  }

  /**
   * Prevent saving the raw result when serializing object.
   */
  public function __sleep() {
    if (!empty($this->raw)) {
      // Save contents of raw to a file for later use.
      $this->saveRawToFile();
    }

    // Save anything but 'raw'.
    unset($this->raw);
    return array_keys(get_object_vars($this));
  }

  /**
   * Returns the raw content.
   *
   * @return string
   *   The raw content from the source as a string.
   *
   * @throws Exception
   *   Extending classes MAY throw an exception if a problem occurred.
   */
  public function getRaw() {
    if (empty($this->raw)) {
      // Return raw contents from file.
      return $this->getFileContents();
    }
    return $this->sanitizeRawOptimized($this->raw);
  }

  /**
   * Get a path to a temporary file containing the resource provided by the
   * fetcher.
   *
   * File will be deleted after DRUPAL_MAXIMUM_TEMP_FILE_AGE.
   *
   * @return string
   *   A path to a file containing the raw content as a source.
   *
   * @throws Exception
   *   If an unexpected problem occurred.
   */
  public function getFilePath() {
    if (empty($this->file_path)) {
      // No file exists yet. Save any raw data that we got.
      $this->saveRawToFile();
    }

    // Check if given file exists now.
    $this->checkFile();

    // Return file path.
    return $this->sanitizeFile($this->file_path);
  }

  /**
   * Returns directory for storing files that are in progress of import.
   *
   * @return string
   *   The cache dir to use.
   */
  public function getFeedsInProgressDir() {
    $dir = variable_get('feeds_in_progress_dir', NULL);
    if ($dir) {
      return $dir;
    }
    else {
      $schemes = file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE);
      $scheme = isset($schemes['private']) ? 'private' : 'public';
      return $scheme . '://feeds/in_progress';
    }
  }

  /**
   * Constructs file name for saving the raw data.
   */
  public function constructFilePath() {
    return $this->getFeedsInProgressDir() . '/' . get_class($this) . REQUEST_TIME;
  }

  /**
   * Returns if raw data exists.
   *
   * This checks if either $this->raw is set or if the raw data exists in a
   * file. This is better then calling just ::getRaw() as that would return a
   * copy of all raw data which may lead to memory issues if the data is very
   * large.
   *
   * @return bool
   *   True if the raw data exists.
   *   False otherwise.
   */
  public function rawExists() {
    return !empty($this->raw) || $this->fileExists();
  }

  /**
   * Returns if the file to parse exists.
   *
   * @return bool
   *   True if the file exists.
   *   False otherwise.
   */
  public function fileExists() {
    if (!empty($this->file_path) && is_readable($this->file_path)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Returns the contents of a file, if it exists.
   *
   * @return string
   *   The file contents.
   */
  public function getFileContents() {
    if ($this->fileExists()) {
      $this->sanitizeFile($this->file_path);
      return file_get_contents($this->file_path);
    }
  }

  /**
   * Checks that a file exists and is readable.
   *
   * @throws RuntimeException
   *   Thrown if the file isn't readable or writable.
   */
  protected function checkFile() {
    if (!file_exists($this->file_path)) {
      throw new RuntimeException(t('File %filepath does not exist.', array(
        '%filepath' => $this->file_path,
      )));
    }

    if (!is_readable($this->file_path)) {
      throw new RuntimeException(t('File %filepath is not readable.', array(
        '%filepath' => $this->file_path,
      )));
    }
  }

  /**
   * Saves the raw fetcher result to a file.
   *
   * @throws RuntimeException
   *   In case the destination wasn't writable.
   */
  public function saveRawToFile() {
    $file_in_progress_dir = $this->getFeedsInProgressDir();
    if (!file_prepare_directory($file_in_progress_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      throw new RuntimeException(t('Feeds directory either cannot be created or is not writable.'));
    }

    $this->file_path = FALSE;
    if ($file = file_save_data($this->getRaw(), $this->constructFilePath())) {
      $file->status = 0;
      file_save($file);
      $this->file_path = $file->uri;

      // Clear raw data to save memory, but also to prevent saving the same raw data
      // to a file again in the same request.
      $this->raw = NULL;
    }
    else {
      throw new RuntimeException(t('Cannot write content to %dest', array('%dest' => $destination)));
    }
  }

  /**
   * Sanitize the raw content string.
   *
   * Currently supported sanitizations:
   * - Remove BOM header from UTF-8 files.
   *
   * Consider using ::sanitizeRawOptimized() instead that receives the variable
   * by reference and thus saves memory.
   *
   * @param string $raw
   *   The raw content string to be sanitized.
   *
   * @return string
   *   The sanitized content as a string.
   */
  public function sanitizeRaw($raw) {
    if (substr($raw, 0, 3) == pack('CCC', 0xef, 0xbb, 0xbf)) {
      $raw = substr($raw, 3);
    }
    return $raw;
  }

  /**
   * Sanitize the raw content string.
   *
   * Currently supported sanitizations:
   * - Remove BOM header from UTF-8 files.
   *
   * This accepts the raw contents by reference to prevent having the whole raw
   * contents in memory again.
   *
   * @param string $raw
   *   The raw content string to be sanitized.
   *
   * @return string
   *   The sanitized content as a string.
   */
  public function sanitizeRawOptimized(&$raw) {
    if (substr($raw, 0, 3) == pack('CCC', 0xef, 0xbb, 0xbf)) {
      $raw = substr($raw, 3);
    }
    return $raw;
  }

  /**
   * Sanitize the file in place.
   *
   * Currently supported sanitizations:
   * - Remove BOM header from UTF-8 files.
   *
   * @param string $filepath
   *   The file path of the file to be sanitized.
   *
   * @return string
   *   The file path of the sanitized file.
   *
   * @throws RuntimeException
   *   Thrown if the file is not writeable.
   */
  public function sanitizeFile($filepath) {
    $handle = fopen($filepath, 'r');
    $line = fgets($handle);
    fclose($handle);

    // If BOM header is present, read entire contents of file and overwrite the
    // file with corrected contents.
    if (substr($line, 0, 3) !== pack('CCC', 0xef, 0xbb, 0xbf)) {
      return $filepath;
    }

    if (!is_writable($filepath)) {
      throw new RuntimeException(t('File %filepath is not writable.', array(
        '%filepath' => $filepath,
      )));
    }

    $contents = file_get_contents($filepath);
    $contents = substr($contents, 3);
    $status = file_put_contents($filepath, $contents);

    return $filepath;
  }

}

/**
 * Abstract class, defines shared functionality between fetchers.
 *
 * Implements FeedsSourceInfoInterface to expose source forms to Feeds.
 */
abstract class FeedsFetcher extends FeedsPlugin {

  /**
   * Implements FeedsPlugin::pluginType().
   */
  public function pluginType() {
    return 'fetcher';
  }

  /**
   * Fetch content from a source and return it.
   *
   * Every class that extends FeedsFetcher must implement this method.
   *
   * @param $source
   *   Source value as entered by user through sourceForm().
   *
   * @return
   *   A FeedsFetcherResult object.
   */
  abstract public function fetch(FeedsSource $source);

  /**
   * Clear all caches for results for given source.
   *
   * @param FeedsSource $source
   *   Source information for this expiry. Implementers can choose to only clear
   *   caches pertaining to this source.
   */
  public function clear(FeedsSource $source) {}

  /**
   * Request handler invoked if callback URL is requested. Locked down by
   * default. For a example usage see FeedsHTTPFetcher.
   *
   * Note: this method may exit the script.
   *
   * @return
   *   A string to be returned to the client.
   */
  public function request($feed_nid = 0) {
    drupal_access_denied();
  }

  /**
   * Construct a path for a concrete fetcher/source combination. The result of
   * this method matches up with the general path definition in
   * FeedsFetcher::menuItem(). For example usage look at FeedsHTTPFetcher.
   *
   * @return
   *   Path for this fetcher/source combination.
   */
  public function path($feed_nid = 0) {
    $id = urlencode($this->id);
    if ($feed_nid && is_numeric($feed_nid)) {
      return "feeds/importer/$id/$feed_nid";
    }
    return "feeds/importer/$id";
  }

  /**
   * Menu item definition for fetchers of this class. Note how the path
   * component in the item definition matches the return value of
   * FeedsFetcher::path().
   *
   * Requests to this menu item will be routed to FeedsFetcher::request().
   *
   * @return
   *   An array where the key is the Drupal menu item path and the value is
   *   a valid Drupal menu item definition.
   */
  public function menuItem() {
    return array(
      'feeds/importer/%feeds_importer' => array(
        'page callback' => 'feeds_fetcher_callback',
        'page arguments' => array(2, 3),
        'access callback' => TRUE,
        'file' => 'feeds.pages.inc',
        'type' => MENU_CALLBACK,
      ),
    );
  }

  /**
   * Subscribe to a source. Only implement if fetcher requires subscription.
   *
   * @param FeedsSource $source
   *   Source information for this subscription.
   */
  public function subscribe(FeedsSource $source) {}

  /**
   * Unsubscribe from a source. Only implement if fetcher requires subscription.
   *
   * @param FeedsSource $source
   *   Source information for unsubscribing.
   */
  public function unsubscribe(FeedsSource $source) {}

  /**
   * Override import period settings. This can be used to force a certain import
   * interval.
   *
   * @param $source
   *   A FeedsSource object.
   *
   * @return
   *   A time span in seconds if periodic import should be overridden for given
   *   $source, NULL otherwise.
   */
  public function importPeriod(FeedsSource $source) {}

  /**
   * Invoked after an import is finished.
   *
   * @param $source
   *   A FeedsSource object.
   */
  public function afterImport(FeedsSource $source) {}

}
