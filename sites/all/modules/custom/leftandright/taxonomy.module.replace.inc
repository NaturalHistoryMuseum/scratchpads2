<?php

/**
 * Replacement function for taxonomy_get_tree!
 */
function taxonomy_get_tree($vid, $parent = 0, $max_depth = 256, $load_entities = FALSE, $start = 0, $max_terms = 1000){
  if(!$parent && array_pop(db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
    ':vid' => $vid
  ))->fetchCol()) > $max_terms){
    watchdog('taxonomy', 'Attempted to get a large tree.', WATCHDOG_NOTICE);
    return array();
  }
  return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);
}