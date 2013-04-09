<?php

function scratchpads_search_block_autocomplete($search = ''){
  $matches = array();
  if($search != ''){
    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');
    // Select rows that match by term name.
    $tags_return = $query->fields('t', array(
      'tid',
      'name'
    ))->condition('t.name', '%' . db_like($search) . '%', 'LIKE')->range(0, 10)->execute()->fetchAllKeyed();
    $term_matches = array();
    foreach($tags_return as $tid => $name){
      $parents = taxonomy_get_parents_all($tid);
      $parents = array_reverse($parents, FALSE);
      foreach($parents as $key => $parent){
        $parents[$key] = $parent->name;
      }
      $name = implode(" » ", $parents);
      $term_matches[$parent->name . ' [' . $tid . ']'] = check_plain($name);
    }
  }
  drupal_json_output($term_matches);
}