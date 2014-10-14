<?php

/**
 * @file
 * Validate export options and manage export process.
 */

/**
 * Invoke associated include file.
 */
$module_dir = drupal_get_path('module', 'taxonomy_csv');
require_once($module_dir . '/taxonomy_csv.api.inc');
require_once($module_dir . '/taxonomy_csv.term.api.inc');
require_once($module_dir . '/taxonomy_csv.vocabulary.api.inc');

/**
 * Process the export of a vocabulary.
 *
 * If not used in a form, don't forget to use batch_process().
 *
 * @param $options
 *   An associative array of options:
 *   - export_format : see format of the csv line (see taxonomy.api.inc)
 *   - vocabulary_id : vid or machine_name (or array of them) of the vocabulary
 *                     to export (default: 0, which means all)
 *   - delimiter     : one character csv delimiter (default: ',')
 *   - enclosure     : zero or one character csv enclosure (default: '"')
 *   - line_ending   : 'Unix' (default), 'Mac' or 'MS-DOS'
 *   - order         : order of terms: 'name' (default), 'tid' or 'weight'
 *   // Level of result process infos:
 *   - check_options : boolean. check or not (default) this array of options
 *   - result_display: boolean. display or not (default). Only used with UI
 *   - result_duplicates: boolean. display or not (default) duplicate terms
 *   All options have default values.
 *   Warning: default values are different with UI.
 *
 * @return
 *   Array of errors or file object to download (need to execute batch process;
 *   result is logged in watchdog).
 */
function taxonomy_csv_export($options) {
  // Complete $options with default values if needed.
  // Default api and UI options are different.
  $options += _taxonomy_csv_values('export_default_api');

  // Presave a file in order to check access to temporary folder.
  $messages = _taxonomy_csv_export_output_presave($options);
  if (count($messages)) {
    return $messages;
  }

  // Process export.
  $messages = taxonomy_csv_vocabulary_export($options);
  // Return errors if any.
  if (count($messages)) {
    return $messages;
  }
  // Else return file infos.
  else {
    return $options['file'];
  }
}

/**
 * Presave output.
 *
 * Check if there is write access and prepare file.
 *
 * @param $options
 *   Array. Same as taxonomy_csv_export.
 *
 * @return
 *   Array of messages errors if any.
 *   By reference options are cleaned and completed.
 */
function _taxonomy_csv_export_output_presave(&$options) {
  $messages = array();

  // Check if there is write access and prepare file.
  $filename = file_unmanaged_save_data(
    '',
    'public://' . 'taxocsv.csv',
    'FILE_EXISTS_RENAME');
  if (!$filename) {
    $messages['file'] = t('Check access rights to temp directory: export needs permission to write and to read in it. Export failed.');
  }
  else {
    $options['file'] = (object) array(
      'filename' => basename($filename),
      'filepath' => drupal_realpath($filename),
      'filesize' => filesize($filename),
    );
  }

  return $messages;
}

/**
 * Prepare the export of a vocabulary.
 *
 * @note
 * If not used in a form, don't forget to use batch_process().
 *
 * @param $options
 *   Array. Same as taxonomy_csv_export.
 *
 * @return
 *   Array of errors or nothing (batch process to execute).
 */
