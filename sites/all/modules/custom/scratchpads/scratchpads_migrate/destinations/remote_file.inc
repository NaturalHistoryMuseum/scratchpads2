<?php

/**
 * Handle spaces in remote files
 */
class MigrateRemoteFileUri extends MigrateFileUri{

  protected function copyFile($destination){
    $this->sourcePath = str_replace('sites/', 'http://', str_replace(" ", "%20", $this->sourcePath));
    return parent::copyFile($destination);
  }

  /**
   * Implementation of MigrateFileInterface::fields().
   *
   * @return array
   */
  static public function fields(){
    return parent::fields() + array(
      'filename' => t('File name')
    );
  }
}