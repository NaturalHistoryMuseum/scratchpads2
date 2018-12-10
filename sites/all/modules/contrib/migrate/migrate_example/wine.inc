<?php

/**
 * @file
 * Advanced migration examples. These serve two purposes:
 *
 * 1. To demonstrate some of the more advanced usages of the Migrate module.
 *    Search for "TIP:" below for features not found in the basic example.
 * 2. To provide thorough test cases for the simpletest suite.
 *
 */

/**
 * Abstract intermediate class holding common settings.
 */
abstract class AdvancedExampleMigration extends Migration {

  /**
   * Text format object for our migrate_example format.
   *
   * @var
   */
  public $basicFormat;

  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->team = array(
      new MigrateTeamMember('Jack Kramer', 'jkramer@example.com', t('Taster')),
      new MigrateTeamMember('Linda Madison', 'lmadison@example.com',
        t('Winemaker')),
    );
    $this->issuePattern = 'http://drupal.org/node/:id:';

    // A format of our own, for testing migration of formats
    $this->basicFormat = filter_format_load('migrate_example');

    // We can do shared field mappings in the common class
    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }
}

/**
 * TIP: While usually you'll create true migrations - processes that copy data
 * from some source into Drupal - you can also define processing steps for
 * either the import or rollback stages that take other actions. In this case,
 * we want to disable auto_nodetitle while the migration steps run. We'll
 * re-enable it over in WineFinishMigration.
 */
class WinePrepMigration extends MigrationBase {

  // Track whether the auto_nodetitle was originally enabled so we know whether
  // to re-enable it. This is public so WineFinishMigration can reference it.
  public static $wasEnabled = FALSE;

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('If auto_nodetitle is present, disable it for the duration');
  }

  // Define isComplete(), returning a boolean, to indicate whether dependent
  // migrations may proceed
  public function isComplete() {
    // If Auto Node Title is disabled, other migrations are free to go
    if (module_exists('auto_nodetitle')) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  // Implement any action you want to occur during an import process in an
  // import() method (alternatively, if you have an action which you want to
  // run during rollbacks, define a rollback() method).
  public function import() {
    if (module_exists('auto_nodetitle')) {
      self::$wasEnabled = TRUE;
      module_disable(array('auto_nodetitle'));
      self::displayMessage(t('Disabled auto_nodetitle module'), 'success');
    }
    else {
      self::$wasEnabled = FALSE;
      self::displayMessage(t('Auto_nodetitle is already disabled'), 'success');
    }
    // import() must return one of the MigrationBase RESULT constants.
    return MigrationBase::RESULT_COMPLETED;
  }
}

// The term migrations are very similar - implement the commonalities here (yes,
// two layers of abstraction).
abstract class WineTermMigration extends AdvancedExampleMigration {

  // The type, vocabulary machine name, and description are the only
  // differences among the incoming vocabularies, so pass them through the
  // constructor - you'll see below that the individual term migrations classes
  // thus become very simple.
  public function __construct($arguments, $type, $vocabulary_name,
                              $description) {
    parent::__construct($arguments);
    $this->description = $description;

    // Best practice as of Migrate 2.6 is to specify dependencies in
    // hook_migrate_api. However, in this case, since we have a common class
    // for a few different migrations, we'll specify this dependency here
    // rather than repeatedly in hook_migrate_api().
    $this->dependencies = array('WinePrep');

    $query = db_select('migrate_example_wine_categories', 'wc')
      ->fields('wc', array(
        'categoryid',
        'name',
        'details',
        'category_parent',
        'ordering',
      ))
      ->condition('type', $type)
      // This sort assures that parents are saved before children.
      ->orderBy('category_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm($vocabulary_name);
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'categoryid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'details');
    $this->addFieldMapping('parent', 'category_parent')
      ->sourceMigration($this->getMachineName());
    $this->addFieldMapping('weight', 'ordering');
    $this->addFieldMapping('format')
      ->defaultValue($this->basicFormat->format);

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('parent_name')
      ->issueGroup(t('DNM'));
  }
}

class WineVarietyMigration extends WineTermMigration {

  public function __construct($arguments) {
    parent::__construct($arguments, 'variety', 'migrate_example_wine_varieties',
      t('Migrate varieties from the source database to taxonomy terms'));
  }
}

class WineRegionMigration extends WineTermMigration {

  public function __construct($arguments) {
    parent::__construct($arguments, 'region', 'migrate_example_wine_regions',
      t('Migrate regions from the source database to taxonomy terms'));
  }
}

class WineBestWithMigration extends WineTermMigration {

