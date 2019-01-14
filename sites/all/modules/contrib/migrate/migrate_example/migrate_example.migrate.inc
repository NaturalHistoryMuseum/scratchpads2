<?php

/**
 * @file
 * Because the name of this file is the module name plus '.migrate.inc', when
 * hook_migrate_api is invoked by the Migrate module this file is automatically
 * loaded - thus, you don't need to implement your hook in the .module file.
 */

/*
 * You must implement hook_migrate_api(), setting the API level to 2, if you are
 * implementing any migration classes. If your migration application is static -
 * that is, you know at implementation time exactly what migrations must be
 * instantiated - then you should register your migrations here. If your
 * application is more dynamic (for example, if selections in the UI determine
 * exactly what migrations need to be instantiated), then you would register
 * your migrations using registerMigration() - see migrate_example_baseball for
 * more information.
 */
function migrate_example_migrate_api() {
  // Usually field mappings are established by code in the migration constructor -
  // a call to addFieldMapping(). They may also be passed as arguments when
  // registering a migration - in this case, they are stored in the database
  // and override any mappings for the same field in the code. To do this,
  // construct the field mapping object and configure it similarly to when
  // you call addFieldMapping, and pass your mappings as an array below.
  $translate_mapping = new MigrateFieldMapping('translate', NULL);
  $translate_mapping->defaultValue(0);
  $ignore_mapping = new MigrateFieldMapping('migrate_example_beer_styles:ignore_case', NULL);
  $ignore_mapping->defaultValue(1);

  $api = array(
    // Required - tells the Migrate module that you are implementing version 2
    // of the Migrate API.
    'api' => 2,
    // Migrations can be organized into groups. The key used here will be the
    // machine name of the group, which can be used in Drush:
    //  drush migrate-import --group=wine
    // The title is a required argument which is displayed for the group in the
    // UI. You may also have additional arguments for any other data which is
    // common to all migrations in the group.
    'groups' => array(
      'beer' => array(
        'title' => t('Beer Imports'),
      ),
      'wine' => array(
        'title' => t('Wine Imports'),
      ),
    ),

    // Here we register the individual migrations. The keys (BeerTerm, BeerUser,
    // etc.) are the machine names of the migrations, and the class_name
    // argument is required. The group_name is optional (defaulting to 'default')
    // but specifying it is a best practice.
    'migrations' => array(
      'BeerTerm' => array(
        'class_name' => 'BeerTermMigration',
        'group_name' => 'beer',
      ),
      'BeerUser' => array(
        'class_name' => 'BeerUserMigration',
        'group_name' => 'beer',
      ),
      'BeerNode' => array(
        'class_name' => 'BeerNodeMigration',
        'group_name' => 'beer',
        // You may optionally declare dependencies for your migration - other
        // migrations which should run first. In this case, terms assigned to our
        // nodes and the authors of the nodes should be migrated before the nodes
        // themselves.
        'dependencies' => array(
          'BeerTerm',
          'BeerUser',
        ),
        // Here is where we add field mappings which may override those
        // specified in the group constructor.
        'field_mappings' => array(
          $translate_mapping,
          $ignore_mapping,
        ),
      ),
      'BeerComment' => array(
        'class_name' => 'BeerCommentMigration',
        'group_name' => 'beer',
        'dependencies' => array(
          'BeerUser',
          'BeerNode',
        ),
      ),
      'WinePrep' => array(
        'class_name' => 'WinePrepMigration',
        'group_name' => 'wine',
      ),
      'WineVariety' => array(
        'class_name' => 'WineVarietyMigration',
        'group_name' => 'wine',
      ),
      'WineRegion' => array(
        'class_name' => 'WineRegionMigration',
        'group_name' => 'wine',
      ),
      'WineBestWith' => array(
        'class_name' => 'WineBestWithMigration',
        'group_name' => 'wine',
      ),
      'WineFileCopy' => array(
        'class_name' => 'WineFileCopyMigration',
        'group_name' => 'wine',
        'dependencies' => array('WinePrep'),
      ),
      'WineFileBlob' => array(
        'class_name' => 'WineFileBlobMigration',
        'group_name' => 'wine',
        'dependencies' => array('WinePrep'),
      ),
      'WineRole' => array(
        'class_name' => 'WineRoleMigration',
        'group_name' => 'wine',
        // TIP: Regular dependencies, besides enforcing (in the absence of
        // --force) the run order of migrations, affect the sorting of
        // migrations on display. You can use soft dependencies to affect just
        // the display order when the migrations aren't technically required to
        // run in a certain order. In this case, we want the role migration to
        // appear after the file migrations.
        'soft_dependencies' => array('WineFileCopy'),
      ),
      'WineUser' => array(
        'class_name' => 'WineUserMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineFileCopy',
          'WineRole',
        ),
      ),
      'WineProducer' => array(
        'class_name' => 'WineProducerMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerXML' => array(
        'class_name' => 'WineProducerXMLMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerNamespaceXML' => array(
        'class_name' => 'WineProducerNamespaceXMLMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerMultiXML' => array(
        'class_name' => 'WineProducerMultiXMLMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerMultiNamespaceXML' => array(
        'class_name' => 'WineProducerMultiNamespaceXMLMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerXMLPull' => array(
        'class_name' => 'WineProducerXMLPullMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineProducerNamespaceXMLPull' => array(
        'class_name' => 'WineProducerNamespaceXMLPullMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineUser',
        ),
      ),
      'WineWine' => array(
        'class_name' => 'WineWineMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineRegion',
          'WineVariety',
          'WineBestWith',
          'WineUser',
          'WineProducer',
        ),
      ),
      'WineComment' => array(
        'class_name' => 'WineCommentMigration',
        'group_name' => 'wine',
        'dependencies' => array(
          'WineUser',
          'WineWine',
        ),
      ),
      'WineTable' => array(
        'class_name' => 'WineTableMigration',
        'group_name' => 'wine',
        'soft_dependencies' => array('WineComment'),
      ),
      'WineFinish' => array(
        'class_name' => 'WineFinishMigration',
        'group_name' => 'wine',
        'dependencies' => array('WineComment'),
      ),
      'WineUpdates' => array(
        'class_name' => 'WineUpdatesMigration',
        'group_name' => 'wine',
        'dependencies' => array('WineWine'),
        'soft_dependencies' => array('WineFinish'),
      ),
      'WineCommentUpdates' => array(
        'class_name' => 'WineCommentUpdatesMigration',
        'group_name' => 'wine',
        'dependencies' => array('WineComment'),
        'soft_dependencies' => array('WineUpdates'),
      ),
      'WineVarietyUpdates' => array(
        'class_name' => 'WineVarietyUpdatesMigration',
        'group_name' => 'wine',
        'dependencies' => array('WineVariety'),
        'soft_dependencies' => array('WineUpdates'),
      ),
      'WineUserUpdates' => array(
        'class_name' => 'WineUserUpdatesMigration',
        'group_name' => 'wine',
        'dependencies' => array('WineUser'),
        'soft_dependencies' => array('WineUpdates'),
      ),
    ),
  );
  return $api;
}
