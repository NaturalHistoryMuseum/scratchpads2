<?php

/**
 * @file
 *   Admin settings and update page.
 */

/**
 * Project has a new release available.
 */
define('L10N_UPDATE_NOT_CURRENT', 4);

/**
 * Project is up to date.
 */
define('L10N_UPDATE_CURRENT', 5);

/**
 * Project's status cannot be checked.
 */
define('L10N_UPDATE_NOT_CHECKED', -1);

/**
 * No available update data was found for project.
 */
define('L10N_UPDATE_UNKNOWN', -2);

/**
 * There was a failure fetching available update data for this project.
 */
define('L10N_UPDATE_NOT_FETCHED', -3);

// Include l10n_update API
module_load_include('check.inc', 'l10n_update');
// And project api
module_load_include('project.inc', 'l10n_update');

/**
 * Page callback: Admin overview page.
 */
function l10n_update_admin_overview() {
  // For now we get package information provided by modules.
  $projects = l10n_update_get_projects();
  $languages = l10n_update_language_list('name');

  $build = array();
  if ($languages) {
    $history = l10n_update_get_history();
    $available = l10n_update_available_releases();
    $updates = l10n_update_build_updates($history, $available);
    $build['project_status'] = array(
      '#theme' => 'l10n_update_project_status',
      '#projects' => $projects,
      '#languages' => $languages,
      '#history' => $history,
      '#available' => $available,
      '#updates' => $updates,
    );
    $build['admin_import_form'] = drupal_get_form('l10n_update_admin_import_form', $projects, $updates);
  }
  else {
    $build['no_languages'] = array('#markup' => t('No translatable language defined. <a href="/admin/config/regional/language">Add a language</a>.'));
  }
  return $build;
}

/**
 * Translation update form.
 *
 * @todo selectable packages
 * @todo check language support in server
 * @todo check file update dates
 *
 * @param $form_state
 *   Form states array.
 * @param $projects
 *   @todo $projects are not used in the form.
 * @param $updates
 *   Updates to be displayed in the form.
 */
function l10n_update_admin_import_form($form, $form_state, $projects, $updates) {
  //module_load_include('inc', 'l10n_update');
  // For now we get package information provided by modules
  $projects = l10n_update_get_projects();
  $languages = l10n_update_language_list('name');

  // Absence of projects is an error and only occurs if the database table
  // was truncated. In this case we rebuild the project data.
  if (!$projects) {
    l10n_update_build_projects();
    $projects = l10n_update_get_projects();
  }

  if ($projects && $languages) {
    $form['updates'] = array(
      '#type' => 'value',
      '#value' => $updates,
    );

    if (count($languages) > 1) {
      $form['lang'] = array(
        '#type' => 'fieldset',
        '#title' => t('Languages'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE ,
        '#description' => t('Select one or more languages to download and update. If you select none, all of them will be updated.'),
      );
      $form['lang']['languages'] = array(
        '#type' => 'checkboxes',
        '#options' => $languages,
      );
    }

    if ($updates) {
      $form['actions']['download'] = array(
        '#type' => 'submit',
        '#value' => t('Update translations'),
      );
    }
  }
  $form['actions']['refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh information'),
  );
  return $form;
}

/**
 * Submit handler for Update form.
 *
 * Handles both submit buttons to update translations and to update the
 * form information.
 */
function l10n_update_admin_import_form_submit($form, $form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $projects = l10n_update_get_projects();

  if ($op == t('Update translations')) {
    $languages = isset($form_state['values']['languages']) ? array_filter($form_state['values']['languages']) : NULL;
    $updates = $form_state['values']['updates'];
    $mode = variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP);

    if ($projects && $updates) {
      module_load_include('batch.inc', 'l10n_update');
      // Filter out updates in other languages. If no languages, all of them will be updated
      $updates = _l10n_update_prepare_updates($updates, NULL, $languages);
      $batch = l10n_update_batch_multiple($updates, $mode);
      batch_set($batch);
    }
  }
  elseif ($op == t('Refresh information')) {
    // Get current version of projects.
    l10n_update_build_projects();

    // Get available translation updates and update file history.
    if ($available = l10n_update_available_releases(TRUE)) {
      l10n_update_flag_history($available);
      drupal_set_message(t('Fetched information about available updates from the server'));
    }
    else {
      drupal_set_message(t('Failed to fetch information about available updates from the server.'), 'error');
    }
  }
}