function taxonomy_csv_vocabulary_export($options) {
  // Check options and return array of messages in case of errors.
  if ($options['check_options']) {
    // Invoke export admin file.
    $module_dir = drupal_get_path('module', 'taxonomy_csv');
    require_once("$module_dir/export/taxonomy_csv.export.admin.inc");
    $result = _taxonomy_csv_export_check_options($options);
    if (count($result)) {
      return $result;
    }
  }

  // Complete $options with some csv variables.
  $options['separator'] = $options['enclosure'] . $options['delimiter'] . $options['enclosure'];
  $line_ending = array(
    'Unix'   => "\n",
    'Mac'    => "\r",
    'MS-DOS' => "\r\n",
  );
  $options['end_of_line'] = $line_ending[$options['line_ending']];

  // Calculates number of terms to be exported.
  $options['total_terms'] = taxonomy_csv_vocabulary_count_terms($options['vocabulary_id']);

  // Get infos about fields of vocabulary.
  $options['vocabulary'] = array();
  foreach ($options['vocabulary_id'] as $vocabulary_id) {
    $options['vocabulary'][$vocabulary_id] = taxonomy_vocabulary_load($vocabulary_id);
    $vocabulary = &$options['vocabulary'][$vocabulary_id];
    $vocabulary->instances = field_info_instances('taxonomy_term', $vocabulary->machine_name);

    // Prepare list of fields to be exported.
    $vocabulary->fields = array();
    // Not included, because referenced terms are exported by name.
    // $vocabulary->fields['tid'] = array('cardinality' => 1);
    $vocabulary->fields['name'] = array('cardinality' => 1);
    // Not included, because there is already 'vocabulary_machine_name'.
    // $vocabulary->fields['vid'] = array('cardinality' => 1);
    $vocabulary->fields['vocabulary_machine_name'] = array('cardinality' => 1);
    $vocabulary->fields['description'] = array('cardinality' => 1);
    $vocabulary->fields['format'] = array('cardinality' => 1);
    $vocabulary->fields['weight'] = array('cardinality' => 1);
    $vocabulary->fields['parent'] = array('cardinality' => -1);
    if (module_exists('i18n_taxonomy')) {
      switch ($vocabulary->i18n_mode) {
        case I18N_MODE_LANGUAGE:
        case I18N_MODE_LOCALIZE:
          $vocabulary->fields['language'] = array('cardinality' => 1);
          break;

        case I18N_MODE_TRANSLATE:
        case I18N_MODE_MULTIPLE:
          $vocabulary->fields['language'] = array('cardinality' => 1);
          $vocabulary->fields['i18n_tsid'] = array('cardinality' => 1);
          break;
      }
    }
    // @todo
    // $vocabulary->fields['guid'] = array('cardinality' => 1);

    // Prepare list of unlimited fields to be exported.
    $vocabulary->fields_unlimited = array();
    $vocabulary->fields_unlimited['parent'] = 1;
    if (is_array($vocabulary->instances)) {
      foreach ($vocabulary->instances as $field_name => $value) {
        $vocabulary->fields[$field_name] = field_info_field($field_name);
        // Get the list of fields with an unlimited number of values to avoid
        // the loop of check (used with custom fields export).
        // The minimum is set to one to avoid zero value.
        if ($vocabulary->fields[$field_name]['cardinality'] == -1) {
          $vocabulary->fields_unlimited[$field_name] = 1;
        }
      }
    }
  }

  // Prepare export batch.
  $batch = array(
    'title'            => t('Exporting !total_terms terms to CSV file...', array('!total_terms' => $options['total_terms'])),
    'init_message'     => t('Starting downloading of datas...') . '<br />'
      . t('Wait some seconds for pre-processing...'),
    'progress_message' => '',
    'error_message'    => t('An error occurred during the export.'),
    'finished'         => '_taxonomy_csv_vocabulary_export_finished',
    'file'             => drupal_get_path('module', 'taxonomy_csv') . '/export/taxonomy_csv.export.api.inc',
    'progressive'      => TRUE,
    'operations'       => array(
      0 => array('_taxonomy_csv_vocabulary_export_process', array($options)),
    ),
  );

  batch_set($batch);
}

/**
 * Batch process of vocabulary export.
 *
 * @todo Check if direct query and only tid save is really less memory
 * intensive (with core taxonomy cache or not).
 * @todo Don't remember terms, but load them one by one in order to decrease
 * memory usage.
 *
 * @param $options
 *   Array of batch options.
 * @param &$context
 *   Batch context to keep results and messages.
 *
 * @return
 *   NULL because use of &$context.
 */
