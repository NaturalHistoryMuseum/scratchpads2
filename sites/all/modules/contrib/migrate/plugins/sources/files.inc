<?php

/**
 * @file
 * Support for migration from files sources.
 */

/**
 * When we migrate HTML files, the IDs of each item is the filename. If however
 * we are splitting up each HTML file into chunks that become separate nodes,
 * we create IDs that contain both the filename and the chunk ID, for example:
 *
 * /var/html/01.html-?MIGRATECHUNK?-5
 *
 * This would be chunk 5 of the file 01.html. Obviously this separator can
 * never legitimately be part of an HTML filename.
 */
define('MIGRATE_CHUNK_SEPARATOR', '-?MIGRATECHUNK?-');

/**
 * MigrateContentParser provides abstract methods that must be supported by an
 * implementing class. We need to be able to get a list of the chunk IDs, a
 * count, and then retrieve each chunk by it's ID.
 */
abstract class MigrateContentParser {

  /**
   * Tells the MigrateListFiles implementation to always use chunk IDs when
   * constructing item IDs.
   */
  public $alwaysUseChunkIDs = FALSE;

  /**
   * The content of the current file to parse.
   */
  protected $content;

  /**
   * The ID of the current file being parsed.
   */
  protected $item_id;

  /**
   * Set the content and file ID for the current file to parse.
   *
   * @param $content
   *   The content of the current file to parse.
   * @param $item_id = NULL
   *   The ID of the file. The chunk IDs are appended to this to form the IDs
   *   that are returned by the MigrateList implementation.
   */
  public function setContent($content, $item_id = NULL) {
    $this->content = $content;
    $this->item_id = $item_id;
  }

  /**
   * Return the list of IDs for all the chunks found in the current file.
   *
   * Unless $alwaysUseChunkIDs is set to TRUE, this is only called if
   * getChunkCount() returned a count greater than 1.
   *
   *
   * @return
   *   An array of IDs.
   */
  abstract public function getChunkIDs();

  /**
   * Get a single chunk from the current file.
   *
   * @param $id
   *   The ID for the chunk. This is one of the IDs returned by getChunkIDs().
   *
   * @return
   *   The content of the chunk.
   */
  abstract public function getChunk($id);

  /**
   * Return the count of chunks in the current file.
   *
   * If this returns 1, then getChunkIDs() is not called and the file is treated
   * as a single chunk. To override this, set $alwaysUseChunkIDs to TRUE.
   *
   * @return int
   *   The number of chunks in the current file.
   */
  abstract public function getChunkCount();

}

/**
 * Simple parser that doesn't actually parse anything - it just returns the
 * whole file as a single chunk. This is the default parser used and is
 * primarily for when your HTML files map one-to-one to nodes.
 */
class MigrateSimpleContentParser extends MigrateContentParser {

  public function getChunkIDs() {
    return array('1');
  }

  public function getChunk($id) {
    return $this->content;
  }

  public function getChunkCount() {
    return 1;
  }
}

/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from a directory listing. Each item is a file, it's ID is the path.
 *
 * If individual files contain more than one source, use a MigrateContentParser
 * to parse the file and split it into chunks.
 */
class MigrateListFiles extends MigrateList {

  protected $listDirs;

  protected $baseDir;

  protected $fileMask;

  protected $directoryOptions;

  protected $parser;

  protected $getContents;

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
   *   an ID for each file. To continue the example from above, you want
   *   base_dir to be = '/var/html_source', so that the files will have IDs in
   *   the format
   *   '/en/news/news_2011_03_4.html'.
   * @param $file_mask
   *   (optional) Passed on and used to filter for certain types of files. Use
   *   a
   *   regular expression, for example '/(.*\.htm$|.*\.html$)/i' to match all
   *   .htm and .html files, case insensitive. Defaults to matching all files.
   * @param $options
   *   Options that will be passed on to file_scan_directory(). See docs of
   *   that
   *   core Drupal function for more information.
   * @param MigrateContentParser $parser
   *   Content parser class to use.
   * @param $get_contents
   *   Whether to load the contents of files.
   */
  public function __construct($list_dirs, $base_dir, $file_mask = '//', $options = array(), MigrateContentParser $parser = NULL, $get_contents = TRUE) {
    if (!$parser) {
      $parser = new MigrateSimpleContentParser();
    }
    parent::__construct();
    $this->listDirs = $list_dirs;
    $this->baseDir = $base_dir;
    $this->fileMask = $file_mask;
    $this->directoryOptions = $options;
    $this->parser = $parser;
    $this->getContents = $get_contents;
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
    Migration::displayMessage(t('Loading of !listuri failed:', array('!listuri' => $this->listUri)));
    return NULL;
  }

  /**
   * Given an array generated from file_scan_directory(), parse out the IDs for
   * processing and return them as an array.
   */
  protected function getIDsFromFiles(array $files) {
    $ids = array();
    foreach ($files as $file) {
      $file_base_id = str_replace($this->baseDir, '', (string) $file->uri);
      if ($this->getContents) {
        $contents = file_get_contents($file->uri);
        $this->parser->setContent($contents, $file_base_id);
        if ($this->parser->alwaysUseChunkIDs || $this->parser->getChunkCount() > 1) {
          foreach ($this->parser->getChunkIDs() as $chunk_id) {
            $ids[] = $file_base_id . MIGRATE_CHUNK_SEPARATOR . $chunk_id;
          }
        }
        else {
          $ids[] = $file_base_id;
        }
      }
      else {
        $ids[] = $file_base_id;
      }

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

  protected $parser;

  /**
   * Constructor.
   *
   * @param $base_dir
   *   The base directory from which all file paths are calculated.
   * @param $get_contents
   *   TRUE if we should try load the contents of each file (in the case
   *   of a text file), or FALSE if we just want to confirm it exists (binary).
   */
  public function __construct($base_dir, $get_contents = TRUE, MigrateContentParser $parser = NULL) {
    parent::__construct();
    if (!$parser) {
      $parser = new MigrateSimpleContentParser();
    }
    $this->baseDir = $base_dir;
    $this->getContents = $get_contents;
    $this->parser = $parser;
  }

  /**
   * Return an object representing a file or piece thereof.
   *
   * @param $id
   *   The file id, which is the file URI.
   *
   * @return object
   *   The item object for migration.
   */
  public function getItem($id) {
    $pieces = explode(MIGRATE_CHUNK_SEPARATOR, $id);
    $item_uri = $this->baseDir . $pieces[0];
    $chunk = !empty($pieces[1]) ? $pieces[1] : '';

    // Get the file data at the specified URI
    $data = $this->loadFile($item_uri);
    if (is_string($data)) {
      $this->parser->setContent($data, $pieces[0]);
      $return = new stdClass;
      $return->filedata = $this->parser->getChunk($chunk);
      return $return;
    }
    else {
      if ($data === TRUE) {
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