/**
 * Page callback: Settings form.
 */
function l10n_update_admin_settings_form($form, &$form_state) {
  $form['l10n_update_check_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Update source'),
    '#default_value' => variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL),
    '#options' => _l10n_update_admin_check_options(),
  );
  $form['l10n_update_import_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Update mode'),
    '#default_value' => variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP),
    '#options' => _l10n_update_admin_import_options(),
  );
  $form['l10n_update_check_frequency'] = array(
    '#type' => 'radios',
    '#title' => t('Check for updates'),
    '#default_value' => variable_get('l10n_update_check_frequency', 0),
    '#options' => array(
      0 => t('Never (manually)'),
      1 => t('Daily'),
      7 => t('Weekly'),
    ),
    '#description' => t('Select how frequently you want to automatically check for updated translations for installed modules and themes.'),
  );
  $form['l10n_update_check_disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for updates of disabled modules and themes'),
    '#default_value' => variable_get('l10n_update_check_disabled', 0),
    '#description' => t('Note that this comes with a performance penalty, so it is not recommended.'),
  );
  $form['l10n_update_download_store'] = array(
    '#title' => t('Store downloaded files'),
    '#type' => 'textfield',
    '#default_value' => variable_get('l10n_update_download_store', ''),
    '#description' => t('A path relative to the Drupal installation directory where translation files will be stored, e.g. sites/all/translations. Saved translation files can be reused by other installations. If left empty the downloaded translation will not be saved.'),
  );
  return system_settings_form($form);
}

/**
 * Additional validation handler for update settings.
 *
 * Check for existing files directory and creates one when required.
 */
function l10n_update_admin_settings_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  if (!empty($form_values['l10n_update_download_store'])) {
    if (!file_prepare_directory($form_values['l10n_update_download_store'], FILE_CREATE_DIRECTORY, 'l10n_update_download_store')) {
      form_set_error('l10n_update_download_store', t('The directory %directory does not exist or is not writable.', array('%directory' => $form_values['l10n_update_download_store'])));
      watchdog('file system', 'The directory %directory does not exist or is not writable.', array('%directory' => $form_values['l10n_update_download_store']), WATCHDOG_ERROR);
    }
  }
}

/**
 * Get array of import options.
 *
 * The import options of the Locale module are used but the UI text is altered
 * to suit the Localization update cases.
 *
 * @return
 *   Keyed array of import options.
 */
function _l10n_update_admin_import_options() {
  return array(
    LOCALE_IMPORT_OVERWRITE => t('Translation updates replace existing ones, new ones are added'),
    LOCALE_UPDATE_OVERRIDE_DEFAULT => t('Edited translations are kept, only previously imported ones are overwritten and new translations are added'),
    LOCALE_IMPORT_KEEP => t('All existing translations are kept, only new translations are added.'),
  );
}

/**
 * Get array of check options.
 *
 * @return
 *   Keyed array of source download options.
 */
function _l10n_update_admin_check_options() {
  return array(
    L10N_UPDATE_CHECK_ALL => t('Local files and remote server.'),
    L10N_UPDATE_CHECK_LOCAL => t('Local files only.'),
    L10N_UPDATE_CHECK_REMOTE => t('Remote server only.'),
  );
}

/**
 * Format project update status.
 *
 * @param $variables
 *   An associative array containing:
 *    - projects: An array containing all enabled projects.
 *    - languages: An array of all enabled languages.
 *    - history: An array of the current translations per project.
 *    - available: An array of translation sources per project.
 *    - updates: An array of available translation updates per project.
 *      Only recommended translations are listed.
 *
 * @return string
 *   HTML output.
 */
