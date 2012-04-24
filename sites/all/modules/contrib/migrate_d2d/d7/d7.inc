<?php

class DrupalVersion7 implements DrupalVersionInterface {
  protected $arguments;

  public function __construct($arguments) {
    $this->arguments = $arguments;
  }

  /**
   * Generate default format mappings based on matching names.
   */
  public function getDefaultFormatMappings() {
    $format_mappings = array();
    $result = Database::getConnection('default', $this->arguments['source_connection'])
              ->select('filter_format', 'f')
              ->fields('f', array('format'))
              ->execute();
    foreach ($result as $format_row) {
      $format_mappings[$format_row->format] = $format_row->format;
    }
    return $format_mappings;
  }

  public function getPath($source) {
    $path = Database::getConnection('default', $this->arguments['source_connection'])
                  ->select('url_alias', 'ua')
                  ->fields('ua', array('alias'))
                  ->condition('source', $source)
                  ->execute()
                  ->fetchField();
    return $path;
  }
}
