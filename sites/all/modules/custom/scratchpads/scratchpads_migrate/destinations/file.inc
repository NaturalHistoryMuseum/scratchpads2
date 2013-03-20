<?php

/**
 * Destination class implementing migration into the files table.
 */
class ScratchpadsMigrateDestinationFile extends MigrateDestinationFile{

  public function import(stdClass $file, stdClass $row){
    scratchpads_migrate_unset_empty_fields($file);
    $status = parent::import($file, $row);
    // Add a path redirect
    if($status && isset($row->path) && !empty($row->path)){
      // Ensure the redirect module is enabled
      if(!function_exists('redirect_save')){
        module_enable(array(
          'redirect'
        ));
        module_load_include('module', 'redirect');
      }
      // Prepare redirection
      $fid = reset($status);
      if (redirect_load_by_source($row->path)) {
        // This can happen if a node with multiple images is being transformed into multiple files. Not much we can do about it.
        watchdog(WATCHDOG_ERROR, "Redirect path %path for file %file already exists, so will not be created.", array(
          '%file' => 'file/' . $fid,
          '%path' => $row->path
        ));
      } else {
        $redirect = (object)(array(
          'status_code' => 301,
          'source' => $row->path,
          'redirect' => 'file/' . $fid,
        ));
        redirect_object_prepare($redirect);
        redirect_save($redirect);
      }
    }
    return $status;
  }

  /**
   * Delete a file entry.
   *
   * @param array $fid
   *  Fid to delete, arrayed.
   */
  public function rollback(array $fid){
    parent::rollback($fid);
    // Delete the path
    $source = 'file/' . reset($fid);
    path_delete(array(
      'source' => $source
    ));
  }
}