function theme_l10n_update_project_status($variables) {
  $header = $rows = array();

  // Get module and theme data for the project title.
  $projects = system_rebuild_module_data();
  $projects += system_rebuild_theme_data();

  foreach ($variables['projects'] as $name => $project) {
    if (isset($variables['history'][$name])) {
      if (isset($variables['updates'][$name])) {
        $project_status = 'updatable';
        $project_class = 'warning';
      }
      else {
        $project_status = 'uptodate';
        $project_class = 'ok';
      }
    }
    elseif (isset($variables['available'][$name])) {
      $project_status = 'available';
      $project_class = 'warning';
    }
    else {
      // Remote information not checked
      $project_status = 'unknown';
      $project_class = 'unknown';
    }

    // Get the project title and module version.
    $project->title = isset($projects[$name]->info['name']) ? $projects[$name]->info['name'] : '';
    $project->module_version = isset($projects[$name]->info['version']) ? $projects[$name]->info['version'] : $project->version;

    // Project with related language states.
    $row = theme('l10n_update_single_project_wrapper', array(
      'project' => $project,
      'project_status' => $project_status,
      'languages' => $variables['languages'],
      'available' => $variables['available'],
      'history' => $variables['history'],
      'updates' => $variables['updates'],
    ));

    $rows[$project->project_type][] = array(
      'data' => array(
        array(
          'data' => $row,
          'class' => 'l10n-update-wrapper collapsed',
        ),
      ),
      'class' => array($project_class),
    );
  }

  // Build tables of update states grouped by project type. Similar to the
  // status report by the Update module.
  $output = '';
  $project_types = array(
    'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'module-disabled' => t('Disabled modules'),
    'theme-disabled' => t('Disabled themes'),
  );
  foreach ($project_types as $type_name => $type_label) {
    if (!empty($rows[$type_name])) {
      ksort($rows[$type_name]);
      $output .= "\n<h3>" . $type_label . "</h3>\n";
      $output .= theme('table', array('header' => $header, 'rows' => $rows[$type_name], 'attributes' => array('class' => array('update l10n-update'))));
    }
  }

  // We use the core update module CSS to re-use the color definitions.
  // Plus add our own css and js.
  drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
  drupal_add_css(drupal_get_path('module', 'l10n_update') . '/css/l10n_update.admin.css');
  drupal_add_js('misc/collapse.js');
  drupal_add_js(drupal_get_path('module', 'l10n_update') . '/js/l10n_update.js');

  return $output;
}

/**
 * Format project translation state with states per language.
 *
 * @param $variables
 *   An associative array containing:
 *   - project: Project data object
 *   - project_status: Project status
 *   - languages: Available languages.
 *  @return string
 *    HTML output.
 */
function theme_l10n_update_single_project_wrapper($variables) {
  $project = $variables['project'];
  $name = $project->name;
  $project_status = $variables['project_status'];
  $languages = $variables['languages'];
  $history = $variables['history'];
  $updates = $variables['updates'];
  $availables = $variables['available'];

  // Output project title and project summary status.
  $output = theme('l10n_update_single_project_status', array(
    'project' => $project,
    'server' => l10n_update_server($project->l10n_server),
    'status' => $project_status,
  ));

  // Translation status per language is displayed in a table, one language per row.
  // For each language the current translation is listed. And optionally the
  // most recent update.
  $rows = array();
  foreach ($languages as $lang => $language) {
    // Determine current translation status and update status.
    $installed = isset($history[$name][$lang]) ? $history[$name][$lang] : NULL;
    $update = isset($updates[$name][$lang]) ? $updates[$name][$lang] : NULL;
    $available = isset($availables[$name][$lang]) ? $availables[$name][$lang] : NULL;
    if ($installed) {
      if ($update) {
        $status = 'updatable';
        $class = 'messages warning';
      }
      else {
        $status = 'uptodate';
        $class = 'ok';
      }
    }
    elseif ($available) {
      $status = 'available';
      $class = 'warning';
    }
    else {
      $status = 'unknown';
      $class = 'unknown';
    }

    // The current translation version.
    $row = theme('l10n_update_current_release', array('language' => $language, 'release' => $installed, 'status' => $status));

    // If an update is available, add it.
    if ($update) {
      $row .= theme('l10n_update_available_release', array('release' => $update));
    }

    $rows[] = array(
      'data' => array($row),
      'class' => array($class),
    );
  }

  // Output tables with translation status per language.
  $output .= '<div class="fieldset-wrapper">' . "\n";
  $output .= theme('table', array('header' => array(), 'rows' => $rows));
  $output .= "</div>\n";

  return $output;
}

