<?php

/**
 * @file
 * Reusable API for l10n remote updates using $source objects
 *
 * These functions may not be safe for the installer as they use variables and report using watchdog
 */

/**
 * Threshold for timestamp comparison.
 *
 * Eliminates a difference between the download time
 * (Database: l10n_update_file.timestamp) and the actual .po file timestamp.
 */
define('L10N_UPDATE_TIMESTAMP_THRESHOLD', 2);

module_load_include('inc', 'l10n_update');

/**
 * Fetch update information for all projects / all languages.
 *
 * @param boolean $refresh
 *   TRUE = refresh the release data.
 *   We refresh anyway if the data is older than a day.
 *
 * @return array
 *   Available releases indexed by project and language.
 */
function l10n_update_available_releases($refresh = FALSE) {
  $frequency = variable_get('l10n_update_check_frequency', 0) * 24 * 3600;
  if (!$refresh && ($cache = cache_get('l10n_update_available_releases', 'cache_l10n_update')) && (!$frequency || $cache->created > REQUEST_TIME - $frequency)) {
    return $cache->data;
  }
  else {
    $projects = l10n_update_get_projects(TRUE);
    $languages = l10n_update_language_list();
    $available = l10n_update_check_projects($projects, array_keys($languages));
    cache_set('l10n_update_available_releases', $available, 'cache_l10n_update', $frequency ? REQUEST_TIME + $frequency : CACHE_PERMANENT);
    return $available;
  }
}

/**
 * Check latest release for project, language.
 *
 * @param $projects
 *   Projects to check (objects).
 * @param $languages
 *   Array of language codes to check, none to check all.
 * @param $check_local
 *   Check local translation file.
 * @param $check_remote
 *   Check remote translation file.
 *
 * @return array
 *   Available sources indexed by project, language.
 */
function l10n_update_check_projects($projects, $languages = NULL, $check_local = NULL, $check_remote = NULL) {
  if (!isset($check_local)) {
    $check_local = (bool) (variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL) & L10N_UPDATE_CHECK_LOCAL);
  }
  if (!isset($check_remote)) {
    $check_remote = (bool) (variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL) & L10N_UPDATE_CHECK_REMOTE);
  }

  $languages = $languages ? $languages : array_keys(l10n_update_language_list());
  $result = array();
  foreach ($projects as $name => $project) {
    foreach ($languages as $lang) {
      $source = l10n_update_source_build($project, $lang);
      if ($update = l10n_update_source_check($source, $check_local, $check_remote)) {
        $result[$name][$lang] = $update;
      }
    }
  }
  return $result;
}

/**
 * Compare available releases with history and get list of downloadable updates.
 *
 * @param $history
 *   Update history of projects.
 * @param $available
 *   Available project releases.
 * @return array
 *   Projects to be updated: 'not yet downloaded', 'newer timestamp available',
 *   'new version available'.
 *   Up to date projects are not included in the array.
 */
function l10n_update_build_updates($history, $available) {
  $updates = array();
  foreach ($available as $name => $project_updates) {
    foreach ($project_updates as $lang => $update) {
      if (!empty($update->timestamp)) {
        $current = !empty($history[$name][$lang]) ? $history[$name][$lang] : NULL;
        // Add when not current, timestamp newer or version difers (newer version)
        if (_l10n_update_source_compare($current, $update) == -1 || $current->version != $update->version) {
          $updates[$name][$lang] = $update;
        }
      }
    }
  }
  return $updates;
}

/**
 * Check updates for active projects and languages.
 *
 * @param $count
 *   Number of package translations to check.
 * @param $before
 *   Unix timestamp, check only updates that haven't been checked for this time.
 * @param $limit
 *   Maximum number of updates to do. We check $count translations
 *   but we stop after we do $limit updates.
 * @return array
 */
function l10n_update_check_translations($count, $before, $limit = 1) {
  $projects = l10n_update_get_projects();
  $updated = $checked = array();

  // Select active projects x languages ordered by last checked time
  $q = db_select('l10n_update_project', 'p');
  $q->leftJoin('l10n_update_file', 'f', 'p.name = f.project');
  $q->innerJoin('languages', 'l', 'l.language = f.language');
  $q->condition('p.status', 1);
  $q->condition('l.enabled', 1);
  // If the file is not there, or it is there, but we did not check since $before.
  $q->condition(db_or()->isNull('f.status')->condition(db_and()->condition('f.status', 1)->condition('f.last_checked', $before, '<')));
  $q->range(0, $count);
  $q->fields('p', array('name'));
  $q->fields('f');
  $q->addField('l', 'language', 'lang');
  $q->orderBy('last_checked');
  $result = $q->execute();

  if ($result) {
    $local = (bool) (variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL) & L10N_UPDATE_CHECK_LOCAL);
    $remote = (bool) (variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL) & L10N_UPDATE_CHECK_REMOTE);
    foreach ($result as $check) {
      if (count($updated) >= $limit) {
        break;
      }
      $checked[] = $check;
      if (!empty($projects[$check->name])) {
        $project = $projects[$check->name];
        $update = NULL;
        $source = l10n_update_source_build($project, $check->lang);
        $current = $check->filename ? $check : NULL;
        if ($available = l10n_update_source_check($source, $local, $remote)) {
          if (!$current || _l10n_update_source_compare($current, $available) == -1 || $current->version != $available->version) {
            $update = $available;
          }
        }
        if ($update) {
          // The update functions will update data and timestamps too
          l10n_update_source_update($update, variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP));
          $updated[] = $update;
        }
        elseif ($current) {
          // No update available, just update timestamp for this row
          db_update('l10n_update_file')
            ->fields(array(
              'last_checked' => REQUEST_TIME,
            ))
            ->condition('project', $current->project)
            ->condition('language', $current->language)
            ->execute();
        }
        elseif ($source) {
          // Create a new record just for keeping last checked time
          $source->last_checked = REQUEST_TIME;
          drupal_write_record('l10n_update_file', $source);
        }
      }
    }
  }
  return array($checked, $updated);
}

