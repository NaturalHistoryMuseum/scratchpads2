<?php

/**
 * Destination class implementing migration into media entities.
 */
class MigrateDestinationMedia extends MigrateDestinationFile {
  /**
   * Call this from the prepare() method of a migration that contains media
   * image files, if you want to rewrite the IMG tags into media references.
   *
   * @param $entity
   *  Entity object being built.
   * @param $field
   *  Name of the text field within the entity to be modified. Defaults to 'body'.
   */
  static public function rewriteImgTags($entity, $field = 'body') {
    if (is_array($entity->$field)) {
      migrate_instrument_start('MigrateDestinationMedia rewriteImgTags');
      foreach ($entity->$field as $language => $values) {
        $body = $values[0]['value'];
        break;
      }
      // Quickly skip any non-candidates
      if (!stristr($body, '<img')) {
        migrate_instrument_stop('MigrateDestinationMedia rewriteImgTags');
        return;
      }
      // Pass full img tags into the callback
      $new_body = preg_replace_callback('|<img [^>]*>|i', array(self, 'replaceCallback'),
        $body);
      $entity->{$field}[$language][0]['value'] = $new_body;
      migrate_instrument_stop('MigrateDestinationMedia rewriteImgTags');
    }
  }

  /**
   * If a referenced image can be found in the files table, replace the <img> tag
   * with a media JSON reference.
   *
   * @param array $matches
   */
  static protected function replaceCallback(array $matches) {
    $src_matches = array();
    // Default to the original <img> tag
    $result = $matches[0];
    // Extract the src parameter
    if (preg_match('|src=[\'"]([^\'"]+)[\'"]|i', $matches[0], $src_matches)) {
      // Replace the scheme and host portions of the referenced URI with the
      // Drupal scheme as it's saved in the file_unmanaged table
      $src = $src_matches[1];
      $fid = db_select('file_managed', 'f')
             ->fields('f', array('fid'))
             ->condition('filename', basename($src))
             ->execute()
             ->fetchField();
      if ($fid) {
        $image_info = array(
          'type' => 'media',
          'view_mode' => 'media_large',
          'fid' => $fid,
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

/**
 * Class for creating Youtube file entities.
 */
class MigrateExtrasFileYoutube implements MigrateFileInterface {
  /**
   * Implementation of MigrateFileInterface::fields().
   *
   * @return array
   */
  static public function fields() {
    return array();
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
    // Convert the Youtube URI into a local stream wrapper.
    $handler = new MediaInternetYouTubeHandler($value);
    $destination = $handler->parse($value);

    // Create a file entity object for this Youtube reference, and see if we
    // can get the video title.
    $file = file_uri_to_object($destination, TRUE);
    if (empty($file->fid) && $info = $handler->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }
    $file = file_save($file);
    if (is_object($file)) {
      return $file;
    }
    else {
      return FALSE;
    }
  }
}
