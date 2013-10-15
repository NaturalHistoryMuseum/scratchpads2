<?php

/**
 * @file
 * Drush interface to l10n-update functionalities.
 */

/**
 * Implements hook_drush_command().
 */
function l10n_update_drush_command() {
  $commands = array();
  $commands['l10n-update-refresh'] = array(
    'description' => 'Refresh available information.',
  );
  $commands['l10n-update-status'] = array(
    'description' => 'Show translation status of available projects.',
    'options' => array(
      'languages' => 'Comma separated list of languages. Defaults to all available languages.',
    )
  );
  $commands['l10n-update'] = array(
    'description' => 'Update translations.',
    'options' => array(
      'languages' => 'Comma separated list of languages. Defaults to all available languages.',
      'mode' => 'Determine if existing translations are overwitten during import. Use "overwrite" to overwrite any existing translation, "replace" to replace previously imported translations but not overwrite edited strings, "keep" to keep any existing translation and only add new translations. Default value: keep'
    ),
  );
  return $commands;
}

/**
 * Callback for command l10n-update-refresh.
 */
function drush_l10n_update_refresh() {
  module_load_include('admin.inc', 'l10n_update');

  // Fake $form_state to leverage _submit function.
  $form_state = array(
    'values' => array('op' => t('Refresh information'))
  );
  l10n_update_admin_import_form_submit(NULL, $form_state);
}

/**
 * Validate command l10n-update-status.
 */
function drush_l10n_update_status_validate() {
  _drush_l10n_update_validate_languages();
}

/**
 * Callback for command l10n-update-status.
 */
function drush_l10n_update_status() {
  $updates = _drush_l10n_update_get_updates();
  if (!is_null($updates)) {
    $languages = drush_get_option('languages');
    // Table header.
    $table = array();
    $header = array(dt('Project'));
    foreach ($languages as $lang => $language) {
      $header[] = $language . ' status';
    }
    $table[] = $header;
    // Iterate projects to obtain per language status.
    $projects = l10n_update_get_projects();
    $history = l10n_update_get_history();
    foreach ($projects as $name => $project) {
      $row = array();
      // Project.
      $title = isset($project->title) ? $project->title : $project->name;
      $row[] = $title . ' ' . $project->version;
      // Language status.
      foreach ($languages as $lang => $language) {
        $current = isset($history[$name][$lang]) ? $history[$name][$lang] : NULL;
        $update = isset($updates[$name][$lang]) ? $updates[$name][$lang] : NULL;
        if ($update) {
          $row[] = ($update->type == 'download') ? t('Remote update available'):t('Local update available');
        }
        elseif ($current) {
          $row[] = t('Up to date');
        }
        else {
          $row[] = t('No information');
        }
      }
      $table[] = $row;
    }
    drush_print_table($table, TRUE);
  }
  else {
    drush_log(dt('No projects or languages to update.'), 'ok');
  }
}

/**
 * Validate command l10n-update.
 */
function drush_l10n_update_validate() {
  _drush_l10n_update_validate_languages();

  // Check provided update mode is valid.
  $mode = drush_get_option('mode', 'keep');
  if (!in_array($mode, array('keep', 'replace', 'overwrite'))) {
    return drush_set_error('L10N_UPDATE_INVALID_MODE', dt('Invalid update mode. Valid options are keep, replace, overwrite.'));
  }
}

/**
 * Callback for command l10n-update.
 */
function drush_l10n_update() {
  $updates = _drush_l10n_update_get_updates();
  if (!is_null($updates)) {
    if (count($updates) > 0) {
      drush_log(dt('Found @count projects to update.', array('@count' => count($updates))), 'status');

      // Batch update all projects for selected languages.
      $mode = drush_get_option('mode', 'keep');

      switch ($mode) {
        case 'keep':
          $mode = LOCALE_IMPORT_KEEP;
          break;
        case 'replace':
          $mode = LOCALE_UPDATE_OVERRIDE_DEFAULT;
          break;
        case 'overwrite':
          $mode = LOCALE_IMPORT_OVERWRITE;
          break;

        default:
          return drush_set_error('L10N_UPDATE_INVALID_MODE', dt('Invalid update mode. Valid options are keep, overwrite.'));
          break;
      }

      $languages = drush_get_option('languages');
      module_load_include('batch.inc', 'l10n_update');
      $updates = _l10n_update_prepare_updates($updates, NULL, array_keys($languages));
      $batch = l10n_update_batch_multiple($updates, $mode);
      drush_log($batch['title'], 'status');
      drush_log($batch['init_message'], 'status');
      batch_set($batch);
      drush_backend_batch_process();
    }
    else {
      drush_log(dt('All translations up to date'), 'status');
    }
  }
}

/**
 * Helper function to validate languages.
 *
 * Used by _validate hooks.
 *  1. Check other languages than english are available.
 *  2. Check user provided languages are valid.
 */
function _drush_l10n_update_validate_languages() {
  // Check there're installed other languages than english.
  $installed_languages = l10n_update_language_list();
  if (empty($installed_languages)) {
    return drush_set_error('L10N_UPDATE_NO_LANGUAGES', dt('No languages to update.'));
  }
  // Check provided languages are valid.
  $languages = drush_get_option('languages', '');
  $languages = array_map('trim', _convert_csv_to_array($languages));
  if (count($languages)) {
    foreach ($languages as $key => $lang) {
      if (!isset($installed_languages[$lang])) {
        drush_set_error('L10N_UPDATE_INVALID_LANGUAGE', dt('Language @lang is not installed.', array('@lang' => $lang)));
      }
      else {
        unset($languages[$key]);
        $languages[$lang] = $installed_languages[$lang];
      }
    }
    if (drush_get_error() != DRUSH_SUCCESS) {
      drush_print(dt('Available languages: @languages', array('@languages' => implode(', ', array_keys($installed_languages)))));
    }
  }
  else {
    $languages = $installed_languages;
  }
  drush_set_option('languages', $languages);
}

/**
 * Helper function to obtain $updates.
 *
 * @return $updates array or NULL.
 */
function _drush_l10n_update_get_updates() {
  $projects = l10n_update_get_projects();
  if ($projects) {
    $history = l10n_update_get_history();
    drush_log(dt('Fetching update information for all projects / all languages.'), 'status');
    module_load_include('check.inc', 'l10n_update');
    $available = l10n_update_available_releases();
    $updates = l10n_update_build_updates($history, $available);
    return $updates;
  }
  else {
    drush_log(dt('No projects or languages to update.'), 'ok');
  }
}
