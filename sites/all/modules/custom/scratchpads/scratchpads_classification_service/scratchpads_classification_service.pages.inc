<?php

/**
 * Handles the search callback
 */
function scratchpads_classification_service_search($search){
  module_load_all_includes('scs.inc');
  $details = array();
  foreach(module_implements('scratchpads_classification_service_info') as $module){
    $details[$module] = module_invoke($module, 'scratchpads_classification_service_info');
  }
  $results = array();
  foreach($details as $module => $detail){
    foreach($detail as $identifier => $service){
      if(function_exists($service['search'])){
        $this_results = $service['search']($search);
        foreach($this_results as $key => $value){
          $results[url('classification/download/0/' . $module . '/' . $identifier . '/' . $key, array(
            'absolute' => TRUE
          ))] = $value . ' (' . $service['name'] . ')';
        }
      }
    }
  }
  return $results;
}

/**
 * Handles the download of the CSV/Excel file
 */
function scratchpads_classification_service_download($page, $module, $service, $id){
  if(module_exists($module)){
    module_load_include('scs.inc', $module);
    $details = module_invoke($module, 'scratchpads_classification_service_info');
    if($details && isset($details[$service])){
      if(function_exists($details[$service]['download'])){
        echo microtime(TRUE)."\n";
        $this_results = $details[$service]['download']($id, $page);
        echo microtime(TRUE)."\n";
        exit;
        // Load the csv file
        $temp_file = drupal_tempnam('temporary://', 'scratchpads_classification_service');
        $csvf = fopen($temp_file, 'a');
        $done_header = FALSE;
        $i = 0;
        while(($row = $this_results->fetchAssoc()) != FALSE){
          $i++;
          if(!$done_header){
            fputcsv($csvf, array_keys($row));
            $done_header = TRUE;
          }
          fputcsv($csvf, $row);
        }
        fclose($csvf);
        // Increase the memory limit
        ini_set('memory_limit', '1500M');
        ini_set('max_execution_time', 600);
        // Load the library, and set the cache type
        // TODO: We need to check for the size of the CSV file so that we can decide
        // if we need to save memory or not.
        // Load the library.
        if(($path = libraries_get_path('PHPExcel')) != FALSE){
          if(file_exists("$path/Classes/PHPExcel/IOFactory.php")){
            $path = "$path/Classes/PHPExcel/IOFactory.php";
          }elseif(file_exists("$path/PHPExcel/IOFactory.php")){
            $path = "$path/PHPExcel/IOFactory.php";
          }
          require_once $path;
        }
        // We use the IGBinary cache method, as it is the quickest.  If we run in to
        // memory issues, then the SQLite3 function is far more efficient with memory.
        $cache_method = PHPExcel_CachedObjectStorageFactory::cache_to_sqlite3;
        if(!PHPExcel_Settings::setCacheStorageMethod($cache_method)){
          $cache_method = PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp;
          PHPExcel_Settings::setCacheStorageMethod($cache_method);
        }
        // Open the file.
        $objReader = new PHPExcel_Reader_CSV();
        $objPHPExcel = $objReader->load($temp_file);
        // Redirect output to a client’s web browser (Excel5)
        header('Content-Type: application/vnd.ms-excel');
        header('Content-Disposition: attachment;filename="classification.xls"');
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
        $objWriter->save('php://output');
      }
    }
  }
}

/**
 * Basic function that seems to be missing to deliver JSON to the browser or to
 * another service.
 *
 * Copied from scratchpads_statistics.module
 *
 * Based on ajax_delver (see documentation there).
 */
function scratchpads_classification_service_json_deliver($json){
  if($json == MENU_ACCESS_DENIED){
    $json = array(
      'ACCESS DENIED'
    );
  }
  $iframe_upload = !empty($_POST['ajax_iframe_upload']);
  if(is_null(drupal_get_http_header('Content-Type'))){
    if(!$iframe_upload){
      drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
    }else{
      drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
    }
  }
  drupal_alter('json', $json);
  $json = drupal_json_encode($json);
  if(!$iframe_upload){
    print $json;
  }else{
    print '<textarea>' . $json . '</textarea>';
  }
  ajax_footer();
}