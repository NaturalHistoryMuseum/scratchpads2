<?php
// $Id: taxonomy_csv.export.result.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Show export result messages.
 */

/**
 * Invoke associated include file.
 */
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once("$module_dir/taxonomy_csv.result.inc");

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
    // List has been prepared only for some formats.
    if (!in_array($options['export_format'], array(
        TAXONOMY_CSV_FORMAT_DEFINITION_LINKS,
      ))) {
      $options['duplicate_terms'] = taxonomy_csv_term_find_duplicate($options['vocabulary_id']);
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

    // @todo Add line level messages.
    // Currently, it's useless, because there isn't any export message in
    // line export process. It can be added as in import.
  }

  _taxonomy_csv_message_result($messages);
}
