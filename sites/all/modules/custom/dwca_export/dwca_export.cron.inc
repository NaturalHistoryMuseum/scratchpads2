<?php

/**
 * Helper for the hook_cron().
 */
function _dwca_export_cron(){
  if(variable_get('dwca_export_rebuild', TRUE) && variable_get('dwca_export_zip_build_time', 0) < time() + 86400){
    // Get a list of views on which we operate.  FIXME - This should perhaps be
    // "altered" using a drupal_alter call.
    module_load_include('views_default.inc', 'dwca_export');
    //$views = dwca_export_views_default_views();
    $views = array();
    foreach(module_implements('dwca_export') as $module){
      if(function_exists($module . '_views_default_views')){
        $func = $module . '_views_default_views';
        $views = array_merge($views, $func());
      }
    }
    // Array of files to add to the zip.
    $files_to_add = array();
    foreach($views as $view){
      // Filename based on the name of the view.
      $filename = 'public://' . substr($view->name, 12) . FILE_EXTENSION;
      // Get the view and render it.
      $view = views_get_view($view->name);
      $data = $view->render('views_data_export_1');
      $data_saved = FALSE;
      $files_to_add[$filename] = $filename;
      // the description.txt file is made up from lots of views.  We do special
      // things for the dwca_export_description* views
      if(substr($filename, 9, 11) == 'description'){
        // Unset the description_*.txt filename, and instead set description.txt
        unset($files_to_add[$filename]);
        $filename = 'public://description' . FILE_EXTENSION;
        $files_to_add[$filename] = $filename;
        // Append if it already exists
        if(file_exists($filename)){
          file_unmanaged_save_data(file_get_contents($filename) . $data, $filename, FILE_EXISTS_REPLACE);
          $data_saved = TRUE;
        }
      }
      if(!$data_saved){
        // Save the data.
        file_unmanaged_save_data($data, $filename, FILE_EXISTS_REPLACE);
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
    //$zip->addFile(drupal_get_path('module', 'dwca_export') . '/static/meta.xml', 'dwca/meta.xml');
    $meta_string = file_get_contents(drupal_get_path('module', 'dwca_export') . '/static/meta.xml');
    drupal_alter('dwca_export_meta_file', $meta_string);
    $zip->addFromString('dwca/meta.xml', $meta_string);
    // We need to close after each file is added.  Fuck knows why PHP requires
    // this.
    $zip->close();
    // add the csv data files
    foreach($files_to_add as $filename){
      $result = $zip->open($tmp_archive_file_name);
      $zip->addFile(drupal_realpath($filename), 'dwca/' . substr($filename, 9));
      $zip->close();
      file_unmanaged_delete($filename);
    }
    if(!file_unmanaged_move($tmp_archive_file_name, 'public://dwca.zip', FILE_EXISTS_REPLACE)){throw new Exception(t('Unable to move the DwC-A'));}
    variable_set('dwca_export_rebuild', FALSE);
  }
}