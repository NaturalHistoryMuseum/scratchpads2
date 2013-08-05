<?php

/**
 * Destination class implementing migration into the files table.
 */
class ScratchpadsMigrateDestinationFile extends MigrateDestinationFile{

  public function import(stdClass $file, stdClass $row){
    scratchpads_migrate_unset_empty_fields($file);
    $status = parent::import($file, $row);
    if (!$status) {
      return;
    }
    $fid = reset($status);
    $sfile = file_load($fid);
    // Overwrite the gallery mapping. As we are now using VIDs as the main key in the tables,
    // this will have mapped to the VID. We want the NID instead.
    if ($sfile && property_exists($sfile, 'field_smg_galleries') && !empty($sfile->field_smg_galleries['und'])) {
      $save_me = FALSE;
      foreach ($sfile->field_smg_galleries['und'] as $k => $v) {
        $g_nid = db_query("
          SELECT nid
            FROM {node}
          WHERE vid = :vid
            AND nid <> vid
        ", array(':vid' => $v['nid']))->fetchField();
        if ($g_nid) {
          $sfile->field_smg_galleries['und'][$k]['nid'] = $g_nid;
          $save_me = TRUE;
        }
      }
      if ($save_me) {
        file_save($sfile);
      }
    }
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