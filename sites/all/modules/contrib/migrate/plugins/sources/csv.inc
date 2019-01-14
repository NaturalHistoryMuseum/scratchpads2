<?php

/**
 * @file
 * Define a MigrateSource for importing from comma separated values files.
 */

/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from a CSV file.
 */
class MigrateListCSV extends MigrateList {

  /**
   * The path to the CSV file containing a list of IDs to be processed.
   *
   * @var string
   */
  protected $file;

  /**
   * File handle for the CSV file being iterated.
   *
   * @var resource
   */
  protected $csvHandle = NULL;

  /**
   * The column which contains the ID.
   *
   * If numeric, this is the index of the column from 0; if non-numeric, this
   * is the column name as given by the header row.
   */
  protected $idColumn;

  /**
   * The number of rows in the CSV file before the data starts.
   *
   * @var integer
   */
  protected $headerRows = 0;

  /**
   * The array keys taken from the first row.
   *
   * @var array
   */
  protected $headers = array();

  /**
   * Parameters for the fgetcsv() call.
   *
   * @var array
   */
  protected $fgetcsv = array();

  /**
   * Constructor for MigrateListCSV.
   *
   * @param $list_path
   *  The path to the CSV file that holds the list of IDs.
   * @param $id_column
   *  The column in the file that holds the IDs. A numeric index for the column,
   *  from 0.
   * @param $options = array()
   *  An array of further options for the CSV file.
   */
  public function __construct($list_path, $id_column, $options = array()) {
    parent::__construct();
    $this->file = $list_path;
    $this->idColumn = $id_column;
    $this->options = $options;

    // fgetcsv specific options
    foreach (array(
               'length' => NULL,
               'delimiter' => ',',
               'enclosure' => '"',
               'escape' => '\\',
             ) as $key => $default) {
      $this->fgetcsv[$key] = isset($options[$key]) ? $options[$key] : $default;
    }

    if (!empty($options['header_rows'])) {
      $this->headerRows = $options['header_rows'];
    }
    else {
      $this->headerRows = 0;
    }

    if (!is_numeric($id_column)) {
      //we need to store the headers.
      $this->csvHandle = fopen($this->file, 'r');
      if (!$this->validResource()) {
        return;
      }
      $this->headers = $this->getNextLine();
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
    }
  }

  /**
   * Return a string representing the source file.
   *
   * @return string
   */
  public function __toString() {
    return $this->file;
  }

  /**
   * Return an array of IDs from the CSV file.
   *
   * @return array
   */
  public function getIdList() {
    $ids = array();

    $this->csvHandle = fopen($this->file, 'r');
    if (!$this->validResource()) {
      return $ids;
    }

    // Skip the headers if any,
    for ($i = 0; $i < $this->headerRows; $i++) {
      $this->getNextLine();
    }

    while ($row = $this->getNextLine()) {
      $ids[] = $row[$this->idColumn];
    }

    fclose($this->csvHandle);
    $this->csvHandle = NULL;

    return $ids;
  }

  /**
   * Return a count of all available IDs from the source listing.
   *
   * @return integer
   */
  public function computeCount() {
    // If the data may have embedded newlines, the file line count won't reflect
    // the number of CSV records (one record will span multiple lines). We need
    // to scan with fgetcsv to get the true count.
    if (!empty($this->options['embedded_newlines'])) {
      $this->csvHandle = fopen($this->file, 'r');
      $count = 0;
      if (!$this->validResource()) {
        return $count;
      }
      // Skip all but the last header
      for ($i = 0; $i < $this->headerRows; $i++) {
        fgets($this->csvHandle);
      }
      while ($this->getNextLine()) {
        $count++;
      }
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
    }
    else {

      $file = new SplFileObject($this->file, 'r');
      $file->seek(PHP_INT_MAX);
      $count = $file->key() + 1;
      $file->rewind();
      $count -= $this->headerRows;
    }
    return $count;
  }

