<?php

/**
 * @file
 * 
 * Description of the tinytax.callbacks.inc file.
 */
function tinytax_js($term){
  return theme('tinytax_branch', array(
    'tid' => $term->tid,
    'vid' => $term->vid
  ));
}

/**
 * Callback for when a term is being searched for.  This should open out the
 * tree for us.
 */
function tinytax_search_js($vocabulary, $tags_typed = ''){
  // lets get the tid this term refers to
  $tags_typed = drupal_explode_tags($tags_typed);
  dpm($tags_typed);
  $open_tids = array();
  foreach($tags_typed as $tag){
    $terms = taxonomy_get_term_by_name($tag);
    foreach($terms as $term){
      if($term->vid == $vocabulary->vid){
        // Get the parents of this tid, and add them all to the open_tids array.
        $parents = taxonomy_get_parents_all($term->tid);
        array_shift($parents);
        foreach($parents as $parent){
          $open_tids[] = $parent->tid;
        }
      }
    }
  }
  return theme('tinytax_branch', array(
    'tid' => 0,
    'vid' => $vocabulary->vid,
    'open_tids' => $open_tids
  ));
}

function tinytax_autocomplete($vocabulary, $tags_typed = ''){
  // Following is largely copied from the taxonomy module, but without the
  // fields stuff.
  // The user enters a comma-separated list of tags. We only autocomplete the last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));
  $matches = array();
  if($tag_last != ''){
    // Part of the criteria for the query come from the field's own settings.
    $vids = array(
      $vocabulary->vid
    );
    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');
    // Do not select already entered terms.
    if(!empty($tags_typed)){
      $query->condition('t.name', $tags_typed, 'NOT IN');
    }
    // Select rows that match by term name.
    $tags_return = $query->fields('t', array(
      'tid',
      'name'
    ))->condition('t.vid', $vids)->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE')->range(0, 10)->execute()->fetchAllKeyed();
    $prefix = count($tags_typed) ? implode(', ', $tags_typed) . ', ' : '';
    $term_matches = array();
    foreach($tags_return as $tid => $name){
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if(strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE){
        $n = '"' . str_replace('"', '""', $name) . '"';
      }else{
        $term_matches[$prefix . $n] = check_plain($name);
      }
    }
  }
  drupal_json_output($term_matches);
}