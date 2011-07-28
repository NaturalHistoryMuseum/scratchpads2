<?php

/**
 * Parses a given file as a Excel file.
 */
class FeedsExcelParser extends FeedsCSVParser{

  /**
   * Parse content fetched by fetcher.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    $source_config = $source->getConfigFor($this);
    $state = $source->state(FEEDS_PARSE);
    // Load and configure parser.
    $parser = new ParserExcel();
    $iterator = new ParserExcelIterator($fetcher_result->getFilePath());
    if(empty($source_config['no_headers'])){
      // Get first line and use it for column names, convert them to lower case.
      $header = $this->parseHeader($parser, $iterator);
      if(!$header){return;}
      $parser->setColumnNames($header);
    }
    // Determine section to parse, parse.
    $start = $state->pointer ? $state->pointer : $parser->lastLinePos();
    $limit = $source->importer->getLimit();
    $rows = $this->parseItems($parser, $iterator, $start, $limit);
    // Report progress.
    $state->total = filesize($fetcher_result->getFilePath());
    $state->pointer = $parser->lastLinePos();
    $progress = $parser->lastLinePos() ? $parser->lastLinePos() : $state->total;
    $state->progress($state->total, $progress);
    // Create a result object and return it.
    return new FeedsParserResult($rows, $source->feed_nid);
  }

  /**
   * Parse header
   */
  protected function parseHeader(ParserExcel $parser, ParserExcelIterator $iterator){
    $parser->setLineLimit(1);
    $rows = $parser->parse($iterator);
    if(!count($rows)){return FALSE;}
    $header = array_shift($rows);
    foreach($header as $i => $title){
      $header[$i] = trim(drupal_strtolower($title));
    }
    return $header;
  }

  /**
   * Parse all of the items from the CSV.
   *
   * @param ParserExcel $parser
   * @param ParserExcelIterator $iterator
   * @return
   * An array of rows of the CSV keyed by the column names previously set
   */
  protected function parseItems(ParserExcel $parser, ParserExcelIterator $iterator, $start = 0, $limit = 0){
    $parser->setLineLimit($limit);
    $parser->setStartByte($start);
    $rows = $parser->parse($iterator);
    return $rows;
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state){
    $form = parent::configForm($form_state);
    unset($form['delimiter']);
    return $form;
  }

  /**
   * Source form.
   *
   * Show mapping configuration as a guidance for import form users.
   */
  public function sourceForm($source_config){
    $form = parent::sourceForm($source_config);
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $sources = $uniques = array();
    foreach($mappings as $mapping){
      $sources[] = check_plain($mapping['source']);
      if($mapping['unique']){
        $uniques[] = check_plain($mapping['source']);
      }
    }
    $output = t('Import !csv_files with one or more of these columns: !columns.', array(
      '!csv_files' => l(t('Excel files'), 'http://en.wikipedia.org/wiki/Microsoft_Excel'),
      '!columns' => implode(', ', $sources)
    ));
    $items = array();
    $items[] = format_plural(count($uniques), t('Column <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.', array(
      '!column' => implode(', ', $uniques)
    )), t('Columns <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these column will be created.', array(
      '!columns' => implode(', ', $uniques)
    )));
    $form['help']['#markup'] = '<div class="help"><p>' . $output . '</p>' . theme('item_list', array(
      'items' => $items
    )) . '</div>';
    unset($form['delimiter']);
    return $form;
  }
}

/**
 * Functionality to parse Excel files into a two dimensional array.
 */
class ParserExcel{

  private $skipFirstLine;

  private $columnNames;

  private $timeout;

  private $timeoutReached;

  private $startLine;

  private $lineLimit;

  private $lastLinePos;

  public function __construct(){
    $this->skipFirstLine = FALSE;
    $this->columnNames = FALSE;
    $this->timeout = FALSE;
    $this->timeoutReached = FALSE;
    $this->startByte = 0;
    $this->lineLimit = 0;
    $this->lastLinePos = 0;
  }