/**
 * Build abstract translation source, to be mapped to a file or a download.
 *
 * @param $project
 *   Project object.
 * @param $langcode
 *   Language code.
 * @param $filename
 *   File name of translation file. May contains placeholders.
 * @return object
 *   Source object, which may have these properties:
 *   - 'project': Project name.
 *   - 'language': Language code.
 *   - 'type': Source type 'download' or 'localfile'.
 *   - 'uri': Local file path.
 *   - 'fileurl': Remote file URL for downloads.
 *   - 'filename': File name.
 *   - 'keep': TRUE to keep the downloaded file.
 *   - 'timestamp': Last update time of the file.
 */
function l10n_update_source_build($project, $langcode, $filename = L10N_UPDATE_DEFAULT_FILENAME) {
  $source = clone $project;
  $source->project = $project->name;
  $source->language = $langcode;
  $source->filename = l10n_update_build_string($source, $filename);
  return $source;
}

/**
 * Check local and remote sources for the file.
 *
 * @param $source
 *   Translation source object.
 *   @see l10n_update_source_build()
 * @param $check_local
 *   File object of local translation file.
 * @param $check_remote
 *   File object of remote translation file.
 * @return object
 *   File object of most recent translation; local or remote.
 */
function l10n_update_source_check($source, $check_local = TRUE, $check_remote = TRUE) {
  $local = $remote = NULL;
  if ($check_local) {
    $check = clone $source;
    if (l10n_update_source_check_file($check)) {
      $local = $check;
    }
  }
  if ($check_remote) {
    $check = clone $source;
    if (l10n_update_source_check_download($check)) {
      $remote = $check;
    }
  }
  // Get remote if newer than local only, they both can be empty
  return _l10n_update_source_compare($local, $remote) < 0 ? $remote : $local;
}

/**
 * Check remote file object.
 *
 * @param $source
 *   Remote translation file object. The object will be update
 *   with data of the remote file:
 *   - 'type': Fixed value 'download'.
 *   - 'fileurl': File name and path.
 *   - 'timestamp': Last updated time.
 *   @see l10n_update_source_build()
 * @return object
 *   An object containing the HTTP request headers, response code, headers,
 *   data, redirect status and updated timestamp.
 *   NULL if failure.
 */
function l10n_update_source_check_download($source) {
  $url = l10n_update_build_string($source, $source->l10n_path);
  $result = l10n_update_http_check($url);
  if ($result && !empty($result->updated)) {
    $source->type = 'download';
    // There may have been redirects so we store the resulting url
    $source->fileurl = isset($result->redirect_url) ? $result->redirect_url : $url;
    $source->timestamp = $result->updated;
    return $result;
  }
}

/**
 * Check whether we've got the file in the filesystem under 'translations'.
 *
 * It will search, similar to modules and themes:
 * - translations
 * - sites/all/translations
 * - sites/mysite/translations
 *
 * Using name as the key will return just the last one found.
 *
 * @param $source
 *   Translation file object. The object will be updated with data of local file.
 *   - 'type': Fixed value 'localfile'.
 *   - 'uri': File name and path.
 *   - 'timestamp': Last updated time.
 *   @see l10n_update_source_build()
 * @param $directory
 *   Files directory.
 * @return Object
 *  File object (filename, basename, name)
 *  NULL if failure.
 */
function l10n_update_source_check_file($source, $directory = 'translations') {
  $filename = '/' . preg_quote($source->filename) . '$/';

  // Using the 'name' key will return
  if ($files = drupal_system_listing($filename, $directory, 'name', 0)) {
    $file = current($files);
    $source->type = 'localfile';
    $source->uri = $file->uri;
    $source->timestamp = filemtime($file->uri);
    return $file;
  }
}

/**
 * Download and import or just import source, depending on type.
 *
 * @param $source
 *   Translation source object with information about the file location.
 *   Object will be updated with :
 *   - 'last_checked': Timestamp of current time;
 *   - 'import_date': Timestamp of current time;
 * @param $mode
 *   Download mode. How to treat exising and modified translations.
 * @return boolean
 *   TRUE on success, NULL on failure.
 */
