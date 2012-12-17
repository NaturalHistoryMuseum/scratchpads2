<?php

/**
 * Destination class implementing migration into the files table.
 */
class ScratchpadsMigrateDestinationFile extends MigrateDestinationFile{

  public function import(stdClass $file, stdClass $row){
    scratchpads_migrate_unset_empty_fields($file);
    $status = parent::import($file, $row);
    // Add a path
    if($status && isset($row->path) && !empty($row->path)){
      $fid = reset($status);
      $path = array(
        'source' => 'file/' . $fid,
        'alias' => $row->path
      );
      path_save($path);
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