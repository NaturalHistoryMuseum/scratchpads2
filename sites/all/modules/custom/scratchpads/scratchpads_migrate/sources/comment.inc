<?php

class ScratchpadsCommentMigration extends DrupalComment6Migration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->addFieldMapping('status', 'status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue('en');
    $this->removeFieldMapping('comment_body:format');
    $this->addFieldMapping('comment_body:format')->defaultValue('comment_filtered_html');
    $this->addUnmigratedDestinations(array(
      'comment_body:language',
      'path'
    ));
    $this->addUnmigratedSources(array(
      'format'
    ));
  }

  public function prepareRow($row){
    // WTF! D6 comments has 1 for unpublished, 0 for published
    // Invert it
    $row->status = ($row->status == 1 ? 0 : 1);
  }
}