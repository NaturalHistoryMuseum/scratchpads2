<?php
// $Id: taxonomy_csv.import.result.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

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
      $message = t('Import from Api (use %file).', array(
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
    case 'path':
      $message = t('File "%file" uploaded.', array(
        '%file' => $options['file']->filename));
      break;
  }

  // Content.
  $list = _taxonomy_csv_values('import_format');
  $message .= '<br />' . t('Source content: "!import_format".', array(
    '!import_format' => $list[$options['import_format']],
  )) . '<br />';

  // Existing items.
  $list = _taxonomy_csv_values('import_option');
  $message .= t('Existing terms choice: "!import_items".', array(
    '!import_items' => $list[$options['existing_items']],
  )) . '<br />';

  // Custom locale.
  if ($options['locale_custom']) {
    $message .= t('Custom locale "!locale" has been used.', array(
      '!locale' => $options['locale_custom'],
    ));
  }

  return $message;
}

/**
 * Check created vocabularies and return formatted info on them.
 */
function _taxonomy_csv_info_vocabulary($options) {
  $messages = array();
  // General info on used or created vocabularies.
  // With some formats, multiple vocabularies can be created.
  $vocabularies = (in_array($options['import_format'], array(
      TAXONOMY_CSV_FORMAT_DEFINITION_LINKS,
    ))) ?
      $options['vocabularies'] :
      array($options['vocabulary_id']);
  $messages[] = t('%count used or created.', array(
    '%count' => format_plural(count($vocabularies), 'A vocabulary has been', '@count vocabularies have been', array())
  ));

  foreach ($vocabularies as $vocabulary_id) {
    $message = '';
    // Check or update hierarchy of vocabularies.
    $vocabulary = taxonomy_vocabulary_load($vocabulary_id);
    if ($options['check_hierarchy']) {
      $result = _taxonomy_csv_check_vocabulary_hierarchy($vocabulary, '');
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
    $message .= (in_array($options['import_format'], array(
          TAXONOMY_CSV_FORMAT_DEFINITION_LINKS,
        ))) ?
        t('Vocabulary "%vocabulary_name" has been used or created.', array('%vocabulary_name' => $vocabulary->name)) :
        _taxonomy_csv_info_vocabulary_destination($vocabulary, $options['vocabulary_target']);
    $message .=  '<br />';

    if (!$options['check_hierarchy']) {
      $message .= t('Hierarchy level has been manually set.') . ' ';
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
  )) . '<br />';
  if (module_exists('taxonomy_manager')) {
    $message .= t('It is recommended to use <a href="!link" title="Taxonomy manager module">Administer > Structure > Taxonomy manager > Vocabularies</a> with big taxonomies because a memory error can occur with default Drupal "List terms" menu.', array(
      '!link' => url("admin/structure/taxonomy_manager/voc/$vocabulary->machine_name"),
    ));
  }
  else {
    $message .= t('Warning: with default Drupal "List terms" function, a memory error can occur with big taxonomies, so use another manager as <a href="!link" title="Taxonomy manager">Taxonomy manager</a>.', array(
      '!link' => url('http://drupal.org/project/taxonomy_manager'),
    ));
  }
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

      $message .= '"';
      foreach ($terms as $term) {
        $message .= $term->name . '", "';
      }
      $message = drupal_substr($message, 0, drupal_strlen($message) - 3) . '.';
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
