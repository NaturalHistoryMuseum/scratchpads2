<?php

class DrupalImageGalleryMigration extends DrupalMigration{

  /**
   * Explicity set the NID for image gallery nodes
   * They're terms so we need to do something to prevent conflicts when the nodes are imported
   *
   * @var integer
   */
  protected static $nid;

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    // Adding a dummy nid field
    $this->sourceFields = array(
      'nid' => 'nid'
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
    $this->addFieldMapping('nid', 'nid');
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
    	'auto_nodetitle_applied'
    ));
    // Set the maximum NID value from the old site
    $this->nid = $this->getMaxNID();
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
   * Get the maximum NID 
   */
  protected function getMaxNID(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
    $query->addExpression('MAX(nid)');
    return $query->execute()->fetchField();
  }

  public function prepareRow($row){
    // Populate the dummy nid field
    $this->nid++;
    $row->nid = $this->nid;
  }
}