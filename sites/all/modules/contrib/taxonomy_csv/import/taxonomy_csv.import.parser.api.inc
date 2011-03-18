<?php
// $Id: taxonomy_csv.import.parser.api.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Check a line of imported terms (duplicates, format...).
 */

/**
 * Helper function to validate an imported line.
 *
 * @param $line
 *   Array of items from a cleaned line.
 * @param $options
 *   Array of available options. See taxonomy_csv_line_import_process.
 * @param $previous_items
 *   (Optional) Cleaned and checked previous imported line names and tids array.
 *   Needed with some contents as one term array structure or Taxonomy manager.
 *   Specificities:
 *   - def_links:
 *       'name' and 'tid' arrays contain sub-arrays 'vocabulary' and 'term' with
 *       imported identifiant as key and name or tid/vid as value in order to
 *       allow duplicate names import and use of number identifiants.
 * @param &$messages
 *   (Optional) By reference array of messages codes to be returned.
 *
 * @return
 *   Array of checked items of imported line.
 */
function _taxonomy_csv_line_import_check($line, $options, $previous_items = array(), &$messages = array()) {
  $checked_items = array();

  // Simplify used options.
  $existing_items = &$options['existing_items'];

  // No input check because line and previous line are already checked.
  // @todo A php callback function may be used to simplify checking.
  switch ($options['import_format']) {
    case TAXONOMY_CSV_FORMAT_ALONE_TERMS:
      // Checks empty first column.
      // Example: ", Item 1, Item 2"
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
        break;
      }
      if (count($line) > 1) {
        $messages[] = 501; // Notice too many items.
      }
      $checked_items = array($line[0]);
      if (drupal_strlen($checked_items[0]) > 255) {
        $messages[] = 454; // Warning too long.
        break;
      }
      break;

    case TAXONOMY_CSV_FORMAT_DEFINITION_LINKS:
      if (empty($line[0])) {
        $messages[] = 464; // Warning no name.
        break;
      }
      // Complete $line for easier check.
      $line = array_pad($line, 9, '');
      // Example: "Term, 1, voc, Description, 0, No,..."
      foreach (array(4, 5, 6, 7, 8) as $key) {
        if (!empty($line[$key]) && !is_numeric($line[$key])) {
          $messages[] = 451; // Warning not a number.
          break 2;
        }
      }
      // Check count of items.
      $links_count = $line[5] + $line[6] + $line[7] + $line[8];
      if (count($line) < (9 + $links_count)) {
        $messages[] = 430; // Warning some empty items.
        break;
      }
      // Check vocabularies of related terms. If empty, use of main term vid.
      if (count($line) < (9 + $links_count + $line[8])) {
        $messages[] = 639; // Info some empty vocabulary related items.
        $line = array_pad($line, 9 + $links_count + $line[8], '');
      }
      elseif (count($line) > (9 + $links_count + $line[8])) {
        $messages[] = 434; // Warning too many items.
        break;
      }
      // Example: "Term, 1, voc, Description, 0, 1, 2, 3, 4, item, , item 2..."
      $imported_items = array_map('strval', array_slice($line, 9, $links_count));
      foreach ($imported_items as $key => $value) {
        if (empty($value)) {
          $messages[] = 430; // Warning some empty items.
          break 2;
        }
      }
      // Check and remove duplicate synonyms.
      $synonyms = array_slice($line, 9, $line[5]);
      $unique_synonyms = array_unique($synonyms);
      if (count($unique_synonyms) < (count($synonyms))) {
        $messages[] = 535; // Notice duplicate items (removed).
        $line[5] = count($unique_synonyms);
        $line = array_merge(array_slice($line, 0, 9), $unique_synonyms, array_slice($line, 9 + count($synonyms)));
      }
      // Check and remove duplicate parents.
      $parents = array_slice($line, 9 + $line[5], $line[6]);
      $unique_parents = array_unique($parents);
      if (count($unique_parents) < (count($parents))) {
        $messages[] = 536; // Notice duplicate items (removed).
        $line[6] = count($unique_parents);
        $line = array_merge(array_slice($line, 0, 9 + $line[5]), $unique_parents, array_slice($line, 9 + $line[5] + count($parents)));
      }
      // Check and remove duplicate children.
      $children = array_slice($line, 9 + $line[5] + $line[6], $line[7]);
      $unique_children = array_unique($children);
      if (count($unique_children) < (count($children))) {
        $messages[] = 537; // Notice duplicate children (removed).
        $line[7] = count($unique_children);
        $line = array_merge(array_slice($line, 0, 9 + $line[5] + $line[6]), $unique_children, array_slice($line, 9 + $line[5] + $line[6] + count($children)));
      }
      // Check and remove duplicate related.
      $related = array_slice($line, 9 + $line[5] + $line[6] + $line[7], $line[8]);
      $unique_related = array_unique($related);
      if (count($unique_related) < (count($related))) {
        $messages[] = 538; // Notice duplicate related (removed).
        $line[8] = count($unique_related);
        $line = array_merge(array_slice($line, 0, 9 + $line[5]) + $line[6] + $line[7], $unique_related, array_slice($line, 9 + $line[5] + $line[6] + $line[7] + count($related)));
      }
      // Example: "Term, 1, voc, Description, 0, 1, 2, 3, 4, item 1, item 1..."
      $imported_items = array_slice($line, 9, $line[5] + $line[6] + $line[7] + $line[8]);
      $imported_unique_items = array_unique($imported_items);
      if (count($imported_unique_items) < (count($imported_items))) {
        $messages[] = 532; // Notice duplicate items (allowed).
      }
      // Example: "Name, 1, voc, Description, 0, 1, 2, 3, 4, Name, item 1..."
      if (in_array($line[0], $imported_unique_items)) {
        $messages[] = 433; // Warning name and some items are same.
        break;
      }
      $checked_items = $line;
      foreach (array_merge(array_slice($checked_items, 0, 3), array_slice($checked_items, 9)) as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
      break;

    case TAXONOMY_CSV_FORMAT_FLAT:
      if (count($line) == 0) {
        $messages[] = 491; // Warning no item.
        break;
      }
      $checked_items = array_unique(array_filter($line));
      if (count($checked_items) < count($line)) {
        $messages[] = 531; // Notice duplicates, which are removed.
      }
      foreach ($checked_items as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
      break;

    case TAXONOMY_CSV_FORMAT_TREE_STRUCTURE:
    case TAXONOMY_CSV_FORMAT_POLYHIERARCHY:
      // Check last empty column before first item with previous imported items.
      for ($first_non_empty = 0; ($first_non_empty < count($line)) && (empty($line[$first_non_empty])); $first_non_empty++) {
      }
      // Example: Previous line("Term 1,Item 2") ; Current line(",,,Item4")
      if ($first_non_empty && (!isset($previous_items['name'][$first_non_empty - 1]))) {
        $messages[] = 410; // Warning impossible to get parent.
        break;
      }
      // Example: Previous line("Term 1,Item 2") ; Current line(",,,Item4")
      // "0" value are lost, but that is not important for a taxonomy.
      $imported_items = array_filter(array_slice($line, $first_non_empty));
      if (count($imported_items) == 0) {
        $messages[] = 491; // Warning no item.
        break;
      }
      if (count($imported_items) < (count($line) - $first_non_empty)) {
        $messages[] = 510; // Notice empty items.
      }
      if (count(array_unique($imported_items)) < count($imported_items)) {
        $messages[] = 632; // Info duplicates (not removed).
      }
      if ($first_non_empty == 0) {
        $checked_items = $imported_items;
      }
      else {
        $checked_items = array_merge(array_fill(0, $first_non_empty, ''), $imported_items);
      }
      foreach ($checked_items as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
      break;

    case TAXONOMY_CSV_FORMAT_PARENTS:
    case TAXONOMY_CSV_FORMAT_CHILDREN:
    case TAXONOMY_CSV_FORMAT_RELATIONS:
    case TAXONOMY_CSV_FORMAT_SYNONYMS:
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
        break;
      }
      // Example: "Term,Item 1,,Item 2"
      $imported_items = array_map('strval', array_slice($line, 1));
      foreach ($imported_items as $key => $value) {
        if ($value == '') {
          unset($imported_items[$key]);
          $messages[] = 530; // Notice some empty items.
        }
      }
      // Example: "Term,Item 1,Item 2,Item 1"
      $imported_unique_items = array_unique($imported_items);
      if (count($imported_unique_items) < (count($imported_items))) {
        $messages[] = 531; // Notice duplicates, which are removed.
      }
      // Example: "Term,Item 1,Term,Item 2"
      $checked_items = array_unique(array_merge(array($line[0]), $imported_unique_items));
      if (count($checked_items) <= count($imported_unique_items)) {
        $messages[] = 533; // Notice name and some items are same, so removed.
      }
      // Example: "Term"
      if ((count($checked_items) == 1)
          && ($existing_items == TAXONOMY_CSV_EXISTING_UPDATE_REPLACE)) {
        $messages[] = 580; // Notice remove items.
      }
      foreach ($checked_items as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
      break;

    case TAXONOMY_CSV_FORMAT_DEFINITIONS:
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
      }
      elseif (count($line) == 1) {
        $messages[] = 510; // Notice empty items.
        if ($existing_items == TAXONOMY_CSV_EXISTING_UPDATE_REPLACE) {
          $messages[] = 580; // Notice remove items.
        }
        $checked_items = array($line[0], 0);
      }
      elseif (!is_numeric($line[1]) && !is_int($line[1]) && $line[1]) {
        $messages[] = 450; // Warning weight is not a number.
      }
      elseif (count($line) <= 3) {
        $checked_items = $line;
      }
      else {
        // Example: "Term,Item 1,,Item 2"
        // "0" value is lost, but this is not important for a taxonomy.
        $imported_items = array_filter(array_slice($line, 3));
        if (count($imported_items) < (count($line) - 3)) {
          $messages[] = 530; // Notice some empty items.
        }
        // Example: "Term,Item 1,Item 2,Item 1"
        $imported_unique_items = array_unique($imported_items);
        if (count($imported_unique_items) < (count($imported_items))) {
          $messages[] = 531; // Notice duplicates, which are removed.
        }
        // Example: "Term,Item 1,Term,Item 2"
        $temp_checked_items = array_unique(array_merge(array($line[0]), $imported_unique_items));
        if (count($temp_checked_items) <= count($imported_unique_items)) {
          $messages[] = 533; // Notice name and some items are same, so removed.
        }
        $checked_items = array_merge(array_slice($line, 0, 3), array_slice($temp_checked_items, 1));
      }
      foreach (array_merge(array_slice($checked_items, 0, 1), array_slice($checked_items, 3)) as $name) {
        if (drupal_strlen($name) > 255) {
          $messages[] = 454; // Warning too long.
          break;
        }
      }
      break;

    case TAXONOMY_CSV_FORMAT_DESCRIPTIONS:
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
        break;
      }
      if ((count($line) == 1)
          && ($existing_items == TAXONOMY_CSV_EXISTING_UPDATE_REPLACE)) {
        $messages[] = 580; // Notice remove items.
        $checked_items = array($line[0], '');
      }
      else {
        if (count($line) > 2) {
          $messages[] = 541; // Notice too many items.
        }
        $checked_items = array($line[0], $line[1]);
      }
      if (drupal_strlen($checked_items[0]) > 255) {
        $messages[] = 454; // Warning too long.
        break;
      }
      break;

    case TAXONOMY_CSV_FORMAT_WEIGHTS:
      if (empty($line[0])) {
        $messages[] = 480; // Warning no first column.
        break;
      }
      if ((count($line) > 1) && !is_numeric($line[1]) && !is_int($line[1]) && $line[1]) {
        $messages[] = 450; // Warning weight is not a number.
        break;
      }
      if ((count($line) == 1) && ($existing_items == TAXONOMY_CSV_EXISTING_UPDATE_REPLACE)) {
        $messages[] = 580; // Notice remove items.
        $checked_items = array($line[0], 0);
      }
      else {
        if (count($line) > 2) {
          $messages[] = 541; // Notice too many items.
        }
        $checked_items = array($line[0], intval($line[1]));
      }
      if (drupal_strlen($checked_items[0]) > 255) {
        $messages[] = 454; // Warning too long.
        break;
      }
      break;

    default:
      // Check external formats. Use it only if it works.
      $funcname = "_taxonomy_csv_line_import_check_{$options['import_format']}";
      if (taxonomy_csv_format_check($options['import_format'], $funcname)) {
        $checked_items = $funcname($line, $options, $previous_items, $messages);
      }
      else {
        $messages[] = 306; // Error unknown source content.
      }
  }

  return array_values($checked_items);
}
