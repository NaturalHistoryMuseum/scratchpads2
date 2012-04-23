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
  public $basicFormat;

  public function __construct() {
    // TIP: Migrations can be organized into groups. In this case, all the migrations
    // derived from AdvancedExampleMigration will be part of the 'wine' group.
    // This enables us to easily run just the wine example migrations:
    //  drush migrate-import --group=wine
    // The second argument to MigrateGroup::getInstance is an array of groups
    // which should come before this when viewing migration statuses, or running
    // migration operations using --all. Since the beer migrations in this module
    // did not specify a group, it is in the 'default' group, so this constructor
    // indicates that the wine migrations come after the beer migrations.
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));

    $this->team = array(
      new MigrateTeamMember('Jack Kramer', 'jkramer@example.com', t('Taster')),
      new MigrateTeamMember('Linda Madison', 'lmadison@example.com', t('Winemaker')),
    );
    $this->issuePattern = 'http://drupal.org/node/:id:';

    // A format of our own, for testing migration of formats
    $this->basicFormat = filter_format_load('migrate_example');
  }
}

/**
 * TIP: While usually you'll create true migrations - processes that copy data
 * from some source into Drupal - you can also define processing steps for either
 * the import or rollback stages that take other actions. In this case, we want
 * to disable auto_nodetitle while the migration steps run. We'll re-enable it
 * over in WineFinishMigration.
 */
class WinePrepMigration extends MigrationBase {
  // Track whether the auto_nodetitle was originally enabled so we know whether
  // to re-enable it. This is public so WineFinishMigration can reference it.
  public static $wasEnabled = FALSE;

  public function __construct() {
    // Because we're derived directly from migrationBase rather than AdvancedExampleMigration,
    // we must specify the group again here.
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
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
    // Must return one of the MigrationBase RESULT constants
    return MigrationBase::RESULT_COMPLETED;
  }
}

// The term migrations are very similar - implement the commonalities here
abstract class WineTermMigration extends AdvancedExampleMigration {
  public function __construct($type, $vocabulary_name, $description) {
    parent::__construct();
    $this->description = $description;
    $this->dependencies = array('WinePrep');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'categoryid' => array('type' => 'int',
                                'unsigned' => TRUE,
                                'not null' => TRUE,
                               )
        ),
        MigrateDestinationTerm::getKeySchema()
      );

    $query = db_select('migrate_example_wine_categories', 'wc')
             ->fields('wc', array('categoryid', 'name', 'details', 'category_parent', 'ordering'))
             ->condition('type', $type)
             // This sort assures that parents are saved before children.
             ->orderBy('category_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm($vocabulary_name);

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
  public function __construct() {
    parent::__construct('variety', 'migrate_example_wine_varieties',
      t('Migrate varieties from the source database to taxonomy terms'));
  }
}

class WineRegionMigration extends WineTermMigration {
  public function __construct() {
    parent::__construct('region', 'migrate_example_wine_regions',
      t('Migrate regions from the source database to taxonomy terms'));
  }
}

class WineBestWithMigration extends WineTermMigration {
  public function __construct() {
    parent::__construct('best_with', 'migrate_example_wine_best_with',
      t('Migrate "Best With" from the source database to taxonomy terms'));
  }
}

/**
 * TIP: Files can be migrated directly by themselves, by using the MigrateDestinationFile
 * class. This will copy the files themselves from the source, and set up the
 * Drupal file tables appropriately.
 */
class WineFileCopyMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Profile images');
    $this->dependencies = array('WinePrep');
    $this->map = new MigrateSQLMap($this->machineName,
        array('imageid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Image ID.'
                )
             ),
        MigrateDestinationFile::getKeySchema()
    );
    $query = db_select('migrate_example_wine_files', 'wf')
             ->fields('wf', array('imageid', 'url'))
             ->isNull('wineid');
    $this->source = new MigrateSourceSQL($query);

    // TIP: Set copy_file to copy the file from its source (which could be a
    // remote server, i.e. the uri is of the form http://example.com/images/foo.jpg).
    $this->destination = new MigrateDestinationFile(array('copy_file' => TRUE));

    // Just map the incoming URL to the destination's 'uri'
    $this->addFieldMapping('uri', 'url');
    $this->addUnmigratedDestinations(array('fid', 'uid', 'filename', 'status',
      'filemime', 'timestamp'));
  }
}

