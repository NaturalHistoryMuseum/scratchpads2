<?php

/**
 * @file
 * Support for file entity as destination. Note that File Fields have their
 * own destination in fields.inc
 */

/**
 * Destination class implementing migration into the files table.
 */
class MigrateDestinationFile extends MigrateDestinationEntity {
  /**
   * Whether to copy a file from the provided URI into the Drupal installation.
   * If FALSE, the URI is presumed to be local to the Drupal install. If TRUE,
   * the file will be copied according to the copyScheme value.
   *
   * @var boolean
   */
  protected $copyFile = FALSE;

  /**
   * When copying files, the destination scheme/directory. Defaults to 'public://'.
   *
   * @var string
   */
  protected $copyDestination = 'public://';

  /**
   * When creating a file entry, add a usage entry so that the file is only
   * deleted when the file itself is deleted. Otherwise, the deletion of a
   * referencing entity may delete the file.
   *
   * @var boolean
   */
  protected $preserveFiles = FALSE;

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
   * Return an options array for file destinations.
   *
   * @param boolean $copy_file
   *  TRUE to have the file copied from the provided URI to Drupal. Defaults to FALSE.
   * @param string $copy_destination
   *  If $copy_file is TRUE, the scheme/directory to use as the destination for the copied file.
   *  Defaults to 'public://'.
   * @param string $language
   *  Default language for files created via this destination class.
   * @param string $text_format
   *  Default text format for files created via this destination class.
   * @param boolean $preserve_files
   *  TRUE will add a self-reference to each file created, so it is not deleted
   *  on rollback of fields referencing it.
   * @param boolean $copy_blob
   *  TRUE to the binary value written to filesystem at $copy_destination. When this
   *  is TRUE, $copy_file should be FALSE. Defaults to FALSE.
   */
  static public function options($copy_file, $copy_destination, $language,
      $text_format, $preserve_files = FALSE, $copy_blob = FALSE) {
    return compact('copy_file', 'copy_destination', 'language',
      'text_format', 'preserve_files', 'copy_blob');
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to files.
   */
  public function __construct(array $options = array(), $bundle = 'file') {
    if (isset($options['copy_file'])) {
      $this->copyFile = $options['copy_file'];
    }
    if (isset($options['copy_blob'])) {
      $this->copyBlob = $options['copy_blob'];
    }
    if (isset($options['copy_destination'])) {
      $this->copyDestination = $options['copy_destination'];
    }
    if (isset($options['preserve_files'])) {
      $this->preserveFiles = $options['preserve_files'];
    }
    parent::__construct('file', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for the entity type (bundle)
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    // First the core properties
    $fields['fid'] = t('File: Existing file ID');
    $fields['uid'] = t('File: Uid of user associated with file');
    $fields['filename'] = t('File: Name of the file with no path components. Required for copy_blob migration.');
    $fields['uri'] = t('URI of the source file. Not used for copy_blob migrations.');
    $fields['contents'] = t('Contents of the file (only used with copy_blob option)');
    $fields['filemime'] = t('File: The file\'s MIME type. Optional.');
    $fields['status'] = t('File: A bitmapped field indicating the status of the file');
    $fields['timestamp'] = t('File: UNIX timestamp for the date the file was added');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle);
    $fields += migrate_handler_invoke_all('File', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Copy from BLOB to filesystem if needed
   *
   * @param $entity
   *  Entity object to build. Prefilled with any fields mapped in the Migration.
   * @param $source_row
   *  Raw source data object - passed through to prepare handlers.
   */
  public function prepare($entity, stdClass $source_row) {
    parent::prepare($entity, $source_row);
    if (!empty($this->copyBlob)) {
      if (empty($entity->filename)) {
        throw new MigrateException("You must map to filename field for copy_blob migrations.");
        return FALSE;
      }
      $destination_dir = $this->copyDestination;
      // file_save() expects $file->uri to be set.
      $entity->uri = $destination_dir . str_replace('/', '-', $entity->filename);
      $destination_file = file_stream_wrapper_uri_normalize($entity->uri);
      if (!file_exists($destination_file)) {
        file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
        try {
          $return = (bool) file_put_contents($destination_file, $entity->contents);
        }
        catch (Exception $exception) {
          throw new MigrateException("Can't write file to: $destination_file");
          return FALSE;
        }
      }
    }
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
      // If we want to preserve the file while wiping the DB evidence of the file,
      // we need to do it ourselves - file_delete() is all-or-nothing.
      if ($this->preserveFiles) {
        migrate_instrument_start('file_delete (preserving)');
        // Let other modules clean up any references to the deleted file.
        module_invoke_all('file_delete', $file);
        module_invoke_all('entity_delete', $file, 'file');
        // Remove the core knowledge of the file.
        db_delete('file_managed')->condition('fid', $file->fid)->execute();
        db_delete('file_usage')->condition('fid', $file->fid)->execute();
        migrate_instrument_stop('file_delete (preserving)');
      }
      else {
        // If we're not preserving the file, make sure we do the job completely.
        migrate_instrument_start('file_delete');
        file_delete($file, TRUE);
        migrate_instrument_stop('file_delete');
      }
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

    if ($this->copyFile) {
      $path = trim(parse_url($file->uri, PHP_URL_PATH), '/');
      $destination = $this->copyDestination . $path;
      $dirname = drupal_dirname($destination);
      if (file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        // We'd like to use file_unmanaged_copy, but it calls file_exists, which
        // won't work for remote URLs
        migrate_instrument_start('MigrateDestinationFile copy');
        $result = @copy(str_replace(' ', '%20', $file->uri), $destination);
        migrate_instrument_stop('MigrateDestinationFile copy');
        if ($result) {
          $file->uri = $destination;
        }
        else {
          throw new MigrateException(t('Could not copy file !uri', array('!uri' => $file->uri)),
            MigrationBase::MESSAGE_ERROR);
        }
      }
      else {
        throw new MigrateException(t('Could not create directory !dirname',
            array('!dirname' => $dirname)),
          MigrationBase::MESSAGE_ERROR);
      }
    }

    // Default filename to the basename of the (final) URI
    if (!isset($file->filename) || !$file->filename) {
      $file->filename = basename($file->uri);
    }

    // Detect the mime type if not provided
    if (!isset($file->filemime) || !$file->filemime) {
      $file->filemime = file_get_mimetype($file->filename);
    }
    // Invoke migration prepare handlers
    $this->prepare($file, $row);

    if (isset($file->fid)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('file_save');
    if (!file_load_multiple(array(), array('uri' => $file->uri))) {
      // Get this orphaned file into the file table.
      $file->fid = NULL;
      $file->status |= FILE_STATUS_PERMANENT; // Save a write in file_field_presave().
    }
    $file = file_save($file);
    migrate_instrument_stop('file_save');
    $this->complete($file, $row);

    // When creating a new file entry, if we want to preserve it against deletion,
    // add a usage entry.
    if (!$updating && $this->preserveFiles) {
      file_usage_add($file, 'migrate', 'file', $file->fid);
    }

    if (isset($file->fid)) {
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
