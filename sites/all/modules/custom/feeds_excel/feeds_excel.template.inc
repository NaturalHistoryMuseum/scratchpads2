<?php
define('FEEDS_EXCEL_CREATOR', 'Drupal - Excel parser');

/**
 * @file
 * 
 */
/**
 * Callback function to provide the template.
 */
function feeds_excel_download_template($feeds_importer){
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(!$feeds_importer->parser->config['no_headers']){
      feeds_excel_download_template_helper($feeds_importer);
    }else{
      drupal_set_message('The current configuration for this importer is set to use no headers.', 'error');
      drupal_goto(arg(0) . '/' . arg(1));
    }
  }else{
    // This isn't an Excel importer.  Give an error message.
    drupal_set_message('The requested Feeds importer does not appear to be using the Excel parser.', 'error');
    drupal_goto(arg(0) . '/' . arg(1));
  }
}

function feeds_excel_download_template_helper($feeds_importer){
  // Load the library.
  if(function_exists('libraries_get_path')){
    $path = libraries_get_path('PHPExcel');
    $path = "$path/PHPExcel/IOFactory.php";
  }else{
    $path = drupal_get_path('module', 'feeds_excel') . '/PHPExcel/PHPExcel/IOFactory.php';
  }
  require_once $path;
  date_default_timezone_set(drupal_get_user_timezone());
  // Create new PHPExcel object
  $objPHPExcel = new PHPExcel();
  // Set properties
  $title = t('Template for !feeds_name', array(
    '!feeds_name' => $feeds_importer->config['name']
  ));
  $objPHPExcel->getProperties()->setCreator(FEEDS_EXCEL_CREATOR)->setLastModifiedBy(FEEDS_EXCEL_CREATOR)->setTitle($title);
  // Set active sheet index to the first sheet, so Excel opens this as the first sheet
  $objPHPExcel->setActiveSheetIndex(0);
  // Add some data
  // Add the Column headers.
  foreach($feeds_importer->processor->config['mappings'] as $key => $header){
    $first_letter = floor(($key + 1) / 26);
    $second_letter = ($key + 1) % 26;
    $cell = chr(64 + $second_letter);
    if($first_letter){
      $cell = chr(64 + $first_letter) . $cell;
    }
    $column = $cell;
    $cell = "{$cell}1";
    $objPHPExcel->getActiveSheet()->setCellValue($cell, $header['source']);
    // Set the column width
    $objPHPExcel->getActiveSheet()->getColumnDimension($column)->setAutoSize(TRUE);
    // Validation
    if(isset($feeds_importer->processor->config['content_type'])){
      $field = field_info_field($header['target']);
      if($field && isset($field['settings']['allowed_values']) && is_array($field['settings']['allowed_values']) && count($field['settings']['allowed_values'])){
        $objValidation = $objPHPExcel->getActiveSheet()->getDataValidation($column.'2:'.$column.'65536');
        $objValidation->setType(PHPExcel_Cell_DataValidation::TYPE_LIST);
        $objValidation->setErrorStyle(PHPExcel_Cell_DataValidation::STYLE_STOP);
        $objValidation->setAllowBlank(FALSE);
        $objValidation->setShowInputMessage(TRUE);
        $objValidation->setShowErrorMessage(TRUE);
        $objValidation->setShowDropDown(TRUE);
        $objValidation->setErrorTitle('Input error');
        $objValidation->setError("Your input did not match one of the allowed values.");
        $objValidation->setPromptTitle('Allowed input');
        $objValidation->setPrompt("Only one of the following values is permitted:\n\n" . implode(", ", $field['settings']['allowed_values']));
        $objValidation->setFormula1('"' . implode(',', $field['settings']['allowed_values']) . '"');
      }
    }
  }
  // setAutoSize for the row
  $objPHPExcel->getActiveSheet()->getRowDimension(1)->setRowHeight(30);
  // Set the font for the header row
  $objPHPExcel->getActiveSheet()->getStyle("A1:$cell")->getFont()->applyFromArray(array(
    'name' => 'Arial',
    'bold' => TRUE,
    'italic' => FALSE,
    'color' => array(
      'rgb' => 'ffffff'
    ),
    'size' => 14
  ));
  // Set the background colour for the header row.
  $objPHPExcel->getActiveSheet()->getStyle("A1:$cell")->getFill()->applyFromArray(array(
    'type' => PHPExcel_Style_fill::FILL_SOLID,
    'color' => array(
      'rgb' => '000000'
    )
  ));
  // Size the cells accordingly.
  $objPHPExcel->getActiveSheet()->getStyle("A1:$cell")->getAlignment()->applyFromArray(array(
    'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
    'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER,
    'rotation' => 0,
    'wrap' => FALSE,
    'shrinkToFit' => FALSE
  ));
  // Rename sheet
  $objPHPExcel->getActiveSheet()->setTitle('Template');
  // Redirect output to a client’s web browser (Excel5)
  header('Content-Type: application/vnd.ms-excel');
  $filename = str_replace(' ', '_', preg_replace('/[^a-z\ ]/', '', strtolower($feeds_importer->config['name'])));
  header('Content-Disposition: attachment;filename="TEMPLATE-' . $filename . '.xls"');
  header('Cache-Control: max-age=0');
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  $objWriter->save('php://output');
  exit();
}