  /**
   * Set this to TRUE if the parser should skip the first line of the CSV text,
   * which might be desired if the first line contains the column names.
   * By default, this is set to FALSE and the first line is not skipped.
   */
  public function setSkipFirstLine($skipFirstLine){
    $this->skipFirstLine = $skipFirstLine;
  }

  /**
   * Specify an array of column names if you know them in advance, or FALSE
   * (which is the default) to unset any prior column names. If no column names
   * are set, the parser will put each row into a simple numerically indexed
   * array. If column names are given, the parser will create arrays with
   * these column names as array keys instead.
   */
  public function setColumnNames($columnNames){
    $this->columnNames = $columnNames;
  }

  /**
   * Define the time (in milliseconds) after which the parser stops parsing,
   * even if it has not yet finished processing the CSV data. If the timeout
   * has been reached before parsing is done, the parse() method will return
   * an incomplete list of rows - a single row will never be cut off in the
   * middle, though. By default, no timeout (@p $timeout == FALSE) is defined.
   *
   * You can check if the timeout has been reached by calling the
   * timeoutReached() method after parse() has been called.
   */
  public function setTimeout($timeout){
    $this->timeout = $timeout;
  }

  /**
   * After calling the parse() method, determine if the timeout (set by the
   * setTimeout() method) has been reached.
   *
   * @deprecated Use lastLinePos() instead to determine whether a file has
   * finished parsing.
   */
  public function timeoutReached(){
    return $this->timeoutReached;
  }

  /**
   * Define the number of lines to parse in one parsing operation.
   *
   * By default, all lines of a file are being parsed.
   */
  public function setLineLimit($lines){
    $this->lineLimit = $lines;
  }

  /**
   * Get the byte number where the parser left off after last parse() call.
   *
   * @return
   * 0 if all lines or no line has been parsed, the byte position of where a
   * timeout or the line limit has been reached otherwise. This position can be
   * used to set the start byte for the next iteration after parse() has
   * reached the timeout set with setTimeout() or the line limit set with
   * setLineLimit().
   *
   * @see ParserExcel::setStartByte($start);
   */
  public function lastLinePos(){
    return $this->lastLinePos;
  }

  /**
   * Set the byte where file should be started to read.
   *
   * Useful when parsing a file in batches.
   */
  public function setStartLine($start){
    return $this->startLine = $start;
  }

  /**
   * Parse CSV files into a two dimensional array.
   *
   * @param Iterator $lineIterator
   * An Iterator object that yields line strings, e.g. ParserExcelIterator.
   * @param $start
   * The byte number from where to start parsing the file.
   * @param $lines
   * The number of lines to parse, 0 for all lines.
   * @return
   * Two dimensional array that contains the data in the CSV file.
   */
  public function parse(Iterator $lineIterator){
    $skipLine = $this->skipFirstLine;
    $rows = array();
    return $rows;
  }
}

/**
 * Contains CSV Parser.
 * Functions in this file are independent of the Feeds specific implementation.
 * Thanks to jpetso http://drupal.org/user/56020 for most of the code in this
 * file.
 */
/**
 * Text lines from file iterator.
 */
class ParserExcelIterator implements Iterator{

  private $currentLine;

  private $currentPos;

  private $phpexcel_obj;

  private $worksheet;

  public function __construct($filepath){
    $path = libraries_get_path('PHPExcel');
    require_once "$path/PHPExcel/IOFactory.php";
    $this->phpexcel_obj = PHPExcel_IOFactory::load($filepath);
    $this->currentLine = NULL;
    $this->currentPos = NULL;
  }

  public function rewind($pos = 0){
    // Not sure what to do here.
  }

  public function next(){
    if($this->handle){
      $this->currentLine = feof($this->handle) ? NULL : fgets($this->handle);
      $this->currentPos = ftell($this->handle);
      return $this->currentLine;
    }
  }

  public function valid(){
    return isset($this->currentLine);
  }

  public function current(){
    return $this->currentLine;
  }

  public function currentPos(){
    return $this->currentPos;
  }

  public function key(){
    return 'line';
  }
}