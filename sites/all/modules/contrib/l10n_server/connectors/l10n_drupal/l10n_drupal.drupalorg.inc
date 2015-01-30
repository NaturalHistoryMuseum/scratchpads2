<?php

/**
 * @file
 *  Drupal.org project database integration for l10n_drupal.
 */

/**
 * Amount of releases parsed in a batch.
 */
define('L10N_DRUPAL_DRUPALORG_BATCH_PARSE_AMOUNT', 50);

/**
 * The term ID for translations on Drupal.org.
 */
define('L10N_DRUPAL_DRUPALORG_TRANSLATION_TID', 29);

// == Front end functionality ==================================================

/**
 * Page callback for manual scanning.
 */
function l10n_drupal_drupalorg_scan_page() {
  return drupal_get_form('l10n_drupal_drupalorg_batch_parse_form');
}

/**
 * Form to initiate batch parsing of projects to speed up catching-up.
 *
 * @see l10n_drupal_drupalorg_batch_parse_form_submit()
 * @ingroup forms
 */
function l10n_drupal_drupalorg_batch_parse_form($form) {
  return array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Parse @count releases in a batch process', array('@count' => L10N_DRUPAL_DRUPALORG_BATCH_PARSE_AMOUNT)),
    ),
  );
}

/**
 * Form submit handler to l10n_drupal_drupalorg_batch_parse_form().
 *
 * @see l10n_drupal_drupalorg_batch_parse_form()
 */
function l10n_drupal_drupalorg_batch_parse_form_submit() {
  // First sync the project and release list. We need to do this before
  // initializing the batch, so we can have a fresh project list, and a better
  // chance to pick new releases.
  l10n_drupal_drupalorg_scan();

  // Then pick the oldest releases we did not parse yet.
  $operations = array();
  $result = db_query_range("
    SELECT r.*
    FROM {l10n_server_project} p INNER JOIN
         {l10n_server_release} r ON p.pid = r.pid
    WHERE p.connector_module = 'l10n_project_drupalorg' AND p.status = 1 AND r.download_link != '' AND r.last_parsed = 0 ORDER BY r.file_date ASC",
            0, L10N_DRUPAL_DRUPALORG_BATCH_PARSE_AMOUNT);
  foreach ($result as $release) {
    $operations[] = array('l10n_drupal_drupalorg_batch_operation', array($release));
  }

  $batch = array(
    'operations' => $operations,
    'title' => t('Processing releases'),
    'init_message' => t('First release is being selected'),
    'progress_message' => t('Parsed @current out of @total.'),
    'error_message' => t('Parsing process has encountered an error.'),
    'file' => drupal_get_path('module', 'l10n_drupal') . '/l10n_drupal.drupalorg.inc',
  );
  batch_set($batch);
}

/**
 * Batch operations function for processing a project release.
 */
function l10n_drupal_drupalorg_batch_operation($release, &$context) {
  // Parse the release we were given.
  l10n_drupal_drupalorg_parse_release($release);
}

// == Backend functionality ====================================================

/**
 * Syncronize the project list from the drupal.org database.
 */
