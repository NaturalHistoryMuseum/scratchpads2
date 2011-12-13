<?php

/**
 * Replacement function for taxonomy_get_tree!
 */
function taxonomy_get_tree($vid, $parent = 0, $max_depth = 256, $load_entities = FALSE, $start = 0, $max_terms = 1000){
  //return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);
  // Here we check for a parent.  If it is null, we simply return an empty array.
  // Why that is, I DO NOT KNOW!
  if($parent === NULL){return array();}
  // Only use this function if the leftandright module is installed.
  if(!module_exists('leftandright')){return taxonomy_get_tree_original($vid, $parent, $max_depth, $load_entities);}
  // If we have been passed a parent, we need to handle it!
  $path = '';
  $min_depth = 0;
  if($parent){
    $result = array_pop(db_select('taxonomy_path', 'p')->fields('p', array(
      'path',
      'depth'
    ))->condition('vid', $vid)->condition('tid', $parent)->execute()->fetchAll());
    $min_depth = $result->depth + 1;
    $path = $result->path;
    if($max_depth){
      $max_depth += $min_depth;
    }
  }
  if($load_entities){
    // Nice and easy, just get the tids and the depths from the taxonomy_path
    // table, and then do a load on each term.  Note, we limit this query to the
    // max_terms variable for safety.
    // Need to do our own query here, as the Drupal PDO stuff does not support 
    // binary sort.  This may therefore break other database implementations.
    $results = db_query_range("SELECT tid FROM {taxonomy_path} WHERE vid = :vid AND path LIKE :path AND depth >= :min_depth AND depth < :max_depth ORDER BY BINARY path", 0, $max_terms, array(
      ':vid' => $vid,
      ':path' => "$path%",
      ':min_depth' => $min_depth,
      ':max_depth' => $max_depth
    ));
    $tree = array();
    foreach($results as $row){
      $tree[] = $row->tid;
    }
    $terms = taxonomy_term_load_multiple($tree);
    foreach($tree as $key => $tid){
      $tree[$key] = $terms[$tid];
    }
  }else{
    // Need to do our own query here, as the Drupal PDO stuff does not support 
    // binary sort.  This may therefore break other database implementations.
    $results = db_query_range("SELECT t.*, parent, depth FROM {taxonomy_path} p INNER JOIN {taxonomy_term_data} t ON t.tid = p.tid INNER JOIN {taxonomy_term_hierarchy} h ON h.tid = t.tid WHERE t.vid = :vid AND path LIKE :path AND depth >= :min_depth AND depth < :max_depth ORDER BY BINARY path", 0, $max_terms, array(
      ':vid' => $vid,
      ':path' => "$path%",
      ':min_depth' => $min_depth,
      ':max_depth' => $max_depth
    ));
    $tree = array();
    foreach($results as $row){
      if(count($tree) && $row->tid == $tree[count($tree) - 1]->tid){
        $tree[count($tree) - 1]->parents[] = $row->parent;
      }else{
        $row->parents = array(
          $row->parent
        );
        unset($row->parent);
        $tree[] = $row;
      }
    }
  }
  return $tree;
}

/*

$tree =taxonomy_get_tree(3, 112234, NULL, TRUE);
dpm($tree);

$tree =taxonomy_get_tree(3);
dpm($tree);


$tree =taxonomy_get_tree(3, 0, NULL, TRUE);
dpm($tree);


 */
/**
 * Ensure that we always have consistent ordering of terms with the same name
 * and weight
 */
function taxonomy_get_children($tid, $vid = 0){
  $children = &drupal_static(__FUNCTION__, array());
  if($tid && !isset($children[$tid])){
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $query->addField('t', 'tid');
    $query->condition('h.parent', $tid);
    if($vid){
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