  public function __construct($arguments) {
    parent::__construct($arguments, 'best_with', 'migrate_example_wine_best_with',
      t('Migrate "Best With" from the source database to taxonomy terms'));
  }
}

/**
 * TIP: Files can be migrated directly by themselves, by using the
 * MigrateDestinationFile class. This will copy the files themselves from the
 * source, and set up the Drupal file tables appropriately. Referencing them
 * in, say, a node field later is then simple.
 */
class WineFileCopyMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Profile images');

    $query = db_select('migrate_example_wine_files', 'wf')
      ->fields('wf', array('imageid', 'url'))
      ->isNull('wineid');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationFile();
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'imageid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Image ID.',
        ),
      ),
      MigrateDestinationFile::getKeySchema()
    );

    // In the simplest case, just map the incoming URL to 'value'.
    $this->addFieldMapping('value', 'url');

    $this->addUnmigratedDestinations(array(
      'destination_dir',
      'destination_file',
      'fid',
      'file_replace',
      'preserve_files',
      'source_dir',
      'timestamp',
      'uid',
      'urlencode',
    ));

    $this->removeFieldMapping('pathauto');
  }
}

/**
 * Migration class to test importing from a BLOB column into a file entity.
 *
 * @see MigrateExampleOracleNode()
 */
class WineFileBlobMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Example migration from BLOB column into files.');

    $query = db_select('migrate_example_wine_blobs', 'wf')
      ->fields('wf', array('imageid', 'imageblob'));
    $this->source = new MigrateSourceSQL($query);

    // Note that the WineFileCopyMigration example let the second argument,
    // the file_class, default to MigrateFileUri, indicating that the
    // 'value' we're passing was a URI. In this case, we're passing a blob, so
    // tell the destination to expect it.
    $this->destination = new MigrateDestinationFile('file', 'MigrateFileBlob');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'imageid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Image ID.',
        ),
      ),
      MigrateDestinationFile::getKeySchema()
    );

    // Basic fields
    $this->addFieldMapping('value', 'imageblob')
      ->description('An image blob in the DB');

    // The destination filename must be specified for blobs
    $this->addFieldMapping('destination_file')
      ->defaultValue('druplicon.png');

    $this->addFieldMapping('uid')
      ->defaultValue(1);

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'destination_dir',
      'fid',
      'file_replace',
      'preserve_files',
      'timestamp',
    ));

    $this->removeFieldMapping('pathauto');
  }
}

class WineRoleMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed (multi items) of roles (positions)');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields
    $fields = array(
      'name' => t('Position name'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a local file path.
    $xml_folder = DRUPAL_ROOT . '/' .
      drupal_get_path('module', 'migrate_example') . '/xml/';
    $items_url = $xml_folder . 'positions.xml';
    // This is the xpath identifying the items to be migrated, relative to the
    // document.
    $item_xpath = '/positions/position';
    // This is the xpath relative to the individual items - thus the full xpath
    // of an ID will be /positions/position/sourceid.
    $item_ID_xpath = 'sourceid';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationRole();

    // The source ID here is the one retrieved from each data item in the XML
    // file, and used to identify specific items
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationRole::getKeySchema()
    );

    $this->addFieldMapping('name', 'name')
      ->xpath('name');

    $this->addUnmigratedDestinations(array('weight'));
  }
}

class WineUserMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Wine Drinkers of the world');

    $query = db_select('migrate_example_wine_account', 'wa')
      ->fields('wa', array(
        'accountid',
        'status',
        'posted',
        'name',
        'password',
        'mail',
        'last_access',
        'last_login',
        'original_mail',
        'sig',
        'sex',
        'imageid',
        'positions',
      ));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'accountid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Account ID.',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    // Mapped fields
    $this->addSimpleMappings(array('name', 'status', 'mail'));
    // Note that these date/time values are coming in as ISO strings, but
    // Drupal uses UNIX timestamps. The user destination automatically
    // translates them as necessary.
    $this->addFieldMapping('created', 'posted');
    $this->addFieldMapping('access', 'last_access');
    $this->addFieldMapping('login', 'last_login');
    $this->addFieldMapping('pass', 'password');
    $this->addFieldMapping('roles', 'positions')
      ->separator(',')
      ->sourceMigration('WineRole');
    $this->addFieldMapping('signature', 'sig');
    $this->addFieldMapping('signature_format')
      ->defaultValue($this->basicFormat->format);
    $this->addFieldMapping('init', 'original_mail');
    $this->addFieldMapping('field_migrate_example_gender', 'sex')
      ->description(t('Map from M/F to 0/1 in prepare method'));
    $this->addFieldMapping('picture', 'imageid')
      ->sourceMigration('WineFileCopy');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'data',
      'is_new',
      'language',
      'role_names',
      'theme',
      'timezone',
    ));
  }

  public function prepare(stdClass $account, stdClass $row) {
    // Gender data comes in as M/F, needs to be saved as Male=0/Female=1
    // TIP: Note that the Migration prepare method is called after all other
    // prepare handlers. Most notably, the field handlers have had their way
    // and created field arrays, so we have to save in the same format. In this
    // case, best practice would be to use a callback instead (see below), we're
    // just demonstrating here what the $account object looks like at this
    // stage.
    switch ($row->sex) {
      case 'm':
      case 'M':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 0;
        break;
      case 'f':
      case 'F':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 1;
        break;
      default:
        unset($account->field_migrate_example_gender);
        break;
    }
  }
}

class WineProducerMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Wine producers of the world');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'producerid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'p',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine_producer', 'p')
      ->fields('p', array(
        'producerid',
        'name',
        'body',
        'excerpt',
        'accountid',
      ));
    // Region term is singletons, handled straighforwardly
    $query->leftJoin('migrate_example_wine_category_producer', 'reg',
      'p.producerid = reg.producerid');
    $query->addField('reg', 'categoryid', 'region');

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // Mapped fields
    $this->addFieldMapping('title', 'name')
      ->description(t('Mapping producer name in source to node title'));
    $this->addFieldMapping('uid', 'accountid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
      ->sourceMigration('WineRegion');
    $this->addFieldMapping('migrate_example_wine_regions:source_type')
      ->defaultValue('tid');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:summary', 'excerpt');
    $this->addFieldMapping('sticky')
      ->defaultValue(0);

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'tnid',
      'translate',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
    ));

    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An example of importing from an XML feed. See the files in the xml
 * directory - index.xml contains a list of IDs to import, and <id>.xml
 * is the data for a given producer.
 *
 * Note that, if basing a migration on an XML source, you need to derive it
 * from XMLMigration instead of Migration.
 */
class WineProducerXMLMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed of wine producers of the world');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.
    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a file path.
    $xml_folder = DRUPAL_ROOT . '/' .
      drupal_get_path('module', 'migrate_example') . '/xml/';
    $list_url = $xml_folder . 'index.xml';
    // Each ID retrieved from the list URL will be plugged into :id in the
    // item URL to fetch the specific objects.
    $item_url = $xml_folder . ':id.xml';

    // We use the MigrateSourceList class for any source where we obtain the
    // list of IDs to process separately from the data for each item. The
    // listing and item are represented by separate classes, so for example we
    // could replace the XML listing with a file directory listing, or the XML
    // item with a JSON item.
    $this->source = new MigrateSourceList(new MigrateListXML($list_url),
      new MigrateItemXML($item_url), $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // The source ID here is the one retrieved from the XML listing file, and
    // used to identify the specific item's file
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to
    // retrieve the value from the XML.
    $this->addFieldMapping('title', 'name')
      ->xpath('/producer/name');
    $this->addFieldMapping('uid', 'authorid')
      ->xpath('/producer/authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
      ->xpath('/producer/region');
    $this->addFieldMapping('body', 'description')
      ->xpath('/producer/description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An example of importing from an XML feed with namespaces.
 * See the files in the xml directory - index2.xml contains a list of IDs
 * to import, and <id>.xml is the data for a given producer.
 *
 * Note that, if basing a migration on an XML source, you need to derive it
 * from XMLMigration instead of Migration.
 */
class WineProducerNamespaceXMLMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Namespaced XML feed of wine producers of the world');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.
    $fields = array(
      'pr:name' => t('Producer name'),
      'pr:description' => t('Description of producer'),
      'pr:authorid' => t('Numeric ID of the author'),
      'pr:region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a file path.
    $xml_folder = DRUPAL_ROOT . '/' .
      drupal_get_path('module', 'migrate_example') . '/xml/';
    $list_url = $xml_folder . 'index2.xml';
    // Each ID retrieved from the list URL will be plugged into :id in the
    // item URL to fetch the specific objects.
    $item_url = $xml_folder . ':id.xml';

    // We use the MigrateSourceList class for any source where we obtain the
    // list of IDs to process separately from the data for each item. The
    // listing and item are represented by separate classes, so for example we
    // could replace the XML listing with a file directory listing, or the XML
    // item with a JSON item.
    $list = new MigrateListXML($list_url, array('wn' => 'http://www.wine.org/wine'));
    $item = new MigrateItemXML($item_url, array('pr' => 'http://www.wine.org/wine-producers'));
    $this->source = new MigrateSourceList($list, $item, $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // The source ID here is the one retrieved from the XML listing file, and
    // used to identify the specific item's file
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to
    // retrieve the value from the XML.
    $this->addFieldMapping('title', 'pr:name')
      ->xpath('/pr:producer/pr:name');
    $this->addFieldMapping('uid', 'pr:authorid')
      ->xpath('/pr:producer/pr:authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'pr:region')
      ->xpath('/pr:producer/pr:region');
    $this->addFieldMapping('body', 'pr:description')
      ->xpath('/pr:producer/pr:description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An example of importing from an XML feed where both the id and the
 * data to import are in the same file.  The id is a part of the data.  See
 * the file in the xml directory - producers.xml which contains all IDs and
 * producer data for this example.
 *
 * Note that, if basing a migration on an XML source, you need to derive it
 * from XMLMigration instead of Migration.
 */
class WineProducerMultiXMLMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description =
      t('XML feed (multi items) of wine producers of the world');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.
    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a file path.
    $xml_folder = DRUPAL_ROOT . '/' . drupal_get_path('module', 'migrate_example') . '/xml/';
    $items_url = $xml_folder . 'producers.xml';

    // We use the MigrateSourceMultiItems class for any source where we obtain
    // the list of IDs to process and the data for each item from the same
    // file. Examples include multiple items defined in a single xml file or a
    // single json file where in both cases the id is part of the item.

    // This is the xpath identifying the items to be migrated, relative to the
    // document.
    $item_xpath = '/producers/producer';
    // This is the xpath relative to the individual items - thus the full xpath
    // of an ID will be /producers/producer/sourceid.
    $item_ID_xpath = 'sourceid';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // The source ID here is the one retrieved from each data item in the XML
    // file, and used to identify specific items
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to
    // retrieve the value from the XML.
    // TIP: Note that all xpaths for fields begin at the last element of the
    // item xpath since each item xml chunk is processed individually.
    // (ex. xpath=name is equivalent to a full xpath of
    // /producers/producer/name).
    $this->addFieldMapping('title', 'name')
      ->xpath('name');
    $this->addFieldMapping('uid', 'authorid')
      ->xpath('authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
      ->xpath('region');
    $this->addFieldMapping('body', 'description')
      ->xpath('description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An example of importing from an XML feed with namespaces, where both
 * the id and the data to import are in the same file.  The id is a part of
 * the data. See the file in the xml directory - producers3.xml which contains
 * all IDs and producer data for this example.
 *
 * Note that, if basing a migration on an XML source, you need to derive it
 * from XMLMigration instead of Migration.
 */
class WineProducerMultiNamespaceXMLMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description =
      t('Namespaced XML feed (multi items) of wine producers of the world');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.
    $fields = array(
      'pr:name' => t('Producer name'),
      'pr:description' => t('Description of producer'),
      'pr:authorid' => t('Numeric ID of the author'),
      'pr:region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a file path.
    $xml_folder = DRUPAL_ROOT . '/' . drupal_get_path('module', 'migrate_example') . '/xml/';
    $items_url = $xml_folder . 'producers3.xml';

    // We use the MigrateSourceMultiItems class for any source where we obtain
    // the list of IDs to process and the data for each item from the same
    // file. Examples include multiple items defined in a single xml file or a
    // single json file where in both cases the id is part of the item.

    // This is the xpath identifying the items to be migrated, relative to the
    // document.
    $item_xpath = '/pr:producers/pr:producer';
    // This is the xpath relative to the individual items - thus the full xpath
    // of an ID will be /producers/producer/sourceid.
    $item_ID_xpath = 'pr:sourceid';
    // All XML namespaces used in the XML file need to be defined here too.
    $namespaces = array('pr' => 'http://www.wine.org/wine-producers');

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath, $namespaces);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // The source ID here is the one retrieved from each data item in the XML
    // file, and used to identify specific items
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to
    // retrieve the value from the XML.
    // TIP: Note that all xpaths for fields begin at the last element of the
    // item xpath since each item xml chunk is processed individually.
    // (ex. xpath=name is equivalent to a full xpath of
    // /producers/producer/name).
    $this->addFieldMapping('title', 'pr:name')
      ->xpath('pr:name');
    $this->addFieldMapping('uid', 'pr:authorid')
      ->xpath('pr:authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'pr:region')
      ->xpath('pr:region');
    $this->addFieldMapping('body', 'pr:description')
      ->xpath('pr:description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An alternative approach using MigrateSourceSQL. This uses a different
 * XML library, which advances element-by-element through the XML file rather
 * than reading in the whole file. This source will work better with large XML
 * files, but is slower for small files and has a more restrictive query
 * language for selecting the elements to process.
 */
class WineProducerXMLPullMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed (pull) of wine producers of the world');

    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a local file path.
    $xml_folder = DRUPAL_ROOT . '/' .
      drupal_get_path('module', 'migrate_example') . '/xml/';
    $items_url = $xml_folder . 'producers2.xml';

    // As with MigrateSourceMultiItems, this applies where there is not a
    // separate list of IDs to process - the source XML file is entirely
    // self-contained. For the ID path, and xpath for each component, we can
    // use the full xpath syntax as usual. However, the syntax to select the
    // elements that correspond to objects to import is more limited. It must
    // be a fully-qualified path to the element (i.e.,
    // /producers/producer rather than just //producer).
    $item_xpath = '/producers/producer';  // relative to document
    $item_ID_xpath = 'sourceid';          // relative to item_xpath

    $this->source = new MigrateSourceXML($items_url, $item_xpath,
      $item_ID_xpath, $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'name')
      ->xpath('name');
    $this->addFieldMapping('uid', 'authorid')
      ->xpath('authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
      ->xpath('region');
    $this->addFieldMapping('body', 'description')
      ->xpath('description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

/**
 * TIP: An alternative approach using MigrateSourceSQL. This uses a different
 * XML library, which advances element-by-element through the XML file rather
 * than reading in the whole file. This source will work better with large XML
 * files, but is slower for small files and has a more restrictive query
 * language for selecting the elements to process.
 */
class WineProducerNamespaceXMLPullMigration extends XMLMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed with namespaces (pull) of wine producers of the world');

    $fields = array(
      'pr:name' => t('Producer name'),
      'pr:description' => t('Description of producer'),
      'pr:authorid' => t('Numeric ID of the author'),
      'pr:region' => t('Name of region'),
    );

    // IMPORTANT: Do not try this at home! We have included importable files
    // with the migrate_example module so it can be very simply installed and
    // run, but you should never include any data you want to keep private
    // (especially user data like email addresses, phone numbers, etc.) in the
    // module directory. Your source data should be outside of the webroot, and
    // should not be anywhere where it may get committed into a revision control
    // system.

    // This can also be an URL instead of a local file path.
    $xml_folder = DRUPAL_ROOT . '/' .
      drupal_get_path('module', 'migrate_example') . '/xml/';
    $items_url = $xml_folder . 'producers4.xml';

    // As with MigrateSourceMultiItems, this applies where there is not a
    // separate list of IDs to process - the source XML file is entirely
    // self-contained. For the ID path, and xpath for each component, we can
    // use the full xpath syntax as usual. However, the syntax to select the
    // elements that correspond to objects to import is more limited. It must
    // be a fully-qualified path to the element (i.e.,
    // /producers/producer rather than just //producer).
    $item_xpath = '/pr:producers/pr:producer';  // relative to document
    $item_ID_xpath = 'pr:sourceid';          // relative to item_xpath
    $namespaces = array('pr' => 'http://www.wine.org/wine-producers');

    $this->source = new MigrateSourceXML($items_url, $item_xpath,
      $item_ID_xpath, $fields,
      array(), $namespaces);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'pr:name')
      ->xpath('pr:name');
    $this->addFieldMapping('uid', 'pr:authorid')
      ->xpath('pr:authorid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    $this->addFieldMapping('migrate_example_wine_regions', 'pr:region')
      ->xpath('pr:region');
    $this->addFieldMapping('body', 'pr:description')
      ->xpath('pr:description');

    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'changed',
      'comment',
      'created',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_regions:source_type',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'sticky',
      'tnid',
      'translate',
    ));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

// TODO: Add node_reference field pointing to producer
class WineWineMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Wines of the world');

    $query = db_select('migrate_example_wine', 'w')
      ->fields('w', array(
        'wineid',
        'name',
        'body',
        'excerpt',
        'accountid',
        'posted',
        'last_changed',
        'variety',
        'region',
        'rating',
      ));
    $query->leftJoin('migrate_example_wine_category_wine', 'cwbw',
      "w.wineid = cwbw.wineid");
    $query->leftJoin('migrate_example_wine_categories', 'bw',
      "cwbw.categoryid = bw.categoryid AND bw.type = 'best_with'");
    // Gives a single comma-separated list of related terms
    $query->groupBy('w.wineid');
    $query->addExpression('GROUP_CONCAT(bw.categoryid)', 'best_with');

    $count_query = db_select('migrate_example_wine', 'w');
    $count_query->addExpression('COUNT(wineid)', 'cnt');

    // TIP: By passing an array of source fields to the MigrateSourceSQL
    // constructor, we can modify the descriptions of source fields (which just
    // default, for SQL migrations, to table_alias.column_name), as well as add
    // additional fields (which may be populated in prepareRow()).
    $source_fields = array(
      'wineid' => t('Wine ID in the old system'),
      'name' => t('The name of the wine'),
      'best_vintages' => t('What years were best for this wine?'),
      'url' => t('Image URLs attached to this wine; populated in prepareRow()'),
      'image_alt' =>
        t('Image alt text attached to this wine; populated in prepareRow()'),
      'image_title' =>
        t('Image titles attached to this wine; populated in prepareRow()'),
    );

    // TIP: By default, each time a migration is run, any previously unprocessed
    // source items are imported (along with any previously-imported items
    // marked for update). If the source data contains a timestamp that is set
    // to the creation time of each new item, as well as set to the update time
    // for any existing items that are updated, then you can have those updated
    // items automatically reimported by setting the field as your highwater
    // field.
    $this->highwaterField = array(
      'name' => 'last_changed', // Column to be used as highwater mark
      'alias' => 'w',           // Table alias containing that column
      'type' => 'int',          // By default, highwater marks are assumed to
      // be lexicographically sortable (e.g.,
      // '2011-05-19 17:53:12'). To properly deal with
      // integer highwater marks (such as UNIX
      // timestamps), indicate so here.
    );

    // Note that it is important to process rows in the order of the highwater
    // mark.
    $query->orderBy('last_changed');

    $this->source = new MigrateSourceSQL($query, $source_fields, $count_query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');

    // You can add a 'track_last_imported' option to the map, to record the
    // timestamp of when each item was last imported in the map table.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        ),
      ),
      MigrateDestinationNode::getKeySchema(),
      'default', // The SQL connection where the map/message tables are created.
      array('track_last_imported' => TRUE)
    );

    // Mapped fields
    $this->addFieldMapping('title', 'name')
      ->description(t('Mapping wine name in source to node title'));
    $this->addFieldMapping('uid', 'accountid')
      ->sourceMigration('WineUser')
      ->defaultValue(1);
    // TIP: By default, term relationship are assumed to be passed by name.
    // In this case, the source values are IDs, so we specify the relevant
    // migration (so the tid can be looked up in the map), and tell the term
    // field handler that it is receiving tids instead of names
    $this->addFieldMapping('migrate_example_wine_varieties', 'variety')
      ->sourceMigration('WineVariety');
    $this->addFieldMapping('migrate_example_wine_varieties:source_type')
      ->defaultValue('tid');
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
      ->sourceMigration('WineRegion');
    $this->addFieldMapping('migrate_example_wine_regions:source_type')
      ->defaultValue('tid');
    $this->addFieldMapping('migrate_example_wine_best_with', 'best_with')
      ->separator(',')
      ->sourceMigration('WineBestWith');
    $this->addFieldMapping('migrate_example_wine_best_with:source_type')
      ->defaultValue('tid');

    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');
    $this->addFieldMapping('field_migrate_example_top_vintag', 'best_vintages');

    // TIP: You can apply one or more functions to a source value using
    // ->callbacks(). The function must take a single argument and return a
    // value which is a transformation of the argument. As this example shows,
    // you can have multiple callbacks, and they can either be straight
    // functions or class methods. In this case, our custom method prepends
    // 'review: ' to the body, and then we call a standard Drupal function to
    // uppercase the whole body. You can also use ->callback() to pass
    // additional arguments.
    $this->addFieldMapping('body', 'body')
      ->callbacks(array($this, 'addTitlePrefix'), 'drupal_strtoupper')
      ->callback('html_entity_decode', ENT_QUOTES, 'ISO8859-1');
    $this->addFieldMapping('body:summary', 'excerpt');

    // We will get the image data from a related table in prepareRow()
    $this->addFieldMapping('field_migrate_example_image', 'url');
    // Indicate that we want each file to maintain its name, replacing any
    // previous file of the same name (as opposed to being renamed to avoid
    // collisions, which is the default).
    $this->addFieldMapping('field_migrate_example_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_migrate_example_image:alt', 'image_alt');
    $this->addFieldMapping('field_migrate_example_image:title', 'image_title');

    $this->addFieldMapping('sticky')
      ->defaultValue(0);

    $this->addFieldMapping('created', 'posted');
    $this->addFieldMapping('changed', 'last_changed');

    // Unmapped source fields
    $this->addUnmigratedSources(array('last_changed'));

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'body:format',
      'comment',
      'field_migrate_example_image:destination_dir',
      'field_migrate_example_image:destination_file',
      'field_migrate_example_image:file_class',
      'field_migrate_example_image:preserve_files',
      'field_migrate_example_image:source_dir',
      'field_migrate_example_image:urlencode',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_best_with:create_term',
      'migrate_example_wine_best_with:ignore_case',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_varieties:create_term',
      'migrate_example_wine_varieties:ignore_case',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'tnid',
      'translate',
    ));

    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }

  protected function addTitlePrefix($source_title) {
    return t('review: !title', array('!title' => $source_title));
  }

  // TIP: Implement a prepareRow() method to manipulate the source row between
  // retrieval from the database and the automatic applicaton of mappings.
  public function prepareRow($current_row) {
    // Always start your prepareRow implementation with this clause. You need to
    // be sure your parent classes have their chance at the row, and that if
    // they return FALSE (indicating the row should be skipped) you pass that
    // on.
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }

    // We used the MySQL GROUP_CONCAT function above to handle a multi-value
    // source field - more portably, we query the related table with multiple
    // values here, so the values can run through the mapping process.
    $source_id = $current_row->wineid;
    $result = db_select('migrate_example_wine_vintages', 'v')
      ->fields('v', array('vintage'))
      ->condition('wineid', $source_id)
      ->execute();
    foreach ($result as $row) {
      $current_row->best_vintages[] = $row->vintage;
    }

    // We can have multiple files per node, so we pull them here along with
    // their related data (alt/title).
    /*
     * This is disabled - see http://drupal.org/node/1679798. To demonstrate
     * remote file migration, edit the migrate_example_wine_files table and enter
     * the URLs of known remote image files, then enable this code.
        $result = db_select('migrate_example_wine_files', 'f')
                  ->fields('f', array('url', 'image_alt', 'image_title'))
                  ->condition('wineid', $source_id)
                  ->execute();
        foreach ($result as $row) {
          $current_row->url[] = $row->url;
          $current_row->image_alt[] = $row->image_alt;
          $current_row->image_title[] = $row->image_title;
        }
    */

    // We could also have used this function to decide to skip a row, in cases
    // where that couldn't easily be done through the original query. Simply
    // return FALSE in such cases.
    return TRUE;
  }
}

class WineCommentMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = 'Comments about wines';

    $query = db_select('migrate_example_wine_comment', 'wc')
      ->fields('wc', array(
        'commentid',
        'comment_parent',
        'name',
        'mail',
        'accountid',
        'body',
        'wineid',
        'subject',
        'commenthost',
        'userpage',
        'posted',
        'lastchanged',
      ))
      ->orderBy('comment_parent');
    $this->source = new MigrateSourceSQL($query);
    $this->destination =
      new MigrateDestinationComment('comment_node_migrate_example_wine');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'commentid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationComment::getKeySchema()
    );

    // Mapped fields
    $this->addSimpleMappings(array('name', 'subject', 'mail'));
    $this->addFieldMapping('status')
      ->defaultValue(COMMENT_PUBLISHED);
    $this->addFieldMapping('nid', 'wineid')
      ->sourceMigration('WineWine');
    $this->addFieldMapping('uid', 'accountid')
      ->sourceMigration('WineUser')
      ->defaultValue(0);
    $this->addFieldMapping('pid', 'comment_parent')
      ->sourceMigration('WineComment')
      ->description('Parent comment');
    $this->addFieldMapping('comment_body', 'body');
    $this->addFieldMapping('hostname', 'commenthost');
    $this->addFieldMapping('created', 'posted');
    $this->addFieldMapping('changed', 'lastchanged');
    $this->addFieldMapping('homepage', 'userpage');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'comment_body:format',
      'language',
      'thread',
    ));

    $this->removeFieldMapping('pathauto');
  }
}

