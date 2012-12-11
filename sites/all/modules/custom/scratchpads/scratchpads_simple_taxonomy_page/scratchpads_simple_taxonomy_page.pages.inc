<?php

function scratchpads_simple_taxonomy_page_autocomplete($vocabulary){
  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  $tags_typed = implode('/', $args);
  // Make sure the field exists and is a taxonomy field.
  if(!$vocabulary){
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('Taxonomy vocabulary not found.');
    exit();
  }
  // The user enters a comma-separated list of tags. We only autocomplete the last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));
  $matches = array();
  if($tag_last != ''){
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
    ))->condition('t.vid', $vocabulary->vid)->condition('t.name', db_like($tag_last) . '%', 'LIKE')->range(0, 10)->execute()->fetchAllKeyed();
    $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';
    $term_matches = array();
    foreach($tags_return as $tid => $name){
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if(strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE){
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      $term_matches[$prefix . $n] = check_plain($name);
    }
  }
  drupal_json_output($term_matches);
}