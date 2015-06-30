<?php

/**
 * @file
 * Webform exporter for creating CSV/TSV delimited files.
 */

class webform_exporter_delimited extends webform_exporter {
  public $delimiter;

  function __construct($options) {
    $this->delimiter = isset($options['delimiter']) ? $options['delimiter'] : ',';
    // Convert tabs.
    if ($this->delimiter == '\t') {
      $this->delimiter = "\t";
    }
    $options['delimiter'] = $this->delimiter;
    parent::__construct($options);
  }

  function add_row(&$file_handle, $data, $row_count) {
    foreach ($data as $key => $value) {
      // Escape inner quotes and wrap all contents in new quotes.
      $data[$key] = '"' . str_replace('"', '""', $data[$key]) . '"';

      // Remove <script> tags, which mysteriously cause Excel not to import.
      $data[$key] = preg_replace('!<(/?script.*?)>!', '[$1]', $data[$key]);
    }
    $row = implode($this->delimiter, $data) . "\n";

    @fwrite($file_handle, $row);
  }

  function set_headers($filename) {
    parent::set_headers($filename);

    // Convert tabs.
    if ($this->delimiter == "\t") {
      $extension = 'tsv';
      $content_type = 'text/tab-separated-values';
    }
    else {
      $extension = 'csv';
      $content_type = 'text/csv';
    }

    drupal_add_http_header('Content-Type', $content_type);
    drupal_add_http_header('Content-Disposition', "attachment; filename=$filename.$extension");
  }
}