// TIP: An easy way to simply migrate into a Drupal table (i.e., one defined
// through the Schema API) is to use the MigrateDestinationTable destination.
// Just pass the table name to getKeySchema and the MigrateDestinationTable
// constructor.
class WineTableMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = 'Miscellaneous table data';

    $table_name = 'migrate_example_wine_table_dest';

    $query = db_select('migrate_example_wine_table_source', 't')
      ->fields('t', array('fooid', 'field1', 'field2'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTable($table_name);
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fooid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTable::getKeySchema($table_name)
    );

    // Mapped fields
    $this->addFieldMapping('drupal_text', 'field1');
    $this->addFieldMapping('drupal_int', 'field2');

    $this->addUnmigratedDestinations(array('recordid'));
    $this->removeFieldMapping('path');
    $this->removeFieldMapping('pathauto');
  }
}

/**
 * This migration works with WinePrepMigration to make sure auto_nodetitle is
 * re-enabled if we disabled it.
 */
class WineFinishMigration extends MigrationBase {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description =
      t('If auto_nodetitle is present and was previously enabled, re-enable it');
  }

  public function isComplete() {
    // There is no incomplete state for this operation.
    return TRUE;
  }

  public function import() {
    if (!module_exists('auto_nodetitle')) {
      if (WinePrepMigration::$wasEnabled) {
        module_enable(array('auto_nodetitle'));
        self::displayMessage(t('Re-enabled auto_nodetitle module'), 'success');
      }
      else {
        self::displayMessage(t('auto_nodetitle was not originally enabled'),
          'success');
      }
    }
    else {
      self::displayMessage(t('Auto_nodetitle module already enabled'),
        'success');
    }
    return Migration::RESULT_COMPLETED;
  }
}

