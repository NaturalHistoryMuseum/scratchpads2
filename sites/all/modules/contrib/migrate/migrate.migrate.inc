<?php
/**
 * @file
 * Our own hook implementation.
 */


/*
 * Implements hook_migrate_api().
 */
function migrate_migrate_api() {
  $api = array(
    'api' => MIGRATE_API_VERSION,
    'destination handlers' => array(
      'MigrateCommentNodeHandler',
      'MigrateFieldsEntityHandler',
      'MigratePathEntityHandler',
      'MigratePollEntityHandler',
      'MigrateStatisticsEntityHandler',
    ),
    'field handlers' => array(
      'MigrateTextFieldHandler',
      'MigrateValueFieldHandler',
      'MigrateNodeReferenceFieldHandler',
      'MigrateUserReferenceFieldHandler',
      'MigrateTaxonomyTermReferenceFieldHandler',
      'MigrateFileFieldHandler',
      'MigrateImageFieldHandler',
    ),
  );
  return $api;
}
