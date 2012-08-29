<?php

class DrupalImageGalleryMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'tid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Source term ID'
      )
    ), MigrateDestinationNode::getKeySchema());
    $this->destination = new MigrateDestinationNode('media_gallery');
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('language')->defaultValue('und');
    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addUnmigratedDestinations(array(
      'is_new',
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
      'comment'
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
}