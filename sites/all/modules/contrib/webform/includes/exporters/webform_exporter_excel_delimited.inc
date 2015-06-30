<?php

/**
 * @file
 * The Excel exporter currently is just a tab-delimited export.
 */

class webform_exporter_excel_delimited extends webform_exporter_delimited {
  function __construct($options) {
    $options['delimiter'] = '\t';
    parent::__construct($options);
  }

  function bof(&$file_handle) {
    $output = '';

    // Include at BOM at the beginning of the file for Little Endian.
    // This makes tab-separated imports work correctly in MS Excel.
    if (function_exists('mb_convert_encoding')) {
      $output = chr(255) . chr(254);
    }
    @fwrite($file_handle, $output);
  }

  function add_row(&$file_handle, $data, $row_count) {
    foreach ($data as $key => $value) {
      // Escape inner quotes and wrap all contents in new quotes.
      $data[$key] = '"' . str_replace('"', '""', $data[$key]) . '"';

      // Remove <script> tags, which mysteriously cause Excel not to import.
      $data[$key] = preg_replace('!<(/?script.*?)>!', '[$1]', $data[$key]);
    }
    $row = implode($this->delimiter, $data) . "\n";

    if (function_exists('mb_convert_encoding')) {
      $row = mb_convert_encoding($row, 'UTF-16LE', 'UTF-8');
    }

    @fwrite($file_handle, $row);
  }

  function set_headers($filename) {
    drupal_add_http_header('Content-Type', 'application/x-msexcel');
    drupal_add_http_header('Content-Disposition', "attachment; filename=$filename.xls");
    drupal_add_http_header('Pragma', 'public');
    drupal_add_http_header('Cache-Control', 'max-age=0');
  }
}
