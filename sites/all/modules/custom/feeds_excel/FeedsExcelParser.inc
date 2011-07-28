<?php

/**
 * Parses a given file as a Excel file.
 */
class FeedsExcelParser extends FeedsParser{

  private $PHPExcel_object;

  private $columnNames;

  private $PHPExcel_ws;

  private $start;

  private $limit;

  /**
   * Parse content fetched by fetcher.
   * 
   * FIXME - Start and the pointer are not working properly due to the headers
   * being set and the pointer being set to "2".  This needs fixing so that it
   * works nicely with batch.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    $this->start = 1;
    $source_config = $source->getConfigFor($this);
    $state = $source->state(FEEDS_PARSE);
    // Load the PHPExcel shenanigans
    $path = libraries_get_path('PHPExcel');
    require_once "$path/PHPExcel/IOFactory.php";
    $this->PHPExcel_object = PHPExcel_IOFactory::load($fetcher_result->getFilePath());
    $PHPExcel_ws = array();
    foreach($this->PHPExcel_object->getWorksheetIterator() as $worksheet){
      $this->PHPExcel_ws[] = $worksheet;
    }
    if(empty($source_config['no_headers'])){
      // Get first line and use it for column names, convert them to lower case.
      $header = $this->parseHeader();
      if(!$header){return;}
      $this->setColumnNames($header);
    }
    // Determine section to parse, parse.
    //$start = $state->pointer ? $state->pointer : $parser->lastLinePos();
    $this->limit = $source->importer->getLimit();
    $rows = $this->parseItems();
    // Report progress.
    $state->total = $this->PHPExcel_ws[0]->getHighestRow();
    $state->pointer = $this->start;
    $state->progress($state->total, $state->pointer);
    // Create a result object and return it.
    return new FeedsParserResult($rows, $source->feed_nid);
  }

  protected function setColumnNames($columnNames){
    $this->columnNames = $columnNames;
  }

  /**
   * Get first line, and return as an array
   */
  protected function parseHeader(){
    $headers = array();
    foreach($this->PHPExcel_ws[0]->getRowIterator() as $row){
      $row_index = $row->getRowIndex();
      $cellIterator = $row->getCellIterator();
      $cellIterator->setIterateOnlyExistingCells(false); // Loop all cells, even if it is not set
      foreach($cellIterator as $cell){
        if(!is_null($cell)){
          $headers[] = trim(drupal_strtolower($cell->getCalculatedValue()));
        }
      }
      break;
    }
    $this->start = 2;
    return $headers;
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key){
    return parent::getSourceElement($source, $result, drupal_strtolower($element_key));
  }

  /**
   * Parse all of the items from the CSV.
   *
   * @param ParserExcel $parser
   * @param ParserExcelIterator $iterator
   * @return
   * An array of rows of the CSV keyed by the column names previously set
   */
  protected function parseItems(){
    $rows = array();
    foreach($this->PHPExcel_ws[0]->getRowIterator() as $row){
      $row_index = $row->getRowIndex();
      if($row_index >= $this->start){
        $cellIterator = $row->getCellIterator();
        $cellIterator->setIterateOnlyExistingCells(false); // Loop all cells, even if it is not set
        $column = 0;
        $row_values = array();
        foreach($cellIterator as $cell){
          if(!is_null($cell)){
            $row_values[count($this->columnNames) ? $this->columnNames[$column] : $column] = $cell->getCalculatedValue();
          }
          $column++;
        }
        $rows[] = $row_values;
        if(count($rows) >= $this->limit){
          break;
        }
        $this->start = $row_index;
      }
    }
    return $rows;
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state){
    return array(
      'no_headers' => array(
        '#type' => 'checkbox',
        '#title' => t('No headers'),
        '#description' => t('Check if the imported CSV file does not start with a header row. If checked, mapping sources must be named \'0\', \'1\', \'2\' etc.'),
        '#default_value' => $this->config['no_headers']
      )
    );
  }

  /**
   * Define default configuration.
   */
  public function configDefaults(){
    return array(
      'no_headers' => 0
    );
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults(){
    return array(
      'no_headers' => $this->config['no_headers']
    );
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources(){
    return FALSE;
  }

  /**
   * Source form.
   *
   * Show mapping configuration as a guidance for import form users.
   */
  public function sourceForm($source_config){
    $form = $sources = $uniques = array();
    $form['#weight'] = -10;
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    foreach($mappings as $mapping){
      $sources[] = check_plain($mapping['source']);
      if($mapping['unique']){
        $uniques[] = check_plain($mapping['source']);
      }
    }
    $output = t('Import !excel_files with one or more of these columns: !columns.', array(
      '!excel_files' => l(t('Excel files'), 'http://en.wikipedia.org/wiki/Microsoft_Excel'),
      '!columns' => implode(', ', $sources)
    ));
    $items = array(
      format_plural(count($uniques), t('Column <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.', array(
        '!column' => implode(', ', $uniques)
      )), t('Columns <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these column will be created.', array(
        '!columns' => implode(', ', $uniques)
      ))),
      $items[] = l(t('Download a template'), 'import/' . $this->id . '/template')
    );
    $form['help']['#markup'] = '<div class="help"><p>' . $output . '</p>' . theme('item_list', array(
      'items' => $items
    )) . '</div>';
    $form['no_headers'] = array(
      '#type' => 'checkbox',
      '#title' => t('No Headers'),
      '#description' => t('Check if the imported CSV file does not start with a header row. If checked, mapping sources must be named \'0\', \'1\', \'2\' etc.'),
      '#default_value' => isset($source_config['no_headers']) ? $source_config['no_headers'] : 0
    );
    return $form;
  }
}