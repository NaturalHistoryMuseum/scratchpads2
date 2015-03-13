<?php
feeds_xls_load_phpexcel();

/**
 * Chunk reader for reading data from an Excel file.
 */
class chunkReadFilter implements PHPExcel_Reader_IReadFilter{
  // Start row
  private $start_row = 0;
  // End row
  private $end_row = 0;
  // Set the rows to parse.
  public function setRows($start, $size){
    if($start > 0){
      $start++;
    }
    $this->start_row = $start;
    $this->end_row = $start + $size;
  }
  // Implements function.  Define whether or not we should read the contents of
  // the cell.
  public function readCell($column, $row, $worksheetName = ''){
    if($row > $this->start_row && $row <= $this->end_row){return TRUE;}
    return FALSE;
  }
}

/**
 * Parses a given file as a Excel file.  This is heavily inspired by Feeds
 * very own CSV parser.
 */
class FeedsExcelParser extends FeedsParser{

  private $state;

  private $chunk_size = 50;

  private $chunk_filter;

  private $reader;

  /**
   * Parse content fetched by fetcher.
   * 
   * FIXME - Start and the pointer are not working properly due to the headers
   * being set and the pointer being set to "2".  This needs fixing so that it
   * works nicely with batch.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    // Increase the memory limit
    ini_set('memory_limit', '1500M');
    ini_set('max_execution_time', 240);
    // Get config and current state
    $source_config = $source->getConfigFor($this);
    $this->state = $source->state(FEEDS_PARSE);
    // Set the total rows to process based on the configuration.
    $this->state->total = $source_config['max_rows'];
    // Get the filepath of the submitted file.
    $import_file_path = drupal_realpath($fetcher_result->getFilePath());
    // Load the PHPExcel shenanigans
    // Note, we're caching this in a file as PDO/MySQL complain when it is 
    // included in the $this->state object (dodgy characters).
    if(!$this->state->reader_object_cache_path || !file_exists($this->state->reader_object_cache_path)){
      // We use drupal_realpath here as it appears that the ZipArchive class is
      // not able to handle our use of public://... Stream URIs.  This will need
      // to be fixed before Drupal 8.
      $file_type = PHPExcel_IOFactory::identify($import_file_path);
      $this->reader = PHPExcel_IOFactory::createReader($file_type);
      // No need to check for the presence of the chunkReadFilter function, if
      // it doesn't exist, we're screwed anyway.
      //if(method_exists($this->reader, 'setReadFilter'))
      $this->chunk_filter = new chunkReadFilter();
      $this->reader->setReadFilter($this->chunk_filter);
      if(method_exists($this->reader, 'setReadDataOnly')){
        $this->reader->setReadDataOnly(true);
      }
      $tempnam = drupal_tempnam('temporary://', 'feeds_xls_data_');
      chmod($tempnam, 0660);
      file_put_contents($tempnam, serialize($this));
      $this->state->reader_object_cache_path = $tempnam;
      // Set pointer to 0
      $this->state->pointer = 0;
    }else{
      $previous_this = unserialize(file_get_contents($this->state->reader_object_cache_path));
      $this->reader = $previous_this->reader;
      $this->chunk_filter = $previous_this->chunk_filter;
    }
    // Upper case the mappings
    foreach($source->importer->processor->config['mappings'] as $key => $mapping){
      $source->importer->processor->config['mappings'][$key]['source'] = strtoupper($mapping['source']);
    }
    // P A R S E !
    $rows = array();
    $this->chunk_filter->setRows($this->state->pointer, $this->chunk_size);
    // Excel object for doing the do.
    $excel_obj = $this->reader->load($import_file_path);
    $excel_obj->setActiveSheetIndex();
    foreach($excel_obj->getActiveSheet()->getRowIterator() as $row){
      $cellIterator = $row->getCellIterator();
      $cellIterator->setIterateOnlyExistingCells(false); // Loop all cells, even if it is not set
      $column = 0;
      $row_values = array();
      foreach($cellIterator as $cell){
        if(!is_null($cell)){
          if(!isset($this->state->column_names)){
            $row_values[] = strtoupper(trim($cell->getCalculatedValue()));
          }else{
            if(strlen(trim('' . $cell->getCalculatedValue())) || (count($this->state->column_names) ? $this->state->column_names[$column] : $column) == 'GUID'){
              $row_values[count($this->state->column_names) ? $this->state->column_names[$column] : $column] = '' . $cell->getCalculatedValue();
            }
          }
        }
        $column++;
      }
      // A special case.  We need to ensure that the GUID column has a value
      // defined, even if that value is the empty string.
      if(count($row_values) && $column < count($this->state->column_names) && !isset($row_values['GUID']) && array_search('GUID', $this->state->column_names)){
        $row_values['GUID'] = '';
      }
      if(!isset($this->state->column_names)){
        $this->state->column_names = $row_values;
      }else{
        if($this->isRowNotEmpty($row_values)){
          $rows[] = $row_values;
        }
      }
    }
    // Set the total here, most likely we've finished!
    if(!count($rows)){
      $this->state->total = $this->state->pointer - 1;
      @drupal_unlink($this->state->reader_object_cache_path);
    }
    // Try to regain some memory!
    $excel_obj->disconnectWorksheets();
    unset($excel_obj);
    if(function_exists('gc_collect_cycles')){
      gc_collect_cycles();
    }
    // Report progress.
    $this->state->pointer += count($rows);
    $this->state->progress($this->state->total, $this->state->pointer);
    // FIXME - does this need $source->feed_nid ??
    return new FeedsParserResult($rows, $source->feed_nid);
  }

  /**
   * Check to see if an array is all empty.
   */
  protected function isRowNotEmpty($row){
    if(is_array($row)){
      foreach($row as $key => $value){
        if($value){return TRUE;}
      }
    }
    return FALSE;
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state){
    return array(
      'all_worksheets' => array(
        '#type' => 'checkbox',
        '#title' => t('All worksheets'),
        '#description' => t('Check this box if you would like to import data from all worksheets, and not just the first sheet.'),
        '#default_value' => $this->config['all_worksheets']
      ),
      'max_rows' => array(
        '#type' => 'select',
        '#title' => t('Maximum number of rows'),
        '#description' => t('Select the maximum number of rows your Excel file contains.  This number MUST be greater than the total number of rows in your file.  Selecting this value will improve the progress bar feedback.'),
        '#default_value' => $this->config['all_worksheets'],
        '#options' => array(
          100 => 100,
          1000 => 1000,
          5000 => 5000,
          10000 => 10000,
          20000 => 20000,
          30000 => 30000,
          40000 => 40000,
          50000 => 50000,
          65535 => t('65535 XLS Maximum'),
          1000000000000 => t('Unlimited. NO PROGRESS FEEDBACK')
        )
      )
    );
  }

