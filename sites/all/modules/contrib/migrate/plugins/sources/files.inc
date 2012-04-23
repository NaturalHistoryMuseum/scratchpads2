<?php

/**
 * @file
 * Support for migration from files sources.
 */

/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from a directory listing. Each item is a file, it's ID is the path.
 */
class MigrateListFiles extends MigrateList {

  protected $listDirs;
  protected $baseDir;
  protected $fileMask;
  protected $directoryOptions;

  /**
   * Constructor.
   *
   * @param $list_dirs
   *   Array of directory paths that will be scanned for files. No trailing
   *   slash. For example:
   *     array(
   *       '/var/html_source/en/news',
   *       '/var/html_source/fr/news',
   *       '/var/html_source/zh/news',
   *     );
   * @param $base_dir
   *   The base dir is the part of the path that will be excluded when making
   *   an ID for each file. To continue the example from above, you want base_dir
   *   to be = '/var/html_source', so that the files will have IDs in the format
   *   '/en/news/news_2011_03_4.html'.
   * @param $file_mask
   *   Passed on and used to filter for certain types of files. Use a regular
   *   expression, for example '/(.*\.htm$|.*\.html$)/i' to match all .htm and
   *   .html files, case insensitive.
   * @param $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   */
  public function __construct($list_dirs, $base_dir, $file_mask = NULL, $options = array()) {
    parent::__construct();
    $this->listDirs = $list_dirs;
    $this->baseDir = $base_dir;
    $this->fileMask = $file_mask;
    $this->directoryOptions = $options;
  }

  /**
   * Our public face is the directories we're getting items from.
   */
  public function __toString() {
    if (is_array($this->listDirs)) {
      return implode(',', $this->listDirs);
    }
    else {
      return $this->listDirs;
    }
  }

  /**
   * Retrieve a list of files based on parameters passed for the migration.
   */
  public function getIdList() {
    $files = array();
    foreach ($this->listDirs as $dir) {
      migrate_instrument_start("Retrieve $dir");
      $files = array_merge(file_scan_directory($dir, $this->fileMask, $this->directoryOptions), $files);
      migrate_instrument_stop("Retrieve $dir");
    }

    if (isset($files)) {
      return $this->getIDsFromFiles($files);
    }
    $migration = Migration::currentMigration();
    $migration->showMessage(t('Loading of !listuri failed:', array('!listuri' => $this->listUri)));
    return NULL;
  }

  /**
   * Given an array generated from file_scan_directory(), parse out the IDs for
   * processing and return them as an array.
   */
  protected function getIDsFromFiles(array $files) {
    $ids = array();
    foreach ($files as $file) {
      $ids[] = str_replace($this->baseDir, '', (string) $file->uri);
    }
    return array_unique($ids);
  }

  /**
   * Return a count of all available IDs from the source listing.
   */
  public function computeCount() {
    $count = 0;
    $files = $this->getIdList();
    if ($files) {
      $count = count($files);
    }
    return $count;
  }

}

/**
 * Implementation of MigrateItem, for retrieving a file from the file system
 * based on source directory and an ID provided by a MigrateList class.
 */
class MigrateItemFile extends MigrateItem {

  protected $baseDir;
  protected $getContents;

  /**
   * Constructor.
   *
   * @param $base_dir
   *   The base directory from which all file paths are calculated.
   * @param $get_contents
   *   TRUE if we should try load the contents of each file (in the case
   *   of a text file), or FALSE if we just want to confirm it exists (binary).
   */
  public function __construct($base_dir, $get_contents = TRUE) {
    parent::__construct();
    $this->baseDir = $base_dir;
    $this->getContents = $get_contents;
  }

  /**
   * Return an object representing a file.
   *
   * @param $id
   *   The file id, which is the file URI.
   *
   * @return object
   *   The item object for migration.
   */
  public function getItem($id) {
    $item_uri = $this->baseDir . $id;
    // Get the file data at the specified URI
    $data = $this->loadFile($item_uri);
    if (is_string($data)) {
      $return = new stdClass;
      $return->filedata = $data;
      return $return;
    }
    else if ($data === TRUE) {
      $return = new stdClass;
      return $return;
    }
    else {
      $migration = Migration::currentMigration();
      $message = t('Loading of !objecturi failed:', array('!objecturi' => $item_uri));
      $migration->getMap()->saveMessage(
              array($id), $message, MigrationBase::MESSAGE_ERROR);
      return NULL;
    }
  }

  /**
   * Default file loader.
   */
  protected function loadFile($item_uri) {
    // Only try load the contents if we have this flag set.
    if ($this->getContents) {
      $data = file_get_contents($item_uri);
    }
    else {
      $data = file_exists($item_uri);
    }
    return $data;
  }
}