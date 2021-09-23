<?php

/**
 * @file
 * Library for querying Drupal projects
 *
 * Most code is taken from update module. We don't want to depend on it though as it may not be enabled.
 *
 * For each project, the information about where to fetch translations may be specified in the info files
 * as follows:
 *
 * - Localization server to be used for this project. Defaults to http://localize.drupal.org
 *   l10n server = localize.drupal.org
 *   (This should be enough if the server url, the one below, is defined somewhere else)
 *
 * - Metadata information for the localization server
 *   l10n url = http://ftp.drupal.org/files/translations/l10n_server.xml
 *   (We can fetch *all* the information we need from this single url)
 *
 * - Translation file URL template, will be used to build the file url to download
 *   l10n path = http://ftp.drupal.org/files/translations/%core/%project/%project-%release.%language.po
 *   (Alternatively you can use the %filename variable that will default to '%project-%release.%language.po')
 */

/**
 * Rebuild project list
 *
 * @param $refresh
 *   TRUE: Refresh project list.
 *
 * @return array
 *   Array of project objects to be considered for translation update.
 */
function l10n_update_build_projects($refresh = FALSE) {
  module_load_include('inc', 'l10n_update');
  // Get all stored projects, including disabled ones
  $current = l10n_update_get_projects($refresh, TRUE);
  // Now get the new project list, just enabled ones
  $projects = l10n_update_project_list();

  // Mark all previous projects as disabled and store new project data
  db_update('l10n_update_project')
    ->fields(array(
      'status' => 0,
    ))
    ->execute();

  $default_server = l10n_update_default_server();

  if (module_exists('update')) {
    $projects_info = update_get_available(TRUE);
  }
  foreach ($projects as $name => $data) {

    // Force update fetch of project data in cases where Drupal's performance
    // optimized approach is missing out on some projects.
    // @see http://drupal.org/node/1671570#comment-6216090
    if (module_exists('update') && !isset($projects_info[$name])) {
      module_load_include('fetch.inc', 'update');
      _update_process_fetch_task($data);
      $available = _update_get_cached_available_releases();
      if (!empty($available[$name])) {
        $projects_info[$name] = $available[$name];
      }
    }

    if (isset($projects_info[$name]['releases']) && $projects_info[$name]['project_status'] != 'not-fetched') {
      // Find out if a dev version is installed.
      if (preg_match("/^[0-9]+\.x-([0-9]+)\..*-dev$/", $data['info']['version'], $matches)) {
        // Find a suitable release to use as alternative translation.
        foreach ($projects_info[$name]['releases'] as $project_release) {
          // The first release with the same major release number which is not
          // a dev release is the one. Releases are sorted the most recent first.
          if ($project_release['version_major'] == $matches[1] &&
              (!isset($project_release['version_extra']) || $project_release['version_extra'] != 'dev')) {
            $release = $project_release;
            break;
          }
        }
      }
      if (!empty($release['version'])) {
        $data['info']['version'] = $release['version'];
      }

      unset($release);
    }

    $data += array(
      'version' => isset($data['info']['version']) ? $data['info']['version'] : '',
      'core' => isset($data['info']['core']) ? $data['info']['core'] : DRUPAL_CORE_COMPATIBILITY,
      // The project can have its own l10n server, we use default if not
      'l10n_server' => isset($data['info']['l10n server']) ? $data['info']['l10n server'] : NULL,
      // A project can provide the server url to fetch metadata, or the update url (path)
      'l10n_url' => isset($data['info']['l10n url']) ? $data['info']['l10n url'] : NULL,
      'l10n_path' => isset($data['info']['l10n path']) ? $data['info']['l10n path'] : NULL,
      'status' => 1,
    );
    $project = (object) $data;
    // Unless the project provides a full l10n path (update url), we try to build one
    if (!isset($project->l10n_path)) {
      $server = NULL;
      if ($project->l10n_server || $project->l10n_url) {
        $server = l10n_update_server($project->l10n_server, $project->l10n_url);
      }
      else {
        // Use the default server
        $server = l10n_update_server($default_server['name'], $default_server['server_url']);
      }
      if ($server) {
        // Build the update path for this project, with project name and release replaced
        $project->l10n_path = l10n_update_build_string($project, $server['update_url']);
      }
    }
    // Create / update project record
    $update = empty($current[$name]) ? array() : array('name');
    // @todo Use db_merge() to avoid problems with saving existing projects.
    try {
      drupal_write_record('l10n_update_project', $project, $update);
    }
    catch (Exception $e) {
      watchdog('l10n_update', 'Could not insert a project into the l10n_update_project table, possibly because it already exists.', NULL, WATCHDOG_INFO);
    }
    $projects[$name] = $project;
  }
  return $projects;
}

