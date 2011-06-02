<?php

/**
 * Replacement function for taxonomy_get_tree!
 */
function taxonomy_get_tree($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $start = 0, $max_terms = 1000){
  // Only use this function if the leftandright module is installed.
  if(!module_exists('leftandright')){
    return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);
  }
  // We need to prepare the lft and rgt values for the query, and also calculate
  // the max depth if required.
  if(!$parent){
    $lft = 0;
    $rgt = (2 * current(db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
      ':vid' => $vid
    ))->fetchCol())) + 1;
    $depth_diff = 0;
  }else{
    // We have a parent set, lets get the lft and rgt
    $row = db_select('taxonomy_leftandright', 'l')->fields('l', array(
      'lft',
      'rgt',
      'depth'
    ))->condition('tid', $parent)->execute()->fetch();
    $lft = $row->lft;
    $rgt = $row->rgt;
    $depth_diff = $row->depth + 1;
  }
  $query = db_select('taxonomy_term_data', 't');
  $query->fields('t')->fields('l', array(
    'depth'
  ))->fields('h', array(
    'parent'
  ));
  $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
  $query->join('taxonomy_leftandright', 'l', 'l.tid = t.tid');
  $query->range($start, $max_terms);
  $query->addTag('translatable')->addTag('term_access');
  $query->condition('l.lft', $lft, '>')->condition('l.rgt', $rgt, '<')->condition('t.vid', $vid);
  if($max_depth){
    $query->condition('depth', $depth_diff + $max_depth, '<');
  }
  $query->orderBy('l.lft');
  $result = $query->execute();
  foreach($result as $term){
    $parents[$vid][$term->tid][] = $term->parent;
    $terms[$vid][$term->tid] = $term;
    unset($terms[$vid][$term->tid]->parent);
    $terms[$vid][$term->tid]->parents = $parents[$vid][$term->tid];
    $terms[$vid][$term->tid]->depth -= $depth_diff;
  }
  // Load full entities, if necessary.
  if($load_entities){
    $term_entities = taxonomy_term_load_multiple(array_keys($terms[$vid]));
  }
  $tree = array();
  if(isset($terms[$vid])){
    foreach($terms[$vid] as $tid => $term){
      $tree[] = $load_entities ? $term_entities[$tid] : $term;
    }
  }
  return $tree;
}

/**
 * Ensure that we always have consistent ordering of terms with the same name
 * and weight
 */
function taxonomy_get_children($tid, $vid = 0) {
  $children = &drupal_static(__FUNCTION__, array());

  if ($tid && !isset($children[$tid])) {
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $query->addField('t', 'tid');
    $query->condition('h.parent', $tid);
    if ($vid) {
      $query->condition('t.vid', $vid);
    }
    $query->addTag('term_access');
    $query->orderBy('t.weight');
    $query->orderBy('t.name');
    // Following added line is the only change to this function
    $query->orderBy('t.tid', 'DESC');
    $tids = $query->execute()->fetchCol();
    $children[$tid] = taxonomy_term_load_multiple($tids);
  }

  return isset($children[$tid]) ? $children[$tid] : array();
}