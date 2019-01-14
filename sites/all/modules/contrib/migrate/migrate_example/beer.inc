<?php

/**
 * @file
 * A basic example of using the Migrate module to import taxonomy, users, nodes,
 * and comments.
 *
 * The basic idea is
 * - The users in the source application are listed in the
 *   migrate_example_beer_account table and are transformed into Drupal users.
 * - Drupal "beer" nodes describe beers; The information to create the nodes
 *   comes from the migrate_example_beer_node table.
 * - Taxonomy terms for the beer nodes (ale, pilsner) come from the
 *   migrate_example_beer_topic table and they are applied to nodes using the
 *   source information in the migrate_example_beer_topic_node table.
 * - Comments to be attached to the beer nodes are described in the source
 *   migrate_example_beer_comment table.
 *
 * We will use the Migrate API to import and transform this data and turn it
 * into a working Drupal site.
 */

/**
 * To define a migration process from a set of source data to a particular
 * kind of Drupal object (for example, a specific node type), you define
 * a class derived from Migration. You must define a constructor to initialize
 * your migration object.
 *
 * For your classes to be instantiated so they can be used to import content,
 * you must register them - look at migrate_example.migrate.inc to see how
 * registration works. Right now, it's important to understand that each
 * migration will have a unique "machine name", which is displayed in the UI
 * and is used to reference the migration in drush commands.
 *
 * In any serious migration project, you will find there are some options
 * which are common to the individual migrations you're implementing. You can
 * define an abstract intermediate class derived from Migration, then derive
 * your individual migrations from that, to share settings, utility functions,
 * etc.
 */
abstract class BasicExampleMigration extends Migration {

  // A Migration constructor takes an array of arguments as its first parameter.
  // The arguments must be passed through to the parent constructor.
  public function __construct($arguments) {
    parent::__construct($arguments);

    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default the
    // list in the shared class; it can be overridden for specific migrations.
    $this->team = array(
      new MigrateTeamMember('Liz Taster', 'ltaster@example.com',
        t('Product Owner')),
      new MigrateTeamMember('Larry Brewer', 'lbrewer@example.com',
        t('Implementor')),
    );

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id:' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    $this->issuePattern = 'http://drupal.org/node/:id:';
  }
}

/**
 * There are four essential components to set up in your constructor:
 *  $this->source - An instance of a class derived from MigrateSource, this
 *    will feed data to the migration.
 *  $this->destination - An instance of a class derived from MigrateDestination,
 *    this will receive data that originated from the source and has been mapped
 *    by the Migration class, and create Drupal objects.
 *  $this->map - An instance of a class derived from MigrateMap, this will keep
 *    track of which source items have been imported and what destination
 *    objects they map to.
 *  Field mappings - Use $this->addFieldMapping to tell the Migration class what
 *    source fields correspond to what destination fields, and additional
 *    information associated with the mappings.
 */