/**
 * Get update module's project list
 *
 * @return array
 */
function l10n_update_project_list() {
  $projects = array();
  $disabled = variable_get('l10n_update_check_disabled', 0);
  // Unlike update module, this one has no cache
  _l10n_update_project_info_list($projects, system_rebuild_module_data(), 'module', $disabled);
  _l10n_update_project_info_list($projects, system_rebuild_theme_data(), 'theme', $disabled);
  // Allow other modules to alter projects before fetching and comparing.
  drupal_alter('l10n_update_projects', $projects);
  return $projects;
}

/**
 * Refresh projects after enabling modules
 *
 * When new projects are installed, set a batch for locale import / update
 *
 * @param $modules
 *   Array of module names.
 */
function l10n_update_project_refresh($modules) {
  module_load_include('check.inc', 'l10n_update');
  $projects = array();

  // Get all current projects, including the recently installed.
  $current_projects = l10n_update_build_projects(TRUE);
  // Collect project data of newly installed projects.
  foreach ($modules as $name) {
    if (isset($current_projects[$name])) {
      $projects[$name] = $current_projects[$name];
    }
  }

  // If a translation is available and if update is required, lets go.
  if ($projects && $available = l10n_update_check_projects($projects)) {
    $history = l10n_update_get_history();
    if ($updates = l10n_update_build_updates($history, $available)) {
      module_load_include('batch.inc', 'l10n_update');
      // Filter out updates in other languages. If no languages, all of them will be updated
      $updates = _l10n_update_prepare_updates($updates);
      $batch = l10n_update_batch_multiple($updates, variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP));
      batch_set($batch);
    }
  }
}

/**
 * Populate an array of project data.
 *
 * Based on _update_process_info_list()
 *
 * @param $projects
 * @param $list
 * @param $project_type
 * @param $disabled
 *   TRUE to include disabled projects too
 */
function _l10n_update_project_info_list(&$projects, $list, $project_type, $disabled = FALSE) {
  foreach ($list as $file) {
    if (!$disabled && empty($file->status)) {
      // Skip disabled modules or themes.
      continue;
    }

    // Skip if the .info file is broken.
    if (empty($file->info)) {
      continue;
    }

    // If the .info doesn't define the 'project', try to figure it out.
    if (!isset($file->info['project'])) {
      $file->info['project'] = l10n_update_get_project_name($file);
    }

    // If we still don't know the 'project', give up.
    if (empty($file->info['project'])) {
      continue;
    }

    // If we don't already know it, grab the change time on the .info file
    // itself. Note: we need to use the ctime, not the mtime (modification
    // time) since many (all?) tar implementations will go out of their way to
    // set the mtime on the files it creates to the timestamps recorded in the
    // tarball. We want to see the last time the file was changed on disk,
    // which is left alone by tar and correctly set to the time the .info file
    // was unpacked.
    if (!isset($file->info['_info_file_ctime'])) {
      $info_filename = dirname($file->uri) . '/' . $file->name . '.info';
      $file->info['_info_file_ctime'] = filectime($info_filename);
    }

    $project_name = $file->info['project'];
    if (!isset($projects[$project_name])) {
      // Only process this if we haven't done this project, since a single
      // project can have multiple modules or themes.
      $projects[$project_name] = array(
        'name' => $project_name,
        'info' => $file->info,
        'datestamp' => isset($file->info['datestamp']) ? $file->info['datestamp'] : 0,
        'includes' => array($file->name => $file->info['name']),
        'project_type' => $project_name == 'drupal' ? 'core' : $project_type,
      );
    }
    else {
      $projects[$project_name]['includes'][$file->name] = $file->info['name'];
      $projects[$project_name]['info']['_info_file_ctime'] = max($projects[$project_name]['info']['_info_file_ctime'], $file->info['_info_file_ctime']);
    }
  }
}

/**
 * Given a $file object (as returned by system_rebuild_module_data()), figure
 * out what project it belongs to.
 *
 * Based on update_get_project_name().
 *
 * @param $file
 * @return string
 * @see system_get_files_database()
 */
function l10n_update_get_project_name($file) {
  $project_name = '';
  if (isset($file->info['project'])) {
    $project_name = $file->info['project'];
  }
  elseif (isset($file->info['package']) && (strpos($file->info['package'], 'Core') === 0)) {
    $project_name = 'drupal';
  }
  return $project_name;
}
