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
    // Increase the memory limit
    ini_set('memory_limit', '512M');
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
      // We use drupal_realpath here as it appears that the ZipArchive class is
      // not able to handle our use of public://... Stream URIs.  This will need
      // to be fixed before Drupal 8.
      $import_file_path = drupal_realpath($fetcher_result->getFilePath());
      $file_type = PHPExcel_IOFactory::identify($import_file_path);
      $reader = PHPExcel_IOFactory::createReader($file_type);
      if(method_exists($reader, 'setReadDataOnly')){
        $reader->setReadDataOnly(true);
      }
      $this->PHPExcel_object = $reader->load($import_file_path);
      $PHPExcel_ws = array();
      foreach($this->PHPExcel_object->getWorksheetIterator() as $worksheet){
        if($worksheet->getTitle() != 'PermittedValues'){
          $this->PHPExcel_ws[] = $worksheet;
        }
        if(!$source_config['all_worksheets']){
          break;
        }
      }
      // Get the number of rows per sheet
      $this->row_counts = array();
      foreach($this->PHPExcel_ws as $sheet){
        $number_of_rows = $sheet->getHighestRow();
        if(isset($source_config['no_headers'])){
          $number_of_rows--;
        }
        if($number_of_rows >= 1){
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
    $this->limit = $source->importer->getLimit();
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
            // FIXME - This isn't ideal, why does the GUID field need to be empty.
            if(trim('' . $cell->getCalculatedValue()) || (count($column_names) ? $column_names[$column] : $column) == 'GUID'){
              $row_values[count($column_names) ? $column_names[$column] : $column] = '' . $cell->getCalculatedValue();
            }
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
    $form['help']['#markup'] = '<div class="help"><p style="font-size:110%">' . t('<a href="!url">Download</a> an Excel template file for this import.  Your template file should contain the following column headers:', array(
      '!url' => url('import/' . $this->id . '/template', array(
        'attributes' => array(
          'target' => '_blank'
        )
      ))
    )) . '<br/><span style="font-size:80%">' . implode(", ", $sources) . '</span></p></div>';
    $form['no_headers'] = array(
      '#prefix' => '<div style="display:none">',
      '#suffix' => '</div>',
      '#type' => 'checkbox',
      '#title' => t('No Headers'),
      '#description' => t('Check if the imported Excel file does not start with a header row. If checked, mapping sources must be named \'0\', \'1\', \'2\' etc.'),
      '#default_value' => isset($source_config['no_headers']) ? $source_config['no_headers'] : 0
    );
    $form['all_worksheets'] = array(
      '#prefix' => '<div style="display:none">',
      '#suffix' => '</div>',
      '#type' => 'checkbox',
      '#title' => t('All worksheets'),
      '#description' => t('Check this box if you would like to import data from all worksheets, and not just the first sheet.'),
      '#default_value' => $this->config['all_worksheets']
    );
    return $form;
  }
}
