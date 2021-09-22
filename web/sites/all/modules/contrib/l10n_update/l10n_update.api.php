<?php

/**
 * @file
 *   API documentation for Localize updater module.
 */

/**
 * Returns available translation servers and server definitions.
 *
 * @return keyed array of available servers.
 *   Example: array('localize.drupal.org' => array(
 *       'name' => 'localize.drupal.org',
 *       'server_url' => 'http://ftp.drupal.org/files/translations/l10n_server.xml',
 *       'update_url' => 'http://ftp.drupal.org/files/translations/%core/%project/%project-%release.%language.po',
 *     ),
 *   );
 */
function hook_l10n_servers() {
  // This hook is used to specify the default localization server(s).
  // Additionally server data can be specified on a per project basis in the
  // .info file or using the hook_l10n_update_projects_alter().

  module_load_include('inc', 'l10n_update');
  $server = l10n_update_default_server();
  return array($server['name'] => $server );
}

/**
 * Alter the list of project to be updated by l10n update.
 *
 * l10n_update uses the same list of projects as update module. Using this hook
 * the list can be altered.
 *
 * @param array $projects
 *   Array of projects.
 */
function hook_l10n_update_projects_alter(&$projects) {
  // The $projects array contains the project data produced by
  // update_get_projects(). A number of the array elements are described in
  // the documentation of hook_update_projects_alter().

  // In the .info file of a project a localization server can be specified.
  // Using this hook the localization server specification can be altered or
  // added. The 'l10n path' element is optional but can be specified to override
  // the translation download path specified in the 10n_server.xml file.
  $projects['existing_example_project'] = array(
    'info' => array(
      'l10n server' => 'example.com',
      'l10n url' => 'http://example.com/files/translations/l10n_server.xml',
      'l10n path' => 'http://example.com/files/translations/%core/%project/%project-%release.%language.po',
    ),
  );

  // With this hook it is also possible to add a new project wich does not
  // exist as a real module or theme project but is treated by the localization
  // update module as one. The below data is the minumum to be specified.
  // As in the previous example the 'l10n path' element is optional.
  $projects['new_example_project'] = array(
    'project_type'  => 'module',
    'name' => 'new_example_project',
    'info' => array(
      'version' => '6.x-1.5',
      'core' => '6.x',
      'l10n server' => 'example.com',
      'l10n url' => 'http://example.com/files/translations/l10n_server.xml',
      'l10n path' => 'http://example.com/files/translations/%core/%project/%project-%release.%language.po',
    ),
  );
}