class BeerTermMigration extends BasicExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    // Human-friendly description of your migration process. Be as detailed as
    // you like.
    $this->description =
      t('Migrate styles from the source database to taxonomy terms');

    // In this example, we're using tables that have been added to the existing
    // Drupal database but which are not Drupal tables. You can examine the
    // various tables (starting here with migrate_example_beer_topic) using a
    // database browser such as phpMyAdmin.
    // First, we set up a query for this data. Note that by ordering on
    // style_parent, we guarantee root terms are migrated first, so the
    // parent_name mapping below will find that the parent exists.
    $query = db_select('migrate_example_beer_topic', 'met')
      ->fields('met', array(
        'style',
        'details',
        'style_parent',
        'region',
        'hoppiness',
      ))
      // This sort assures that parents are saved before children.
      ->orderBy('style_parent', 'ASC');

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);

    // Set up our destination - terms in the migrate_example_beer_styles
    // vocabulary (note that we pass the machine name of the vocabulary).
    $this->destination =
      new MigrateDestinationTerm('migrate_example_beer_styles');

    // Create a map object for tracking the relationships between source rows
    // and their resulting Drupal objects. We will use the MigrateSQLMap class,
    // which uses database tables for tracking. Pass the machine name (BeerTerm)
    // of this migration to use in generating map and message table names.
    // And, pass schema definitions for the primary keys of the source and
    // destination - we need to be explicit for our source, but the destination
    // class knows its schema already.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'style' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Topic ID',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Assign mappings TO destination fields FROM source fields. To discover
    // the names used in these calls, use the drush commands
    //  drush migrate-fields-destination BeerTerm
    //  drush migrate-fields-source BeerTerm
    // or review the detail pages in the UI.
    $this->addFieldMapping('name', 'style');
    $this->addFieldMapping('description', 'details');

    // Documenting your mappings makes it easier for the whole team to see
    // exactly what the status is when developing a migration process.
    $this->addFieldMapping('parent_name', 'style_parent')
      ->description(t('The incoming style_parent field is the name of the term parent'));

    // Mappings are assigned issue groups, by which they are grouped on the
    // migration info page when the migrate_ui module is enabled. The default
    // is 'Done', indicating active mappings which need no attention. A
    // suggested practice is to use groups of:
    // Do Not Migrate (or DNM) to indicate source fields which are not being
    //  used, or destination fields not to be populated by migration.
    // Client Issues to indicate input from the client is needed to determine
    //  how a given field is to be migrated.
    // Implementor Issues to indicate that the client has provided all the
    //  necessary information, and now the implementor needs to complete the
    //  work.
    $this->addFieldMapping(NULL, 'hoppiness')
      ->description(t('This info will not be maintained in Drupal'))
      ->issueGroup(t('DNM'));

    // Open mapping issues can be assigned priorities (the default is
    // MigrateFieldMapping::ISSUE_PRIORITY_OK). If you're using an issue
    // tracking system, and have defined issuePattern (see BasicExampleMigration
    // above), you can specify a ticket/issue number in the system on the
    // mapping and migrate_ui will link directly to it.
    $this->addFieldMapping(NULL, 'region')
      ->description('Will a field be added to the vocabulary for this?')
      ->issueGroup(t('Client Issues'))
      // This priority wil cause the mapping to be highlighted in the UI.
      ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_MEDIUM)
      ->issueNumber(770064);

    // It is good practice to account for all source and destination fields
    // explicitly - this makes sure that everyone understands exactly what is
    // being migrated and what is not. Also, migrate_ui highlights unmapped
    // fields, or mappings involving fields not in the source and destination,
    // so if (for example) a new field is added to the destination typ it's
    // immediately visible, and you can decide if anything needs to be
    // migrated into it.
    $this->addFieldMapping('format')
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('weight')
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('parent')
      ->issueGroup(t('DNM'));

    // We conditionally DNM these fields, so your field mappings will be clean
    // whether or not you have path and/or pathauto enabled.
    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }
}

/**
 * And that's it for the BeerTerm migration! For a simple migration, all you
 * have to do is define the source, the destination, and mappings between the
 * two - to import the data you simply do:
 * drush migrate-import BeerTerm
 *
 * However, in real-world migrations not everything can be represented simply
 * through static mappings - you will frequently need to do some run-time
 * transformations of the data.
 */
class BeerUserMigration extends BasicExampleMigration {

  public function __construct($arguments) {
    // The basic setup is similar to BeerTermMigraiton
    parent::__construct($arguments);
    $this->description = t('Beer Drinkers of the world');
    $query = db_select('migrate_example_beer_account', 'mea')
      ->fields('mea', array(
        'aid',
        'status',
        'posted',
        'name',
        'nickname',
        'password',
        'mail',
        'sex',
        'beers',
      ));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'aid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'Account ID.',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    // One good way to organize your mappings in the constructor is in three
    // groups - mapped fields, unmapped source fields, and unmapped destination
    // fields.

    // Mapped fields

    // This is a shortcut you can use when the source and destination field
    // names are identical (e.g., the email address field is named 'mail' in
    // both the source table and in Drupal).
    $this->addSimpleMappings(array('status', 'mail'));

    // Our source table has two entries for 'alice', but we must have a unique
    // username in the Drupal 'users' table. dedupe() creates new, unique
    // destination values when the source field of that value already exists.
    // For example, if we're importing a user with name 'test' and a user
    // 'test' already exists in the target, we'll create a new user named
    // 'test_1'.
    // dedupe() takes the Drupal table and column for determining uniqueness.
    $this->addFieldMapping('name', 'name')
      ->dedupe('users', 'name');

    // The migrate module automatically converts date/time strings to UNIX
    // timestamps.
    $this->addFieldMapping('created', 'posted');

    $this->addFieldMapping('pass', 'password');

    // Instead of mapping a source field to a destination field, you can
    // hardcode a default value. You can also use both together - if a default
    // value is provided in addition to a source field, the default value will
    // be applied to any rows where the source field is empty or NULL.
    $this->addFieldMapping('roles')
      ->defaultValue(DRUPAL_AUTHENTICATED_RID);

    $this->addFieldMapping('field_migrate_example_gender', 'sex');

    // The source field has beer names separated by a pipe character ('|'). By
    // adding ->separator('|'), the migration will automatically break them out,
    // look up the node with each title, and assign the node reference to this
    // user.
    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_migrate_example_favbeers', 'beers')
        ->separator('|');
    }
    else {
      $this->addFieldMapping(NULL, 'beers')
        ->issueGroup(t('DNM'));
    }

