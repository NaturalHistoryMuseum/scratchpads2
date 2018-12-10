<?php

/**
 * @file
 * Define a MigrateSource for importing from spreadsheet files.
 *
 * Requires the PHPExcel library to be installed.
 * - Download PHPExcel at http://phpexcel.codeplex.com/.
 * - Extract the archive to a temporary folder.
 * - Ensure the hosting environment fulfills the requirements found in
 *   install.txt.
 * - Copy the contents of the Classes folder to an appropriate location
 *   (sites/all/libraries/PHPExcel).
 */

/**
 * Implements MigrateSource, to handle imports from XLS files.
 */
class MigrateSourceSpreadsheet extends MigrateSource {

  /**
   * PHPExcel object for storing the workbook data.
   *
   * @var PHPExcel
   */
  protected $workbook;

  /**
   * PHPExcel object for storing the worksheet data.
   *
   * @var PHPExcel_Worksheet
   */
  protected $worksheet;

  /**
   * The name of the worksheet that will be processed.
   *
   * @var string
   */
  protected $sheetName;

  /**
   * Number of rows in the worksheet that is being processed.
   *
   * @var integer
   */
  protected $rows = 0;

  /**
   * Number of columns in the worksheet that is being processed.
   *
   * @var integer
   */
  protected $cols = 0;

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * The current row number in the XLS file.
   *
   * @var integer+
   */
  protected $rowNumber;

  /**
   * The columns to be read from Excel
   */
  protected $columns;

  /**
   * The first row from where the table starts. It's a "zero based" value.
   *
   * @var int
   */
  protected $headerRows = 0;

  /**
   * Simple initialization.
   *
   * @param string $path
   *   The path to the source file.
   * @param string $sheet_name
   *   The name of the sheet to be processed.
   * @param array $options
   *   Options applied to this source. If the source data begins on different
   *   row than the first row, pass this "zero based" value as 'header_rows' in
   *   options, along with other options inherited from MigrateSource.
   */
  public function __construct($path, $sheet_name, $columns = array(), array $options = array()) {
    parent::__construct($options);
    $this->file = $path;
    $this->sheetName = $sheet_name;
    $this->columns = $columns;
    if (!empty($options['header_rows'])) {
      $this->headerRows = $options['header_rows'];
    }

    // Load the workbook.
    if ($this->load()) {
      // Get the dimensions of the worksheet.
      $this->rows = $this->worksheet->getHighestDataRow();
      $this->cols = PHPExcel_Cell::columnIndexFromString($this->worksheet->getHighestDataColumn());

      // Map field names to their column index.
      for ($col = 0; $col < $this->cols; ++$col) {
        $this->fields[$col] = trim($this->worksheet->getCellByColumnAndRow($col, $this->headerRows + 1)
          ->getValue());
      }

      $this->unload();
    }
  }

  /**
   * Loads the workbook.
   *
   * @return bool
   *   Returns true if the workbook was successfully loaded, otherwise false.
   */
  public function load() {
    // Check that the file exists.
    if (!file_exists($this->file)) {
      Migration::displayMessage(t('The file !filename does not exist.', array('!filename' => $this->file)));
      return FALSE;
    }

    // Check that required modules are enabled.
    if (!module_exists('libraries')) {
      Migration::displayMessage(t('The Libraries API module is not enabled.'));
      return FALSE;
    }
    if (!module_exists('phpexcel')) {
      Migration::displayMessage(t('The PHPExcel module is not enabled.'));
      return FALSE;
    }

    $library = libraries_load('PHPExcel');
    if (empty($library['loaded'])) {
      Migration::displayMessage(t('The PHPExcel library could not be found.'));
      return FALSE;
    }

    // Load the workbook.
    try {
      // Identify the type of the input file.
      $type = PHPExcel_IOFactory::identify($this->file);
      // Create a new Reader of the file type.
      $reader = PHPExcel_IOFactory::createReader($type);
      // Advise the Reader that we only want to load cell data.
      $reader->setReadDataOnly(TRUE);
      // Advise the Reader of which worksheet we want to load.
      $reader->setLoadSheetsOnly($this->sheetName);
      // Load the source file.
      $this->workbook = $reader->load($this->file);
      $this->worksheet = $this->workbook->getSheet();
    } catch (Exception $e) {
      Migration::displayMessage(t('Error loading file: %message', array('%message' => $e->getMessage())));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Unloads the workbook.
   */
  public function unload() {
    $this->workbook->disconnectWorksheets();
    unset($this->workbook);
  }

  /**
   * Returns a string representing the source query.
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
   *   Keys: machine names of the fields (to be passed to addFieldMapping).
   *   Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();

    foreach ($this->fields as $name) {
      $fields[$name] = $name;
    }

    return $fields;
  }

  /**
   * Returns a count of all available source records.
   */
  public function computeCount() {
    // Subtract the non-data rows (rows before header and the header).
    return $this->rows - $this->headerRows - 1;
  }

  /**
   * Implements MigrateSource::performRewind().
   *
   * @return void
   */
  public function performRewind() {
    // Initialize the workbook if it isn't already.
    if (!isset($this->workbook)) {
      $this->load();
    }

    $this->rowNumber = $this->headerRows + 1;
  }

  /**
   * Implements MigrateSource::getNextRow().
   *
   * @return null|object
   */
  public function getNextRow() {
    migrate_instrument_start('MigrateSourceSpreadsheet::next');
    ++$this->rowNumber;

    if ($this->rowNumber <= $this->rows) {
      $row_values = array();
      for ($col = 0; $col < $this->cols; ++$col) {
        if (in_array($this->fields[$col], $this->columns) || empty($this->columns)) {
          $row_values[$this->fields[$col]] = trim($this->worksheet->getCellByColumnAndRow($col, $this->rowNumber)
            ->getValue());
        }
      }

      return (object) $row_values;
    }
    else {
      // EOF, close the workbook.
      $this->unload();

      migrate_instrument_stop('MigrateSourceSpreadsheet::next');
      return NULL;
    }
  }
}
