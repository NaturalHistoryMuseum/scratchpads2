<?php

/**
 * @file
 *   Reusable API for creating and running l10n update batches.
 */

// module_load_include will not work in batch.
include_once 'l10n_update.check.inc';

/**
 * Create a batch to just download files.
 *
 * @param $updates
 *   Translations sources to be downloaded.
 *   Note: All update sources must have a 'fileurl'.
 * @return array
 *   A batch definition for this download.
 */
function l10n_update_batch_download($updates) {
  foreach ($updates as $update) {
    $id = $update->filename;
    $operations[] = array('_l10n_update_batch_download', array($id, $update));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a batch to just import files.
 *
 * All update sources must have a 'uri'.
 *
 * @param $updates
 *   Translations sources to be imported.
 *   Note: All update sources must have a 'fileurl'.
 * @param $import_mode
 *   Import mode. How to treat existing and modified translations.
 * @return array
 *   A batch definition for this import.
 */
function l10n_update_batch_import($updates, $import_mode) {
  foreach ($updates as $update) {
    $id = $update->filename;
    $operations[] = array('_l10n_update_batch_import', array($id, $update, $import_mode));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a big batch for multiple projects and languages.
 *
 * @param $updates
 *   Array of update sources to be run.
 * @param $mode
 *   Import mode. How to treat existing and modified translations.
 * @return array
 */
function l10n_update_batch_multiple($updates, $import_mode) {
  foreach ($updates as $update) {
    $id = $update->filename;
    if ($update->type == 'download') {
      $operations[] = array('_l10n_update_batch_download', array($id, $update));
      $operations[] = array('_l10n_update_batch_import', array($id, NULL, $import_mode));
    }
    else {
      $operations[] = array('_l10n_update_batch_import', array($id, $update, $import_mode));
    }
    // This one takes always parameters from results.
    $operations[] = array('_l10n_update_batch_history', array($id));
  }
  if (!empty($operations)) {
    return _l10n_update_create_batch($operations);
  }
}

/**
 * Create batch stub for this module.
 *
 * @param $operations
 *   Operations to perform in this batch.
 * @return array
 *   A batch definition:
 *   - 'operations': Batch operations
 *   - 'title': Batch title.
 *   - 'init_message': Initial batch UI message.
 *   - 'error_message': Batch error message.
 *   - 'file': File containing callback function.
 *   - 'finished': Batch completed callback function.
 */
function _l10n_update_create_batch($operations = array()) {
  $t = get_t();
  $batch = array(
    'operations'    => $operations,
    'title'         => $t('Updating translation.'),
    'init_message'  => $t('Downloading and importing files.'),
    'error_message' => $t('Error importing interface translations'),
    'file'          => drupal_get_path('module', 'l10n_update') . '/l10n_update.batch.inc',
    'finished'      => '_l10n_update_batch_finished',
  );
  return $batch;
}

/**
 * Batch process: Download a file.
 *
 * @param $id
 *   Batch id to identify batch results.
 *   Result of this batch function are stored in $context['result']
 *   identified by this $id.
 * @param $file
 *   File to be downloaded.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_download($id, $file, &$context) {
  $t = get_t();
  if (l10n_update_source_download($file)) {
    $context['message'] = $t('Importing: %name.', array('%name' => $file->filename));
    $context['results'][$id] = array('file' => $file);
  }
  else {
    $context['results'][$id] = array('file' => $file, 'fail' => TRUE);
  }
}

/**
 * Batch process: Update the download history table.
 *
 * @param $id
 *   Batch id to identify batch results.
 *   Result of this batch function are stored in $context['result']
 *   identified by this $id.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_history($id, &$context) {
  $t = get_t();
  // The batch import is performed in a number of steps. History update is
  // always the last step. The details of the downloaded/imported file are
  // stored in $context['results'] array.
  if (isset($context['results'][$id]['file']) && !isset($context['results'][$id]['fail'])) {
    $file = $context['results'][$id]['file'];
    l10n_update_source_history($file);
    $context['message'] = $t('Imported: %name.', array('%name' => $file->filename));
  }
}

/**
 * Batch process: Import translation file.
 *
 * This takes a file parameter or continues from previous batch
 * which should have downloaded a file.
 *
 * @param $id
 *   Batch id to identify batch results.
 *   Result of this batch function are stored in $context['result']
 *   identified by this $id.
 * @param $file
 *   File to be imported. If empty, the file will be taken from $context['results'].
 * @param $mode
 *   Import mode. How to treat existing and modified translations.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_import($id, $file, $mode, &$context) {
  $t = get_t();
  // The batch import is performed in two or three steps.
  // If import is performed after file download the file details of the download
  // are used which are stored in the $context['results'] array.
  // If a locally stored file is imported, the file details are placed in $file.
  if (empty($file) && isset($context['results'][$id]['file']) && !isset($context['results'][$id]['fail'])) {
    $file = $context['results'][$id]['file'];
  }
  if ($file) {
    // Create a result if none exists yet.
    if (!isset($context['results'][$id])) {
      $context['results'][$id] = array();
    }
    if ($import_result = l10n_update_source_import($file, $mode)) {
      $context['message'] = $t('Imported: %name.', array('%name' => $file->filename));
      $context['results'][$id] = array_merge((array)$context['results'][$id], $import_result, array('file' => $file));
    }
    else {
      $context['results'][$id] = array_merge((array)$context['results'][$id], array('fail' => TRUE), array('file' => $file));
    }
  }
}

/**
 * Batch finished callback: Set result message.
 *
 * @param $success
 *   TRUE if batch succesfully completed.
 * @param $results
 *   Batch results.
 */
function _l10n_update_batch_finished($success, $results) {
  $totals = array();    // Sum of added, updated and deleted translations.
  $total_skip = 0;      // Sum of skipped translations
  $messages = array();  // User feedback messages.
  $project_fail = $project_success = array(); // Project names of succesfull and failed imports.
  $t = get_t();

  if ($success) {
    // Summarize results of added, updated, deleted and skiped translations.
    // Added, updated and deleted are summarized per language to be displayed accordingly.
    foreach ($results as $result) {
      if (isset($result['fail'])) {
        // Collect project names of the failed imports.
        $project_fail[$result['file']->name] = $result['file']->name;
      }
      else {
        $language = $result['language'];

        // Initialize variables to prevent PHP Notices.
        if (!isset($totals[$language])) {
          $totals[$language] = array();
          $totals[$language]['add'] = $totals[$language]['update'] = $totals[$language]['delete'] = 0;
        }

        // Summarize added, updated, deleted and skiped translations.
        $totals[$language]['add'] += $result['add'];
        $totals[$language]['update'] += $result['update'];
        $totals[$language]['delete'] += $result['delete'];
        $total_skip += $result['skip'];

        // Collect project names of the succesfull imports.
        $project_success[$result['file']->name] = $result['file']->name;
      }
    }

    // Messages of succesfull translation update results.
    if ($project_success) {
      $messages[] = format_plural(count($project_success), 'One project updated: @projects.', '@count projects updated: @projects.', array('@projects' => implode(', ', $project_success)));
      $languages = language_list();
      foreach ($totals as $language => $total) {
        $messages[] = $t('%language translation strings added: !add, updated: !update, deleted: !delete.', array(
          '%language' => $languages[$language]->name,
          '!add' => $total['add'],
          '!update' => $total['update'],
          '!delete' => $total['delete'],
        ));
      }
      drupal_set_message(implode("<br />\n", $messages));

      // Warning for disallowed HTML.
      if ($total_skip) {
        drupal_set_message(
          format_plural(
            $total_skip,
            'One translation string was skipped because it contains disallowed HTML. See !log_messages for details.',
            '@count translation strings were skipped because they contain disallowed HTML. See !log_messages for details.',
            array('!log_messages' => l(t('Recent log messages'), 'admin/reports/dblog'))),
        'warning');
      }

      // Clear cache and force refresh of JavaScript translations and rebuild
      // the menu as strings may have changed.
      foreach (array_keys($totals) as $langcode) {
        _locale_invalidate_js($langcode);
      }
      cache_clear_all('locale:', 'cache', TRUE);
      menu_rebuild();
    }

    // Error for failed imports.
    if ($project_fail) {
      drupal_set_message(
        format_plural(
          count($project_fail),
          'Translations of one project were not imported: @projects.',
          'Translations of @count projects were not imported: @projects',
          array('@projects' => implode(', ', $project_fail))),
      'error');
    }
  }
  else {
    drupal_set_message($t('Error importing translations.'), 'error');
  }
}