    // Unmapped source fields
    $this->addFieldMapping(NULL, 'nickname')
      ->issueGroup(t('DNM'));

    // Unmapped destination fields

    // This is a shortcut you can use to mark several destination fields as DNM
    // at once.
    $this->addUnmigratedDestinations(array(
      'access',
      'data',
      'is_new',
      'language',
      'login',
      'picture',
      'role_names',
      'signature',
      'signature_format',
      'theme',
      'timezone',
    ));

    // Oops, we made a typo - this should have been 'init'! If you have
    // migrate_ui enabled, look at the BeerUser info page - you'll see that it
    // displays a warning "int used as destination field in mapping but not in
    // list of destination fields", and also lists "1 unmapped" under Destination,
    // where it highlights "init" as unmapped.
    $this->addFieldMapping('int')
      ->issueGroup(t('DNM'));

    $destination_fields = $this->destination->fields();
    if (isset($destination_fields['path'])) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (isset($destination_fields['pathauto'])) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }
}

/**
 * The BeerNodeMigration uses the migrate_example_beer_node table as source
 * and creates Drupal nodes of type 'Beer' as destination.
 */
class BeerNodeMigration extends BasicExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Beers of the world');

    // We have a more complicated query. The Migration class fundamentally
    // depends on taking a single source row and turning it into a single
    // Drupal object, so how do we deal with zero or more terms attached to
    // each node? One way (valid for MySQL only) is to pull them into a single
    // comma-separated list.
    $query = db_select('migrate_example_beer_node', 'b')
      ->fields('b', array(
        'bid',
        'name',
        'body',
        'excerpt',
        'aid',
        'countries',
        'image',
        'image_alt',
        'image_title',
        'image_description',
      ));
    $query->leftJoin('migrate_example_beer_topic_node', 'tb', 'b.bid = tb.bid');
    // Gives a single comma-separated list of related terms
    $query->groupBy('tb.bid');
    $query->addExpression('GROUP_CONCAT(tb.style)', 'terms');

    // By default, MigrateSourceSQL derives a count query from the main query -
    // but we can override it if we know a simpler way
    $count_query = db_select('migrate_example_beer_node', 'b');
    $count_query->addExpression('COUNT(bid)', 'cnt');

    // Passing the cache_counts option means the source count (shown in
    // drush migrate-status) will be cached - this can be very handy when
    // dealing with a slow source database.
    $this->source = new MigrateSourceSQL($query, array(), $count_query,
      array('cache_counts' => TRUE));

    // Set up our destination - nodes of type migrate_example_beer
    $this->destination = new MigrateDestinationNode('migrate_example_beer');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'bid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'Beer ID.',
          'alias' => 'b',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Mapped fields
    $this->addFieldMapping('title', 'name')
      ->description(t('Mapping beer name in source to node title'));
    $this->addFieldMapping('sticky')
      ->description(t('Should we default this to 0 or 1?'))
      ->issueGroup(t('Client questions'))
      ->issueNumber(765736)
      ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_LOW);

    // References to related objects (such as the author of the content) are
    // most likely going to be identifiers from the source data, not Drupal
    // identifiers (such as uids). You can use the mapping from the relevant
    // migration to translate from the old ID to the Drupal identifier.
    // Note that we also provide a default value of 1 - if the lookup fails to
    // find a corresponding uid for the aid, the owner will be the administrative
    // account.
    $this->addFieldMapping('uid', 'aid')
      // Note this is the machine name of the user migration.
      ->sourceMigration('BeerUser')
      ->defaultValue(1);

    // This is a multi-value text field - in the source data the values are
    // separated by |, so we tell migrate to split it by that character.
    $this->addFieldMapping('field_migrate_example_country', 'countries')
      ->separator('|');
    // These are related terms, which by default will be looked up by name.
    $this->addFieldMapping('migrate_example_beer_styles', 'terms')
      ->separator(',');

    // Some fields may have subfields such as text formats or summaries. These
    // can be individually mapped as we see here.
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:summary', 'excerpt');

    // File fields are more complex - the file needs to be copied, a Drupal
    // file entity (file_managed table row) created, and the field populated.
    // There are several different options involved. It's usually best to do
    // migrate the files themselves in their own migration (see wine.inc for an
    // example), but they can also be brought over through the field mapping.

    // We map the filename (relative to the source_dir below) to the field
    // itself.
    $this->addFieldMapping('field_migrate_example_image', 'image');
    // The file_class determines how the 'image' value is interpreted, and what
    // other options are available. In this case, MigrateFileUri indicates that
    // the 'image' value is a URI.
    $this->addFieldMapping('field_migrate_example_image:file_class')
      ->defaultValue('MigrateFileUri');
    // Here we specify the directory containing the source files.
    $this->addFieldMapping('field_migrate_example_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'migrate_example'));
    // And we map the alt and title values in the database to those on the image.
    $this->addFieldMapping('field_migrate_example_image:alt', 'image_alt');
    $this->addFieldMapping('field_migrate_example_image:title', 'image_title');

    // No description for images, only alt and title
    $this->addUnmigratedSources(array('image_description'));

    // Unmapped destination fields
    // Some conventions we use here: with a long list of fields to ignore, we
    // arrange them alphabetically, one distinct field per line (although
    // subfields of the same field may be grouped on the same line), and indent
    // subfields to distinguish them from top-level fields.
    $this->addUnmigratedDestinations(array(
      'body:format',
      'changed',
      'comment',
      'created',
      'field_migrate_example_image:destination_dir',
      'field_migrate_example_image:destination_file',
      'field_migrate_example_image:file_replace',
      'field_migrate_example_image:preserve_files',
      'field_migrate_example_image:urlencode',
      'is_new',
      'language',
      'log',
      'migrate_example_beer_styles:source_type',
      'migrate_example_beer_styles:create_term',
      'promote',
      'revision',
      'revision_uid',
      'status',
      'tnid',
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
 * Import items from the migrate_example_beer_comment table and make them into
 * Drupal comment objects.
 */
class BeerCommentMigration extends BasicExampleMigration {

  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = 'Comments about beers';

    $query = db_select('migrate_example_beer_comment', 'mec')
      ->fields('mec', array(
        'cid',
        'cid_parent',
        'name',
        'mail',
        'aid',
        'body',
        'bid',
        'subject',
      ))
      ->orderBy('cid_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query);
    // Note that the machine name passed for comment migrations is
    // 'comment_node_' followed by the machine name of the node type these
    // comments are attached to.
    $this->destination =
      new MigrateDestinationComment('comment_node_migrate_example_beer');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'cid' => array(
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationComment::getKeySchema()
    );

    // Mapped fields
    $this->addSimpleMappings(array('name', 'subject', 'mail'));
    $this->addFieldMapping('status')
      ->defaultValue(COMMENT_PUBLISHED);

    $this->addFieldMapping('nid', 'bid')
      ->sourceMigration('BeerNode');

    $this->addFieldMapping('uid', 'aid')
      ->sourceMigration('BeerUser')
      ->defaultValue(0);

    $this->addFieldMapping('pid', 'cid_parent')
      ->sourceMigration('BeerComment')
      ->description('Parent comment.');

    $this->addFieldMapping('comment_body', 'body');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'changed',
      'comment_body:format',
      'created',
      'homepage',
      'hostname',
      'language',
      'thread',
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
  }
}
