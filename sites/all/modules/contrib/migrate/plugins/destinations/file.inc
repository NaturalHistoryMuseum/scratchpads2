<?php

/**
 * @file
 * Support for file entity as destination. Note that File Fields have their
 * own destination in fields.inc
 */


/**
 * Interface for taking some value representing a file and returning
 * a Drupal file entity (creating the entity if necessary).
 */
interface MigrateFileInterface {
  /**
   * Return a list of subfields and options specific to this implementation,
   * keyed by name.
   */
  public static function fields();

  /**
   * Create or link to a Drupal file entity.
   *
   * @param $value
   *  A class-specific value (URI, pre-existing file ID, file blob, ...)
   *  representing file content.
   *
   * @param $owner
   *  uid of an account to be recorded as the file owner.
   *
   * @return object
   *  File entity being created or referenced.
   */
  public function processFile($value, $owner);
}

/**
 * Handle the degenerate case where we already have a file ID.
 */
class MigrateFileFid implements MigrateFileInterface {
  /**
   * Implementation of MigrateFileInterface::fields().
   *
   * @return array
   */
  static public function fields() {
    return array();
  }

  /**
   * Implementation of MigrateFileInterface::processFile().
   *
   * @param $value
   *  An existing file entity ID (fid).
   * @param $owner
   *  User ID (uid) to be the owner of the file. Ignored in this case.
   * @return int
   *  The file entity corresponding to the fid that was passed in.
   */
  public function processFile($value, $owner) {
    return file_load($value);
  }
}

/**
 * Base class for creating core file entities.
 */
abstract class MigrateFile implements MigrateFileInterface {
  /**
   * Extension of the core FILE_EXISTS_* constants, offering an alternative to
   * reuse the existing file if present as-is (core only offers the options of
   * replacing it or renaming to avoid collision).
   */
  const FILE_EXISTS_REUSE = -1;

  /**
   * The destination directory within Drupal.
   *
   * @var string
   */
  protected $destinationDir = 'public://';

  /**
   * The filename relative to destinationDir to which to save the current file.
   *
   * @var string
   */
  protected $destinationFile = '';

  /**
   * How to handle destination filename collisions.
   *
   * @var int
   */
  protected $fileReplace = FILE_EXISTS_RENAME;

  /**
   * Set to TRUE to prevent file deletion on rollback.
   *
   * @var bool
   */
  protected $preserveFiles = FALSE;

  /**
   * An optional file object to use as a default starting point for building the
   * file entity.
   *
   * @var stdClass
   */
  protected $defaultFile;

  public function __construct($arguments = array(), $default_file = NULL) {
    if (isset($arguments['destination_dir'])) {
      $this->destinationDir = $arguments['destination_dir'];
    }
    if (isset($arguments['destination_file'])) {
      $this->destinationFile = $arguments['destination_file'];
    }
    if (isset($arguments['file_replace'])) {
      $this->fileReplace = $arguments['file_replace'];
    }
    if (isset($arguments['preserve_files'])) {
      $this->preserveFiles = $arguments['preserve_files'];
    }
    if ($default_file) {
      $this->defaultFile = $default_file;
    }
    else {
      $this->defaultFile = new stdClass;
    }
  }

  /**
   * Implementation of MigrateFileInterface::fields().
   *
   * @return array
   */
  static public function fields() {
    return array(
      'destination_dir' => t('Subfield: <a href="@doc">Path within Drupal files directory to store file</a>',
        array('@doc' => 'http://drupal.org/node/1540106#destination_dir')),
      'destination_file' => t('Subfield: <a href="@doc">Path within destination_dir to store the file.</a>',
        array('@doc' => 'http://drupal.org/node/1540106#destination_file')),
      'file_replace' => t('Option: <a href="@doc">Value of $replace in that file function. Does not apply to file_fast(). Defaults to FILE_EXISTS_RENAME.</a>',
        array('@doc' => 'http://drupal.org/node/1540106#file_replace')),
      'preserve_files' => t('Option: <a href="@doc">Boolean indicating whether files should be preserved or deleted on rollback</a>',
        array('@doc' => 'http://drupal.org/node/1540106#preserve_files')),
    );
  }

  /**
   * Setup a file entity object suitable for saving.
   *
   * @param $destination
   *  Path to the Drupal copy of the file.
   * @param $owner
   *  Uid of the file owner.
   * @return stdClass
   *  A file object ready to be saved.
   */
  protected function createFileEntity($destination, $owner) {
    $file = clone $this->defaultFile;
    $file->uri = $destination;
    $file->uid = $owner;
    if (!isset($file->filename)) {
      $file->filename = drupal_basename($destination);
    }
    if (!isset($file->filemime)) {
      $file->filemime = file_get_mimetype($destination);
    }
    if (!isset($file->status)) {
      $file->status = FILE_STATUS_PERMANENT;
    }
    // If we are replacing an existing file re-use its database record.
    if ($this->fileReplace == FILE_EXISTS_REPLACE) {
      $existing_files = file_load_multiple(array(), array('uri' => $destination));
      if (count($existing_files)) {
        $existing = reset($existing_files);
        $file->fid = $existing->fid;
        $file->filename = $existing->filename;
      }
    }
    return $file;
  }

