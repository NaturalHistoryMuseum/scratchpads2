<?php

/**
 * @file
 */
/**
 * tui_form_hierarchy - Function to do stuff!
 */
function tui_hierarchy_page($vocabulary, $tids = FALSE, $highlight_tids = FALSE){
  $tids = _tui_get_tids_from_string($vocabulary, $highlight_tids ? $highlight_tids . "," . $tids : $tids);
  $highlight_tids = _tui_get_tids_from_string($vocabulary, $highlight_tids, FALSE);
  return theme('tui_page', array(
    'vocabulary' => $vocabulary,
    'tids' => $tids,
    'highlight_tids' => $highlight_tids
  ));
}

/**
 * Get array of tids from comma delimited string
 */
function _tui_get_tids_from_string($vocabulary, $string, $parents = TRUE){
  $tids = array();
  if($string){
    // Sanity check to ensure the tids are from the vocabulary.
    $tids = explode(",", $string);
    $results = db_select('taxonomy_term_data', 't')->fields('t', array(
      'tid'
    ))->condition('tid', $tids, 'IN')->condition('vid', $vocabulary->vid)->execute();
    $tids = array();
    foreach($results as $row){
      if($parents){
        // Get the parents of each term - that then gives us every term that
        // needs to be visible.
        $parents = taxonomy_get_parents_all($row->tid);
        // Unset the first one, as that doesn't actually need to be opened (by
        // opening the parent, we can see the child).
        unset($parents[0]);
        foreach($parents as $parent){
          $tids[] = $parent->tid;
        }
      }else{
        $tids[] = $row->tid;
      }
    }
  }
  return $tids;
}

/**
 * Autocomplete callback
 */
/**
 * Helper function for autocompletion
 */
function tui_autocomplete($vid, $search = ''){
  $query = db_select('taxonomy_term_data', 't');
  $query->addTag('translatable');
  $query->addTag('term_access');
  // Select rows that match by term name.
  $tags_return = $query->fields('t', array(
    'tid',
    'name'
  ))->condition('t.vid', $vid)->condition('t.name', db_like($search) . '%', 'LIKE')->range(0, 10)->execute()->fetchAllKeyed();
  $term_matches = array();
  foreach($tags_return as $tid => $name){
    $n = $name;
    // Term names containing commas or quotes must be wrapped in quotes.
    if(strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE){
      $n = '"' . str_replace('"', '""', $name) . '"';
    }
    $term_matches[$n] = check_plain($name);
  }
  drupal_json_output($term_matches);
}

/**
 * JSON Callback to get branches
 */
function tui_hierarchy_page_callback($vocabulary, $term){
  echo json_encode(theme('tui_branch', array(
    'vocabulary' => $vocabulary,
    'tid' => $term->tid
  )));
  exit();
}