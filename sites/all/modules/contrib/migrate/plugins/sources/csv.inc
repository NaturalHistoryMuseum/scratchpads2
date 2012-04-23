<?php

/**
 * @file
 * Define a MigrateSource for importing from comma separated values files.
 */

/**
 * Implementation of MigrateSource, to handle imports from CSV files.
 *
 * If the CSV file contains non-ASCII characters, make sure it includes a
 * UTF BOM (Byte Order Marker) so they are interpreted correctly.
 */
class MigrateSourceCSV extends MigrateSource {
  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Parameters for the fgetcsv() call.
   *
   * @var array
   */
  protected $fgetcsv = array();

  /**
   * File handle for the CSV file being iterated.
   *
   * @var resource
   */
  protected $csvHandle = NULL;

  /**
   * The number of rows in the CSV file before the data starts.
   *
   * @var integer
   */
  protected $headerRows = 0;

  /**
   * Simple initialization.
   *
   * @param string $path
   *  The path to the source file
   * @param array $csvcolumns
   *  Keys are integers. values are array(field name, description).
   * @param array $options
   *  Options applied to this source.
   * @param array $fields
   *  Optional - keys are field names, values are descriptions. Use to override
   *  the default descriptions, or to add additional source fields which the
   *  migration will add via other means (e.g., prepareRow()).
   */
  public function __construct($path, array $csvcolumns = array(), array $options = array(), array $fields = array()) {
    parent::__construct($options);
    $this->file = $path;
    if (!empty($options['header_rows'])) {
      $this->headerRows = $options['header_rows'];
    }
    else {
      $this->headerRows = 0;
    }
    $this->options = $options;
    $this->fields = $fields;
    // fgetcsv specific options
    foreach (array('length' => NULL, 'delimiter' => ',', 'enclosure' => '"', 'escape' => '\\') as $key => $default) {
      $this->fgetcsv[$key] = isset($options[$key]) ? $options[$key] : $default;
    }
    // One can either pass in an explicit list of column names to use, or if we have
    // a header row we can use the names from that
    if ($this->headerRows && empty($csvcolumns)) {
      $this->csvcolumns = array();
      $result = fopen($this->file, 'r');
      // Skip all but the last header
      for ($i = 0; $i < $this->headerRows - 1; $i++) {
        fgets($this->result);
      }

      $row = fgetcsv($result, $this->fgetcsv['length'], $this->fgetcsv['delimiter'],
        $this->fgetcsv['enclosure']);
      foreach ($row as $header) {
        $header = trim($header);
        $this->csvcolumns[] = array($header, $header);
      }
      fclose($result);
    }
    else {
      $this->csvcolumns = $csvcolumns;
    }
  }


  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    return $this->file;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    foreach ($this->csvcolumns as $values) {
      $fields[$values[0]] = $values[1];
    }

    // Any caller-specified fields with the same names as extracted fields will
    // override them; any others will be added
    if ($this->fields) {
      $fields = $this->fields + $fields;
    }

    return $fields;
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    // If the data may have embedded newlines, the file line count won't reflect
    // the number of CSV records (one record will span multiple lines). We need
    // to scan with fgetcsv to get the true count.
    if (!empty($this->options['embedded_newlines'])) {
      $result = fopen($this->file, 'r');
      // Skip all but the last header
      for ($i = 0; $i < $this->headerRows; $i++) {
        fgets($result);
      }
      $count = 0;
      while (fgetcsv($result, $this->fgetcsv['length'], $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure'])) {
        $count++;
      }
      fclose($result);
    }
    else {
      // TODO. If this takes too much time/memory, use exec('wc -l')
      $count = count(file($this->file));
      $count -= $this->headerRows;
    }
    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   *
   * @return void
   */
  public function performRewind() {
    // Close any previously-opened handle
    if (!is_null($this->csvHandle)) {
      fclose($this->csvHandle);
    }
    // Load up the first row, skipping the header(s) if necessary
    $this->csvHandle = fopen($this->file, 'r');
    for ($i = 0; $i < $this->headerRows; $i++) {
      fgets($this->csvHandle);
    }
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   * Return the next line of the source CSV file as an object.
   *
   * @return null|object
   */
  public function getNextRow() {
    // @todo $this->fgetcsv['escape'] not used as it gives warning in 5.2 - http://drupal.org/node/1039808.
    $row = fgetcsv($this->csvHandle, $this->fgetcsv['length'], $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure']);
    if ($row) {
      // Set meaningful keys for the columns mentioned in $this->csvcolumns().
      foreach ($this->csvcolumns as $int => $values) {
        list($key, $description) = $values;
        // Copy value to more descriptive string based key and then unset original.
        $row[$key] = isset($row[$int]) ? $row[$int] : NULL;
        unset($row[$int]);
      }
      return (object)$row;
    }
    else {
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
      return NULL;
    }
  }
}
