<?php

/**
 * Parses a given file as a Excel file.  This is heavily inspired by Feeds
 * very own CSV parser.
 */
class FeedsExcelParser extends FeedsParser{

  private $PHPExcel_object;

  private $PHPExcel_ws;

  private $current_worksheet;

  private $limit;

  private $row_counts;

  private $state;

  /**
   * Parse content fetched by fetcher.
   * 
   * FIXME - Start and the pointer are not working properly due to the headers
   * being set and the pointer being set to "2".  This needs fixing so that it
   * works nicely with batch.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    // Get config and current state
    $source_config = $source->getConfigFor($this);
    $this->state = $source->state(FEEDS_PARSE);
    // Load the PHPExcel shenanigans
    if(is_null($this->PHPExcel_object)){
      if(function_exists('libraries_get_path')){
        $path = libraries_get_path('PHPExcel');
        $path = "$path/PHPExcel/IOFactory.php";
      }else{
        $path = drupal_get_path('module', 'feeds_xls') . '/PHPExcel/PHPExcel/IOFactory.php';
      }
      require_once $path;
      $this->PHPExcel_object = PHPExcel_IOFactory::load($fetcher_result->getFilePath());
      $PHPExcel_ws = array();
      foreach($this->PHPExcel_object->getWorksheetIterator() as $worksheet){
        $this->PHPExcel_ws[] = $worksheet;
        if(!$source_config['all_worksheets']){
          break;
        }
      }
      // Get the number of rows per sheet.  Note, sheets with only one row are
      // considered empty.
      $this->row_counts = array();
      foreach($this->PHPExcel_ws as $sheet){
        $number_of_rows = $sheet->getHighestRow();
        if(empty($source_config['no_headers'])){
          $number_of_rows--;
        }
        if($number_of_rows > 1){
          $this->row_counts[] = $number_of_rows;
        }else{
          // We stop at the first empty sheet, to make things simpler.
          break;
        }
      }
    }
    // Work out what sheet we are on.
    $total = 0;
    $this->current_worksheet = 0;
    foreach($this->row_counts as $count){
      $total += $count;
      if($this->state->pointer >= $total){
        $this->current_worksheet++;
        // Check that this worksheet exists, if it doesn't we should return
        if(!isset($this->PHPExcel_ws[$this->current_worksheet])){return new FeedsParserResult(array(), $source->feed_nid);}
      }
    }
    // We stop if we don't have any sheets with data in.
    if(!count($this->row_counts)){
      drupal_set_message(t('The Excel file you uploaded appears to be empty. Ensure the first sheet has data in it.'), 'error');
      return new FeedsParserResult(array(), $source->feed_nid);
    }
    // Set the limit
    // FIXME - I've increased the limit by four (200) as it appears to be much
    // quicker.  This may cause memory issues, so should be looked at.
    $this->limit = $source->importer->getLimit() * 4;
    // P A R S E !
    $rows = $this->parseItems($source_config);
    // Report progress.
    $this->state->total = array_sum($this->row_counts);
    $this->state->pointer += count($rows);
    $this->state->progress($this->state->total, $this->state->pointer);
    return new FeedsParserResult($rows, $source->feed_nid);
  }

  /**
   * Get first line, and return as an array
   */
  protected function parseHeader(){
    $headers = array();
    foreach($this->PHPExcel_ws[$this->current_worksheet]->getRowIterator() as $row){
      $row_index = $row->getRowIndex();
      $cellIterator = $row->getCellIterator();
      $cellIterator->setIterateOnlyExistingCells(false); // Loop all cells, even if it is not set
      foreach($cellIterator as $cell){
        if(!is_null($cell)){
          $headers[] = trim($cell->getCalculatedValue());
        }
      }
      break;
    }
    return $headers;
  }

  /**
   * Parse all of the items from the Excel file.
   *
   * @param ParserExcel $parser
   * @param ParserExcelIterator $iterator
   * @return
   * An array of rows of the Excel file keyed by the column names previously set
   */
  protected function parseItems($source_config){
    // We get the headers of the sheet we are working on, if required.
    if(empty($source_config['no_headers'])){
      $column_names = $this->parseHeader();
    }
    $rows = array();
    foreach($this->PHPExcel_ws[$this->current_worksheet]->getRowIterator() as $row){
      $row_index = $row->getRowIndex();
      $start_row = $this->getRowPosition($source_config);
      if($row_index >= $start_row){
        $cellIterator = $row->getCellIterator();
        $cellIterator->setIterateOnlyExistingCells(false); // Loop all cells, even if it is not set
        $column = 0;
        $row_values = array();
        foreach($cellIterator as $cell){
          if(!is_null($cell)){
            $row_values[count($column_names) ? $column_names[$column] : $column] = '' . $cell->getCalculatedValue();
          }
          $column++;
        }
        $rows[] = $row_values;
        if(count($rows) >= $this->limit){
          break;
        }
      }
    }
    return $rows;
  }

  /**
   * Calculate what row to start from.  Used by parseItems function
   */
  protected function getRowPosition($source_config){
    // Total imported - Total of previous worksheets
    // Note, we add one due to the fact that Excel sheets are numbered from 1
    // and not 0.
    $start_row = $this->state->pointer ? $this->state->pointer + 1 : 1;
    if($this->current_worksheet){
      for($i = 0; $i < $this->current_worksheet; $i++){
        $start_row -= $this->row_counts[$i];
      }
    }
    if(empty($source_config['no_headers'])){
      $start_row++;
    }
    return $start_row;
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state){
    return array(
      'no_headers' => array(
        '#type' => 'checkbox',
        '#title' => t('No headers'),
        '#description' => t('Check if the imported Excel file does not start with a header row. If checked, mapping sources must be named \'0\', \'1\', \'2\' etc.'),
        '#default_value' => $this->config['no_headers']
      ),
      'all_worksheets' => array(
        '#type' => 'checkbox',
        '#title' => t('All worksheets'),
        '#description' => t('Check this box if you would like to import data from all worksheets, and not just the first sheet.'),
        '#default_value' => $this->config['all_worksheets']
      )
    );
  }

  /**
   * Define default configuration.
   */
  public function configDefaults(){
    return array(
      'no_headers' => 0,
      'all_worksheets' => 0
    );
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults(){
    return array(
      'no_headers' => $this->config['no_headers'],
      'all_worksheets' => $this->config['all_worksheets']
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
      '#description' => t('Check if the imported Excel file does not start with a header row. If checked, mapping sources must be named \'0\', \'1\', \'2\' etc.'),
      '#default_value' => isset($source_config['no_headers']) ? $source_config['no_headers'] : 0
    );
    $form['all_worksheets'] = array(
      '#type' => 'checkbox',
      '#title' => t('All worksheets'),
      '#description' => t('Check this box if you would like to import data from all worksheets, and not just the first sheet.'),
      '#default_value' => $this->config['all_worksheets']
    );
    return $form;
  }
}