/**
 * Format a single project translation state.
 *
 * @param $variables
 *   An associative array containing:
 *   - project: project data object.
 *   - server: (optional) remote server data object.
 *   - status: project summary status.
 *  @return string
 *    HTML output.
 */
function theme_l10n_update_single_project_status($variables) {
  $project = $variables['project'];
  $server = $variables['server'];
  $title = $project->title ? $project->title : $project->name;

  $output = '<div class="project">';
  $output .= '<span class="project-title">' . check_plain($title) . '</span>' . ' ' . check_plain($project->module_version) ;
  if ($server = l10n_update_server($project->l10n_server)) {
    $output .= '<span class="project-server">' . t('(translation source: !server)', array('!server' => l($server['name'], $server['link']))) . '</span>';
  }
  $output .= theme('l10n_update_version_status', array('status' => $variables['status']));
  $output .= "</div>\n";

  return $output;
}

/**
 * Format current translation version.
 *
 * @param $variables
 *   An associative array containing:
 *   - language: Language name.
 *   - release: Current file data.
 *   - status: Release status.
 * @return string
 *   HTML output.
 */
function theme_l10n_update_current_release($variables) {
  if (isset($variables['release'])) {
    $date = $variables['release']->timestamp;
    $version = $variables['release']->version;
    $text = t('@language: @version (!date)', array('@language' => $variables['language'], '@version' => $version, '!date' => format_date($date, 'custom', 'Y-M-d')));
  }
  else {
    $text = t('@language: <em>No installed translation</em>', array('@language' => $variables['language']));
  }

  $output = '<div class="language">';
  $output .= $text;
  $output .= theme('l10n_update_version_status', $variables);
  $output .= "</div>\n";

  return $output;
}

/**
 * Format current translation version.
 *
 * @param object $release
 *   Update file data.
 * @return string
 *   HTML output.
 */
function theme_l10n_update_available_release($variables) {
  $date = $variables['release']->timestamp;
  $version = $variables['release']->version;
  if (!empty($variables['release']->fileurl)) {
    // Remote file, straight link
    $link = l(t('Download'), $variables['release']->fileurl);
  }
  elseif (!empty($variables['release']->uri)) {
    // Local file, try something
    $link = l(t('Download'), $variables['release']->uri, array('absolute' => TRUE));
  }

  $output = '<div class="version version-recommended">';
  $output .=  t('Recommended version: @version (!date)', array('@version' => $version, '!date' => format_date($date, 'custom', 'Y-M-d')));
  $output .= '<span class="version-links">' . $link . '</span>';
  $output .= "</div>\n";
  return $output;
}

/**
 * Format version status with icon.
 *
 * @param string $status
 *   Version status: 'uptodate', 'updatable', 'available', 'unknown'.
 * @param string $type
 *   Update type: 'download', 'localfile'.
 *
 * @return sting
 *   HTML output.
 */
function theme_l10n_update_version_status($variables) {
  $icon = '';
  $msg = '';

  switch ($variables['status']) {
    case 'uptodate':
      $icon = theme('image', array('path' => 'misc/watchdog-ok.png', 'alt' => t('ok'), 'title' => t('ok')));
      $msg = '<span class="current">' . t('Up to date') . '</span>';
      break;
    case 'updatable':
      $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning')));
      $msg = '<span class="not-current">' . t('Update available') . '</span>';
      break;
    case 'available':
      $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning')));
      $msg = '<span class="not-current">' . t('Uninstalled translation available') . '</span>';
      break;
    case 'unknown':
      $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning')));
      $msg = '<span class="not-supported">' . t('No available translations found') . '</span>';
      break;
  }
  $output = '<div class="version-status">';
  $output .= $msg;
  $output .= '<span class="icon">' . $icon . '</span>';
  $output .= "</div>\n";
  return $output;
}