  /**
   * Get the next line of the CSV file.
   * Returns an array of the next line, keyed by headers if required
   *
   * @return array
   */
  protected function getNextLine() {
    // escape parameter was added in PHP 5.3.
    if (version_compare(phpversion(), '5.3', '<')) {
      $row = fgetcsv($this->csvHandle, $this->fgetcsv['length'],
        $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure']);
    }
    else {
      $row = fgetcsv($this->csvHandle, $this->fgetcsv['length'],
        $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure'],
        $this->fgetcsv['escape']);
    }
    // Key the array with the headers
    if (!empty($this->headers) && $row) {
      $row = array_combine($this->headers, $row);
    }
    return $row;
  }

  /**
   * Check if resource loaded correctly.
   *
   * @return bool
   */
  public function validResource() {
    if (!$this->csvHandle) {
      Migration::displayMessage(t('Could not open CSV file !url',
        array('!url' => $this->file)));
    }
    return (bool) $this->csvHandle;
  }

}

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
   * The current row/line number in the CSV file.
   *
   * @var integer
   */
  protected $rowNumber;

  /**
   * The path to the source file.
   *
   * @var string
   */
  protected $file;

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
    foreach (array(
               'length' => NULL,
               'delimiter' => ',',
               'enclosure' => '"',
               'escape' => '\\',
             ) as $key => $default) {
      $this->fgetcsv[$key] = isset($options[$key]) ? $options[$key] : $default;
    }
    // One can either pass in an explicit list of column names to use, or if we have
    // a header row we can use the names from that
    if ($this->headerRows && empty($csvcolumns)) {
      $this->csvcolumns = array();
      $this->csvHandle = fopen($this->file, 'r');
      if (!$this->validResource()) {
        return;
      }
      // Skip all but the last header
      for ($i = 0; $i < $this->headerRows - 1; $i++) {
        $this->getNextLine();
      }

      $row = $this->getNextLine();
      foreach ($row as $header) {
        $header = trim($header);
        $this->csvcolumns[] = array($header, $header);
      }
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
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
    $fields = array();
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
      $this->csvHandle = fopen($this->file, 'r');
      $count = 0;
      if (!$this->validResource()) {
        return $count;
      }
      // Skip all but the last header
      for ($i = 0; $i < $this->headerRows; $i++) {
        fgets($this->csvHandle);
      }
      while ($this->getNextLine()) {
        $count++;
      }
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
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
      $this->csvHandle = NULL;
    }
    // Load up the first row, skipping the header(s) if necessary
    $this->csvHandle = fopen($this->file, 'r');
    if (!$this->validResource()) {
      return;
    }
    for ($i = 0; $i < $this->headerRows; $i++) {
      $this->getNextLine();
    }
    $this->rowNumber = 1;
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   * Return the next line of the source CSV file as an object.
   *
   * @return null|object
   */
  public function getNextRow() {
    $row = $this->getNextLine();
    if ($row) {
      // only use rows specified in $this->csvcolumns().
      $row = array_intersect_key($row, $this->csvcolumns);
      // Set meaningful keys for the columns mentioned in $this->csvcolumns().
      foreach ($this->csvcolumns as $int => $values) {
        list($key, $description) = $values;
        // Copy value to more descriptive string based key and then unset original.
        $row[$key] = isset($row[$int]) ? $row[$int] : NULL;
        unset($row[$int]);
      }
      $row['csvrownum'] = $this->rowNumber++;
      return (object) $row;
    }
    else {
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
      return NULL;
    }
  }

  protected function getNextLine() {
    // escape parameter was added in PHP 5.3.
    if (version_compare(phpversion(), '5.3', '<')) {
      $row = fgetcsv($this->csvHandle, $this->fgetcsv['length'],
        $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure']);
    }
    else {
      $row = fgetcsv($this->csvHandle, $this->fgetcsv['length'],
        $this->fgetcsv['delimiter'], $this->fgetcsv['enclosure'],
        $this->fgetcsv['escape']);
    }
    return $row;
  }

  /**
   * Check if resource loaded correctly.
   *
   * @return bool
   */
  public function validResource() {
    if (!$this->csvHandle) {
      Migration::displayMessage(t('Could not open CSV file !url',
        array('!url' => $this->file)));
    }
    return (bool) $this->csvHandle;
  }

}