function _taxonomy_csv_vocabulary_export_process($options, &$context) {
  // First callback.
  if (empty($context['sandbox'])) {
    // Remember options as batch_set can't use form_storage.
    // It allows too that first line in result is numbered 1 and not 0.
    $context['results'][0] = $options;

    // Initialize some variables.
    $context['results'][0]['current_term'] = 0;
    $context['results'][0]['current_name'] = '';
    $context['results'][0]['worst_tid'] = 0;
    $context['results'][0]['worst_term'] = 0;
    $context['results'][0]['worst_name'] = '';
    $context['results'][0]['worst_message'] = 799;
    // No pointer because new line is appended to file.
    $context['results'][0]['handle'] = fopen($options['file']->filepath, 'a+');

    // Fetch terms to be exported and order them (by tree or by specific order).
    // Prepare a hierarchical tree of terms.
    if (in_array($options['export_format'], array(
        TAXONOMY_CSV_FORMAT_STRUCTURE,
        TAXONOMY_CSV_FORMAT_TREE,
        TAXONOMY_CSV_FORMAT_POLYHIERARCHY,
      ))) {
      $context['sandbox']['terms'] = array();
      // Export only selected vocabularies.
      if ($options['vocabulary_id'] && ($options['vocabulary_id'] != array(0))) {
        $vocabularies = array();
        foreach ($options['vocabulary_id'] as $vocabulary) {
          $vocabularies[] = taxonomy_vocabulary_load($vocabulary);
        }
      }
      // Export all vocabularies.
      else {
        $vocabularies = taxonomy_vocabulary_get_names();
      }

      foreach ($vocabularies as $vocabulary) {
        $context['sandbox']['terms'] = array_merge($context['sandbox']['terms'], taxonomy_get_tree($vocabulary->vid));
      }
    }
    // Prepare a list of normal terms.
    // @todo Prepare a simple list of term names or definitions without links in
    // order to reduce memory usage for some formats.
    // @todo Use a one term approach to reduce memory usage (anyway, all terms
    // will be load)?
    else {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'taxonomy_term');
      if ($options['vocabulary_id'] && ($options['vocabulary_id'] != array(0))) {
        $query->propertyCondition('vid', $options['vocabulary_id']);
      }
      $query->propertyOrderBy($options['order'], 'ASC');
      $terms = $query->execute();

      if ($terms['taxonomy_term'] === NULL) {
        $terms['taxonomy_term'] = array();
      }

      $context['sandbox']['terms'] = taxonomy_term_load_multiple(array_keys($terms['taxonomy_term']));
    }

    // Get max number of values for fields with a undefined number of values.
    if ($options['export_format'] == TAXONOMY_CSV_FORMAT_FIELDS) {
      // Currently, manage only undefined language.
      $language = 'und';

      foreach ($context['sandbox']['terms'] as $term) {
        foreach (array_keys($options['vocabulary'][$term->vid]->fields_unlimited) as $field_name) {
          if (isset($term->{$field_name})) {
            // Remember the number of items if this term field has got more
            // items than previous one.
            if ($field_name == 'parent'
                && (count($term->parent) > $options['vocabulary'][$term->vid]->fields_unlimited['parent'])
              ) {
              $options['vocabulary'][$term->vid]->fields_unlimited['parent'] = count($term->parent);
            }
            elseif (count($term->{$field_name}[$language]) > $options['vocabulary'][$term->vid]->fields_unlimited[$field_name]) {
              $options['vocabulary'][$term->vid]->fields_unlimited[$field_name] = count($term->{$field_name}[$language]);
            }
          }
        }
      }

      // Keep updated the list of vocabulary with undefined fields.
      $context['results'][0]['vocabulary'] = $options['vocabulary'];
      $context['sandbox']['vocabulary'] = $options['vocabulary'];
    }

    // Display a warning if terms count in taxonomy_term_data table is different
    // of terms counted with Drupal taxonomy_term_load_multiple() function.
    // See http://drupal.org/node/1359260.
    if ($options['total_terms'] != count($context['sandbox']['terms'])) {
      // Memorize the first wrong term.
      $first_wrong_term = array_shift(array_diff_key($terms['taxonomy_term'], taxonomy_term_load_multiple(array_keys($terms['taxonomy_term']))));
      $context['results'][0]['worst_tid'] = $first_wrong_term->tid;
      $context['results'][0]['worst_term'] = '';
      $context['results'][0]['worst_name'] = 'tid:' . ' ' . $first_wrong_term->tid;
      $context['results'][0]['worst_message'] = 408; // Warning not a good term.
    }

    // Clean memory.
    if (isset($terms)) {
      unset($terms);
    }

    // Duplicate names will be searched after export process.
    $context['results'][0]['duplicate_terms'] = array();
  }
  elseif (!is_resource($context['results'][0]['handle'])) {
    // Reopen file in case of memory out.
    $context['results'][0]['handle'] = fopen($options['file']->filepath, 'a+');
  }

  // Get list of unlimited multivalued fields.
  // @todo Currently, these fields are not saved in batch.
  if ($options['export_format'] == TAXONOMY_CSV_FORMAT_FIELDS) {
    $options['vocabulary'] = $context['sandbox']['vocabulary'];
  }

  // To simplify use of variables.
  $worst_tid       = &$context['results'][0]['worst_tid'];
  $worst_term      = &$context['results'][0]['worst_term'];
  $worst_name      = &$context['results'][0]['worst_name'];
  $worst_message   = &$context['results'][0]['worst_message'];
  $handle          = &$context['results'][0]['handle'];
  $duplicate_terms = &$context['results'][0]['duplicate_terms'];
  $term_number     = &$context['results'][0]['current_term'];
  $current_name    = &$context['results'][0]['current_name'];
  $terms_list      = &$context['sandbox']['terms'];
  $terms_list_ids  = array_keys($terms_list);

  // Process one term until end of vocabulary.
  if ($term_number < count($context['sandbox']['terms'])) {
    $term = $terms_list[$terms_list_ids[$term_number]];
    ++$term_number;

    // Remember current name/tid in case of error.
    $current_name = $term->name;
    $current_tid  = $term->tid;

    // Process export of current term.
    $result = taxonomy_csv_term_export($term, $options, $terms_list, $duplicate_terms);
    // If result is empty, there is nothing to export (with Translation export,
    // terms can be skipped and exported with another term).
    $result['msg'] = taxonomy_csv_line_export($result['line'], $options, $handle, $result['msg']);

    // Remember worst message of exported terms.
    $worst_message_new = _taxonomy_csv_worst_message($result['msg']);
    if ($worst_message > $worst_message_new) {
      $worst_tid     = $current_tid;
      $worst_term    = $term_number;
      $worst_name    = $current_name;
      $worst_message = $worst_message_new;
    };

    // Remember messages. Currently useless because there isn't any warning or
    // notice message (only error). A result level can be added here if needed.
    if (count($result['msg'])) {
      $context['results'][$term_number] = $result['msg'];
    }

    // Inform about progress.
    $context['message'] = t('Term !term_number of !total_terms processed: %term', array(
      '!term_number' => $term_number,
      '!total_terms' => $options['total_terms'],
      '%term'        => $current_name,
    ));

    // Check worst message of exported terms and update progress.
    if ($worst_message >= TAXONOMY_CSV_PROCESS_WARNING) {
      // Count should be <= 0.99 to avoid batch stop before end (Drupal 7 bug).
      $context['finished'] = floor($term_number / count($context['sandbox']['terms']) * 100) / 100;
    }
    else {
      $context['finished'] = 1;
    }
  }
}

