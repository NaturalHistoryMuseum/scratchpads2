<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include    routines for CSV parsing and taxonomy/term creation.
 *
 * This is an alternate CSV format, taking each term on a new line with its
 * ancestors in previous columns.
 *
 * drinks
 * drinks, beer
 * drinks, wine
 * drinks, wine, red wine
 * drinks, wine, white wine
 * drinks, milk
 *
 * ...etc, It's very limited (and wordy), but also about as obvious as possible.
 *
 * This format was used by google base for its merchant product taxonomy.
 *
 * Much of the code here is just reporting. The meat could be done in less than
 * a page.
 *
 * @author Dan Morrison (dman) http://coders.co.nz/
 */

module_load_include('inc', 'taxonomy_xml', 'csv_format');

/**
 * Scan the input CSV file and create a taxonomy structure out of it.
 *
 * See the sample files for the expected format of the CSV
 *
 */
function taxonomy_xml_csvancestry_parse(&$data, $vid) {
  $output = '';

  // Unset the global variables before we use them:
  unset($GLOBALS['$_taxonomy_xml_terms']);
  $terms         = array();
  $new_terms     = array();
  $skipped_terms = array();
  $vocabulary    = array();
  if ($vid) {
    $vocabulary = taxonomy_vocabulary_load($vid);
  }
  else {
    drupal_set_message(t('No vocab to import into. Either make one or choose one.'));
    return;
  }

  $rows = explode("\n", $data);
  #drupal_set_message(t('%rowcount rows of data', array('%rowcount' => count($rows))));

  // Unlike all other formats, each line and term is complete.
  // It's required that parents are created before children.
  // We don't need to mess around with parsing, just create the term.
  $terms = array();

  // Prepare a batch config
  $batch_settings = array(
    'title' => t('Processing all queued import requests.'),
    'file' => drupal_get_path('module', 'taxonomy_xml') . '/csvancestry_format.inc',
    'operations' => array(),
    'finished' => 'cvsancestry_import_finished',
  );

  foreach ($rows as $row) {
    $row_data = csv_string_to_array($row);
    // Queue the import of this line
    $batch_settings['operations'][] = array('cvsancestry_import_row', array($vid, $row_data));
  }
  batch_set($batch_settings);

  drupal_set_message(t('Queued %rowcount rows of data', array('%rowcount' => count($rows))));
  return "OK, processing is being done in batch...";
}

/**
 * Summarize the results.
 */
function cvsancestry_import_finished($success, $results, $operations) {
  $results = array_merge(array('new_terms' => array(), 'old_terms' => array()), $results);

  if ($success) {
    $message = t('%new new and %old old terms processed', array('%new' => count($results['new_terms']), '%old' => count($results['old_terms'])));
  }
  else {
    $message = t('Failed ', array());
  }
  drupal_set_message($message);
}

function cvsancestry_import_row($vid, $row_data, &$context) {
  // As we may be calling back via batch, ensure the functions are loaded
  module_load_include('inc', 'taxonomy_xml', 'taxonomy_xml.process');
  
  // $_taxonomy_xml_terms is semi-persistant.
  // We expect to be called atomically, but if the term has been cached recently,
  // keep a note of it in the meantime.
  global $_taxonomy_xml_terms;

  while (!empty($row_data) && empty($term_name)) {
    $term_name = array_pop($row_data);
  }
  if (empty($term_name)) {
    // blank line;
    return;
  }
  $term = isset($_taxonomy_xml_terms[$term_name]) ? $_taxonomy_xml_terms[$term_name] : NULL;
  if (! $term) {
    // Start by looking for it
    $term = _taxonomy_xml_get_term_placeholder($term_name, $vid);
    if (empty($term->tid)) {
      $context['results']['new_terms'][] = $term_name;
    }
    else {
      $context['results']['old_terms'][] = $term_name;
    }
    $_taxonomy_xml_terms[$term_name] = $term;
  }

  // Attach parent, if it exists
  $parent_name = array_pop($row_data);
  if (! empty($parent_name)) {
    csvancestry_set_parent($term, $parent_name);
  }

  // And save it.
  $term_data = (array) $term;
  taxonomy_term_save($term /* TODO Term object replaces array $term_data */);
  // Re-retrieve it, so we know the tid.
  $retrieved_term = taxonomy_xml_get_term_by_name_from_vocab($term_name, $vid);
  $_taxonomy_xml_terms[$term_name] = $term;
}

/**
 * Set the parent of this term.
 * $term must be a valid term. parent term may be invented on the fly.
 */
function csvancestry_set_parent($term, $parent_name) {
  if ($parent_name == $term->name) {
    drupal_set_message(t("Not setting %name as a child of itself", array('%name' => $term->name)));
    continue;
  }

  $parent_term = _taxonomy_xml_get_term_placeholder($parent_name, $term->vid);
  if (empty($parent_term->tid)) {
    $parent_data = (array) $parent_term;
    taxonomy_term_save($term /* TODO Term object replaces array $parent_data */);
    // Retrieve the term object to get hold of the tid if needed
    $parent_term = taxonomy_xml_get_term_by_name_from_vocab($parent_name, $term->vid);
  }

  if ($parent_term && isset($parent_term->tid) ) {
    drupal_set_message(t("!name # %tid is a child of !parent # %ptid ", array('!name' => $term->name, '%tid' => $term->tid, '!parent' => l($parent_term->name, 'taxonomy/term/' . $parent_term->tid), '%ptid' => $parent_term->tid)));
    $term->parent[$parent_term->tid] = $parent_term->tid;
  }
}
