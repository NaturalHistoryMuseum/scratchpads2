<?php
// $Id: geotaxonomy.format.inc,v 1.2.2.3 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 *   Plug-in that manages geotaxonomy vocabularies import/export.
 *
 * @see http://drupal.org/project/geotaxonomy
 */

/**
 * Return infos on the format.
 */
function taxonomy_csv_format_geotaxonomy() {
  return array(
    'format'          => 'geotaxonomy',
    'name'            => t('Geotaxonomy'),
    'needed_module'   => 'geotaxonomy',
    'import_format'   => t('Geotaxonomy'),
    'export_format'   => t('Geotaxonomy'),
    'import_allowed'  => array(
      TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
      TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
      TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
    ),
    'import_previous' => TRUE, // Because import remembers all previous_items.
    'specific_fields' => TRUE,
    'description'     => t('Import <a href="!link">geotaxonomy</a> terms.', array(
      '!link' => url('http://drupal.org/project/geotaxonomy'),
    )),
    'description_format'  => t('name, latitude, longitude, parent, top, right, bottom, left boundaries'),
    'description_example' => t('"Paris", 48.833, 2.333, "Île-de-France"') . '<br />'
      . t('"Île-de-France", 47, 2, "France"'),
    'description_long'    => t('Only name, latitude and longitude are needed. To set a parent is recommended to get a hierarchical structure.'),
    'import_options_help' => t('"Merge" and "Replace" are very similar, because geotaxonomy items are unique: there are only one latitude, longitude, etc. Difference is for parents. With "Merge", existing parents will be kept. With "Replace", they will be removed.'),
  );
}

/**
 * Check if a vocabulary is a geotaxonomy one.
 *
 * @param $vid
 *   Vocabulary id to check.
 *
 * @return
 *   TRUE if vocabulary has specific fields, FALSE else.
 */
function taxonomy_csv_vocabulary_check_geotaxonomy($vid) {
  $vocabulary_settings = variable_get('geotaxonomy_vocabularies', array());
  return (isset($vocabulary_settings[$vid]) && ($vocabulary_settings[$vid] == 1));
}

/**
 * Add option to use geotaxonomy with vocabulary. Wrapper of geotaxonomy hook.
 *
 * @param $vocabulary
 *   Vocabulary object to update.
 *
 * @return
 *   Nothing because hook return nothing.
 */
function _taxonomy_csv_import_vocabulary_prepare_geotaxonomy($vocabulary) {
  // Needed because form_state is a by reference variable.
  $values = array('values' => array(
    'vid'                => $vocabulary->vid,
    'geotaxonomy_status' => 1,
  ));

  $result = geotaxonomy_form_vocabulary_submit(
    array(),
    $values
  );
}

/**
 * Check items of an imported line.
 */
function _taxonomy_csv_line_import_check_geotaxonomy($line, $options, $previous_items = array(), &$messages = array()) {
  $checked_items = $line;

  switch ($checked_items) { // Simulate a goto with break.
    default:
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
        break;
      }
      if (count($line) < 3) {
        $messages[] = 450; // Warning some items lacks.
        break;
      }
      if (!is_numeric($line[1]) || !is_numeric($line[2])) {
        $messages[] = 451; // Warning item are not numbers.
        break;
      }
      if (count($line) > 8) {
        $messages[] = 564; // Notice too many items.
      }
      // Complete line with NULL if needed and check numeric values.
      $line = array_pad($line, 8, NULL);
      $checked_items = $line = array_slice($line, 0, 8);
      foreach (array_slice($line, 4) as $value) {
        if (!is_numeric($value) && !is_null($value)) {
          $messages[] = 451; // Warning item are not numbers.
          break 2;
        }
      }
      foreach (array($line[0], $line[3]) as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break 2;
        }
      }
  }

  return array_values($checked_items);
}

/**
 * Import a line of items.
 *
 * @param $previous_items
 *   Specificity: tid and name arrays are indexed with unique ids, and with name
 *   when unique id is not defined.
 */