/**
 * Callback for finished batch export and display result informations.
 */
function _taxonomy_csv_vocabulary_export_finished($success, $results, $operations) {
  $options = &$results[0];
  unset($results[0]);

  // Close exported file.
  if ($options['handle']) {
    fclose($options['handle']);
  }

  // Invoke export stats file if user wants to display results.
  if ($options['result_display']) {
    $module_dir = drupal_get_path('module', 'taxonomy_csv');
    require_once("$module_dir/export/taxonomy_csv.export.result.inc");
  }

  // Short summary information is different if batch succeeded or not.
  if ($success) {
    if (!empty($options['worst_tid'])) {
      $worst_path = drupal_get_path_alias('taxonomy/term/' . $options['worst_tid']);
      $options['worst_name'] = l($options['worst_name'], $worst_path);
    }
    $variables = array(
      '!total_terms' => $options['total_terms'],
      '!worst_count' => $options['worst_term'],
      '!worst_name'  => $options['worst_name'],
      '!worst_msg'   => $options['result_display'] ?
          _taxonomy_csv_message_text($options['worst_message']) :
          t('Message code') . ' = ' . $options['worst_message'],
    );
    $messages = array(
      WATCHDOG_DEBUG   => t('No error, warnings or notices have been reported during export process of !total_terms terms.', $variables),
      WATCHDOG_INFO    => t('No error, warnings or notices have been reported during export process of !total_terms terms.', $variables),
      WATCHDOG_NOTICE  => t('Notices have been reported during export process (bad formatted or empty terms). !total_terms terms processed. First notice occurred on term !worst_count (!worst_name) [!worst_msg].', $variables),
      WATCHDOG_WARNING => t('Warnings have been reported during export process (bad formatted terms). !total_terms terms processed. First term skipped is term !worst_count (!worst_name) [!worst_msg].', $variables),
      WATCHDOG_ERROR   => t('Errors have been reported during export process. Process failed at term !worst_count (!worst_name) of a total of !total_terms [!worst_msg].', $variables),
    );
    $worst_level = intval($options['worst_message'] / 100);
    $message = $messages[$worst_level];
  }
  else {
    $message = t('Exportation failed. Export process was successful until the term !term_count (!term_name) of a total of !total_terms.', array(
        '!term_count'  => $options['current_term'],
        '!term_name'   => $options['current_name'],
        '!total_terms' => $options['total_terms'],
      )) . '<br />'
      . t('This issue is related to export process and may be caused by a memory overrun of the database. If not, you can reinstall module from a fresh release or submit an issue on <a href="!link">Taxonomy CSV import/export module</a>.', array(
        '!link' => url('http://drupal.org/project/issues/taxonomy_csv/'),
      ));
    $worst_level = WATCHDOG_ERROR;
  }

  // Set result message in watchdog and eventually in user interface.
  // Use of a $message variable is unrecommended, but simpler and working.
  // See http://drupal.org/node/323101
  watchdog('taxonomy_csv', $message, NULL, $worst_level);
  if ($options['result_display']) {
    _taxonomy_csv_export_result($options, $worst_level, $message, $results);
  }
}