/**
 * TIP: This demonstrates a migration designed not to import new content, but
 * to update existing content (in this case, revised wine ratings)
 */
class WineUpdatesMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Update wine ratings');

    $query = db_select('migrate_example_wine_updates', 'w')
      ->fields('w', array('wineid', 'rating'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Indicate we're updating existing data. The default, Migration::SOURCE,
    // would cause existing nodes to be completely replaced by the source data.
    // In this case, the existing node will be loaded and only the rating
    // altered.
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    // The destination handler needs the nid to change - since the incoming data
    // has a source id, not a nid, we need to apply the original wine migration
    // mapping to populate the nid.
    $this->addFieldMapping('nid', 'wineid')
      ->sourceMigration('WineWine');
    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');

    // No unmapped source fields

    // Unmapped destination fields - these will be left unchanged in this case
    // (normally they would be overwritten with their default values).
    $this->addFieldMapping('uid');
    $this->addFieldMapping('migrate_example_wine_varieties');
    $this->addFieldMapping('migrate_example_wine_regions');
    $this->addFieldMapping('migrate_example_wine_best_with');
    $this->addFieldMapping('body');
    $this->addFieldMapping('field_migrate_example_image');
    $this->addFieldMapping('sticky');
    $this->addFieldMapping('created');
    $this->addFieldMapping('changed');
    $this->addUnmigratedDestinations(array(
      'body:format',
      'body:summary',
      'comment',
      'field_migrate_example_image:alt',
      'field_migrate_example_image:destination_dir',
      'field_migrate_example_image:destination_file',
      'field_migrate_example_image:file_class',
      'field_migrate_example_image:file_replace',
      'field_migrate_example_image:preserve_files',
      'field_migrate_example_image:source_dir',
      'field_migrate_example_image:title',
      'field_migrate_example_image:urlencode',
      'field_migrate_example_top_vintag',
      'is_new',
      'language',
      'log',
      'migrate_example_wine_best_with:source_type',
      'migrate_example_wine_best_with:create_term',
      'migrate_example_wine_best_with:ignore_case',
      'migrate_example_wine_regions:source_type',
      'migrate_example_wine_regions:create_term',
      'migrate_example_wine_regions:ignore_case',
      'migrate_example_wine_varieties:source_type',
      'migrate_example_wine_varieties:create_term',
      'migrate_example_wine_varieties:ignore_case',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'title',
      'tnid',
      'translate',
    ));
    if (module_exists('statistics')) {
      $this->addUnmigratedDestinations(
        array('totalcount', 'daycount', 'timestamp'));
    }
  }
}

class WineCommentUpdatesMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = 'Update wine comments';

    $query = db_select('migrate_example_wine_comment_updates', 'wc')
      ->fields('wc', array('commentid', 'subject'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination =
      new MigrateDestinationComment('comment_node_migrate_example_wine');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'commentid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationComment::getKeySchema()
    );

    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('cid', 'commentid')
      ->sourceMigration('WineComment');
    $this->addFieldMapping('subject', 'subject');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'changed',
      'comment_body',
      'comment_body:format',
      'created',
      'homepage',
      'hostname',
      'language',
      'mail',
      'name',
      'nid',
      'pid',
      'status',
      'thread',
      'uid',
    ));

    $this->removeFieldMapping('pathauto');
  }
}

class WineVarietyUpdatesMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description =
      t('Migrate varieties from the source database to taxonomy terms');

    $query = db_select('migrate_example_wine_variety_updates', 'wc')
      ->fields('wc', array('categoryid', 'details'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination =
      new MigrateDestinationTerm('migrate_example_wine_varieties');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'categoryid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('tid', 'categoryid')
      ->sourceMigration('WineVariety');
    $this->addFieldMapping('description', 'details');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'format',
      'name',
      'parent',
      'parent_name',
      'weight',
    ));
  }
}

class WineUserUpdatesMigration extends AdvancedExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Account updates');

    $query = db_select('migrate_example_wine_account_updates', 'wa')
      ->fields('wa', array('accountid', 'sex'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'accountid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Account ID.',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('uid', 'accountid')
      ->sourceMigration('WineUser');
    $this->addFieldMapping('field_migrate_example_gender', 'sex')
      ->description(t('Map from M/F to 0/1 in prepare method'));

    // Unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'access',
      'created',
      'data',
      'init',
      'is_new',
      'language',
      'login',
      'mail',
      'name',
      'pass',
      'picture',
      'role_names',
      'roles',
      'signature',
      'signature_format',
      'status',
      'theme',
      'timezone',
    ));
  }

  public function prepare(stdClass $account, stdClass $row) {
    // Gender data comes in as M/F, needs to be saved as Male=0/Female=1
    // TIP: Note that the Migration prepare method is called after all other
    // prepare handlers. Most notably, the field handlers have had their way
    // and created field arrays, so we have to save in the same format.
    switch ($row->sex) {
      case 'm':
      case 'M':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 0;
        break;
      case 'f':
      case 'F':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 1;
        break;
      default:
        $account->field_migrate_example_gender = NULL;
        break;
    }
  }
}
