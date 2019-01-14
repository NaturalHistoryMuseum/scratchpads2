<?php

/**
 * @file
 * Documentation for hooks defined by Migrate.
 */

/**
 * Registers your module as an implementor of Migrate-based classes and
 * provides
 * default configuration for migration processes.
 *
 * @return
 *   An associative array with the following keys (of which only 'api' is
 *   required):
 *   - api: Always 2 for any module implementing the Migrate 2 API.
 *   - groups: An associative array, keyed by group machine name, defining one
 *     or more migration groups. Each value is an associative array - the
 *   'title' key defines a user-visible name for the group; any other values
 *   are passed as arguments to all migrations in the group.
 *   - migrations: An associative array, keyed by migration machine name,
 *     defining one or more migrations. Each value is an associative array -
 *   any
 *     keys other than the following are passed as arguments to the migration
 *     constructor:
 *     - class_name (required): The name of the class implementing the
 *   migration.
 *     - group_name: The machine name of the group containing the migration.
 *     - disable_hooks: An associative array, keyed by hook name, listing hook
 *       implementations to be disabled during migration. Each value is an
 *       array of module names whose implementations of the hook in the key is
 *       to be disabled.
 *   - destination handlers: An array of classes implementing destination
 *     handlers.
 *   - field handlers: An array of classes implementing field handlers.
 *   - wizard classes: An array of classes that provide Migrate UI wizards.
 *   - wizard extenders: An array of classes that extend Migrate UI wizards.
 *     Keys are the wizard classes, values are arrays of extender classes.
 *
 * See system_hook_info() for all hook groups defined by Drupal core.
 *
 * @see hook_migrate_api_alter().
 */
function hook_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'legacy' => array(
        'title' => t('Import from legacy system'),
        // Default format for all content migrations
        'default_format' => 'filtered_html',
      ),
    ),
    'migrations' => array(
      'ExampleUser' => array(
        'class_name' => 'ExampleUserMigration',
        'group_name' => 'legacy',
        'default_role' => 'member', // Added to constructor $arguments
      ),
      'ExampleNode' => array(
        'class_name' => 'ExampleNodeMigration',
        'group_name' => 'legacy',
        'default_uid' => 1, // Added to constructor $arguments
        'disable_hooks' => array(
          // Improve migration performance, and prevent accidental emails.
          'node_insert' => array(
            'expensive_module',
            'email_notification_module',
          ),
          'node_update' => array(
            'expensive_module',
            'email_notification_module',
          ),
        ),
      ),
    ),
  );
  return $api;
}

/**
 * Alter information from all implementations of hook_migrate_api().
 *
 * @param array $info
 *   An array of results from hook_migrate_api(), keyed by module name.
 *
 * @see hook_migrate_api().
 */
function hook_migrate_api_alter(array &$info) {
  // Override the class for another module's migration - say, to add some
  // additional preprocessing in prepareRow().
  if (isset($info['MODULE_NAME']['migrations']['ExampleNode'])) {
    $info['MODULE_NAME']['migrations']['ExampleNode']['class_name'] = 'MyBetterExampleNodeMigration';
  }
}

/**
 * Provides text to be displayed at the top of the dashboard page (migrate_ui).
 *
 * @return
 *  Translated text for display on the dashboard page.
 */
function hook_migrate_overview() {
  return t('<p>Listed below are all the migration processes defined for migration
    of our old site to Drupal. Open issues applying to specific migrations
    can be viewed by clicking the migration name. Also, details on how each
    migration will behave when incrementally migrated are provided.</p>
    <p><a href="http://issuetracker.example.com/?project=migration&status=open">Open migration tickets</a></p>');
}