function l10n_update_source_update($source, $mode) {
  if ($source->type == 'localfile' || l10n_update_source_download($source)) {
    if (l10n_update_source_import($source, $mode)) {
      l10n_update_source_history($source);
      return TRUE;
    }
  }
}

/**
 * Import source into locales table.
 *
 * @param $source
 *   Translation source object with information about the file location.
 *   Object will be updated with :
 *   - 'last_checked': Timestamp of current time;
 *   - 'import_date': Timestamp of current time;
 * @param $mode
 *   Download mode. How to treat exising and modified translations.
 * @return boolean
 *   Result array on success, FALSE on failure.
 */
function l10n_update_source_import($source, $mode) {
  if (!empty($source->uri) && $result = l10n_update_import_file($source->uri, $source->language, $mode)) {
    $source->last_checked = REQUEST_TIME;

    // We override the file timestamp here. The default file time stamp is the
    // release date from the l.d.o server. We change the timestamp to the
    // creation time on the webserver. On multi sites that share a common
    // sites/all/translations directory, the sharing sites use the local file
    // creation date as release date. Without this correction the local
    // file is always newer than the l.d.o. file, which results in unnecessary
    // translation import.
    $source->timestamp = time();

    return $result;
  }
}

/**
 * Download source file from remote server.
 *
 * If succesful this function returns the downloaded file in two ways:
 * - As a temporary $file object
 * - As a file path on the $source->uri property.
 *
 * @param $source
 *   Source object with all parameters
 *   - 'fileurl': url to download.
 *   - 'uri': alternate destination. If not present a temporary file
 *                 will be used and the path returned here.
 * @return object
 *   $file object if download successful.
 */
function l10n_update_source_download($source) {
  if (!empty($source->uri)) {
    $destination = $source->uri;
  }
  elseif ($directory = variable_get('l10n_update_download_store', '')) {
    $destination = $directory . '/' . $source->filename;
  }
  else {
    $destination = NULL;
  }
  if ($file = l10n_update_download_file($source->fileurl, $destination)) {
    $source->uri = $file;
    return $file;
  }
}

/**
 * Update the file history table and delete the file if temporary.
 *
 * @param $file
 *   Source object representing the file just imported or downloaded.
 */
function l10n_update_source_history($file) {
  // Update history table
  l10n_update_file_history($file);

  // If it's a downloaded file and not marked for keeping, delete the file.
  if ($file->type == 'download' && empty($file->keep)) {
    file_unmanaged_delete($file->uri);
    $file->uri = '';
  }
}

/**
 * Compare two update sources, looking for the newer one (bigger timestamp).
 *
 * This function can be used as a callback to compare two source objects.
 *
 * @param $current
 *   Source object of current project.
 * @param $update
 *   Source object of available update.
 * @return integer
 *   - '-1': $current < $update OR $current is missing
 *   - '0': $current == $update OR both $current and $updated are missing
 *   - '1': $current > $update OR $update is missing
 */
function _l10n_update_source_compare($current, $update) {
  if ($current && $update) {
    if (abs($current->timestamp - $update->timestamp) < L10N_UPDATE_TIMESTAMP_THRESHOLD) {
      return 0;
    }
    else {
      return $current->timestamp > $update->timestamp ? 1 : -1;
    }
  }
  elseif ($current && !$update) {
    return 1;
  }
  elseif (!$current && $update) {
    return -1;
  }
  else {
    return 0;
  }
}

/**
 * Prepare update list.
 *
 * @param $updates
 *   Array of update sources that may be indexed in multiple ways.
 * @param $projects
 *   Array of project names to be included, others will be filtered out.
 * @param $languages
 *   Array of language codes to be included, others will be filtered out.
 * @return array
 *   Plain array of filtered updates with directory applied.
 */
function _l10n_update_prepare_updates($updates, $projects = NULL, $languages = NULL) {
  $result = array();
  foreach ($updates as $key => $update) {
    if (is_array($update)) {
      // It is a sub array of updates yet, process and merge
      $result = array_merge($result, _l10n_update_prepare_updates($update, $projects, $languages));
    }
    elseif ((!$projects || in_array($update->project, $projects)) && (!$languages || in_array($update->language, $languages))) {
      $directory = variable_get('l10n_update_download_store', '');
      if ($directory && empty($update->uri)) {
        // If we have a destination folder set just if we have no uri
        if (empty($update->uri)) {
          $update->uri = $directory . '/' . $update->filename;
          $update->keep = TRUE;
        }
      }
      $result[] = $update;
    }
  }
  return $result;
}

/**
 * Language refresh. Runs a batch for loading the selected languages.
 *
 * To be used after adding a new language.
 *
 * @param $languages
 *   Array of language codes to check and download.
 */
function l10n_update_language_refresh($languages) {
  $projects = l10n_update_get_projects();
  if ($available = l10n_update_check_projects($projects, $languages)) {
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