/**
 * Export a line.
 *
 * @param $line
 *   Array to be exported to a line.
 * @param $options
 *   An associative array of export options:
 *   - 'separator'  : string separator (formatted delimiter and enclosure).
 *   - 'enclosure'  : item enclosure.
 *   - 'end_of_line': end of line string.
 * @param $handle
 *   Handle of the open file where to save line.
 * @param $result_message
 *   (Optional) Array of messages.
 *
 * @return
 *   Result array of messages.
 */
function taxonomy_csv_line_export($line, $options, &$handle, $result = array()) {
  if (!$line) {
    return $result;
  }

  // Check if separator, enclosure or line ending exist in line.
  $check_line = implode('', $line);
  if ((strpos($check_line, $options['separator']) !== FALSE)
      || (($options['enclosure'] != '')
        && (strpos($check_line, $options['enclosure']) !== FALSE))
      || (($options['enclosure'] == '')
        && (strpos($check_line, $options['end_of_line']) !== FALSE))) {
    $result[] = 313; // Error delimiter or enclosure.
  }
  // Skip a term with warning.
  elseif (_taxonomy_csv_worst_message($result) < TAXONOMY_CSV_PROCESS_NOTICE) {
  }
  else {
    // Save line to file.
    $line = $options['enclosure'] . implode($options['separator'], $line) . $options['enclosure'] . $options['end_of_line'];
    if (fwrite($handle, $line) === FALSE) {
      $result[] = 312; // Unable to write to file.
    }
  }

  return $result;
}

/**
 * Export a term to a line matching the options.
 *
 * @param $term
 *   Full term object to export.
 * @param $options
 *   An associative array of export options:
 *   - export_format : format of the csv line (see taxonomy.api.inc)
 * @param $terms_list
 *   (Optional) Array of all term objects to export, used to avoid to repeat
 *   fetch of terms.
 * @param $duplicate_terms
 *   (Optional) Array of duplicate terms names indexed by tid.
 *
 * @return
 *   Result array with:
 *   - 'line' => array of exported items,
 *   - 'msg'  => array of messages arrays.
 */
