<?php

/**
 * @file
 * Show export result messages.
 */

/**
 * Invoke associated include file.
 */
$module_dir = drupal_get_path('module', 'taxonomy_csv');
require_once($module_dir . '/taxonomy_csv.result.inc');

/**
 * Display result messages of export process.
 *
 * @param $options
 *   Array. Same as taxonomy_csv_export.
 * @param $worst_level
 *   Integer. Worst watchdog level of the process.
 * @param $summary
 *   Translated string. Short summary of informations on process.
 * @param $results
 *   Array. Batch process results.
 *
 * @return
 *   Nothing.
 */
function _taxonomy_csv_export_result($options, $worst_level, $summary, $results) {
  // Prepare info and warning messages.
  $messages = array();

  // Summary message.
  $messages[] = array($worst_level => $summary);

  // Prepare batch result message.
  if ($options['total_terms'] == 0) {
    $message = t('Notice: Chosen vocabulary has no term to export.');
    $messages[] = array(WATCHDOG_NOTICE => $message);
  }
  // Only summary is displayed in case of error.
  elseif ($worst_level >= WATCHDOG_WARNING) {
    $list_format = _taxonomy_csv_values('export_format');
    // General infos.
    $message = t('!term_count / !total_terms terms of chosen vocabularies have been exported to file !file (!filesize KB) with the format "!format". Click on link to view it or right click to download it.', array(
      '!term_count'  => $options['current_term'],
      '!total_terms' => $options['total_terms'],
      '!file'        => l($options['file']->filename, file_create_url('public://' . $options['file']->filename)),
      '!filesize'    => number_format(filesize($options['file']->filepath) / 1000, 1),
      '!format'      => $list_format[$options['export_format']],
    ));
    // Main check of end process.
    $watchdog_level = (($options['current_term'] == $options['total_terms']) && ($options['current_term'] > 0)) ?
        WATCHDOG_INFO :
        WATCHDOG_ERROR;
    $messages[] = array($watchdog_level => $message);

    // Infos on duplicate term names.
    $options['duplicate_terms'] = array();
    foreach ($options['vocabulary_id'] as $item) {
      $options['duplicate_terms'] += taxonomy_csv_term_find_duplicate($item);
    }

    $duplicate_count = count($options['duplicate_terms']);
    // If there are duplicate term names, display them if user chooses it.
    if ($duplicate_count) {
      $message = t('!count duplicate term names have been found in all vocabularies. Error can occur when you will import this list, except if you choose "fields and links" export format or import option "ignore existing items".', array('!count' => $duplicate_count));
      if ($options['result_duplicates']) {
        $message .=  '<br />' . t('List of duplicated term names:')
          . ' "' . implode('", "', array_keys(array_flip($options['duplicate_terms']))) . '".';
      }
      $watchdog_level = WATCHDOG_NOTICE;
    }
    else {
      // No duplicate term names.
      $message = t('No duplicate term name has been found.');
      $watchdog_level = WATCHDOG_INFO;
    }
    $messages[] = array($watchdog_level => $message);

    // Info on specific format.
    foreach ($options['vocabulary'] as $vocabulary) {
      switch ($options['export_format']) {
        case TAXONOMY_CSV_FORMAT_TREE:
          if ($vocabulary->hierarchy == 2) {
            $message = t('Vocabulary "@vocabulary" is polyhierarchical. With the selected format (tree), only the first parent of each term was exported.', array(
              '@vocabulary' => $vocabulary->name,
            ));
            $messages[] = array(WATCHDOG_NOTICE => $message);
          }
          break;

        case TAXONOMY_CSV_FORMAT_FIELDS:
          $format = array();
          foreach ($vocabulary->fields as $field_name => &$field) {
            if (isset($vocabulary->fields_unlimited[$field_name])) {
              $format = array_merge($format, array_fill(0, $vocabulary->fields_unlimited[$field_name], $field_name));
            }
            else {
              $format = array_merge($format, array_fill(0, $field['cardinality'], $field_name));
            }
          }

          $message = t('CSV format of vocabulary "@vocabulary" is:', array(
            '@vocabulary' => $vocabulary->name,
          ));
          $message .= '<br /><pre>' . implode(', ', $format) . '</pre>';
          $messages[] = array(WATCHDOG_INFO => $message);
          break;

        case TAXONOMY_CSV_FORMAT_TRANSLATE:
          $format = array();
          switch ($vocabulary->i18n_mode) {
            case I18N_MODE_NONE:
              $format[] = 'und';
              break;

            case I18N_MODE_LANGUAGE:
              $format[] = $vocabulary->language;
              break;

            case I18N_MODE_LOCALIZE:
              $format = locale_language_list('name');
              unset($format[language_default('language')]);
              $format = array_flip($format);
              array_unshift($format, language_default('language'));
              break;

            case I18N_MODE_TRANSLATE:
            case I18N_MODE_MULTIPLE:
              $format = locale_language_list('name');
              $format = array_flip($format);
              array_unshift($format, 'und');
              break;
          }

          $message = t('CSV format of vocabulary "@vocabulary" is:', array(
            '@vocabulary' => $vocabulary->name,
          ));
          $message .= '<br /><pre>' . implode(', ', $format) . '</pre>';
          $messages[] = array(WATCHDOG_INFO => $message);
          break;
      }
    }
  }

  _taxonomy_csv_message_result($messages);
}
