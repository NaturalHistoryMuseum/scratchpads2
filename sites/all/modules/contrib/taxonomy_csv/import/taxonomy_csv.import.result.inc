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
 * Display result messages of import process.
 *
 * @param $options
 *   Array. Same as taxonomy_csv_import.
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
function _taxonomy_csv_import_result($options, $worst_level, $summary, $results) {
  // Prepare info and warning messages.
  $messages = array();

  // Summary message.
  $messages[] = array($worst_level => $summary);

  // Prepare batch result message.
  if ($options['total_lines'] == 0) {
    $message = t('Notice: Input has no term to import.');
    $messages[] = array(WATCHDOG_NOTICE => $message);
  }
  // Only summary is displayed in case of error.
  elseif ($worst_level >= WATCHDOG_WARNING) {
    // General infos.
    $messages[] = array(WATCHDOG_INFO => _taxonomy_csv_info_chosen_options($options));

    // Check and info on used or created vocabularies.
    $messages[] = array(WATCHDOG_INFO => _taxonomy_csv_info_vocabulary($options));

    if (!$options['check_line']) {
      $messages[] = array(WATCHDOG_NOTICE => t('Line checks have been disabled. Some warnings and notices may have not been reported.'));
    }

    // Display stats and eventually lists about imported terms.
    if ($options['result_stats']
        || $options['result_terms']) {
      $messages[] = array(WATCHDOG_INFO => _taxonomy_csv_info_terms($options));
    }

    // Display messages about line process.
    if ($options['result_level'] == 'first') {
      $messages[] = array(WATCHDOG_NOTICE => t('No more information: choice is to report only first warning or notice if any.'));
    }
    else {
      // Display detailled result of import. First info was already displayed.
      $messages = array_merge($messages, _taxonomy_csv_info_lines($options, $results));
    }
  }

  _taxonomy_csv_message_result($messages);
}

/**
 * Return formatted list of chosen options.
 */
function _taxonomy_csv_info_chosen_options($options) {
  if (!isset($options['source_choice'])) {
    $options['source_choice'] = 'file';
  }
  // Local file, distant file or text.
  switch ($options['source_choice']) {
    case 'file':
      $message = t('File "%file" uploaded.', array(
        '%file' => $options['file']->filename));
      break;
    case 'text':
      $message = t('Import terms from text.');
      break;
    case 'url':
      $message = t('File "<a href="!url">%file</a>" uploaded from an url.', array(
        '!url'  => $options['url'],
        '%file' => basename($options['url'])));
      break;
    default:
      $message = t('Import from Api.');
  }

  // Content.
  $message .= '<br />' . t('Source content: "!import_format".', array(
    '!import_format' => $options['import_format'],
  )) . '<br />';

  // Existing items.
  $message .= t('Existing terms choice: "!import_items".', array(
    '!import_items' => $options['update_or_ignore'],
  )) . '<br />';

  // Internationalization.
  if ($options['import_format'] != TAXONOMY_CSV_FORMAT_TRANSLATE) {
    $message .= t('Language of terms: "!language".', array(
      '!language' => $options['language'],
    )) . '<br />';
  }

  // Custom locale.
  if ($options['locale_custom']) {
    $message .= t('Custom locale "!locale" has been used.', array(
      '!locale' => $options['locale_custom'],
    )) . '<br />';
  }

  return $message;
}

/**
 * Check created vocabularies and return formatted info on them.
 */
function _taxonomy_csv_info_vocabulary($options) {
  $messages = array();
  // General info on used or created vocabularies.
  $vocabularies = array($options['vocabulary_id']);
  $messages[] = t('%count used or created.', array(
    '%count' => format_plural(count($vocabularies), 'A vocabulary has been', '@count vocabularies have been', array())
  ));

  foreach ($vocabularies as $vocabulary_id) {
    $message = '';
    // Check or update hierarchy of vocabularies.
    $vocabulary = taxonomy_vocabulary_load($vocabulary_id);
    if ($options['check_hierarchy']) {
      $result = taxonomy_check_vocabulary_hierarchy($vocabulary, array('tid' => 0));
    }
    // Set vocabulary hierarchy only if needed.
    else {
      if ($options['set_hierarchy'] != $vocabulary->hierarchy) {
        $vocabulary->hierarchy = $options['set_hierarchy'];
        $result = taxonomy_vocabulary_save($vocabulary);
      }
      $result = $options['set_hierarchy'];
    }
    $new_hierarchy[$vocabulary_id] = $result;

    // Display general info about vocabulary.
    $message .= _taxonomy_csv_info_vocabulary_destination($vocabulary, $options['vocabulary_target']);
    $message .=  '<br />';

    if (isset($vocabulary->language) && module_exists('i18n_taxonomy')) {
      $message .= t('Language of vocabulary: "!language".', array(
        '!language' => $vocabulary->language,
      )) . '<br />';
      $language_mode = array(
        I18N_MODE_NONE => t('None'),
        I18N_MODE_LOCALIZE => t('Localize'),
        I18N_MODE_TRANSLATE => t('Translate'),
        I18N_MODE_MULTIPLE => t('Multiple'),
        I18N_MODE_LANGUAGE => t('Fixed language'),
      );
      $message .= t('Internationalization mode of vocabulary: "!language".', array(
        '!language' => $language_mode[$vocabulary->i18n_mode],
      )) . '<br />';

      if ($options['language'] != $vocabulary->language) {
        $message .= '<strong>' . t('Warning') . '</strong>: ' . t('Language of imported terms is not the language of the vocabulary. You may check multilingual options to see terms.') . '<br />';
      }
    }

    if ($options['delete_terms']) {
      $message .= t('Existing terms of the vocabulary have been deleted.') . '<br />';
    }

    if (!$options['check_hierarchy']) {
      $message .= t('Hierarchy level has been manually set.') . '<br />';
    }

    $message .= _taxonomy_csv_info_vocabulary_result($vocabulary, $options['vocabulary_target'], $new_hierarchy[$vocabulary_id]) . '<br />';
    $messages[] = $message;
  }

  return implode('<br />', $messages);
}

