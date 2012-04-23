<?php
/**
 * @file
 * Examples and test fodder for migration with Pathauto enabled.
 */

/**
 * Migration class to test import with Pathauto enabled.
 */
class MigrateExamplePathautoMigration extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration with Pathauto enabled');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Pathauto ID',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Source fields available in the XML file.
    $fields = array(
      'id' => t('Source id'),
      'title' => t('Title'),
      'body' => t('Description'),
    );

    // Our test data is in an XML file
    $xml_folder = drupal_get_path('module', 'migrate_extras_pathauto');
    $items_url = $xml_folder . '/migrate_extras_pathauto.xml';
    $item_xpath = '/source_data/item';
    $item_ID_xpath = 'id';
    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    $this->destination = new MigrateDestinationNode('migrate_example_pathauto');

    // Basic fields
    $this->addFieldMapping('title', 'title')
         ->xpath('title');
    $this->addFieldMapping('uid')
         ->defaultValue(1);
    $this->addFieldMapping('body', 'body')
         ->xpath('body');

    // Disable application of pathauto during migration
    $this->addFieldMapping('pathauto')
         ->defaultValue(FALSE);

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'created', 'changed', 'revision_uid',
      'path'));
  }
}