/**
 * Migration class to test importing from a BLOB column into a file entity. Typically,
 * one would use this OR import into a File Field (@see MigrateExampleOracleNode).
 */
class WineFileBlobMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration from BLOB column into files.');
    $this->dependencies = array('WinePrep');
    $this->map = new MigrateSQLMap($this->machineName,
        array('imageid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Image ID.'
                )
             ),
        MigrateDestinationFile::getKeySchema()
    );
    $query = db_select('migrate_example_wine_blobs', 'wf')
             ->fields('wf', array('imageid', 'imageblob'));
    $this->source = new MigrateSourceSQL($query);

    $options = array(
      'copy_blob' => TRUE,
      'copy_destination' => 'public://', // This is the default. Specified for completeness.
    );
    $this->destination = new MigrateDestinationFile($options);

    // Basic fields
    $this->addFieldMapping('uid')
         ->defaultValue(1);
    $this->addFieldMapping('filename')
         ->defaultValue('druplicon.png');
    $this->addFieldMapping('contents', 'imageblob')
         ->description('An image blob in the DB');
    $this->addFieldMapping('timestamp', 'CREATED');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('fid', 'uri', 'filemime', 'status'));
  }
}

class WineRoleMigration extends XMLMigration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
    $this->description = t('XML feed (multi items) of roles (positions)');

    // TIP: Regular dependencies, besides enforcing (in the absence of --force)
    // the run order of migrations, affect the sorting of migrations on display.
    // You can use soft dependencies to affect just the display order when the
    // migrations aren't technically required to run in a certain order. In this
    // case, we want the role migration to appear after the file migration.
    $this->softDependencies = array('WineFileCopy');

    // There isn't a consistent way to automatically identify appropriate "fields"
    // from an XML feed, so we pass an explicit list of source fields
    $fields = array(
      'name' => t('Position name'),
    );

    // The source ID here is the one retrieved from each data item in the XML file, and
    // used to identify specific items
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationRole::getKeySchema()
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
    $items_url = $xml_folder . 'positions.xml';
    $item_xpath = '/positions/position';  // relative to document
    $item_ID_xpath = 'sourceid';         // relative to item_xpath and gets assembled
                                         // into full path /producers/producer/sourceid

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationRole();

    $this->addFieldMapping('name', 'name')
         ->xpath('name');
    $this->addUnmigratedDestinations(array('weight'));
  }
}

class WineUserMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wine Drinkers of the world');
    $this->dependencies = array('WinePrep', 'WineFileCopy', 'WineRole');
    $this->map = new MigrateSQLMap($this->machineName,
        array('accountid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Account ID.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = db_select('migrate_example_wine_account', 'wa')
             ->fields('wa', array('accountid', 'status', 'posted', 'name',
                'password', 'mail', 'last_access', 'last_login',
                'original_mail', 'sig', 'sex', 'imageid', 'positions'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();

    // Mapped fields
    $this->addSimpleMappings(array('name', 'status', 'mail'));
    $this->addFieldMapping('created', 'posted')
         ->description('See prepare method');
    $this->addFieldMapping('access', 'last_access')
         ->description('See prepare method');
    $this->addFieldMapping('login', 'last_login')
         ->description('See prepare method');
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
    $this->addUnmigratedDestinations(array('theme', 'timezone', 'language', 'is_new'));
  }

  public function prepare(stdClass $account, stdClass $row) {
    // Source dates are in ISO format.
    // Because the mappings above have been applied, $account->created contains
    // the date/time string now - we could also pass $row->posted here.
    $account->created = strtotime($account->created);
    $account->access = strtotime($account->access);
    $account->login = strtotime($account->login);

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
        unset($account->field_migrate_example_gender);
        break;
    }
  }
}

class WineProducerMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wine producers of the world');
    $this->dependencies = array('WineRegion', 'WineUser');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'producerid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'p',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine_producer', 'p')
             ->fields('p', array('producerid', 'name', 'body', 'excerpt', 'accountid'));
    // Region term is singletons, handled straighforwardly
    $query->leftJoin('migrate_example_wine_category_producer', 'reg',
      "p.producerid = reg.producerid");
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
         ->sourceMigration('WineRegion')
         ->arguments(array('source_type' => 'tid'));
    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'excerpt'));
    $this->addFieldMapping('body', 'body')
         ->arguments($arguments);
    $this->addFieldMapping(NULL, 'excerpt');
    $this->addFieldMapping('sticky')
         ->defaultValue(0);

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'name', 'created', 'changed',
      'status', 'promote', 'revision', 'language'));
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
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
    $this->description = t('XML feed of wine producers of the world');
    $this->dependencies = array('WineRegion', 'WineUser');

    // There isn't a consistent way to automatically identify appropriate "fields"
    // from an XML feed, so we pass an explicit list of source fields
    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // The source ID here is the one retrieved from the XML listing file, and
    // used to identify the specific item's file
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
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
    $list_url = $xml_folder . 'index.xml';
    // Each ID retrieved from the list URL will be plugged into :id in the
    // item URL to fetch the specific objects.
    $item_url = $xml_folder . ':id.xml';

    // We use the MigrateSourceList class for any source where we obtain the list
    // of IDs to process separately from the data for each item. The listing
    // and item are represented by separate classes, so for example we could
    // replace the XML listing with a file directory listing, or the XML item
    // with a JSON item.
    $this->source = new MigrateSourceList(new MigrateListXML($list_url),
      new MigrateItemXML($item_url), $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.
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
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
    $this->description = t('XML feed (multi items) of wine producers of the world');
    $this->dependencies = array('WineRegion', 'WineUser');

    // There isn't a consistent way to automatically identify appropriate "fields"
    // from an XML feed, so we pass an explicit list of source fields
    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // The source ID here is the one retrieved from each data item in the XML file, and
    // used to identify specific items
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
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

    // We use the MigrateSourceMultiItems class for any source where we obtain the list
    // of IDs to process and the data for each item from the same file. Typically the data
    // for an item is not contained in a single line within the source file. Examples include
    // multiple items defined in a single xml file or a single json file where in both cases
    // the id is part of the item.

    $item_xpath = '/producers/producer';  // relative to document

    $item_ID_xpath = 'sourceid';         // relative to item_xpath and gets assembled
                                         // into full path /producers/producer/sourceid

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.
    // TIP: Note that all xpaths for fields begin at the last element of the item
    // xpath since each item xml chunk is processed individually.
    // (ex. xpath=name is equivalent to a full xpath of /producers/producer/name)
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
  }
}

/**
 * TIP: An alternative approach using MigrateSourceSQL. This uses a different
 * XML library, which advances element-by-element through the XML file rather
 * than reading in the whole file. This source will work better with large XML
 * files, but is slower for small files and has a more restrictive query lanaguage
 * for selecting the elements to process.
 */
class WineProducerXMLPullMigration extends XMLMigration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
    $this->description = t('XML feed (pull) of wine producers of the world');
    $this->dependencies = array('WineRegion', 'WineUser');

    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
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
    $items_url = $xml_folder . 'producers2.xml';

    // As with MigrateSourceMultiItems, this applies where there is not a separate
    // list of IDs to process - the source XML file is entirely self-contained.
    // For the ID path, and xpath for each component, we can use the full xpath
    // syntax as usual. However, the syntax to select the elements that correspond
    // to objects to import is more limited. It must be a fully-qualified path
    // to the element (i.e., /producers/producer rather than just //producer).
    $item_xpath = '/producers/producer';  // relative to document
    $item_ID_xpath = 'sourceid';          // relative to item_xpath

    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath,
      $fields);

    $this->destination = new MigrateDestinationNode('migrate_example_producer');
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
  }
}

