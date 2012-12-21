<?php

class ScratchpadsCommentMigration extends DrupalComment6Migration{
  // Turn of comment notifcation
  protected function preImport(){
    parent::preImport();
    variable_set('comment_notify_node_types', array());
    variable_set('comment_notify_node_notify_default_mailalert', 0);
  }

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
    // We're using VID in the mapping tables
    // Changing the nid to the VID allows us to look up the dest NID by VID
    $row->nid = $row->vid;
  }
  // Turn on comment notifcation
  protected function postImport(){
    parent::postImport();
    $array_node_types = array();
    foreach(node_type_get_names() as $type => $name){
      $array_node_types[$type] = $type;
    }
    variable_set('comment_notify_node_types', $array_node_types);
    variable_set('comment_notify_node_notify_default_mailalert', 1);
  }

  protected function commentQuery($sourceType){
    $query = Database::getConnection('default', $this->sourceConnection)->select('comments', 'c')->fields('c', array(
      'cid',
      'pid',
      'nid',
      'uid',
      'subject',
      'comment',
      'hostname',
      'timestamp',
      'status',
      'thread',
      'name',
      'mail',
      'homepage',
      'format'
    ));
    $query->join('node', 'n', 'c.nid = n.nid');
    $query->addField('n', 'vid');
    $query->condition('n.type', $sourceType);
    return $query;
  }
}