  /**
   * Define default configuration.
   */
  public function configDefaults(){
    return array(
      'all_worksheets' => 0,
      'max_rows' => 65535
    );
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults(){
    return array(
      'all_worksheets' => $this->config['all_worksheets'],
      'max_rows' => $this->config['max_rows']
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
      if(!empty($mapping['unique'])){
        $uniques[] = check_plain($mapping['source']);
      }
    }
    if(substr($this->id, 0, 5) == 'file_'){
      $empty_template_text = '';
    }else{
      $empty_template_text = t('<a href="!url" target="_blank" class="overlay-exclude">Excel template file</a> for this import.  Your template file should contain the following column headers:', array(
        '!url' => url('import/' . $this->id . '/xlstemplate')
      )) . '<br/><span style="font-size:80%">' . implode(", ", $sources) . '</span>';
    }
    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Template'),
      'text' => array(
        '#markup' => '<div class="help"><p>' . $empty_template_text . '</p><p>' . t('<a href="!popurl" target="_blank" class="overlay-exclude">Excel template file</a> for updating data already present on your site. <span class="error">Please ensure all data from your site is present in your downloaded file.</span></p>', array(
          '!popurl' => url('import/' . $this->id . '/populated-template')
        )) . '</div>'
      )
    );
    $form['all_worksheets'] = array(
      '#prefix' => '<div style="display:none">',
      '#suffix' => '</div>',
      '#type' => 'checkbox',
      '#title' => t('All worksheets'),
      '#description' => t('Check this box if you would like to import data from all worksheets, and not just the first sheet.'),
      '#default_value' => $this->config['all_worksheets']
    );
    $form['max_rows'] = array(
      '#type' => 'select',
      '#title' => t('Maximum number of rows'),
      '#description' => t('Select the maximum number of rows your Excel file contains.  This number MUST be greater than the total number of rows in your file.  Selecting this value will improve the progress bar feedback.'),
      '#default_value' => 65535,
      '#options' => array(
        100 => 100,
        1000 => 1000,
        5000 => 5000,
        10000 => 10000,
        20000 => 20000,
        30000 => 30000,
        40000 => 40000,
        50000 => 50000,
        65535 => t('65535 XLS Maximum'),
        1000000000000 => t('Unlimited. NO PROGRESS FEEDBACK')
      )
    );
    return $form;
  }
}