// TODO: Add node_reference field pointing to producer
class WineWineMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wines of the world');
    $this->dependencies = array('WineVariety', 'WineRegion',
      'WineBestWith', 'WineUser', 'WineProducer');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine', 'w')
             ->fields('w', array('wineid', 'name', 'body', 'excerpt', 'accountid',
              'posted', 'last_changed', 'variety', 'region', 'rating'));
    $query->leftJoin('migrate_example_wine_category_wine', 'cwbw',
      "w.wineid = cwbw.wineid");
    $query->leftJoin('migrate_example_wine_categories', 'bw',
      "cwbw.categoryid = bw.categoryid AND bw.type = 'best_with'");
    // Gives a single comma-separated list of related terms
    $query->groupBy('w.wineid');
    $query->addExpression('GROUP_CONCAT(bw.categoryid)', 'best_with');

    $count_query = db_select('migrate_example_wine', 'w');
    $count_query->addExpression('COUNT(wineid)', 'cnt');

    // TIP: By passing an array of source fields to the MigrateSourceSQL constructor,
    // we can modify the descriptions of source fields (which just default, for
    // SQL migrations, to table_alias.column_name), as well as add additional fields
    // (which may be populated in prepareRow()).
    $source_fields = array(
      'wineid' => t('Wine ID in the old system'),
      'name' => t('The name of the wine'),
      'best_vintages' => t('What years were best for this wine?'),
      'images' => t('Images attached to this wine; populated in prepareRow()'),
    );

    // TIP: By default, each time a migration is run, any previously unimported source items
    // are imported (along with any previously-imported items marked for update). If the
    // source data contains a timestamp that is set to the creation time of each new item,
    // as well as set to the update time for any existing items that are updated, then
    // you can have those updated items automatically reimported by setting the field as
    // your highwater field.
    $this->highwaterField = array(
      'name' => 'last_changed', // Column to be used as highwater mark
      'alias' => 'w',           // Table alias containing that column
      'type' => 'int',          // By default, highwater marks are assumed to be lexicographically
                                // sortable (e.g., '2011-05-19 17:53:12'). To properly
                                // deal with integer highwater marks (such as UNIX
                                // timestamps), indicate so here.
    );
    // Note that it is important to process rows in the order of the highwater mark
    $query->orderBy('last_changed');

    $this->source = new MigrateSourceSQL($query, $source_fields, $count_query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');

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
         ->sourceMigration('WineVariety')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('migrate_example_wine_regions', 'region')
         ->sourceMigration('WineRegion')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('migrate_example_wine_best_with', 'best_with')
         ->separator(',')
         ->sourceMigration('WineBestWith')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');
    $this->addFieldMapping('field_migrate_example_top_vintag', 'best_vintages');

    // TIP: You can apply one or more functions to a source value using ->callbacks().
    // The function must take a single argument and return a value which is a
    // transformation of the argument. As this example shows, you can have multiple
    // callbacks, and they can either be straight functions or class methods. In
    // this case, our custom method prepends 'review: ' to the body, and then we
    // call a standard Drupal function to uppercase the whole body.
    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'excerpt'));
    $this->addFieldMapping('body', 'body')
         ->arguments($arguments)
         ->callbacks(array($this, 'addTitlePrefix'), 'drupal_strtoupper');
    $this->addFieldMapping(NULL, 'excerpt');
    // We will get the image data from a related table in prepareRow()
    $arguments = MigrateFileFieldHandler::arguments(NULL, 'file_copy', FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_migrate_example_image', 'images')
         ->arguments($arguments);
    $this->addFieldMapping('sticky')
         ->defaultValue(0);
    // These are already UNIX timestamps, so just pass through
    $this->addFieldMapping('created', 'posted');
    $this->addFieldMapping('changed', 'last_changed');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language'));
  }

  protected function addTitlePrefix($source_title) {
    return t('review: ') . $source_title;
  }

  // TIP: Implement a prepareRow() method to manipulate the source row between
  // retrieval from the database and the automatic applicaton of mappings
  public function prepareRow($current_row) {
    // We used the MySQL GROUP_CONCAT function above to handle a multi-value source
    // field - more portably, we query the related table with multiple values here,
    // so the values can run through the mapping process
    $source_id = $current_row->wineid;
    $result = db_select('migrate_example_wine_vintages', 'v')
              ->fields('v', array('vintage'))
              ->condition('wineid', $source_id)
              ->execute();
    foreach ($result as $row) {
      $current_row->best_vintages[] = $row->vintage;
    }

    // An advanced feature of the file field handler is that in addition to the
    // path to the image itself, we can add image properties like ALT text,
    // encapsulating them as JSON
    $result = db_select('migrate_example_wine_files', 'f')
              ->fields('f', array('url', 'image_alt', 'image_title'))
              ->condition('wineid', $source_id)
              ->execute();
    $current_row->images = array();
    foreach ($result as $row) {
      $image_data = array(
        'path' => $row->url,
        'alt' => $row->image_alt,
        'title' => $row->image_title,
      );
      $current_row->images[] = drupal_json_encode($image_data);
    }

    // We could also have used this function to decide to skip a row, in cases
    // where that couldn't easily be done through the original query. Simply
    // return FALSE in such cases.
    return TRUE;
  }
}

class WineCommentMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Comments about wines';
    $this->dependencies = array('WineUser', 'WineWine');
    $this->map = new MigrateSQLMap($this->machineName,
      array('commentid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
             )
           ),
        MigrateDestinationComment::getKeySchema()
      );
    $query = db_select('migrate_example_wine_comment', 'wc')
             ->fields('wc', array('commentid', 'comment_parent', 'name', 'mail',
              'accountid', 'body', 'wineid', 'subject', 'commenthost', 'userpage',
              'posted', 'lastchanged'))
             ->orderBy('comment_parent');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_wine');

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
    $this->addUnmigratedDestinations(array('thread', 'language'));
  }
}

// TIP: An easy way to simply migrate into a Drupal table (i.e., one defined
// through the Schema API) is to use the MigrateDestinationTable destination.
// Just pass the table name to getKeySchema and the MigrateDestinationTable constructor.
class WineTableMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Miscellaneous table data';
    $this->softDependencies = array('WineComment');
    $table_name = 'migrate_example_wine_table_dest';
    $this->map = new MigrateSQLMap($this->machineName,
      array('fooid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
             )
           ),
        MigrateDestinationTable::getKeySchema($table_name)
      );
    $query = db_select('migrate_example_wine_table_source', 't')
             ->fields('t', array('fooid', 'field1', 'field2'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTable($table_name);

    // Mapped fields
    $this->addFieldMapping('drupal_text', 'field1');
    $this->addFieldMapping('drupal_int', 'field2');

    $this->addUnmigratedDestinations(array('recordid'));
  }
}

/**
 * This migration works with WinePrepMigration to make ensure auto_nodetitle
 * is re-enabled if we disabled it.
 */
class WineFinishMigration extends MigrationBase {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('wine', array('default')));
    $this->description = t('If auto_nodetitle is present and was previously enabled,
      re-enable it');
    $this->dependencies = array('WineComment');
  }
  public function isComplete() {
    if (module_exists('auto_nodetitle')) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  public function import() {
    if (!module_exists('auto_nodetitle')) {
      if (WinePrepMigration::$wasEnabled) {
        module_enable(array('auto_nodetitle'));
        self::displayMessage(t('Re-enabled auto_nodetitle module'), 'success');
      }
      else {
        self::displayMessage(t('auto_nodetitle was not originally enabled'), 'success');
      }
    }
    else {
      self::displayMessage(t('Auto_nodetitle module already enabled'), 'success');
    }
    return Migration::RESULT_COMPLETED;
  }
}

/**
 * TIP: This demonstrates a migration designed not to import new content, but
 * to update existing content (in this case, revised wine ratings)
 */
class WineUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Update wine ratings');
    $this->dependencies = array('WineWine');
    $this->softDependencies = array('WineFinish');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine_updates', 'w')
             ->fields('w', array('wineid', 'rating'));

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');

    // Indicate we're updating existing data. The default, Migration::SOURCE, would
    // cause existing nodes to be completely replaced by the source data. In this
    // case, the existing node will be loaded and only the rating altered.
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    // The destination handler needs the nid to change - since the incoming data
    // has a source id, not a nid, we need to apply the original wine migration
    // mapping to populate the nid.
    $this->addFieldMapping('nid', 'wineid')
         ->sourceMigration('WineWine');
    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('uid');
    $this->addFieldMapping('migrate_example_wine_varieties');
    $this->addFieldMapping('migrate_example_wine_regions');
    $this->addFieldMapping('migrate_example_wine_best_with');
    $this->addFieldMapping('body');
    $this->addFieldMapping('field_migrate_example_image');
    $this->addFieldMapping('sticky');
    $this->addFieldMapping('created');
    $this->addFieldMapping('changed');
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language'));
  }
}

class WineCommentUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Update wine comments';
    $this->dependencies = array('WineComment');
    $this->softDependencies = array('WineUpdates');
    $this->map = new MigrateSQLMap($this->machineName,
      array('commentid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
             )
           ),
        MigrateDestinationComment::getKeySchema()
      );
    $query = db_select('migrate_example_wine_comment_updates', 'wc')
             ->fields('wc', array('commentid', 'subject'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_wine');
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('cid', 'commentid')
         ->sourceMigration('WineComment');
    $this->addFieldMapping('subject', 'subject');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('mail');
    $this->addFieldMapping('status');
    $this->addFieldMapping('nid');
    $this->addFieldMapping('uid');
    $this->addFieldMapping('pid');
    $this->addFieldMapping('comment_body');
    $this->addFieldMapping('hostname');
    $this->addFieldMapping('created');
    $this->addFieldMapping('changed');
    $this->addFieldMapping('homepage');
    $this->addUnmigratedDestinations(array('thread', 'language'));
  }
}

class WineVarietyUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate varieties from the source database to taxonomy terms');
    $this->dependencies = array('WineVariety');
    $this->softDependencies = array('WineUpdates');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'categoryid' => array('type' => 'int',
                                'unsigned' => TRUE,
                                'not null' => TRUE,
                               )
        ),
        MigrateDestinationTerm::getKeySchema()
      );

    $query = db_select('migrate_example_wine_variety_updates', 'wc')
             ->fields('wc', array('categoryid', 'details'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('migrate_example_wine_varieties');
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('tid', 'categoryid')
         ->sourceMigration('WineVariety');
    $this->addFieldMapping('description', 'details');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('parent');
    $this->addFieldMapping('weight');
    $this->addFieldMapping('format');
    $this->addFieldMapping('parent_name')
         ->issueGroup(t('DNM'));
  }
}


class WineUserUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Account updates');
    $this->dependencies = array('WineUser');
    $this->softDependencies = array('WineUpdates');
    $this->map = new MigrateSQLMap($this->machineName,
        array('accountid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Account ID.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = db_select('migrate_example_wine_account_updates', 'wa')
             ->fields('wa', array('accountid', 'sex'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('uid', 'accountid')
         ->sourceMigration('WineUser');
    $this->addFieldMapping('field_migrate_example_gender', 'sex')
         ->description(t('Map from M/F to 0/1 in prepare method'));

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('status');
    $this->addFieldMapping('created');
    $this->addFieldMapping('access');
    $this->addFieldMapping('login');
    $this->addFieldMapping('mail');
    $this->addFieldMapping('pass');
    $this->addFieldMapping('roles');
    $this->addFieldMapping('signature');
    $this->addFieldMapping('signature_format');
    $this->addFieldMapping('init');
    $this->addUnmigratedDestinations(array('theme', 'timezone', 'language', 'picture'));
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
