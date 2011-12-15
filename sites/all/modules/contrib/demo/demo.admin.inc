<?php
// $Id: demo.admin.inc,v 1.35 2011/01/09 03:11:12 sun Exp $

/**
 * @file
 * Demonstration Site administrative pages
 */

/**
 * Current version of SQL dump structure.
 */
define('DEMO_DUMP_VERSION', '1.1');

/**
 * Form builder for Demo module settings.
 */
function demo_admin_settings($form, &$form_state) {
  if (!file_stream_wrapper_valid_scheme('private')) {
    form_set_error('', t('The <a href="@file-settings-url">private filesystem</a> must be configured in order to create or load snapshots.', array(
      '@file-settings-url' => url('admin/config/media/file-system', array(
        'query' => drupal_get_destination(),
      )),
    )));
  }

  $form['demo_dump_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Snapshot file system path'),
    '#field_prefix' => 'private://',
    '#default_value' => variable_get('demo_dump_path', 'demo'),
    '#required' => TRUE,
  );
  $form['#validate'][] = 'demo_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for demo_admin_settings().
 */
function demo_admin_settings_validate($form, &$form_state) {
  if (!file_prepare_directory($form_state['values']['demo_dump_path'], FILE_CREATE_DIRECTORY)) {
    form_set_error('demo_dump_path', t('The snapshot directory %directory could not be created.', array('%directory' => $form_state['values']['demo_dump_path'])));
  }
}

/**
 * Form builder to manage snapshots.
 */
function demo_manage_form($form, &$form_state) {
  $form['status'] = array(
    '#type' => 'container',
    '#title' => t('Status'),
    '#attributes' => array(
      'class' => array('demo-status', 'clearfix'),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'demo') . '/demo.admin.css'),
    ),
  );
  $reset_date = variable_get('demo_reset_last', 0);
  $form['status']['reset_last'] = array(
    '#type' => 'item',
    '#title' => t('Last reset'),
    '#markup' => $reset_date ? format_date($reset_date) : t('Never'),
  );

  $form['dump'] = demo_get_dumps();

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('demo_manage_delete_submit'),
  );

  // If there are no snapshots yet, hide the selection and form actions.
  if (empty($form['dump']['#options'])) {
    $form['dump']['#access'] = FALSE;
    $form['actions']['#access'] = FALSE;
  }

  return $form;
}

/**
 * Delete button submit handler for demo_manage_form().
 */
function demo_manage_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/demo/delete/' . $form_state['values']['filename'];
}

/**
 * Form builder to confirm deletion of a snapshot.
 */
function demo_delete_confirm($form, &$form_state, $filename) {
  $fileconfig = demo_get_fileconfig($filename);
  if (!file_exists($fileconfig['infofile'])) {
    return drupal_access_denied();
  }

  $form['filename'] = array(
    '#type' => 'value',
    '#value' => $filename,
  );
  return confirm_form($form, t('Are you sure you want to delete the snapshot %title?', array('%title' => $filename)), 'admin/structure/demo', t('This action cannot be undone.'), t('Delete'));
}

/**
 * Form submit handler for demo_delete_confirm().
 */
function demo_delete_confirm_submit($form, &$form_state) {
  $files = demo_get_fileconfig($form_state['values']['filename']);
  unlink($files['sqlfile']);
  unlink($files['infofile']);
  drupal_set_message(t('Snapshot %title has been deleted.', array(
    '%title' => $form_state['values']['filename'],
  )));
  $form_state['redirect'] = 'admin/structure/demo';
}

/**
 * Form builder to create a new snapshot.
 */
function demo_dump_form($form, &$form_state) {
  $form['#tree'] = TRUE;

  $form['dump']['filename'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'demo/autocomplete',
    '#required' => TRUE,
    '#maxlength' => 128,
    '#description' => t('Allowed characters: a-z, 0-9, dashes ("-"), underscores ("_") and dots.'),
  );
  $form['dump']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#rows' => 2,
    '#description' => t('Leave empty to retain the existing description when replacing a snapshot.'),
  );
  $form['dump']['tables'] = array(
    '#type' => 'value',
    '#value' => demo_enum_tables(),
  );

  if (empty($form_state['demo']['dump_exists'])) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
    );
  }
  else {
    $form = confirm_form($form,
      t('Are you sure you want to replace the existing %name snapshot?', array(
        '%name' => $form_state['values']['dump']['filename'],
      )),
      'admin/structure/demo',
      t('A snapshot with the same name already exists and will be replaced. This action cannot be undone.')
    );
  }
  return $form;
}

