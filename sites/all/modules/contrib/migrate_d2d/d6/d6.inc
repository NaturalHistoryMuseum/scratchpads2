<?php

/**
 * Drupal 6 implementations of functions shared among multiple types of objects.
 */
class DrupalVersion6 implements DrupalVersionInterface {
  protected $arguments;

  public function __construct($arguments) {
    $this->arguments = $arguments;
  }

  /**
   * Generate default format mappings based on matching names. E.g., if the
   * Drupal 6 database has format 5 with name 'Filtered HTML', and the Drupal 7
   * databas has format filtered_html with name 'Filtered HTML', the resulting
   * array will contain the row '5' => 'filtered_html'.
   */
  public function getDefaultFormatMappings() {
    migrate_instrument_start('DrupalVersion6::getDefaultFormatMappings');
    $format_mappings = array();
    $result = Database::getConnection('default', $this->arguments['source_connection'])
              ->select('filter_formats', 'f')
              ->fields('f', array('format', 'name'))
              ->execute();
    foreach ($result as $format_row) {
      $format = db_select('filter_format', 'f')
                ->fields('f', array('format'))
                ->condition('name', $format_row->name)
                ->execute()
                ->fetchField();
      if ($format) {
        $format_mappings[$format_row->format] = $format;
      }
    }
    migrate_instrument_stop('DrupalVersion6::getDefaultFormatMappings');
    return $format_mappings;
  }

  /**
   * Given a source path (e.g, 'node/123'), return the first alias for that path.
   *
   * @param $source
   * @return string
   */
  public function getPath($source) {
    migrate_instrument_start('DrupalVersion6::getPath');
    $path = Database::getConnection('default', $this->arguments['source_connection'])
                  ->select('url_alias', 'ua')
                  ->fields('ua', array('dst'))
                  ->condition('src', $source)
                  ->execute()
                  ->fetchField();
    migrate_instrument_stop('DrupalVersion6::getPath');
    return $path;
  }
}
