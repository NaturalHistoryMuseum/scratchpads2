<?php
define('FEEDS_EXCEL_CREATOR', 'Drupal - Excel parser');

/**
 * @file
 * 
 */
/**
 * Callback function to provide the template.
 */
function feeds_xls_download_template($feeds_importer){
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(!$feeds_importer->parser->config['no_headers']){
      feeds_xls_download_template_helper($feeds_importer);
    }else{
      drupal_set_message(t('The current configuration for this importer is set to use no headers.'), 'error');
      drupal_goto(arg(0) . '/' . arg(1));
    }
  }else{
    // This isn't an Excel importer.  Give an error message.
    drupal_set_message(t('The requested Feeds importer does not appear to be using the Excel parser.'), 'error');
    drupal_goto(arg(0) . '/' . arg(1));
  }
}

function feeds_xls_download_template_helper($feeds_importer){
  // Load the library.
  if(function_exists('libraries_get_path')){
    $path = libraries_get_path('PHPExcel');
    $path = "$path/PHPExcel/IOFactory.php";
  }else{
    $path = drupal_get_path('module', 'feeds_xls') . '/PHPExcel/PHPExcel/IOFactory.php';
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
    $first_letter = floor(($key) / 26);
    $second_letter = ($key % 26)+1;
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
      if($field){
        // Get the field instance, so that we can check if this field is required.
        $class = get_class($feeds_importer->processor);
        $field_instance = field_info_instance(strtolower(substr($class, 5, strlen($class) - 14)), $field['field_name'], $feeds_importer->processor->config['content_type']);
        $objValidation = $objPHPExcel->getActiveSheet()->getDataValidation($column . '2:' . $column . '65536');
        $objValidation->setAllowBlank($field_instance['required'] ? FALSE : TRUE);
        if($field_instance['required']){
          $objValidation->setPromptTitle(t('Required'));
          $objValidation->setPrompt(t('This cell may not be left blank'));
        }
        $objValidation->setShowInputMessage(TRUE);
        $objValidation->setShowErrorMessage(TRUE);
        if(isset($field['settings']['allowed_values']) && is_array($field['settings']['allowed_values']) && count($field['settings']['allowed_values'])){
          $objValidation->setType(PHPExcel_Cell_DataValidation::TYPE_LIST);
          $objValidation->setErrorStyle(PHPExcel_Cell_DataValidation::STYLE_STOP);
          $objValidation->setShowDropDown(TRUE);
          $objValidation->setErrorTitle('Input error');
          $objValidation->setError(t('Your input did not match one of the allowed values.'));
          $objValidation->setPromptTitle(t('Allowed input'));
          if(isset($field['settings']['allowed_values'][0]['vocabulary'])){
            $num_sheets = count($objPHPExcel->getAllSheets());
            if($num_sheets == 1){
              $objPHPExcel->createSheet();
              $objPHPExcel->getSheet(1)->setTitle('PermittedValues');
            }
            // Get the vocabulary so we can get the vid.
            $vocabulary = taxonomy_vocabulary_machine_name_load($field['settings']['allowed_values'][0]['vocabulary']);
            $results = db_select('taxonomy_term_data', 't')->fields('t', array(
              'name'
            ))->condition('vid', $vocabulary->vid)->distinct()->execute();
            $row = 0;
            foreach($results as $term){
              $row++;
              $objPHPExcel->getSheet(1)->setCellValue("{$column}{$row}", $term->name);
            }
            $objValidation->setPrompt(t("Only values from colum '!column' in the 'PermittedValues' worksheet are allowed.", array(
              '!column' => $column
            )) . "\n\n" . ($field_instance['required'] ? t('This cell may not be left blank.') : t('This cell may be left blank.')));
            $objValidation->setFormula1('PermittedValues!$' . $column . '$1:$' . $column . '$' . ($row));
          }else{
            $objValidation->setPrompt(t('Only one of the following values is permitted:') . "\n\n" . implode(", ", $field['settings']['allowed_values']) . "\n\n" . ($field_instance['required'] ? t('This cell may not be left blank.') : t('This cell may be left blank.')));
            $objValidation->setFormula1('"' . implode(',', $field['settings']['allowed_values']) . '"');
          }
        }
      }else{
        // Here we should set the title to be required, and also other random
        // fields that aren't standard "fields".
        // Set title to required.
        if($header['target'] == 'title' && strtolower(substr(get_class($feeds_importer->processor), 5, strlen(get_class($feeds_importer->processor)) - 14)) == 'node'){
          $objValidation = $objPHPExcel->getActiveSheet()->getDataValidation($column . '2:' . $column . '65536');
          $objValidation->setAllowBlank(FALSE);
          $objValidation->setPromptTitle(t('Required'));
          $objValidation->setPrompt(t('This cell may not be left blank'));
          $objValidation->setShowInputMessage(TRUE);
        }
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