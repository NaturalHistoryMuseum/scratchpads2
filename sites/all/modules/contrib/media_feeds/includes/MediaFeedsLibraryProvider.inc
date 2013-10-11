<?php

/**
 * @file
 * Map existing files from the library.
 */

/**
 * Class for mapping values using existing files from the media library.
 */
class MediaFeedsLibraryProvider extends MediaFeedsProvider {
  protected $file;

  public function __construct($value, $config = array()) {
    // Query existing files.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'file');

    switch (file_uri_scheme($value)) {
      // No URI scheme given. Use as a file name.
      case FALSE:
        // By exact match or without file extension.
        if (empty($config['no_file_extensions'])) {
          $query->propertyCondition('filename', $value);
        }
        else {
          $query->propertyCondition('filename', $value . '.', 'STARTS_WITH');
        }
        break;

      // Query by URI.
      default:
        $query->propertyCondition('uri', $value);
        break;
    }

    // Get the result.
    $result = $query->execute();
    if (!empty($result)) {
      $this->file = reset(file_load_multiple(array_keys($result['file'])));
    }
  }

  public function getFileObject() {
    return $this->file;
  }

  public function save() {
    return $this->file;
  }

  public static function summaryCallback($mapping, $target, $form, $form_state) {
    if (empty($mapping['no_file_extensions'])) {
      return t('Source is <strong>exactly</strong> the filename');
    }
    else {
      return t('Source <strong>doesn\'t have file extensions</strong>');
    }
  }

  public static function formCallback($mapping, $target, $form, $form_state) {
    return array(
      'no_file_extensions' => array(
        '#type' => 'checkbox',
        '#title' => t('Source has no file extensions'),
        '#default_value' => !empty($mapping['no_file_extensions']),
      ),
    );
  }
}
