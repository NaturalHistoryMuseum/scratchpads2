<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
abstract class ExampleNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_legacy_nid', 'nid')
         ->description('We have a common field to save the D6 nid');
  }
}

/**
 * Image-specific mappings and handling.
 */
class ExampleImageMigration extends ExampleNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_credit', 'copyright');
    $arguments = array('source_path' => '/mnt/source_images');
    $this->addFieldMapping('field_main_image', 'filename')
         ->arguments($arguments);
  }

  /**
   * Implementation of DrupalNodeMigration::sourceFieldList.
   *
   * The parent implementation automatically identifies all basic node fields and
   * CCK fields - here we document any other data we're pulling into the source
   * row.
   *
   * @return array
   */
  public function sourceFieldList() {
    $fields = parent::sourceFieldList();
    $fields += array(
      'copyright' => 'Media copyright notice',
      'filename' => 'Path of image within mounted filesystem',
    );
    return $fields;
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * Here we pull in additional data from a non-core, non-CCK table.
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $image_row = Database::getConnection('default', $this->sourceConnection)
                 ->select('custom_image_table', 'i')
                 ->fields('i', array('copyright', 'filename'))
                 ->condition('vid', $row->vid)
                 ->execute()
                 ->fetchObject();
    if ($image_row) {
      $row->copyright = $image_row->copyright;
      $row->filename = $image_row->filename;
    }
    else {
      $this->saveMessage(t('No custom_image_table data for image node with vid !vid',
                         array('!vid' => $row->vid)));
    }
  }
}

class ExampleArticleMigration extends ExampleNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_image', 'field_lead_graphic')
         ->sourceMigration('Image');
    // Note that we map migrated terms by the vocabulary ID.
    $this->addFieldMapping('field_countries', '1')
         ->sourceMigration('Countries')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_topics', '2')
         ->sourceMigration('Topics')
         ->arguments(array('source_type' => 'tid'));
  }

  /**
   * Implementation of DrupalNodeMigration::sourceFieldList.
   * Add the dynamically-added fields.
   *
   * @return array
   */
  public function sourceFieldList() {
    $fields = parent::sourceFieldList();
    $fields += array(
      'lead_graphic' => 'Lead graphic string from D6',
      'summary' => 'Summary string from D6',
    );
    return $fields;
  }
}