function l10n_drupal_drupalorg_scan() {

  // settings.php has a $db_url['projects'] entry, which gives us access to
  // the projects database.
  db_set_active('projects');

  // Got the limited list of translation projects first to exclude from the
  // upcoming query. We cannot join on the taxonomy_index table there, since we
  // can have nodes with multiple terms, so just adding
  // tid != L10N_DRUPAL_DRUPALORG_TRANSLATION_TID would not help, the project
  // would still match on other terms.
  // (after applying http://drupal.org/node/224333#taxonomy_tables)
  // TODO needs work when drupal.org goes D7
  $result = db_query("SELECT DISTINCT pp.nid FROM project_projects pp
    INNER JOIN term_node tn ON tn.nid = pp.nid WHERE tn.tid = :tid",
    array(':tid' => L10N_DRUPAL_DRUPALORG_TRANSLATION_TID));
  $exclude_projects = $result->fetchCol();

  // Get all projects - published, unpublished equally, so we can update project
  // status on our copy for projects which were previously published. Only get
  // projects however, where there were any releases. We only parse releases,
  // so there is no point to have data from other projects. Also, drupal.org
  // has some pseudo-projects for the webmasters or infrastructure team to
  // maintain issues queues, which are also only identifiable by looking at
  // the list of releases. Don't use {} wrapping in query, since it is a
  // different DB and it does not use prefixes anyway.
  $query = db_select('project_projects', 'pp');
  $query->innerJoin('project_release_nodes', 'prn', 'pp.nid = prn.pid');
  $query->leftJoin('node', 'n', 'pp.nid = n.nid');
  $query
    ->fields('pp', array('nid', 'uri'))
    ->fields('n', array('title', 'status'))
    ->distinct()
    ->condition('pp.nid', $exclude_projects, 'NOT IN')
    ->condition('prn.rebuild', 0);
  $result = $query->execute();
  // Fill in an array, so we can go back to our database and add in these
  // details.
  $projects = $result->fetchAllAssoc('uri');

  // Switch back to our local database.
  db_set_active();

  // Record all projects in our local database.
  $local_projects = l10n_server_get_projects(array('all' => TRUE));
  foreach ($projects as $project) {
    $result = db_query("SELECT * FROM {l10n_server_project} WHERE uri = :uri", array(':uri' => $project->uri));
    if ($existing_project = $result->fetchObject()) {
      if ($existing_project->connector_module == 'l10n_project_drupalorg') {
        $projects[$project->uri]->pid = $existing_project->pid;
        if (($project->title != $existing_project->title) ||
          ($project->status != $existing_project->status)) {
          // Update if title or status is different (uri should not change,
          // other data is internal to our database).
          db_update('l10n_server_project')
            ->fields(array(
                'title' => $project->title,
                'status' => $project->status,
                'last_parsed' => REQUEST_TIME,
              ))
            ->condition('uri', $project->uri)
            ->condition('connector_module', 'l10n_project_drupalorg')
            ->execute();
        }
      }
      else {
        // Log error on existing project with another connector and skip the
        // rest of this function.
        $t_args = array(
          '%uri' => $project->uri,
          '%other_connector' => $existing_project->connector_module,
          '%this_connector' => 'l10n_project_drupalorg',
        );
        watchdog('l10n_drupalorg', 'An existing project under the URI %uri is already handled by the %other_connector module. Not possible to add it with %this_connector.', $t_args);
        drupal_set_message(t('An existing project under the URI %uri is already handled by the %other_connector module. Not possible to add it with %this_connector.', $t_args), 'error');
        continue;
      }
    }
    elseif (isset($project->title)) {
      // New project, not recorded before.
      $last_pid = db_insert('l10n_server_project')
        ->fields(array(
            'uri' => $project->uri,
            'title' => $project->title,
            'last_parsed' => REQUEST_TIME,
            'home_link' => 'http://drupal.org/project/' . $project->uri,
            'connector_module' => 'l10n_project_drupalorg',
            'status' => $project->status,
          ))
        ->execute();
      // Save last insert ID for release update code.
      $project->pid = $last_pid;
      $projects[$project->uri] = $project;
    }
    unset($local_projects[$project->uri]);
  }
  if (count($local_projects)) {
    // If we still have local projects lingering, those are not anymore
    // available with non-dev releases on drupal.org, so we should turn off
    // their listing in our database.
    $disabled_projects = array_keys($local_projects);
    db_update('l10n_server_project')
      ->fields(array('status' => 0))
      ->condition('uri', $disabled_projects, 'IN')
      ->execute();
  }

  $last_sync = variable_get('l10n_server_connector_l10n_project_drupalorg_last_sync', 0);
  $last_saved_timestamp = variable_get('l10n_drupal_drupalorg_last_saved_timestamp', 0);

  // Go back to projects database for releases.
  db_set_active('projects');

  // Only sync releases which are at most one day older then our last
  // sync date. This ensures time zone issues and releases published while the
  // previous cron run will not be a problem, but we only look at a relatively
  // small list of releases at any given time. We only sync tagged releases,
  // which will not get rebuilt later anytime.
  $result = db_query("
    SELECT pp.uri, prn.version, f.filepath, f.timestamp, prf.filehash, nn.status
    FROM project_projects pp INNER JOIN
         project_release_nodes prn ON pp.nid = prn.pid LEFT JOIN
         node n ON pp.nid = n.nid INNER JOIN
         project_release_file prf ON prn.nid = prf.nid INNER JOIN
         files f ON prf.fid = f.fid INNER JOIN
         node nn ON prn.nid = nn.nid
    WHERE prn.rebuild = 0 AND f.timestamp > :timestamp",
            array('timestamp' => $last_sync - 60 * 60 * 24));

  // Fill in an array, so we can go back to our database and add in these
  // details.
  $releases = array();
  foreach ($result as $release) {
    // Drupal.org builds both .zip and .tar.gz files but we should only
    // include tar.gz files here. Not done in the SQL query to avoid
    // affecting performance of that.
    if (strpos($release->filepath, '.tar.gz') !== FALSE) {
      $releases[] = $release;
    }
  }

  // Save last reporting timestamp for getting latest usage data.
  $project_usage = array();
  $last_usage_timestamp = db_query('SELECT timestamp FROM project_usage_week_release ORDER BY timestamp DESC LIMIT 1')->fetchField();
  if ($last_saved_timestamp < $last_usage_timestamp) {
    $result = db_query("
      SELECT pp.uri, prn.version, puwr.count
      FROM project_projects pp INNER JOIN
           project_release_nodes prn ON pp.nid = prn.pid LEFT JOIN
           project_usage_week_release puwr ON puwr.nid = prn.nid
      WHERE prn.rebuild = 0 AND puwr.timestamp = :timestamp",
              array('timestamp' => $last_usage_timestamp));
    foreach ($result as $release) {
      $project_usage[$release->uri][$release->version] = $release->count;
    }
  }

  // Switch back to our local database.
  db_set_active();

  // Record all releases in our local database.
  foreach ($releases as $release) {
    if (!isset($projects[$release->uri]->pid)) {
      // Not a project the upper project list knows about (eg. a translation).
      continue;
    }

    $download_link = 'http://ftp.drupal.org/' . $release->filepath;
    $result = db_query(
      "SELECT * FROM {l10n_server_release} WHERE download_link = :download_link",
      array(':download_link' => $download_link)
    );
    if ($existing_release = $result->fetchObject()) {
      // @todo: what happens to unpublished releases? drop data outright?
    }
    elseif (!empty($release->status)) {
      // New published release, not recorded before.
      db_insert('l10n_server_release')
        ->fields(array(
            'pid' => $projects[$release->uri]->pid,
            'title' => $release->version,
            'download_link' => $download_link,
            'file_date' => $release->timestamp,
            'file_hash' => $release->filehash,
            'last_parsed' => 0,
          ))
        ->execute();
    }
  }

  // Project usage data will only be present if we have updates.
  foreach ($project_usage as $uri => $project_data) {
    if (!isset($projects[$uri]->pid)) {
      // Not a project the upper project list knows about (eg. a translation).
      continue;
    }

    // We save project usage as negative weight because the lower the weight,
    // the earlier an item will be in the list (Drupal standard). But usage
    // stats are better if higher, so we need to reverse the effect.

    // Add up and save all project usage as negative weight.
    $all_project_usage = array_sum($project_data);
    db_update('l10n_server_project')
      ->fields(array('weight' => - $all_project_usage))
      ->condition('uri', $uri)
      ->execute();

    // Save individual release count data as negative weight.
    foreach ($project_data as $title => $count) {
      db_update('l10n_server_release')
        ->fields(array('weight' => - $count))
        ->condition('pid', $projects[$uri]->pid)
        ->condition('title', $title)
        ->execute();
    }
  }

  // Set last sync time to limit number of releases to look at next time.
  variable_set('l10n_server_connector_l10n_project_drupalorg_last_sync', REQUEST_TIME);
  // Remember last usage timestamp we handled.
  variable_set('l10n_drupal_drupalorg_last_saved_timestamp', $last_usage_timestamp);
}