  /**
   * By whatever appropriate means, put the file in the right place.
   *
   * @param $destination
   *  Destination path within Drupal.
   * @return bool
   *  TRUE if the file is successfully saved, FALSE otherwise.
   */
  abstract protected function copyFile($destination);

  /**
   * Default implementation of MigrateFileInterface::processFiles().
   *
   * @param $value
   *  The URI or local filespec of a file to be imported.
   * @param $owner
   *  User ID (uid) to be the owner of the file.
   * @return object
   *  The file entity being created or referenced.
   */
  public function processFile($value, $owner) {
    $migration = Migration::currentMigration();

    // Determine the final path we want in Drupal - start with our preferred path.
    $destination = file_stream_wrapper_uri_normalize(
      $this->destinationDir . '/' .
      ltrim($this->destinationFile, "/\\"));

    // Our own file_replace behavior - if the file exists, use it without
    // replacing it
    if ($this->fileReplace == self::FILE_EXISTS_REUSE) {
      // See if we this file already (we'll reuse a file entity if it exists).
      if (file_exists($destination)) {
        $file = $this->createFileEntity($destination, $owner);
        // File entity didn't already exist, create it
        if (empty($file->fid)) {
          $file = file_save($file);
        }
        return $file;
      }
      // No existing one to reuse, reset to REPLACE
      $this->fileReplace = FILE_EXISTS_REPLACE;
    }

    // Prepare the destination directory.
    if (!file_prepare_directory(drupal_dirname($destination),
                                FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      $migration->saveMessage(t('Could not create destination directory for !dest',
                              array('!dest' => $destination)));
      return FALSE;
    }

    // Determine whether we can perform this operation based on overwrite rules.
    $destination = file_destination($destination, $this->fileReplace);
    if ($destination === FALSE) {
      $migration->saveMessage(t('The file could not be copied because ' .
        'file %dest already exists in the destination directory.',
        array('%dest' => $destination)));
      return FALSE;
    }

    // Make sure the .htaccess files are present.
    file_ensure_htaccess();

    // Put the file where it needs to be.
    if (!$this->copyFile($destination)) {
      return FALSE;
    }

    // Set the permissions on the new file.
    drupal_chmod($destination);

    // Create and save the file entity.
    $file = file_save($this->createFileEntity($destination, $owner));

    // Prevent deletion of the file on rollback if requested.
    if (is_object($file)) {
      if (!empty($this->preserveFiles)) {
        // We do this directly instead of calling file_usage_add, to force the
        // count to 1 - otherwise, updates will increment the counter and the file
        // will never be deletable
        db_merge('file_usage')
          ->key(array(
            'fid' => $file->fid,
            'module' => 'migrate',
            'type' => 'file',
            'id' => $file->fid,
          ))
          ->fields(array('count' => 1))
          ->execute();
      }
      return $file;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Handle cases where we're handed a URI, or local filespec, representing a file
 * to be imported to Drupal.
 */
class MigrateFileUri extends MigrateFile {
  /**
   * The source directory for the file, relative to which the value (source
   * file) will be taken.
   *
   * @var string
   */
  protected $sourceDir = '';

  /**
   * The full path to the source file.
   *
   * @var string
   */
  protected $sourcePath = '';

  public function __construct($arguments = array(), $default_file = NULL) {
    parent::__construct($arguments, $default_file);
    if (isset($arguments['source_dir'])) {
      $this->sourceDir = rtrim($arguments['source_dir'], "/\\");
    }
  }

  /**
   * Implementation of MigrateFileInterface::fields().
   *
   * @return array
   */
  static public function fields() {
    return parent::fields() +
      array(
        'source_dir' => t('Subfield: <a href="@doc">Path to source file.</a>',
          array('@doc' => 'http://drupal.org/node/1540106#source_dir')),
      );
  }

  /**
   * Implementation of MigrateFile::copyFile().
   *
   * @param $destination
   *  Destination within Drupal.
   *
   * @return bool
   *  TRUE if the copy succeeded, FALSE otherwise.
   */
  protected function copyFile($destination) {
    // Perform the copy operation.
    if (!@copy($this->sourcePath, $destination)) {
      throw new MigrateException(t('The specified file %file could not be copied to ' .
        '%destination.',
        array('%file' => $this->sourcePath, '%destination' => $destination)));
    }
    else {
      return TRUE;
    }
  }

  /**
   * Implementation of MigrateFileInterface::processFiles().
   *
   * @param $value
   *  The URI or local filespec of a file to be imported.
   * @param $owner
   *  User ID (uid) to be the owner of the file.
   * @return object
   *  The file entity being created or referenced.
   */
  public function processFile($value, $owner) {
    // Identify the full path to the source file
    if (!empty($this->sourceDir)) {
      $this->sourcePath = rtrim($this->sourceDir, "/\\") . '/' . ltrim($value, "/\\");
    }
    else {
      $this->sourcePath = $value;
    }

    if (empty($this->destinationFile)) {
      $this->destinationFile = basename($this->sourcePath);
    }

    // MigrateFile has most of the smarts - the key is that it will call back
    // to our copyFile() implementation.
    $file = parent::processFile($value, $owner);

    return $file;
  }
}

/**
 * Handle cases where we're handed a blob (i.e., the actual contents of a file,
 * such as image data) to be stored as a real file in Drupal.
 */
class MigrateFileBlob extends MigrateFile {
  /**
   * The file contents we will be writing to a real file.
   *
   * @var
   */
  protected $fileContents;

  /**
   * Implementation of MigrateFile::copyFile().
   *
   * @param $destination
   *  Drupal destination path.
   * @return bool
   *  TRUE if the file contents were successfully written, FALSE otherwise.
   */
  protected function copyFile($destination) {
    if (file_put_contents($destination, $this->fileContents)) {
      return TRUE;
    }
    else {
      $migration = Migration::currentMigration();
      $migration->saveMessage(t('Failed to write blob data to %destination',
                              array('%destination' => $destination)));
      return FALSE;
    }
  }

  /**
   * Implementation of MigrateFileInterface::processFile().
   *
   * @param $value
   *  The file contents to be saved as a file.
   * @param $owner
   *  User ID (uid) to be the owner of the file.
   * @return object
   *  File entity being created or referenced.
   */
  public function processFile($value, $owner) {
    $this->fileContents = $value;
    $file = parent::processFile($value, $owner);
    return $file;
  }
}

/**
 * Destination class implementing migration into the files table.
 */
class MigrateDestinationFile extends MigrateDestinationEntity {
  /**
   * File class (MigrateFileUri etc.) doing the dirty wrk.
   *
   * @var string
   */
  protected $fileClass;

  /**
   * Implementation of MigrateDestination::getKeySchema().
   *
   * @return array
   */
  static public function getKeySchema() {
    return array(
      'fid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'file_managed ID',
      ),
    );
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to files.
   */
  public function __construct($bundle = 'file', $file_class = 'MigrateFileUri',
                              $options = array()) {
    parent::__construct('file', $bundle, $options);
    $this->fileClass = $file_class;
  }

  /**
   * Returns a list of fields available to be mapped for the entity type (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core properties
    $fields['fid'] = t('File: Existing file ID');
    $fields['uid'] = t('File: Uid of user associated with file');
    $fields['value'] = t('File: Representation of the source file (usually a URI)');
    $fields['timestamp'] = t('File: UNIX timestamp for the date the file was added');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('File', 'fields', $this->entityType, $this->bundle, $migration);

    // Plus anything provided by the file class
    $fields += call_user_func(array($this->fileClass, 'fields'));

    return $fields;
  }

  /**
   * Delete a file entry.
   *
   * @param array $fid
   *  Fid to delete, arrayed.
   */
  public function rollback(array $fid) {
    migrate_instrument_start('file_load');
    $file = file_load(reset($fid));
    migrate_instrument_stop('file_load');
    if ($file) {
      // If we're not preserving the file, make sure we do the job completely.
      migrate_instrument_start('file_delete');
      file_delete($file, TRUE);
      migrate_instrument_stop('file_delete');
    }
  }

  /**
   * Import a single file record.
   *
   * @param $file
   *  File object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (fid only in this case) of the file that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $file, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      if (isset($file->fid)) {
        if ($file->fid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming fid !fid and map destination fid !destid1 don't match",
            array('!fid' => $file->fid, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $file->fid = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($file->fid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination fid provided'));
      }
      $old_file = file_load($file->fid);
    }

    // Invoke migration prepare handlers
    $this->prepare($file, $row);

    if (isset($file->fid)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    if (!isset($file->uid)) {
      $file->uid = 1;
    }

    // file_save() unconditionally sets timestamp - if we have an explicit
    // value we want, we need to set it manually after file_save.
    if (isset($file->timestamp)) {
      $timestamp = MigrationBase::timestamp($file->timestamp);
    }

    $file_class = $this->fileClass;
    $source = new $file_class((array)$file, $file);
    $file = $source->processFile($file->value, $file->uid);

    if (is_object($file) && isset($file->fid)) {
      $this->complete($file, $row);
      if (isset($timestamp)) {
        db_update('file_managed')
          ->fields(array('timestamp' => $timestamp))
          ->condition('fid', $file->fid)
          ->execute();
        $file->timestamp = $timestamp;
      }
      $return = array($file->fid);
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
    }
    else {
      $return = FALSE;
    }
    return $return;
  }
}