function taxonomy_csv_line_import_geotaxonomy($line, $options, $previous_items = array(), $terms_count = 0) {
  // Define default values.
  $result = array(
    'name' => array(),
    'tid'  => array(),
    'msg'  => array(),
    'terms_count' => $terms_count,
  );

  switch ($result) { // Simulate a goto with break.
    default:
      // Keep index (names and tids) with unique id.
      $result['name'] = $previous_items['name'];
      $result['tid']  = $previous_items['tid'];

      // First, import parent if needed using a recursive call and check result.
      if (!empty($line[3])) {
        $parent_unique_id = $line[3];

        // Import a new term if it is not already imported.
        if (!isset($previous_items['tid'][$parent_unique_id])) {
          $term = (object) array(
            'name' => $line[3],
            'vid'  => $options['vocabulary_id'],
          );
          // Weight is not set with the name and vid above in order to keep
          // existing one if it exists.
          ++$terms_count;
          if ($options['keep_order']) {
            $term->weight = $terms_count;
          }
          $current_result = taxonomy_csv_term_import($term, $options['existing_items']);
          $result['name'][$parent_unique_id] = $current_result['name'];
          $result['tid'][$parent_unique_id]  = $current_result['tid'];
          $result['msg'] = $current_result['msg'];
          $result['terms_count'] = $terms_count;
          if (_taxonomy_csv_worst_message($result['msg']) < TAXONOMY_CSV_PROCESS_NOTICE) {
            break;
          }
        }
        $parent_name = $result['name'][$parent_unique_id];
        $parent_tid  = $result['tid'][$parent_unique_id];
      }

      // Second, import (save or update) main term.
      $unique_id = $line[0];

      // Complete line with NULL if needed.
      $line = array_pad($line, 8, NULL);

      $term = (object) array(
        'name'         => $line[0],
        'vid'          => $options['vocabulary_id'],
        'parents'      => array(isset($parent_tid) ? $parent_tid : 0),
        'synonyms'     => array($line[1], $line[2]),
      // Geotaxonomy specific fields.
        'lat'          => $line[1],
        'lon'          => $line[2],
        'bound_top'    => $line[4],
        'bound_right'  => $line[5],
        'bound_bottom' => $line[6],
        'bound_left'   => $line[7],
      );
      // Add unique ids only if they are set.
      if (isset($result['tid'][$unique_id])) {
        $term->tid = $result['tid'][$unique_id];
      }
      if (isset($line[3])) {
        $term->parent_name = $line[3];
      }
      // Weight is not set with the name and vid above in order to keep
      // existing one if it exists.
      ++$terms_count;
      if ($options['keep_order']) {
        $term->weight = $terms_count;
      }

      // Import term then store result. No check because only one term.
      $current_result = taxonomy_csv_term_import($term, $options['existing_items'], FALSE, '', $options['import_format']);
      $result['name'][$unique_id] = $current_result['name'];
      $result['tid'][$unique_id]  = $current_result['tid'];
      $result['msg'] = array_merge($result['msg'], $current_result['msg']);
      $result['terms_count'] = $terms_count;
  }

  return $result;
}

/**
 * Save specific fields of a term and only them.
 *
 * @param $term
 *   Term object to be imported.
 *
 * @return
 *   Nothing because geotaxonomy_taxonomy() return nothing.
 */
function taxonomy_csv_term_import_geotaxonomy($term) {
  return geotaxonomy_taxonomy('update', 'term', (array) $term);
}

/**
 * Export a term.
 */
function taxonomy_csv_term_export_geotaxonomy($term, $options = array(), $duplicate_terms = array()) {
  // Define default values.
  $result = array(
    'line' => array(),
    'msg'  => array(),
  );

  // Get extra fields.
  $fields = taxonomy_csv_term_get_geotaxonomy($term->tid);

  // Doesn't manage duplicate terms because two specific fields do this.
  $result['line'] = array(
    $term->name,
    $fields['lat'],
    $fields['lon'],
    $fields['parent_name'],
    $fields['bound_top'],
    $fields['bound_right'],
    $fields['bound_bottom'],
    $fields['bound_left'],
  );

  return $result;
}

/**
 * Get full term with specific geotaxonomy fields from a term id.
 *
 * @param $tid
 *   The term id to get.
 *
 * @return
 *   Full term object with specific fields.
 */
function taxonomy_csv_term_load_geotaxonomy($tid) {
  $term = taxonomy_csv_term_load($tid);
  return ($term) ?
    taxonomy_csv_term_get_full_geotaxonomy($term) :
    FALSE;
}

/**
 * Complete a term with geotaxonomy fields.
 *
 * @param $term
 *   The term object to complete.
 *
 * @return
 *   Full term object with specific fields if any.
 */
function taxonomy_csv_term_get_full_geotaxonomy($term) {
  if ($term->tid) {
    $fields = taxonomy_csv_term_get_geotaxonomy($term->tid);

    return ($fields) ?
      (object) array_merge((array) $term, $fields) :
      $term;
  }
}

/**
 * Get specific fields of a geotaxonomy term.
 *
 * geotaxonomy_get_term($tid) doesn't return all fieds (parent_name...).
 *
 * @param $tid
 *   The term id to get.
 *
 * @return
 *   Specific fields array.
 */
function taxonomy_csv_term_get_geotaxonomy($tid) {
  return db_fetch_array(db_query("SELECT lat, lon, bound_top, bound_right, bound_bottom, bound_left,  geometry, external_id, parent_name, parent_external_id FROM {term_geo} WHERE tid = %d", $tid));
}