/**
 * Download and parse the picked release.
 */
function l10n_drupal_drupalorg_parse_release($release) {
  // This could take a long time.
  @set_time_limit(0);

  $error = '';
  $filename = basename($release->download_link);
  $file = file_directory_temp() . '/' . $filename;

  // Check filename for a limited set of allowed chars. Help make sure
  // that all the command line code later is not exploited.
  if (!preg_match('!^([a-zA-Z0-9_.-])+$!', $filename)) {
    $error_arguments = array('%file' => $file);
    $error_t = t('Filename %file contains malicious characters.', $error_arguments);
    $error = 'Filename %file contains malicious characters.';
  }

  // Already downloaded. Probably result of faulty file download left around,
  // so remove file, letting the code come back to this release later.
  elseif (file_exists($file)) {
    $error_arguments = array('%file' => $file);
    $error_t = t('File %file already exists, deleting.', $error_arguments);
    $error = 'File %file already exists, deleting.';
    unlink($file);
  }

  // Download the tar.gz file from drupal.org and save it in the local filesystem.
  elseif (!(($contents = drupal_http_request($release->download_link)) && ($contents->code == 200) && file_put_contents($file, $contents->data))) {
    $error_arguments = array(
      '%download_link' => $release->download_link,
      '%error' => $contents->code . ' ' . $contents->error,
    );
    $error_t = t('Unable to download and save %download_link file (%error).', $error_arguments);
    $error = 'Unable to download and save %download_link file (%error).';
  }

  // Extract and parse the local file and remove the tarball.
  else {
    $return = l10n_drupal_drupalorg_parse_package($file, $release);
    // Clear stats cache, so new data shows up.
    cache_clear_all('l10n:stats', 'cache');
  }

  if ($error) {
    watchdog('l10n_drupal', $error, $error_arguments, WATCHDOG_ERROR);
    $return['error'] = $error_t;
  }
  return $return;
}

/**
 * Parses contents of a specific local package file.
 *
 * @param $package_file
 *   Path to the package file to be extracted and parsed.
 * @param $release
 *   Release object.
 */