function taxonomy_csv_term_export($term, $options, &$terms_list = array(), $duplicate_terms = array()) {
  // Define default values.
  $result = array(
    'line' => array(),
    'msg'  => array(),
  );

  // Only count check because term and options are already checked.
  if (count($term)) {
    switch ($options['export_format']) {
      case TAXONOMY_CSV_FORMAT_FLAT:
        $result['line'][] = $term->name;
        break;

      case TAXONOMY_CSV_FORMAT_STRUCTURE:
      case TAXONOMY_CSV_FORMAT_TREE:
        $terms = taxonomy_csv_term_get_first_path($term, $terms_list);
        foreach ($terms as $parent) {
          $result['line'][] = $parent->name;
        }
        $result['line'][] = $term->name;
        break;

      case TAXONOMY_CSV_FORMAT_POLYHIERARCHY:
        // @todo
        // Warning : taxonomy_csv_term_get_first_path() returns only first path.
        break;

      case TAXONOMY_CSV_FORMAT_FIELDS:
        // Currently, manage only undefined language.
        $language = 'und';

        // Use of field_get_items is slower.
        foreach ($options['vocabulary'][$term->vid]->fields as $field_name => $field) {
          $count = 0;

          // Item is a Field.
          if (isset($field['type'])) {
            if (isset($term->{$field_name}[$language])) {
              // For taxonomy term reference, use name instead of value.
              if ($field['type'] == 'taxonomy_term_reference') {
                foreach ($term->{$field_name}[$language] as &$item) {
                  $result['line'][] = isset($terms_list[$item['tid']]) ?
                    $terms_list[$item['tid']]->name :
                    taxonomy_term_load($item['tid'])->name;
                }
              }
              // For long text, need to escape the value.
              elseif ($field['type'] == 'text_long'
                  || $field['type'] == 'text_with_summary'
                ) {
                foreach ($term->{$field_name}[$language] as &$item) {
                  $result['line'][] = _taxonomy_csv_escape_line_break($item['value']);
                }
              }
              else {
                // Key is generally 'value' but it can be something else.
                reset($field['columns']);
                foreach ($term->{$field_name}[$language] as &$item) {
                  $result['line'][] = $item[key($field['columns'])];
                }
              }
              $count = count($term->{$field_name}[$language]);
            }
          }
          // Item is a database term field (name, tid, language...).
          else {
            // For taxonomy term parent, use name instead of value.
            if ($field_name == 'parent') {
              if (isset($term->parent)) {
                foreach ($term->parent as $tid) {
                  $result['line'][] = isset($terms_list[$tid]) ?
                    $terms_list[$tid]->name :
                    taxonomy_term_load($tid)->name;
                }
                $count = count($term->parent);
              }
            }
            elseif ($field_name == 'description') {
              $result['line'][] = _taxonomy_csv_escape_line_break($term->description);
              $count = 1;
            }
            else {
              $result['line'][] = $term->{$field_name};
              $count = 1;
            }
          }

          // Add empty value the max number of values in the vocabulary times.
          if (isset($options['vocabulary'][$term->vid]->fields_unlimited[$field_name])) {
            if ($count < $options['vocabulary'][$term->vid]->fields_unlimited[$field_name]) {
              $result['line'] = array_merge($result['line'], array_fill(0, $options['vocabulary'][$term->vid]->fields_unlimited[$field_name] - $count, ''));
            }
          }
          elseif ($count < $field['cardinality']) {
            $result['line'] = array_merge($result['line'], array_fill(0, $field['cardinality'] - $count, ''));
          }
        }
        break;

      case TAXONOMY_CSV_FORMAT_TRANSLATE:
        if (!module_exists('i18n_taxonomy')) {
          $result['msg'][] = 360; // Translation error.
          break;
        }

        switch ($options['vocabulary'][$term->vid]->i18n_mode) {
          case I18N_MODE_NONE:
          case I18N_MODE_LANGUAGE:
            $result['line'][] = $term->name;
            break;

          case I18N_MODE_LOCALIZE:
            $result['line'][] = $term->name;

            $languages = locale_language_list('name');
            unset($languages[language_default('language')]);

            foreach ($languages as $language => $value) {
              $translation = i18n_string_translate(
                array('taxonomy', 'term', $term->tid, 'name'),
                $term->name,
                array('langcode' => $language)
              );
              $result['line'][] = $translation ?
                $translation :
                '';
            }
            break;

          case I18N_MODE_TRANSLATE:
          case I18N_MODE_MULTIPLE:
            $languages = array(
              'und' => t('Language neutral'),
            );
            $languages += locale_language_list('name');
            $languages = array_flip(array_keys($languages));

            $result['line'] = array_fill(0, count($languages), '');
            if ($term->i18n_tsid == 0) {
              $result['line'][$languages[$term->language]] = $term->name;
            }
            else {
              $translation_set = i18n_translation_set_load($term->i18n_tsid);
              $translations = $translation_set->get_translations();
              $language_min = $languages[$term->language];
              foreach ($translations as $language => $translated_term) {
                $result['line'][$languages[$language]] = $translated_term->name;
                // Check if this term is already exported (when there is a term
                // with a language before the current one).
                if ($languages[$language] < $language_min) {
                  $language_min = $languages[$language];
                }
              }
              // This term is already exported or will be exported with another
              // term.
              if ($language_min < $languages[$term->language]) {
                $result['line'] = array();
              }
            }
            break;
        }
        break;

      default:
        $result['msg'][] = 307; // Error unknown export format.
    }
  }
  else {
    $result['msg'][] = 385; // Error no term to process.
  }

  // Clean result.
  $result['msg'] = array_unique($result['msg']);
  sort($result['msg']);

  return $result;
}
