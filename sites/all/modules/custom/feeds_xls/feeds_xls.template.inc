<?php
define('FEEDS_EXCEL_CREATOR', 'Drupal - Excel parser');

/**
 * @file
 * 
 */
/**
 * Callback function to actually retrieve the file.
 */
function feeds_xls_get_populated_template(){
  // The path of the XLS file should be saved to a session.
  if(isset($_SESSION['feeds_xls']) && is_array($_SESSION['feeds_xls'])){
    // We should have the file path in the $results array, so we will output it
    // to the browser.
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="' . $_SESSION['feeds_xls']['filename'] . '"');
    header('Cache-Control: max-age=0');
    readfile($_SESSION['feeds_xls']['xls_path']);
    // delete the file
    drupal_unlink($_SESSION['feeds_xls']['xls_path']);
    unset($_SESSION['feeds_xls']);
    exit();
  }else{
    drupal_set_message(t('Generation of the populated Excel template has failed'), 'error');
    drupal_goto();
  }
}

/**
 * Callback function to provide the populated template.
 */
function feeds_xls_download_populated_template($feeds_importer, $bid = FALSE){
  get_class($feeds_importer->processor);
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(!$feeds_importer->parser->config['no_headers']){
      // We can go ahead, so here we create the batch and start it.
      $batch = array(
        'title' => t('Creating Populated Excel template'),
        'operations' => array(
          array(
            'feeds_xls_populate_template_with_data',
            array(
              $feeds_importer
            )
          )
        ),
        'finished' => 'feeds_xls_redirect_to_populated_file',
        'file' => drupal_get_path('module', 'feeds_xls') . '/feeds_xls.template.inc'
      );
      batch_set($batch);
      batch_process();
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

/**
 * Populate the XLS file with data.
 */
function feeds_xls_populate_template_with_data($feeds_importer, &$context){
  if(!isset($context['sandbox']['progress'])){
    // Calculate the total number of rows to add to this file.
    // Use the processor class to guess what entity type we're importing (this
    // relies on other coders following a similar convention to the feeds
    // module).
    $processor_class = get_class($feeds_importer->processor);
    $entity_guess = strtolower(substr($processor_class, 5, strpos($processor_class, 'Processor') - 5));
    $bundle = '';
    $context['sandbox']['entity'] = '';
    $context['sandbox']['entity_ids'] = array();
    $context['sandbox']['row'] = 0;
    switch($entity_guess){
      case 'node':
        $results = db_select('node', 'e')->fields('e', array(
          'nid'
        ))->condition('type', $feeds_importer->processor->config['content_type'])->execute();
        $entity_key = 'nid';
        $context['sandbox']['entity'] = 'node';
        break;
      case 'term':
        // The bundle for a term is actually the vid.
        $vocabulary = taxonomy_vocabulary_machine_name_load($feeds_importer->processor->config['vocabulary']);
        $results = db_select('taxonomy_term_data', 'e')->fields('e', array(
          'tid'
        ))->condition('vid', $vocabulary->vid)->execute();
        $entity_key = 'tid';
        $entity_guess = 'taxonomy_term';
        $context['sandbox']['entity'] = 'taxonomy_term';
        break;
      case 'file':
        $results = db_select('file_managed', 'e')->fields('e', array(
          'fid'
        ))->condition('type', $feeds_importer->processor->config['bundle'])->condition('uri', 'public://eolapi/%', 'NOT LIKE')->execute();
        $entity_key = 'fid';
        $context['sandbox']['entity'] = 'file';
        break;
    }
    $context['sandbox']['progress'] = 0;
    foreach($results as $row){
      $context['sandbox']['entity_ids'][] = $row->{$entity_key};
    }
    $context['sandbox']['max'] = count($context['sandbox']['entity_ids']);
    // Create the template before adding any rows to it.
    $context['sandbox']['xls_path'] = drupal_tempnam(file_directory_temp(), $feeds_importer->id);
    $context['results']['xls_path'] = $context['sandbox']['xls_path'];
    $context['results']['filename'] = $feeds_importer->id . '.xls';
    feeds_xls_download_template_helper($feeds_importer, $context['sandbox']['xls_path']);
    $context['finished'] = FALSE;
    $context['progress'] = 0;
    return;
  }
  // Load the library.
  if(function_exists('libraries_get_path')){
    $path = libraries_get_path('PHPExcel');
    $path = "$path/PHPExcel/IOFactory.php";
  }else{
    $path = drupal_get_path('module', 'feeds_xls') . '/PHPExcel/PHPExcel/IOFactory.php';
  }
  require_once $path;
  // Open the file.
  $objPHPExcel = PHPExcel_IOFactory::load($context['sandbox']['xls_path']);
  // Set active sheet index to the first sheet, so Excel opens this as the first sheet
  $objPHPExcel->setActiveSheetIndex(0);
  // Add the data.
  $loop = 0;
  $ids_to_load = array();
  while(($id = array_pop($context['sandbox']['entity_ids'])) != FALSE && $loop < 500){
    $ids_to_load[] = $id;
    $loop++;
  }
  $entities = entity_load($context['sandbox']['entity'], $ids_to_load);
  foreach($entities as $entity_id => $entity){
    foreach($feeds_importer->processor->config['mappings'] as $key => $mapping){
      $first_letter = floor(($key) / 26);
      $second_letter = ($key % 26) + 1;
      $column = chr(64 + $second_letter);
      if($first_letter){
        $column = chr(64 + $first_letter) . $column;
      }
      $cell = $column . ($context['sandbox']['row'] + 2);
      if(strpos($mapping['target'], ':')){
        // We currently know of two mappings (node:title and node:nid) that use
        // this format.  We'll try to work out which this one is.        
        $target = $mapping['target'];
        $target = explode(':', $target);
        // First we check to see if we have a value, and proceed if we do.
        if(isset($entity->{$target[0]}) && is_array($entity->{$target[0]}) && count($entity->{$target[0]})){
          // Next, if this is the "nid" mapping, we don't need to retrieve any
          // values, else if it is the title mapping, we need to retrieve the
          // node's title.
          // Loop through each of the values
          $cell_values = array();
          foreach($entity->{$target[0]} as $language => $values){
            foreach($entity->{$target[0]}[$language] as $value){
              if($target[1] == 'nid'){
                $cell_values[] = $value['nid'];
              }elseif($target[1] == 'uid'){
                $cell_values[] = $value['uid'];
              }
            }
          }
          $objPHPExcel->getActiveSheet()->setCellValue($cell, implode('|', $cell_values));
        }
      }else{
        switch($mapping['target']){
          case 'guid':
            // Look for this entity in the feeds_item table.  If not there, we
            // generate an entry for it.
            $guid = FALSE;
            if(isset($entity->uuid)){
              $guid = $entity->uuid;
            }
            $feeds_item = feeds_xls_get_or_generate_feeds_item_entry($context['sandbox']['entity'], $entity_id, $feeds_importer->id, $guid);
            $objPHPExcel->getActiveSheet()->setCellValue($cell, $feeds_item['guid']);
            break;
          case 'parentguid':
            // For some unknown reason, we don't have the parent of this entity,
            // so we need to query the taxonomy_term_hierarchy table.
            $row = db_select('taxonomy_term_hierarchy', 't')->fields('t')->condition('tid', $entity_id)->execute()->fetch();
            if($row && $row->parent){
              $parent_term = taxonomy_term_load($row->parent);
              $guid = FALSE;
              if(isset($parent_term->uuid)){
                $guid = $parent_term->uuid;
              }
              $feeds_item = feeds_xls_get_or_generate_feeds_item_entry($context['sandbox']['entity'], $parent_term->tid, $feeds_importer->id, $guid);
              $objPHPExcel->getActiveSheet()->setCellValue($cell, $feeds_item['guid']);
            }
            break;
          case 'parent':
            // We don't do anything for parent, as we use parentguid instead.
            break;
          default:
            if(isset($entity->{$mapping['target']})){
              if(is_array($entity->{$mapping['target']}) && count($entity->{$mapping['target']})){
                foreach($entity->{$mapping['target']} as $language => $values){
                  $cell_values = array();
                  $implosion = '|';
                  foreach($entity->{$mapping['target']}[$language] as $value){
                    foreach($value as $key => $item){
                      // Change to the allowed values array().
                      $field = field_info_field($mapping['target']);
                      if($field && @isset($field['settings']['allowed_values'][$item])){
                        $item = $field['settings']['allowed_values'][$item];
                      }
                      if($item){
                        switch($key){
                          case 'tid':
                            // Links to a term.  We need to load the term, and get the
                            // value
                            $term = taxonomy_term_load($item);
                            $cell_values[] = $term->name;
                            break;
                          case 'licence':
                            $licences = creative_commons_get_licence_types();
                            $cell_values[] = $licences[$item];
                            break;
                          default:
                            $cell_values[] = $item;
                            break;
                          // The following cases should not be saved to the file
                          case 'safe_value':
                          case 'format':
                            break;
                          // Handle gm3 fields together.
                          case 'latitude':
                          case 'longitude':
                          case 'region_id':
                          case 'polygon':
                          case 'polyline':
                          case 'rectangle':
                          case 'gm3_type':
                            $field = field_info_field($mapping['target']);
                            switch($field['type']){
                              // Polygon data.
                              case 'gm3_region':
                              case 'gm3_rectangle':
                              case 'gm3_polygon':
                              case 'gm3_polyline':
                                $implosion = "\n";
                                $cell_values[] = $item;
                                break;
                              case 'gm3_point':
                                // Only enter the data once!
                                if($key == 'latitude'){
                                  $implosion = "\n";
                                  $cell_values[] = "POINT:({$value['latitude']},{$value['longitude']})";
                                }
                                break 2;
                              case 'gm3_combination':
                                if($key == 'gm3_type'){
                                  // Get the field, and work out which one we have.
                                  $implosion = "\n";
                                  $type = array_pop(explode("_", $item));
                                  if($type == 'region'){
                                    // Fuck me, I am SHIT!
                                    if(isset($value['region_id']) && $value['region_id']){
                                      $cell_values[] = 'REGION:' . $value['region_id'];
                                    }
                                  }else{
                                    if(isset($value[$type]) && $value[$type]){
                                      $cell_values[] = strtoupper($type) . ':' . $value[$type];
                                    }
                                  }
                                }
                                break 2;
                            }
                        }
                      }
                    }
                  }
                  $objPHPExcel->getActiveSheet()->setCellValue($cell, implode($implosion, $cell_values));
                }
              }elseif(is_string($entity->{$mapping['target']})){
                $objPHPExcel->getActiveSheet()->setCellValue($cell, $entity->{$mapping['target']});
              }
            }
        }
      }
    }
    // Increase the row number
    $context['sandbox']['row']++;
  }
  // Close the file.
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  $objWriter->save($context['sandbox']['xls_path']);
  // Set the progress
  $context['progress'] = $context['sandbox']['row'] / $context['sandbox']['max'];
  $context['finished'] = $context['progress'];
  if(!count($context['sandbox']['entity_ids'])){
    $context['finished'] = TRUE;
  }
}

/**
 * Finish function for populate data.
 */
function feeds_xls_redirect_to_populated_file($success, $results, $operations){
  if($success){
    $_SESSION['feeds_xls'] = $results;
    drupal_goto('feeds_xls/getfile');
  }
}

/**
 * 
 */
/**
 * Callback function to provide the template.
 */
function feeds_xls_download_template($feeds_importer){
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(!$feeds_importer->parser->config['no_headers']){
      feeds_xls_download_template_helper($feeds_importer);
      exit();
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

/**
 * Help text for the import template.
 */
function feeds_xls_field_type_help_text($field){
  switch($field['type']){
    case 'date':
      return "Dates should be entered in the format 'YYYY/MM/DD'.  You may need to specify that this column is a 'text' column type to ensure the formatting is preserved.";
    case 'gm3_combination':
      return 'Please enter data in the following format, entering multiple values on a single line (this is easier to do in a text editor, and then paste into Excel):

{TYPE OF DATA}:{DATA}
REGION:{TDWG region code}
POINT:{latitude},{longitude}
POLYGON:{Well known text}
POLYLINE:{Well known text}
RECTANGLE:{Well known text}

e.g.

REGION:SPA-SP
REGION:FRA-FR
REGION:GER-OO
REGION:1
POINT:56.802292017627,-3.1201171875
POINT:53.2798967926641,3.0322265625
POLYGON:POLYGON ((-33.0029296875 60.37170546875135,-20.5224609375 60.84616839706054,-18.6767578125 59.22225529448783,-21.4892578125 55.926032385960966,-29.4873046875 55.579804150399035,-35.4638671875 57.08991816945666,-35.9912109375 59.22225529448783))';
    case 'default':
      return FALSE;
  }
}

function feeds_xls_download_template_helper($feeds_importer, $output = 'php://output'){
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
  $required_cells = array();
  foreach($feeds_importer->processor->config['mappings'] as $key => $header){
    $first_letter = floor(($key) / 26);
    $second_letter = ($key % 26) + 1;
    $cell = chr(64 + $second_letter);
    if($first_letter){
      $cell = chr(64 + $first_letter) . $cell;
    }
    $column = $cell;
    $cell = "{$cell}1";
    $objPHPExcel->getActiveSheet()->setCellValue($cell, $header['source']);
    // Set the columnn width
    $objPHPExcel->getActiveSheet()->getColumnDimension($column)->setAutoSize(TRUE);
    // Validation
    if(isset($feeds_importer->processor->config['content_type']) || isset($feeds_importer->processor->config['vocabulary'])){
      if(isset($feeds_importer->processor->config['content_type'])){
        $bundle = $feeds_importer->processor->config['content_type'];
        $entity_type = 'node';
      }else if(isset($feeds_importer->processor->config['vocabulary'])){
        $bundle = $feeds_importer->processor->config['vocabulary'];
        $entity_type = 'taxonomy_term';
      }
      if(strpos($header['target'], ':')){
        $field = field_info_field(array_shift(explode(':', $header['target'])));
      }else{
        $field = field_info_field($header['target']);
      }
      if($field){
        // Get the field instance, so that we can check if this field is required.
        $class = get_class($feeds_importer->processor);
        $field_instance = field_info_instance($entity_type, $field['field_name'], $bundle);
        $objValidation = $objPHPExcel->getActiveSheet()->getDataValidation($column . '2:' . $column . '65536');
        $objValidation->setAllowBlank($field_instance['required'] ? FALSE : TRUE);
        if($help_text = feeds_xls_field_type_help_text($field)){
          $objValidation->setPromptTitle(t('Help'));
          $objValidation->setPrompt($help_text);
        }
        if($field_instance['required']){
          $required_cells[] = $column;
          $objValidation->setPromptTitle(t('Required'));
          $objValidation->setPrompt(t('This cell may not be left blank') . $help_text ? $help_text : '');
        }
        $objValidation->setShowInputMessage(TRUE);
        $objValidation->setShowErrorMessage(TRUE);
        $allowed_values = array();
        if(isset($field['settings']['allowed_values_function']) && function_exists($field['settings']['allowed_values_function'])){
          $allowed_values = call_user_func($field['settings']['allowed_values_function'], $field);
        }else if(isset($field['settings']['allowed_values']) && is_array($field['settings']['allowed_values'])){
          $allowed_values = $field['settings']['allowed_values'];
          // Especially for the creative_commons module
          // FIXME - there must be a nicer way of doing this.
          if($field['type'] == 'creative_commons'){
            $allowed_values = creative_commons_get_licence_types();
          }
        }else if($field['type'] == 'node_reference'){
          // If the view name is set, we can not do anything.
          if(!$field['settings']['view']['view_name']){
            $results = db_select('node', 'n')->condition('type', $field['settings']['referenceable_types'])->fields('n', array(
              'nid',
              'title'
            ))->execute();
            $target_parts = explode(':', $header['target']);
            foreach($results as $row){
              $allowed_values[] = $row->$target_parts[1];
            }
          }
        }
        if(count($allowed_values)){
          $objValidation->setType(PHPExcel_Cell_DataValidation::TYPE_LIST);
          $objValidation->setErrorStyle(PHPExcel_Cell_DataValidation::STYLE_STOP);
          $objValidation->setShowDropDown(TRUE);
          $objValidation->setErrorTitle('Input error');
          $objValidation->setError(t('Your input did not match one of the allowed values.'));
          $objValidation->setPromptTitle(t('Allowed input'));
          foreach($allowed_values as $allowed_value){
            $num_sheets = count($objPHPExcel->getAllSheets());
            if($num_sheets == 1){
              $objPHPExcel->createSheet();
              $objPHPExcel->getSheet(1)->setTitle('PermittedValues');
            }
            $row = 0;
            if(is_array($allowed_value) && isset($allowed_value['vocabulary'])){
              // Get the vocabulary so we can get the vid.
              $vocabulary = taxonomy_vocabulary_machine_name_load($allowed_value['vocabulary']);
              if($vocabulary){
                $results = db_select('taxonomy_term_data', 't')->fields('t', array(
                  'name'
                ))->condition('vid', $vocabulary->vid)->distinct()->execute();
                foreach($results as $term){
                  $row++;
                  $objPHPExcel->getSheet(1)->setCellValue("{$column}{$row}", $term->name);
                }
              }
              $break_at_end = FALSE;
            }else{
              foreach($allowed_values as $term){
                $row++;
                $objPHPExcel->getSheet(1)->setCellValue("{$column}{$row}", $term);
              }
              $break_at_end = TRUE;
            }
            $objValidation->setPrompt(t("Only values from column '!column' in the 'PermittedValues' worksheet are allowed.", array(
              '!columnn' => $column
            )) . "\n\n" . ($field_instance['required'] ? t('This cell may not be left blank.') : t('This cell may be left blank.')));
            $objValidation->setFormula1('PermittedValues!$' . $column . '$1:$' . $column . '$' . ($row));
            if($break_at_end){
              break;
            }
          }
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
  // Set font colour for required cells
  foreach($required_cells as $required_cell){
    $objPHPExcel->getActiveSheet()->getStyle($required_cell . '1')->getFont()->applyFromArray(array(
      'name' => 'Arial',
      'bold' => TRUE,
      'italic' => FALSE,
      'color' => array(
        'rgb' => 'ff0000'
      ),
      'size' => 14
    ));
  }
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
  $objWriter->save($output);
}