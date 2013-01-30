<?php

class DrupalImageGalleryMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    // Adding a dummy vid field
    $this->sourceFields = array(
      'nid' => 'nid',
      'vid' => 'vid'
    );
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'tid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Source term ID'
      )
    ), MigrateDestinationNode::getKeySchema());
    $this->destination = new ScratchpadsMigrateDestinationNode('media_gallery');
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('language')->defaultValue('und');
    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('is_new')->defaultValue(1);
    $this->addFieldMapping('weight_weight')->defaultValue(0);
    $this->addFieldMapping('nid', 'nid');
    $this->addFieldMapping('vid', 'vid');
    $this->addUnmigratedDestinations(array(
      'revision',
      'revision_uid',
      'log',
      'tnid',
      'promote',
      'sticky',
      'created',
      'changed',
      'body:summary',
      'body:format',
      'body:language',
      'group_audience',
      'group_content_access',
      'field_media',
      'field_media:file_class',
      'field_media:language',
      'field_media:description',
      'field_media:display',
      'field_media:destination_dir',
      'field_media:destination_file',
      'field_media:file_replace',
      'field_media:preserve_files',
      'field_media:source_dir',
      'path',
      'comment',
      'menu',
      'auto_nodetitle_applied',
      'totalcount',
      'daycount',
      'timestamp'
    ));
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  protected function sourceQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('term_data', 'td')->fields('td', array(
      'tid',
      'name',
      'description'
    ))->condition('vid', scratchpads_migrate_get_image_gallery_vid());
  }

  /**
   * Get the maximum NID - needs to be greatwer than all other NIDs to prevent conflicts later
   */
  protected function getNID(){
    // Do we already have media gallery nodes (if eg: the import has looped to a new batch)
    $result = db_query("SELECT MAX(nid) FROM node WHERE type=:type", array(
      ':type' => 'media_gallery'
    ));
    if(!$nid = $result->fetchField()){
      $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
      $query->addExpression('MAX(nid)');
      $nid = $query->execute()->fetchField();
    }
    // Increment max nid by 1
    $nid++;
    return $nid;
  }

  /**
   * Get the maximum VID - needs to be greatwer than all other VIDs to prevent conflicts later
   */
  protected function getVID(){
    // Do we already have media gallery nodes (if eg: the import has looped to a new batch)
    $result = db_query("SELECT MAX(vid) FROM node WHERE type=:type", array(
      ':type' => 'media_gallery'
    ));
    if(!$vid = $result->fetchField()){
      $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
      $query->addExpression('MAX(vid)');
      $vid = $query->execute()->fetchField();
    }
    // Increment max nid by 1
    $vid++;
    return $vid;
  }

  public function prepareRow($row){
    // Explicity set the NID for image gallery nodes
    // They're terms so we need to do something to prevent conflicts when the nodes are imported
    // BUG fix - keeping this as a static vars isn't working with batches
    // Look it up each time - not many anyway so it shouldn't be too much of a performance hit
    $row->nid = $this->getNID();
    $row->vid = $this->getVID();
  }
}