function l10n_drupal_drupalorg_parse_package($package_file, $release) {
  $error = $message = '';

  // Potx module is already a dependency.
  module_load_include('inc', 'potx');
  module_load_include('inc', 'l10n_drupal', 'l10n_drupal.files');

  // Set up status messages if not in automated mode.
  potx_status('set', POTX_STATUS_MESSAGE);

  // Generate temp folder to extract the tarball. tempnam() creates a regular
  // file, thus reserving the name. But we need a directory. Because $temp_name
  // is unique, we can append '_dir', and it will still be unique.
  $temp_name = tempnam(file_directory_temp(), 'l10n_drupal_drupalorg_');
  $temp_path = $temp_name . '_dir';

  // Nothing to do if the file is not there or the extraction folder is taken.
  if (!file_exists($package_file)) {
    $error_arguments = array('%file' => $package_file);
    $error_t = t('Package to parse (%file) does not exist.', $error_arguments);
    $error = 'Package to parse (%file) does not exist.';
  }
  elseif (is_dir($temp_path)) {
    $error_arguments = array('%path' => $temp_path);
    $error_t = t('Temporary directory %path already exists.', $error_arguments);
    $error = 'Temporary directory %path already exists.';
  }

  // Extract the local file to the temporary directory.
  else {
    if (l10n_drupal_drupalorg_exec('mkdir ' . escapeshellarg($temp_path))) {
      // Try extracting the tarball to this directory.
      if (l10n_drupal_drupalorg_exec('tar -xvvzf ' . escapeshellarg($package_file) . ' -C ' . escapeshellarg($temp_path))) {

        // Get all source files and save strings with our callback for this release.
        $files = _potx_explore_dir($temp_path);
        l10n_drupal_save_file(array($release->pid, $release->rid));
        $version = l10n_drupal_detect_major_version($package_file);
        l10n_drupal_added_string_counter(NULL, TRUE);
        foreach ($files as $name) {
          _potx_process_file($name, strlen($temp_path) + 1, 'l10n_drupal_save_string', 'l10n_drupal_save_file', $version);
        }
        $sid_count = l10n_drupal_added_string_counter();

        // Delete directory now that parsing is done.
        l10n_drupal_drupalorg_exec('rm -rf ' . escapeshellarg($temp_path));
        unlink($package_file);

        // Record changes of the scanned project in the database.
        $message_arguments = array(
          '@filename' => basename($package_file),
          '@files' => count($files),
          '@sids' => $sid_count,
        );
        $message_t = t('@filename (@files files, @sids strings) scanned.', $message_arguments);
        $message = '@filename (@files files, @sids strings) scanned.';

        // Parsed this releases files.
        db_update('l10n_server_release')
	  ->fields(array(
              'sid_count' => $sid_count,
              'last_parsed' => REQUEST_TIME,
            ))
	  ->condition('rid', $release->rid)
	  ->execute();

        // Update error list for this release. Although the errors are related to
        // files, we are not interested in the fine details, the file names are in
        // the error messages as text. We assume no other messages are added while
        // importing, so we can safely use drupal_get_message() to grab our errors.
        db_delete('l10n_server_error') ->condition('rid', $release->rid) ->execute();
        $messages = drupal_get_messages('error');
        if (isset($messages['error']) && is_array($messages['error'])) {
          foreach ($messages['error'] as $error_message) {
            db_insert('l10n_server_error')
	      ->fields(array(
                  'rid' => $release->rid,
                  'value' => $error_message,
                ))
	      ->execute();
          }
        }
      }
      else {
        // Delete directory if we did not manage to extract the tarball.
        l10n_drupal_drupalorg_exec('rm -rf ' . escapeshellarg($temp_path));
      }
    }
  }

  // Remove temporary file we used to reserve the name for the directory.
  unlink($temp_name);

  $return = array();
  if ($error) {
    watchdog('l10n_drupal', $error, $error_arguments, WATCHDOG_ERROR);
    $return['error'] = $error_t;
  }
  if ($message) {
    watchdog('l10n_drupal', $message, $message_arguments);
    $return['message'] = $message_t;
  }

  return $return;
}

/**
 * Wrapper for exec() that logs errors to the watchdog.
 *
 * Taken from project/release/package-release-nodes.php.
 *
 * @param $cmd
 *   String of the command to execute (assumed to be safe, the caller is
 *   responsible for calling escapeshellcmd()).
 *
 * @return
 *   TRUE if the command was successful (0 exit status), else FALSE.
 */
function l10n_drupal_drupalorg_exec($cmd) {
  // Make sure we grab stderr, too.
  $output = array();
  exec("$cmd 2>&1", $output, $rval);
  if ($rval) {
    $output = htmlspecialchars(is_array($output) ? implode("\n", $output) : $output);
    watchdog('l10n_drupal', '%cmd failed with status !rval<pre>' . $output . '</pre>', array('%cmd' => $cmd, '!rval' => $rval), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}
