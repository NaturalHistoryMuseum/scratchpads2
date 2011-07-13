<?php
// $Id: taxonomy_manager.format.inc,v 1.2.2.3 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 *   Plug-in that manages Taxonomy manager vocabularies import/export .
 *
 * @see http://drupal.org/project/taxonomy_manager
 */

/**
 * Return infos on the format.
 */
function taxonomy_csv_format_taxonomy_manager() {
  return array(
    'format'          => 'taxonomy_manager',
    'name'            => t('Taxonomy manager'),
    'needed_module'   => '',
    'import_format'   => t('Taxonomy manager'),
    'export_format'   => t('Taxonomy manager'),
    'import_allowed'  => array(
      TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
      TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
      TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
    ),
    'import_previous' => TRUE, // Because import remembers all previous_items.
    'specific_fields' => FALSE,
    'description'     => t('Allow to import a vocabulary exported with <a href="!link">Taxonomy manager</a>.', array(
      '!link' => url('http://drupal.org/project/taxonomy_manager'),
    )),
    'description_format'  => t('vocabulary id, term id, term name, term description, list of first level parent ids'),
    'description_example' => t(', 1, "Île-de-France", "Main region"') . '<br />'
      . t(', 2, "Paris", "Capital of France", 1'),
    'description_long'    => t("In this format, order of all lines is important, because it's impossible to attach a parent to an item if this parent hasn't been imported in a previous line.") . '<br />'
      . t('Ids are not required, and may be numbers or not, but they need to be unique to avoid duplicate terms.') . '<br />'
      . t("The first item (vocabulary id) is not used. You need to choose the option 'autocreate' or 'existing' vocabulary."),
    'import_options_help' => t("When a vocabulary is imported in an existing one, only third option (ignore existing terms) can be used.") . '<br />'
      . t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: update term or create it if not exists and merge eventual parents with new ones.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: update term or create it if not exists and replace eventual parents with new ones.') . '</li>
          <li>' . t('<em>"Ignore and create"</em>: term is always created.') . '</li>
        </ul>',
  );
}

/**
 * Check items of an imported line.
 *
 * Specificities: tid array is indexed with old tids.
 */
function _taxonomy_csv_line_import_check_taxonomy_manager($line, $options, $previous_items = array(), &$messages = array()) {
  $checked_items = $line;

  switch ($checked_items) { // Simulate a goto with break.
    default:
      if (empty($line[0])) {
        $messages[] = 550; // Notice no first column.
      }
      if (count($line) < 3) {
        $messages[] = 450; // Warning some items lacks.
        break;
      }
      if (empty($line[1]) || empty($line[2])
          || ($line[1] == 0) || ($line[2] == '')) {
        $messages[] = 481; // Warning no item in second or third column.
        break;
      }
      if (count($line) == 4) {
        $messages[] = 551; // Notice root term.
        $checked_items[] = 0;
      }
      $list_parents = array_unique(array_slice($checked_items, 4));
      sort($list_parents);
      if (count($list_parents) != count(array_slice($checked_items, 4))) {
        $messages[] = 531; // Notice duplicates, which are removed.
      }
      foreach ($list_parents as $value) {
        if ($value == $checked_items[1]) {
          $messages[] = 452; // Warning a term can't be a parent of itself.
          break 2;
        }
      }
      $checked_items = array_merge(array_slice($checked_items, 0, 4), $list_parents);
      if (($list_parents[0] === 0) && (count($list_parents) > 1)) {
        $messages[] = 453; // Warning: root term has a parent.
        break;
      }
      // $previous_items keys contain all previous lines source tid.
      if (isset($previous_items['tid'][$checked_items[1]])) {
        $messages[] = 552; // Notice: term has been already imported.
      }
      foreach ($list_parents as $value) {
        if (($value != 0) && !isset($previous_items['tid'][$value])) {
          $messages[] = 352; // Error: parent tid hasn't been already imported.
          break;
        }
      }
      foreach (array_merge(array_slice($checked_items, 0, 3), array_slice($checked_items, 4)) as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
  }

  return array_values($checked_items);
}

/**
 * Import a line of items.
 *
 * @param $previous_items
 *   Specificity: tid and name arrays are indexed with old tids.
 */
function taxonomy_csv_line_import_taxonomy_manager($line, $options, $previous_items = array(), $terms_count = 0) {
  // Define default values.
  $result = array(
    'name' => array(),
    'tid'  => array(),
    'msg'  => array(),
    'terms_count' => $terms_count,
  );

  $term = (object) array(
    'name'        => $line[2],
    'vid'         => $options['vocabulary_id'],
    'description' => $line[3],
  );
  // Weight is not set with the name and vid above in order to keep
  // existing one if it exists.
  ++$terms_count;
  if ($options['keep_order']) {
    $term->weight = $terms_count;
  }

  // Complete with parents except root. All parents are already checked.
  $list_parents = array_slice($line, 4);
  if ($list_parents[0] != 0) {
    foreach ($list_parents as $value) {
      $term->parents[] = $previous_items['tid'][$value];
    }
  }

  // Use tid if term is already imported.
  if (isset($previous_items['tid'][$line[1]])) {
    $term->tid = $previous_items['tid'][$line[1]];
  }
  else {
    // No tid is given, so it's always a new term.
    $options['existing_items'] = TAXONOMY_CSV_EXISTING_IGNORE_CREATE;
  }

  // Import term then store result. No check because only one term.
  $current_result = taxonomy_csv_term_import($term, $options['existing_items']);
  // Keep old index (names and tids).
  $result = $previous_items;
  $result['name'][$line[1]] = $current_result['name'];
  $result['tid'][$line[1]]  = $current_result['tid'];
  $result['msg'] = $current_result['msg'];
  $result['terms_count'] = $terms_count;

  return $result;
}

/**
 * Export a term.
 */
function taxonomy_csv_term_export_taxonomy_manager($term, $options = array(), $duplicate_terms = array()) {
  // Define default values.
  $result = array(
    'line' => array(),
    'msg'  => array(),
  );

  $result['line'] = array_merge(
    array(
      $term->vid,
      $term->tid,
      $term->name,
      $term->description,
    ),
    taxonomy_csv_term_get_parents_names($term->tid)
  );

  return $result;
}
