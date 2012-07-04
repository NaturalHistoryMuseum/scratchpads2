<?php
/**
 * @file
 * Examples and test fodder for migration of Media entities and fields.
 */

/**
 * Migration class for media images.
 */
class MigrateExampleMediaImageMigration extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration of media images');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'filename' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Image filename',
        )
      ),
      MigrateDestinationMedia::getKeySchema()
    );

    // Source fields available in the XML file.
    $fields = array(
      'filename' => t('Image filename, relative to the source directory'),
      'description' => t('Description of the image'),
    );

    // Our test data is in an XML file
    $xml_folder = drupal_get_path('module', 'migrate_extras_media');
    $items_url = $xml_folder . '/migrate_extras_media.xml';
    $item_xpath = '/source_data/item/image';
    $item_ID_xpath = 'filename';
    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    // In the simplest case, just pass the media type.
    $this->destination = new MigrateDestinationMedia('image');

    // The source images are in a local directory - specify the parent.
    $this->addFieldMapping('source_dir')
         ->defaultValue(drupal_get_path('module', 'migrate_extras_media') . '/source_files');
    // The 'value' of the media destination is mapped to the source field
    // representing the media itself - in this case, a filename relateive to
    // source_dir.
    $this->addFieldMapping('value', 'filename')
         ->xpath('filename');
    // Fields on the entity can be mapped in the usual way.
    $this->addFieldMapping('field_image_description', 'description')
         ->xpath('description');

    $this->addFieldMapping('uid')
         ->defaultValue(1);

    $this->addUnmigratedDestinations(array('field_image_description:format',
      'field_image_description:language', 'destination_dir', 'destination_file',
      'file_replace', 'preserve_files', 'timestamp'));
    if (module_exists('path')) {
      $this->addUnmigratedDestinations(array('path'));
    }
  }
}

/**
 * Migration class for media_youtube entities.
 */
class MigrateExampleMediaVideoMigration extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration of Youtube videos');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uri' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'YouTube URI',
        )
      ),
      MigrateDestinationMedia::getKeySchema()
    );

    // Source fields available in the XML file.
    $fields = array(
      'uri' => t('URI of a YouTube video'),
      'description' => t('Description of the video'),
    );

    // Our test data is in an XML file
    $xml_folder = drupal_get_path('module', 'migrate_extras_media');
    $items_url = $xml_folder . '/migrate_extras_media.xml';
    $item_xpath = '/source_data/item/video';
    $item_ID_xpath = 'uri';
    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    // In this case, we need to specify the file_class in the second paramter -
    // this class understands how to translate a http://www.youtube.com/ URI
    // into Drupal's Youtube file scheme (youtube://).
    $this->destination = new MigrateDestinationMedia('video',
      'MigrateExtrasFileYoutube');

    // We just need to map the 'value' in the media destination to the Youtube
    // URI.
    $this->addFieldMapping('value', 'uri')
         ->xpath('uri');

    $this->addFieldMapping('field_video_description', 'description')
         ->xpath('description');
    $this->addFieldMapping('uid')
         ->defaultValue(1);

    $this->addUnmigratedDestinations(array('field_video_description:format',
      'field_video_description:language', 'timestamp'));
    if (module_exists('path')) {
      $this->addUnmigratedDestinations(array('path'));
    }
  }
}

/**
 * Migration class for nodes with media fields.
 */
class MigrateExampleMediaNodeMigration extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration into the Media module');
    $this->dependencies =
      array('MigrateExampleMediaImage', 'MigrateExampleMediaImage');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Media ID',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Source fields available in the XML file.
    $fields = array(
      'id' => t('Source id'),
      'title' => t('Title'),
      'body' => t('Description'),
      'video_uri' => t('A YouTube URI'),
      'video_description' => t('Description for a YouTube video'),
      'image_filename' => t('Image filename'),
      'image_description' => t('Image description'),
      'document_filename' => t('Document filename'),
    );

    // Our test data is in an XML file
    $xml_folder = drupal_get_path('module', 'migrate_extras_media');
    $items_url = $xml_folder . '/migrate_extras_media.xml';
    $item_xpath = '/source_data/item';
    $item_ID_xpath = 'id';
    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    $this->destination = new MigrateDestinationNode('migrate_extras_media_example');

    // Basic fields
    $this->addFieldMapping('title', 'title')
         ->xpath('title');
    $this->addFieldMapping('uid')
         ->defaultValue(1);
    $this->addFieldMapping('body', 'body')
         ->xpath('body');
    $this->addFieldMapping('body:format')
         ->defaultValue('filtered_html');

    // The image and Youtube media entities are imported via their own
    // migrations above, we just need to link the fields to them. We do this
    // by mapping the primary keys of those migrations (URL and filename) to
    // the primary field values, and specifying a file_class of MigrateFileFid.
    $this->addFieldMapping('field_youtube_video', 'video_uri')
         ->xpath('video/uri')
         ->sourceMigration('MigrateExampleMediaVideo');
    $this->addFieldMapping('field_youtube_video:file_class')
         ->defaultValue('MigrateFileFid');

    $this->addFieldMapping('field_media_image', 'image_filename')
         ->xpath('image/filename')
         ->sourceMigration('MigrateExampleMediaImage');
    $this->addFieldMapping('field_media_image:file_class')
         ->defaultValue('MigrateFileFid');

    // We have not created a separate migration for documents, we're using the
    // file field handler to get those. This works just like it does for regular
    // file fields.
    $this->addFieldMapping('field_document', 'document_filename')
         ->xpath('document/filename');
    // This isn't technically necessary - MigrateFileUri is the default
    $this->addFieldMapping('field_document:file_class')
         ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_document:source_dir')
         ->defaultValue(drupal_get_path('module', 'migrate_extras_media') . '/source_files');
    $this->addFieldMapping('field_document:destination_file', 'document_filename')
         ->xpath('document/filename');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'created', 'changed', 'revision_uid',
      'log', 'tnid', 'body:summary', 'body:language',
      'comment'));
    $this->addUnmigratedDestinations(array('field_media_image:language',
      'field_media_image:display', 'field_media_image:description',
      'field_youtube_video:language', 'field_youtube_video:description',
      'field_youtube_video:display', 'field_document:language', 'field_document:destination_dir',
      'field_document:file_replace', 'field_document:preserve_files',
      'field_document:description', 'field_document:display'));
    if (module_exists('path')) {
      $this->addUnmigratedDestinations(array('path'));
      if (module_exists('pathauto')) {
        $this->addUnmigratedDestinations(array('pathauto'));
      }
    }

    $this->addUnmigratedSources(array('image_description', 'video_description'));
  }

  /**
   * Implementation of Migration::prepare().
   */
  public function prepare($node, $row) {
    // This will replace any <img> tags in the body with the media module's
    // JSON references.
    MigrateDestinationMedia::rewriteImgTags($node);
  }
}

/*
 * Implementation of hook_migrate_api().
 */
function migrate_extras_media_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}
