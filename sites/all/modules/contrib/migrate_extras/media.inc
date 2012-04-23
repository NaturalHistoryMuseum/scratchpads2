<?php

/**
 * @file
 * Support for media destinations.
 */

class MigrateMediaFieldHandler extends MigrateFileFieldHandler {
  public function __construct() {
    $this->registerTypes(array('media'));
  }

  protected function destinationDir($field_info, $instance) {
    // Use the first allowed scheme
    // TODO: Support an argument to pick a specific scheme
    foreach ($instance['widget']['settings']['allowed_schemes'] as $scheme => $value) {
      if (!empty($value)) {
        // Hard-coded hack for Youtube
        if ($value == 'youtube') {
          return $value . '://v';
        }
        else {
          return $value . '://';
        }
      }
    }
    // Fall back to the system default
    return file_default_scheme() . '://';
  }
}

/**
 * Destination class implementing migration into media entities.
 */
class MigrateDestinationMedia extends MigrateDestinationFile {
  /**
   * List of node migrations, whose bodies need image references fixed up.
   *
   * @var array
   */
  protected $sourceMigrations = array();

  /**
   * Node ID of the node whose body is currently being rewritten.
   *
   * @var int
   */
  protected $parentNid;

  /**
   * Return an options array for media destinations.
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
   * @param array $source_migrations
   *  List of node migrations, whose bodies need image references fixed up.
   */
  static public function options($copy_file, $copy_destination, $language, $text_format, $source_migrations = array()) {
    return compact('copy_file', 'copy_destination', 'language', 'text_format', 'source_migrations');
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to files.
   */
  public function __construct($media_type = 'default', array $options = array()) {
    if (isset($options['source_migrations'])) {
      $this->sourceMigrations = $options['source_migrations'];
    }
    parent::__construct($options, $media_type);
  }

  /**
   * Called at the completion of a migration into a media destination. Rewrite
   * <img> tags into the media module's embedded JSON references.
   */
  public function postImport() {
    migrate_instrument_start('MigrateDestinationMedia postImport');
    // Scan the destination nodes
    foreach ($this->sourceMigrations as $source_migration) {
      $migration = MigrationBase::getInstance($source_migration);
      $map = $migration->getMap();
      foreach ($map as $map_row) {
        $this->parentNid = $map_row->destid1;
        $node = node_load($this->parentNid);
        $body = $node->body[LANGUAGE_NONE][0]['value'];
        // Quickly skip any non-candidates
        if (!strstr($body, 'img')) {
          continue;
        }
        // Pass full img tags into the callback
        $new_body = preg_replace_callback('|<img [^>]*>|i', array($this, 'replaceCallback'),
          $body);
        $node->body[LANGUAGE_NONE][0]['value'] = $new_body;
        node_save($node);
      }
    }
    migrate_instrument_stop('MigrateDestinationMedia postImport');
  }

  /**
   * If a referenced image can be found in the files table, replace the <img> tag
   * with a media JSON reference.
   *
   * @param array $matches
   */
  protected function replaceCallback(array $matches) {
    $src_matches = array();
    // Default to the original <img> tag
    $result = $matches[0];
    // Extract the src parameter
    if (preg_match('|src=[\'"]([^\'"]+)[\'"]|i', $matches[0], $src_matches)) {
      // Replace the scheme and host portions of the referenced URL with the
      // Drupal scheme as it's saved in the file_unmanaged table
      $src = preg_replace('|^https?://[^/]+/|', $this->copyDestination, $src_matches[1]);
      $file = db_select('file_managed', 'f')
              ->fields('f', array('fid'))
              ->condition('filename', basename($src))
              ->execute()
              ->fetchObject();
      if ($file) {
        // Update file_usage
        // Note: Usages not cleaned up on file_delete() - NYI in media module
        file_usage_add($file, 'media', 'node', $this->parentNid);

        // TODO: Anything to do about alt/title?
        $image_info = array(
          'type' => 'media',
          'view_mode' => 'media_large',
          'fid' => $file->fid,
          'attributes' => array(
            'alt' => '',
            'title' => '',
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
            'wysiwyg' => 1,
          ),
        );
        $result = '[[' . drupal_json_encode($image_info) . ']]';
      }
    }
    return $result;
  }
}
