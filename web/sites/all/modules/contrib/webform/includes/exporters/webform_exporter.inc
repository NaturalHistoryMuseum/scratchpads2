<?php
/**
 * Base class defining the common methods available to exporters.
 */
class webform_exporter {
  public $options = array();
  public $export_wordrap;

  /**
   * Constructor for webform_exporter classes.
   *
   * @param $options
   *   The array of export options as provided by the user-interface.
   */
  function __construct($options) {
    $this->options = $options;
    $this->export_wordwrap = webform_variable_get('webform_export_wordwrap');
  }

  /**
   * Determines whether a cell is eligible for word-wrapping based upon
   * position in file and the contents of cell.
   *
   * Return true when the global word-wrapping option is enabled and the cell
   * is anything other than the first column in either of the first two rows.
   * By default, these rows are long and are intended to overlap the columns
   * to the right. Also returns true when the cell contains a return character.
   *
   * @param int $row
   *   Row number, counting from 0.
   * @param int $column
   *   Column number, counting from 0.
   * @param string $value
   *   The value of the cell.
   * @return boolean
   *   Whether the cell position is elegible for wordwrapping.
   */
  function wrappable($row, $column, $value) {
    return strpos($value, "\n") !== FALSE ||
           $this->export_wordwrap && ($row > 2 || $column > 0 || $this->options['header_keys'] < 0);
  }

  /**
   * Add a single row to the export file.
   *
   * @param $file_handle
   *   A PHP file handle to the export file.
   * @param array $data
   *   An array of formatted data for this row. One cell per item.
   * @param int $row_count
   *   The current number of rows in the export file.
   */
  function add_row(&$file_handle, $data, $row_count) {
  }

  /**
   * Provide headers to the page when an export file is being downloaded.
   *
   * @param $filename
   *   The name of the file being downloaded. e.g. export.xls.
   */
  function set_headers($filename) {
    drupal_add_http_header('Content-Type', 'application/force-download');
    drupal_add_http_header('Pragma', 'public');
    drupal_add_http_header('Cache-Control', 'max-age=0');
  }

  /**
   * Write the start of the export file.
   *
   * @param $file_handle
   *   A PHP file handle to the export file.
   */
  function bof(&$file_handle) {
  }

  /**
   * Write the end of the export file.
   *
   * @param $file_handle
   *   A PHP file handle to the export file.
   */
  function eof(&$file_handle, $row_count, $col_count) {
  }

  /**
   * Allow final processing of the results.
   *
   * @param $results
   *   An array of result data, including:
   *   - node: The node whose results are being downloaded.
   *   - file_name: The full file path of the generated file.
   *   - row_count: The final number of rows in the generated file.
   */
  function post_process(&$results) {
  }
}