/**
 * Return informations about destination vocabulary.
 */
function _taxonomy_csv_info_vocabulary_destination($vocabulary, $vocabulary_target) {
  // Destination vocabulary.
  $list_messages = array(
    'autocreate' => t('New vocabulary "%vocabulary_name" has been created.', array(
      '%vocabulary_name' => $vocabulary->name,
    )),
    'duplicate'  => t('Duplicate vocabulary "%vocabulary_name" has been created.', array(
      '%vocabulary_name' => $vocabulary->name,
    )),
    'existing'   => t('Terms are imported into existing vocabulary "%vocabulary_name".', array(
      '%vocabulary_name' => $vocabulary->name,
    )),
  );

  return $list_messages[$vocabulary_target];
}

/**
 * Return informations about result vocabulary.
 */
function _taxonomy_csv_info_vocabulary_result($vocabulary, $vocabulary_target, $new_hierarchy) {
  $message = '';

  // Level of vocabulary.
  $hierarchy_text = array(
    0 => t('no parent (flat)'),
    1 => t('single parent (tree)'),
    2 => t('multiple parents (polyhierarchy)'),
  );
  if ($vocabulary_target != 'autocreate') {
    $message .= t('Old hierarchy level was !level (!level_text).', array(
      '!level'      => $vocabulary->hierarchy,
      '!level_text' => $hierarchy_text[$vocabulary->hierarchy],
    )) . '<br />';
  }
  $message .= t('Hierarchy level is !level (!level_text).', array(
    '!level'      => $new_hierarchy,
    '!level_text' => $hierarchy_text[$new_hierarchy],
  )) . '<br />';

  // Direct links to edit vocabulary and view terms.
  $message .= t('Properties can be edited at <a href="!vocabulary_edit_link">Administer > Structure > Taxonomy > edit vocabulary</a>.', array(
    '!vocabulary_edit_link' => url("admin/structure/taxonomy/$vocabulary->machine_name/edit"),
  )) . '<br />';
  $message .= t('You can view terms at <a href="!vocabulary_list">Administer > Structure > Taxonomy > list terms</a>.', array(
    '!vocabulary_list' => url("admin/structure/taxonomy/$vocabulary->machine_name"),
  ));
  return $message;
}

/**
 * Display stats and eventually lists of terms about imported terms.
 *
 * Currently, display only total and list of imported terms.
 */
function _taxonomy_csv_info_terms($options) {
  if (isset($options['imported_terms'])) {
    $tids = array();
    foreach ($options['imported_terms'] as $imported_tids) {
      $tids = array_unique(array_merge($tids, $imported_tids));
    }

    $message = '';
    // Display basic stats.
    if ($options['result_stats']) {
      // Display total of imported terms.
      $message .= t('Total created or updated terms : !count.', array('!count' => count($tids))) . '<br />';
    }

    // Display list of terms.
    if ($options['result_terms']) {
      $terms = taxonomy_term_load_multiple($tids);

      $term_list = array();
      foreach ($terms as $term) {
        $term_list[] = $term->name;
      }
      $message .= '"' . implode('", "',  $term_list) . '".';
    }
  }
  else {
    $message = t('No term was imported.');
  }

  return $message;
}

/**
 * Display detailled result of import.
 */
function _taxonomy_csv_info_lines($options, $results) {
  $messages = array();

  if (count($results)) {
    $messages[] = array(WATCHDOG_INFO => t('Available informations about lines import.'));
    switch ($options['result_type']) {
      case 'by_line':
        $messages = array_merge($messages, _taxonomy_csv_message_result_by_line($results));
        break;
      case 'by_message':
        $messages = array_merge($messages, _taxonomy_csv_message_result_by_message($results));
        break;
    }
  }
  else {
    $messages[] = array(WATCHDOG_INFO => t('No more information reported about lines import.'));
  }

  return $messages;
}
