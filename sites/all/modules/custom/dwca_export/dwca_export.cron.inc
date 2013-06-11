<?php
// File extension for the CSV files in the DwC-A file.
define('DWCA_EXTENSIONS_FILE_EXTENSION', ".txt");

function _dwca_export_cron($break = TRUE){
  // Ensure the directory has been created.
  @drupal_mkdir('public://dwca');
  // This only works from Drush, so return if we're not using Drush.
  if(!function_exists('drush_get_context')){return;}
  // Get a list of views on which to operate
  module_load_include('views_default.inc', 'dwca_export');
  $views = array();
  foreach(module_implements('dwca_export') as $module){
    if(function_exists($module . '_views_default_views')){
      $func = $module . '_views_default_views';
      $views = array_merge($views, $func());
    }
  }
  // We return a boolean to tell the calling function to build the zip.  If
  // we've had to create a CSV file, we don't create the zip
  $return = TRUE;
  // An array of the files to add to the ZIP at the end.
  $files_to_add = variable_get('dwca_export_files_to_add', array());
  // We do a maximum of five views at a time, to help reduce load on the server.
  $i = 0;
  foreach($views as $view){
    // Filename based on the name of the view with "dwca_export_" removed.
    $filename = substr($view->name, 12) . DWCA_EXTENSIONS_FILE_EXTENSION;
    if(!isset($files_to_add[$filename]) || !file_exists('public://dwca/' . $filename)){
      // Get the view and render it.
      $output_filename = drupal_realpath('public://dwca') . '/' . $filename;
      // FIXME - Why does drush_backend_fork not work for us?  We need to find
      // out why and fix it.  This may well be fixed when we upgrade to Drush 5,
      // so we should look at this then.
      /*drush_backend_fork('vde', array(
        $view->name,
        'views_data_export_1',
        $output_filename
      ));*/
      global $base_url;
      drush_log(dt("Forking: drush vde {$view->name} views_data_export_1 $output_filename"), 'success');
      exec("nohup drush @" . parse_url($base_url, PHP_URL_HOST) . " vde {$view->name} views_data_export_1 {$output_filename}");
      // FIXME-END
      $files_to_add[$filename] = $filename;
      variable_set('dwca_export_files_to_add', $files_to_add);
      $return = FALSE;
      $i++;
      if($break && $i >= 5){
        break;
      }
    }
  }
  return $return;
}

/**
 * Create the zip file
 */
function _dwca_export_cron_create_zip(){
  // First we combine all of the description files
  // the description.txt file is made up from lots of views.  We do special
  // things for the dwca_export_description* views
  $filenames = variable_get('dwca_export_files_to_add', array());
  foreach($filenames as $key => $filename){
    if(substr($filename, 0, 12) == 'description_'){
      // This file doesn't get added to the zip, so we remove it.
      unset($filenames[$key]);
      $data = file_get_contents('public://dwca/' . $filename);
      drupal_unlink('public://dwca/' . $filename);
      $filename = 'description' . DWCA_EXTENSIONS_FILE_EXTENSION;
      $filenames[$filename] = $filename;
      // Append if it already exists
      if(file_exists('public://dwca/' . $filename)){
        $data .= file_get_contents('public://dwca/' . $filename);
      }
      $saved = file_put_contents('public://dwca/' . $filename, $data, FILE_EXISTS_REPLACE);
    }
  }
  // If we have reached here, then we have all the files we require, and can
  // therefore build the zip file and delete the files.
  variable_set('dwca_export_zip_build_time', time());
  $tmp_archive_file_name = drupal_realpath(drupal_tempnam("temporary://", "dwca_export_archive_"));
  // Unfortunately we cannot use drupals ArchiverZip because there ís
  // no way to pass in ZipArchive::CREATE to the constructor to create the archive
  // TODO test if zip functionality is available (i.e. if(function_exists('zip_open'))
  // but I don't know where the proper location for such a check would be
  $zip = new ZipArchive();
  if(!$zip->open($tmp_archive_file_name, ZipArchive::CREATE)){throw new Exception(t('Could not create temporary zip_archive for DwC-A'));}
  // add metadata
  $meta_string = file_get_contents(drupal_get_path('module', 'dwca_export') . '/static/meta.xml');
  drupal_alter('dwca_export_meta_file', $meta_string);
  $zip->addFromString('dwca/meta.xml', $meta_string);
  // We need to close after each file is added.  Fuck knows why PHP requires
  // this.
  $zip->close();
  // add the csv data files
  foreach($filenames as $filename){
    if($filename){
      $result = $zip->open($tmp_archive_file_name);
      // Sort the file and ensure we have unique lines
      $real_path = drupal_realpath('public://dwca/' . $filename);
      exec("sort -u $real_path > {$real_path}.sorted && mv {$real_path}.sorted $real_path");
      $zip->addFile($real_path, 'dwca/' . $filename);
      $zip->close();
      drupal_unlink('public://dwca/' . $filename);
    }
  }
  if(!file_unmanaged_move($tmp_archive_file_name, 'public://dwca.zip', FILE_EXISTS_REPLACE)){throw new Exception(t('Unable to move the DwC-A'));}
  variable_del('dwca_export_files_to_add');
  variable_set('dwca_export_rebuild', FALSE);
}