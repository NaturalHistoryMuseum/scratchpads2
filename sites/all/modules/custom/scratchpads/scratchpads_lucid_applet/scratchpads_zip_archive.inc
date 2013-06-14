<?php

/**
 * class ScratchpadsZipArchive
 *
 * This class is used to extract and manage ZIP archives on demand.
 *
 * The expected workflow is that the archive file is managed by Drupal,
 * but the extracted files are not - they are generated on demand, and
 * are re-created if they go missing.
 *
 * The archives is extracted in a folder composed of the basename of the
 * archive itself and the associated file id, typically in a subfolder
 * of the 'files' folder.
 *
 * As the extracted files are not managed by Drupal, it is up to the
 * user of the class to clean up when needed - for instance when the
 * archive file is deleted, by implementing hook_file_delete.
 *
 * Example usage:
 *
 * $archive = new ScratchpadsZipArchive($file); // File can be a file object or a fid
 * $archive->extract(); // Will only extract if needed
 * foreach ($archive->extractedFiles() as $file_uri) {
 *   // ...
 * }
 *
 * Note that this does not support extracting to a non-local folder.
 *
 */
class ScratchpadsZipArchive{
  /* For safety (in particular when deleting files), this setting
   * limits the allowed wrappers.
  */
  static $allowedWrappers = array(
    'public',
    'private',
    'temporary'
  );
  /* Defaults */
  const DEFAULT_ROOT = 'public://extracted';

  const DEFAULT_MODE = 0775;
  /* The file object representing the archive */
  private $archive;
  /* The extraction folder */
  private $root;
  /* The list of extracted files */
  private $extractedFiles;
  /* The mode we use when extracting */
  private $mode;

  /**
   * function __construct
   *
   * Create a new ScratchpadsArchive object from an
   * ZIP file.
   *
   * $archive can be a file object, a fid, or an array
   * defining a 'fid' element. The mimetype of the file 
   * must be 'application/zip'.
   *
   * $root is the extraction base, and if ScratchpadsZipArchive::$allowedWrappers
   * is not empty, then $root must be within one of the defined wrappers.
   *
   * An exception is thrown on error.
   *
   */
  public function __construct($archive, $root = ScratchpadsZipArchive::DEFAULT_ROOT, $mode = ScratchpadsZipArchive::DEFAULT_MODE){
    if(!is_object($archive)){
      if(is_array($archive) && isset($archive['fid'])){
        $fid = (int)($archive['fid']);
      }else{
        $fid = (int)($archive);
      }
      $archive = file_load($fid);
    }
    if(!$archive){throw new Exception("No file could be found to extract from");}
    if($archive->filemime != 'application/zip'){throw new Exception("File $archive->fid is not a ZIP archive");}
    if(!empty(ScratchpadsZipArchive::$allowedWrappers) && !in_array(file_uri_scheme($root), ScratchpadsZipArchive::$allowedWrappers)){throw new Exception("Disalowed wrapper");}
    $this->archive = $archive;
    $this->root = $root . '/' . drupal_basename($archive->uri) . '.' . $archive->fid;
    $this->mode = $mode;
    $this->extractedFiles = NULL;
  }

  /**
   * function needsUpdate
   *
   * Returns TRUE if the archive needs creating/updating
   */
  public function needsUpdate(){
    if(!$this->folderExists()){return TRUE;}
    return filemtime($this->root) <= filemtime($archive->uri);
  }

  /**
   * function archiveFid
   *
   * Returns the fid of the archive file object
   */
  public function archiveFid(){
    return $this->archive->fid;
  }

  /**
   * function folderExists
   *
   * Returns TRUE if the extraction folder exists
   */
  public function folderExists(){
    return file_exists($this->root) && is_dir($this->root);
  }

  /**
   * function extract
   * 
   * Extract the archive to the extraction root.
   * 
   * If $extensions is set (as an array of file extensions
   * without the leading dot), then only files matching
   * the extensions will be extracted.
   * 
   * Unless $force is set to TRUE, this will only
   * extract the archive if the extraction folder is
   * missing, or it's modification time older than the
   * archive file itself (this is true even if
   * the list of allowed extensions has changed).
   * 
   * Returns the list of extracted files as an array
   * of file uris.
   */
  public function extract($extensions = array(), $force = FALSE){
    if(file_exists($this->root)){
      if(!$force && !$this->needsUpdate()){return $this->extractedFiles();}
      $this->deleteExtractedFiles();
    }
    $this->createFolder();
    $zip = new ZipArchive();
    $this->extractedFiles = array();
    if($zip->open(drupal_realpath($this->archive->uri)) !== TRUE){throw new Exception("File $archive->filename is not a ZIP file");}
    for($i = 0; $i < $zip->numFiles; $i++){
      $file_name = $zip->getNameIndex($i);
      $file_info = pathinfo($file_name);
      if(!preg_match('|/$|', $file_name)){
        if($extensions && !in_array($file_info['extension'], $extensions)){
          continue;
        }
        $this->extractedFiles[] = $this->root . '/' . $file_name;
      }
      $zip->extractTo(drupal_realpath($this->root), array(
        $file_name
      ));
      drupal_chmod($this->root . '/' . $file_name, $this->mode);
    }
    return $this->extractedFiles;
  }

  /**
   * function extractedFiles
   * 
   * Returns the list of extracted files as an array of
   * filename uris.
   * 
   * If we have a cached version of the extracted files,
   * this is returned. Otherwise the directory itself
   * is scanned.
   */
  public function extractedFiles(){
    if($this->extractedFiles !== NULL){return $this->extractedFiles;}
    $this->extractedFiles = array();
    $it = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($this->root));
    foreach($it as $name => $object){
      if(!preg_match('|/\.*$|', $name)){
        $this->extractedFiles[] = $name;
      }
    }
    return $this->extractedFiles;
  }

  /**
   * function deleteExtractedFiles
   * 
   * Delete the extracted files folder.
   * 
   * For safety this will only delete files under
   * 'public://' or 'private://' folders.
   * 
   */
  public function deleteExtractedFiles(){
    if($this->folderExists() && preg_match('/^(public|private):\/\//', $this->root)){
      file_unmanaged_delete_recursive($this->root);
    }
  }

  /**
   * function createFolder
   *
   * Create the root folder
   */
  private function createFolder(){
    // No point using recursive directory creation, as we don't have recursive
    // chmod. chmod is needed because umask messes up our permissions.
    $create = $this->root;
    $stack = array();
    while(!file_exists($create)){
      $stack[] = drupal_basename($create);
      $create = drupal_dirname($create);
    }
    foreach(array_reverse($stack) as $folder){
      $create = $create . '/' . $folder;
      drupal_mkdir($create);
      drupal_chmod($create, $this->mode);
    }
  }
}