/**
 * Form validation handler for demo_dump_form().
 */
function demo_dump_form_validate(&$form, &$form_state) {
  if (empty($form_state['values']['confirm'])) {
    $fileconfig = demo_get_fileconfig($form_state['values']['dump']['filename']);
    if (file_exists($fileconfig['infofile']) || file_exists($fileconfig['sqlfile'])) {
      $form_state['demo']['dump_exists'] = TRUE;
      $form_state['rebuild'] = TRUE;
    }
  }
}

/**
 * Form submit handler for demo_dump_form().
 */
function demo_dump_form_submit($form, &$form_state) {
  if ($fileconfig = _demo_dump($form_state['values']['dump'])) {
    drupal_set_message(t('Snapshot %filename has been created.', array(
      '%filename' => $form_state['values']['dump']['filename'],
    )));
  }
  $form_state['redirect'] = 'admin/structure/demo';
}

/**
 * Create a new snapshot.
 *
 * @param $options
 *   A structured array of snapshot options:
 *   - filename: The base output filename, without extension.
 *   - default: Whether to set this dump as new default snapshot.
 *   - description: A description for the snapshot. If a snapshot with the same
 *     name already exists and this is left blank, the new snapshot will reuse
 *     the existing description.
 *   - tables: An array of tables to dump, keyed by table name (including table
 *     prefix, if any). The value is an array of dump options:
 *     - schema: Whether to dump the table schema.
 *     - data: Whether to dump the table data.
 */
function _demo_dump($options) {
  // Load database specific functions.
  if (!demo_load_include()) {
    return FALSE;
  }

  // Increase PHP's max_execution_time for large dumps.
  drupal_set_time_limit(600);

  // Generate the info file.
  $info = demo_set_info($options);
  if (!$info) {
    return FALSE;
  }

  // Allow other modules to alter the dump options.
  $fileconfig = demo_get_fileconfig($info['filename']);
  drupal_alter('demo_dump', $options, $info, $fileconfig);

  // Perform database dump.
  if (!demo_dump_db($fileconfig['sqlfile'], $options)) {
    return FALSE;
  }

  // Adjust file permissions.
  drupal_chmod($fileconfig['infofile']);
  drupal_chmod($fileconfig['sqlfile']);

  // Allow other modules to act on successful dumps.
  module_invoke_all('demo_dump', $options, $info, $fileconfig);

  return $fileconfig;
}

/**
 * Form builder to reset site to a snapshot.
 */
function demo_reset_confirm($form, &$form_state) {
  $form['dump'] = demo_get_dumps();

  $form['warning'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('messages', 'warning'),
    ),
  );
  $form['warning']['message'] = array(
    '#markup' => t('This action cannot be undone.'),
  );

  return confirm_form($form,
    t('Are you sure you want to reset the site?'),
    'admin/structure/demo',
    t('Overwrites all changes that made to this site since the chosen snapshot.'),
    t('Reset')
  );
}

/**
 * Form submit handler for demo_reset_confirm().
 */
function demo_reset_confirm_submit($form, &$form_state) {
  // Reset site to chosen snapshot.
  _demo_reset($form_state['values']['filename']);

  // Do not redirect from the reset confirmation form by default, as it is
  // likely that the user wants to reset all over again (e.g., keeping the
  // browser tab open).
}

/**
 * Reset site using snapshot.
 *
 * @param $filename
 *   Base snapshot filename, without extension.
 * @param $verbose
 *   Whether to output status messages.
 */
function _demo_reset($filename, $verbose = TRUE) {
  // Load database specific functions.
  if (!demo_load_include()) {
    return FALSE;
  }

  // Increase PHP's max_execution_time for large dumps.
  drupal_set_time_limit(600);

  $fileconfig = demo_get_fileconfig($filename);
  if (!file_exists($fileconfig['sqlfile']) || !($fp = fopen($fileconfig['sqlfile'], 'r'))) {
    if ($verbose) {
      drupal_set_message(t('Unable to read file %filename.', array(
        '%filename' => $fileconfig['sqlfile'],
      )), 'error');
    }
    watchdog('demo', 'Unable to read file %filename.', array('%filename' => $fileconfig['sqlfile']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Load any database information in front of reset.
  $info = demo_get_info($fileconfig['infofile']);
  module_invoke_all('demo_reset_before', $filename, $info, $fileconfig);

  // Retain special variables, so the (demonstration) site keeps operating after
  // the reset. Specify NULL instead of default values, so unconfigured
  // variables are not retained, resp., deleted after the reset.
  $variables = array(
    // Without the snapshot path, subsequent resets will not work.
    'demo_dump_path' => variable_get('demo_dump_path', NULL),
  );

  // Temporarily disable foreign key checks for the time of import and before
  // dropping existing tables. Foreign key checks should already be re-enabled
  // as one of the last operations in the SQL dump file.
  // @see demo_dump_db()
  db_query("SET FOREIGN_KEY_CHECKS = 0;");

  // Drop tables.
  $is_version_1_0_dump = version_compare($info['version'], '1.1', '<');
  $watchdog = Database::getConnection()->prefixTables('{watchdog}');
  foreach (demo_enum_tables() as $table => $dump_options) {
    // Skip watchdog, except for legacy dumps that included the watchdog table
    if ($table != $watchdog || $is_version_1_0_dump) {
      db_query("DROP TABLE $table");
    }
  }

  // Load data from snapshot.
  $success = TRUE;
  $query = '';
  while (!feof($fp)) {
    $line = fgets($fp, 16384);
    if ($line && $line != "\n" && strncmp($line, '--', 2) && strncmp($line, '#', 1)) {
      $query .= $line;
      if (substr($line, -2) == ";\n") {
        $options = array(
          'target' => 'default',
          'return' => Database::RETURN_NULL,
          // 'throw_exception' => FALSE,
        );
        $stmt = Database::getConnection($options['target'])->prepare($query);
        if (!$stmt->execute(array(), $options)) {
          if ($verbose) {
            // Don't use t() here, as the locale_* tables might not (yet) exist.
            drupal_set_message(strtr('Query failed: %query', array('%query' => $query)), 'error');
          }
          $success = FALSE;
        }
        $query = '';
      }
    }
  }
  fclose($fp);

  // Retain variables.
  foreach ($variables as $key => $value) {
    if (isset($value)) {
      variable_set($key, $value);
    }
    else {
      variable_del($key);
    }
  }

  if ($success) {
    if ($verbose) {
      drupal_set_message(t('Restored site from %filename.', array('%filename' => $fileconfig['sqlfile'])));
    }
    watchdog('demo', 'Restored site from %filename.', array('%filename' => $fileconfig['sqlfile']), WATCHDOG_NOTICE);

    // Allow other modules to act on successful resets.
    module_invoke_all('demo_reset', $filename, $info, $fileconfig);
  }
  else {
    if ($verbose) {
      drupal_set_message(t('Failed to restore site from %filename.', array('%filename' => $fileconfig['sqlfile'])), 'error');
    }
    watchdog('demo', 'Failed to restore site from %filename.', array('%filename' => $fileconfig['sqlfile']), WATCHDOG_ERROR);
  }

  // Save request time of last reset, but not during re-installation via
  // demo_profile.
  if (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE !== 'install') {
    variable_set('demo_reset_last', REQUEST_TIME);
  }

  return $success;
}

function demo_get_fileconfig($filename = 'demo_site') {
  $fileconfig = array();

  // Build dump path.
  if (!file_stream_wrapper_valid_scheme('private')) {
    // @todo Temporarily throwing a form error here.
    // Don't break demo_profile.
    if (!defined('MAINTENANCE_MODE')) {
      form_set_error('', t('The <a href="@file-settings-url">private filesystem</a> must be configured in order to create or load snapshots.', array(
        '@file-settings-url' => url('admin/config/media/file-system', array(
          'query' => drupal_get_destination(),
        )),
      )));
    }
    return FALSE;
  }
  $fileconfig['path'] = 'private://' . variable_get('demo_dump_path', 'demo');
  $fileconfig['dumppath'] = $fileconfig['path'];
  // @todo Update to D7?
  // Append site name if it is not included in file_directory_path() and if not
  // storing files in sites/all/files.
  $fileconfig['site'] = str_replace('sites', '', conf_path());
  /*
  if (strpos($fileconfig['path'], conf_path()) === FALSE && strpos($fileconfig['path'], '/all/') === FALSE) {
    $fileconfig['dumppath'] .= $fileconfig['site'];
  }
  */

  // Check if directory exists.
  if (!file_prepare_directory($fileconfig['dumppath'], FILE_CREATE_DIRECTORY)) {
    return FALSE;
  }

  // Protect dump files.
  file_create_htaccess($fileconfig['path'], TRUE);

  // Build SQL filename.
  $fileconfig['sql'] = $filename . '.sql';
  $fileconfig['sqlfile'] = $fileconfig['dumppath'] . '/' . $fileconfig['sql'];

  // Build info filename.
  $fileconfig['info'] = $filename . '.info';
  $fileconfig['infofile'] = $fileconfig['dumppath'] . '/' . $fileconfig['info'];

  return $fileconfig;
}

/**
 * Load database specific functions.
 */
function demo_load_include() {
  $engine = db_driver();
  if (!module_load_include('inc', 'demo', 'database_' . $engine . '_dump')) {
    drupal_set_message(t('@database is not supported yet.', array('@database' => ucfirst($engine))), 'error');
    return FALSE;
  }
  return TRUE;
}

function demo_get_dumps() {
  $fileconfig = demo_get_fileconfig();

  // Fetch list of available info files
  $files = file_scan_directory($fileconfig['dumppath'], '/\.info$/');

  foreach ($files as $file => $object) {
    $files[$file]->filemtime = filemtime($file);
    $files[$file]->filesize = filesize(substr($file, 0, -4) . 'sql');
  }

  // Sort snapshots by date (ascending file modification time).
  uasort($files, create_function('$a, $b', 'return ($a->filemtime < $b->filemtime);'));

  $element = array(
    '#type' => 'radios',
    '#title' => t('Snapshot'),
    '#required' => TRUE,
    '#parents' => array('filename'),
    '#options' => array(),
    '#attributes' => array(
      'class' => array('demo-snapshots-widget'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'demo') . '/demo.admin.js'),
    ),
  );

  foreach ($files as $filename => $file) {
    $info = demo_get_info($filename);

    // Prepare snapshot title.
    $title = t('@snapshot <small>(!date, !size)</small>', array(
      '@snapshot' => $info['filename'],
      '!date' => format_date($file->filemtime, 'small'),
      '!size' => format_size($file->filesize),
    ));

    // Prepare snapshot description.
    $description = '';
    if (!empty($info['description'])) {
      $description .= '<p>' . $info['description'] . '</p>';
    }
    // Add download links.
    $description .= '<p>' . t('Download: <a href="@info-file-url">.info file</a>, <a href="@sql-file-url">.sql file</a>', array(
      '@info-file-url' => url('demo/download/' . $file->name . '/info'),
      '@sql-file-url' => url('demo/download/' . $file->name . '/sql'),
    )) . '</p>';
    // Add module list.
    if (count($info['modules']) > 1) {
      // Remove required core modules and Demo from module list.
      $modules = array_diff($info['modules'], array('filter', 'node', 'system', 'user', 'demo'));
      // Sort module list alphabetically.
      sort($modules);
      $description .= t('Modules: @modules', array('@modules' => implode(', ', $modules)));
    }

    // Add the radio option element.
    $element['#options'][$info['filename']] = $title;
    $element[$info['filename']] = array(
      '#description' => $description,
      '#file' => $file,
      '#info' => $info,
    );
  }

  return $element;
}

function demo_get_info($filename, $field = NULL) {
  $info = array();

  if (file_exists($filename)) {
    $info = parse_ini_file($filename);

    if (isset($info['modules'])) {
      $info['modules'] = explode(" ", $info['modules']);
    }
    else {
      $info['modules'] = NULL;
    }

    if (!isset($info['version'])) {
      $info['version'] = '1.0';
    }
  }

  if (isset($field)) {
    return isset($info[$field]) ? $info[$field] : NULL;
  }
  else {
    return $info;
  }
}

function demo_set_info($values = NULL) {
  if (isset($values['filename']) && is_array($values)) {
    // Check for valid filename
    if (!preg_match('/^[-_\.a-zA-Z0-9]+$/', $values['filename'])) {
      drupal_set_message(t('Invalid filename. It must only contain alphanumeric characters, dots, dashes and underscores. Other characters, including spaces, are not allowed.'), 'error');
      return FALSE;
    }

    if (!empty($values['description'])) {
      // parse_ini_file() doesn't allow certain characters in description
      $s = array("\r\n", "\r", "\n", '"');
      $r = array(' ', ' ', ' ', "'");
      $values['description'] = str_replace($s, $r, $values['description']);
    }
    else {
      // If new description is empty, try to use previous description.
      $old_file = demo_get_fileconfig($values['filename']);
      $old_description = demo_get_info($old_file['infofile'], 'description');
      if (!empty($old_description)) {
        $values['description'] = $old_description;
      }
    }

    // Set values
    $infos = array();
    $infos['filename'] = $values['filename'];
    $infos['description'] = '"' . $values['description'] . '"';
    $infos['modules'] = implode(' ', module_list());
    $infos['version'] = DEMO_DUMP_VERSION;

    // Write information to .info file
    $fileconfig = demo_get_fileconfig($values['filename']);
    $infofile = fopen($fileconfig['infofile'], 'w');
    foreach ($infos as $key => $info) {
      fwrite($infofile, $key . ' = ' . $info . "\n");
    }
    fclose($infofile);

    return $infos;
  }
}

/**
 * Returns a list of tables in the active database.
 *
 * Only returns tables whose prefix matches the configured one (or ones, if
 * there are multiple).
 */
function demo_enum_tables() {
  $tables = array();

  // Load database specific functions.
  if (!demo_load_include()) {
    return FALSE;
  }

  $connection = Database::getConnection();
  $db_options = $connection->getConnectionOptions();

  // Create a regex that matches the table prefix(es).
  // We are only interested in non-empty table prefixes.
  $prefixes = array();
  if (!empty($db_options['prefix'])) {
    if (is_array($db_options['prefix'])) {
      $prefixes = array_filter($db_options['prefix']);
    }
    elseif ($db_options['prefix'] != '') {
      $prefixes['default'] = $db_options['prefix'];
    }
    $rx = '/^' . implode('|', $prefixes) . '/';
  }

  // Query the database engine for the table list.
  $result = _demo_enum_tables();
  foreach ($result as $table) {
    if (!empty($prefixes)) {
      // Check if table name matches a configured prefix.
      if (preg_match($rx, $table, $matches)) {
        $table_prefix = $matches[0];
        $plain_table = substr($table, strlen($table_prefix));
        if ($prefixes[$plain_table] == $table_prefix || $prefixes['default'] == $table_prefix) {
          $tables[$table] = array('schema' => TRUE, 'data' => TRUE);
        }
      }
    }
    else {
      $tables[$table] = array('schema' => TRUE, 'data' => TRUE);
    }
  }

  // Apply default exclude list.
  $excludes = array(
    // Drupal core.
    '{cache}',
    '{cache_bootstrap}',
    '{cache_block}',
    '{cache_content}',
    '{cache_field}',
    '{cache_filter}',
    '{cache_form}',
    '{cache_menu}',
    '{cache_page}',
    '{cache_path}',
    '{cache_update}',
    '{watchdog}',
    // CTools.
    '{ctools_object_cache}',
    // Administration menu.
    '{cache_admin_menu}',
    // Panels.
    '{panels_object_cache}',
    // Views.
    '{cache_views}',
    '{cache_views_data}',
    '{views_object_cache}',
  );
  foreach (array_map(array($connection, 'prefixTables'), $excludes) as $table) {
    if (isset($tables[$table])) {
      $tables[$table]['data'] = FALSE;
    }
  }

  return $tables;
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for existing snapshots.
 */
function demo_autocomplete($string = '') {
  $matches = array();
  if ($string && $fileconfig = demo_get_fileconfig()) {
    $string = preg_quote($string);
    $files = file_scan_directory($fileconfig['dumppath'], '/' . $string . '.*\.info$/');
    foreach ($files as $file) {
      $matches[$file->name] = check_plain($file->name);
    }
  }
  drupal_json_output($matches);
}

/**
 * Transfer (download) a snapshot file.
 *
 * @param $filename
 *   The snapshot filename to transfer.
 * @param $type
 *   The file type, i.e. extension to transfer.
 *
 * @todo Allow to download an bundled archive of snapshot files.
 */
function demo_download($filename, $type) {
  $fileconfig = demo_get_fileconfig($filename);
  if (!isset($fileconfig[$type . 'file']) || !file_exists($fileconfig[$type . 'file'])) {
    return MENU_NOT_FOUND;
  }
  // Force the client to re-download and trigger a file save download.
  $headers = array(
    'Cache-Control: private',
    'Content-Type: application/octet-stream',
    'Content-Length: ' . filesize($fileconfig[$type . 'file']),
    'Content-Disposition: attachment, filename=' . $fileconfig[$type],
  );
  file_transfer($fileconfig[$type . 'file'], $headers);
}

