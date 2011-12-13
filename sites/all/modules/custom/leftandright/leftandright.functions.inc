<?php
// Set the internal encoding first, before we do anything else!
mb_internal_encoding('UTF-8');

/**
 * Helper function to convert an integer to a UTF-8 string.  Note, we will never
 * need to go in the reverse to this function, so this is nice and simple!
 */
function _leftandright_int_to_utf8_char($index){
  return $index;
  return mb_convert_encoding('&#x' . dechex($index) . ';', 'UTF-8', 'HTML-ENTITIES');
}

/**
 * Helper for the insert and update functions.
 */
function _leftandright_get_new_paths($term){
  $new_paths = array();
  if(!isset($term->parent)){
    $term->parent = array(
      0
    );
  }
  foreach($term->parent as $parent_tid){
    // We have a parent term, we load it to get its path, and also load the
    // siblings to work out where we need to be (and also to punch a hole if
    // required).
    if(!$parent_tid){
      $parent_term = new stdClass();
      $parent_term->paths = array(
        ''
      );
    }else{
      $parent_term = taxonomy_term_load($parent_tid);
    }
    $query = db_select('taxonomy_term_data', 't');
    $query->fields('t', array(
      'tid'
    ));
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $query->condition('parent', $parent_tid);
    $query->condition('vid', $term->vid);
    $query->orderBy('weight');
    $query->orderBy('name');
    $result = $query->execute();
    $siblings = array();
    foreach($result as $sibling){
      $siblings[] = $sibling->tid;
    }
    // Load the terms.
    $paths_to_change = array();
    $siblings = taxonomy_term_load_multiple($siblings);
    $index = 0;
    foreach($siblings as $sibling){
      $last_char = _leftandright_int_to_utf8_char($index);
      foreach($parent_term->paths as $parent_path){
        if($sibling->tid == $term->tid){
          // Add this path to the $term
          $new_paths[] = $parent_path . $last_char;
        }else{
          // Add this path to the sibling
          if(!in_array($parent_path . $last_char, $sibling->paths)){
            // The path has changed. We need to find what it was.
            $parent_path_length = mb_strlen($parent_path);
            foreach($sibling->paths as $sibling_path){
              if(mb_substr($sibling_path, 0, $parent_path_length) == $parent_path){
                // Change this path in the database, and all other paths that 
                // start like it to the new path.
                $paths_to_change[] = array(
                  'old' => $sibling_path,
                  'new' => $parent_path . $last_char,
                  'vid' => $term->vid
                );
              }
            }
          }
        }
      }
      // Increase the index for the next sibling.
      $index++;
    }
  }
  _leftandright_move_paths($paths_to_change);
  return $new_paths;
}

/**
 * Insert a term
 */
function leftandright_insert($term){
  $term->paths = _leftandright_get_new_paths($term);
  _leftandright_save_term($term);
}

/**
 * Update a term.
 */
function leftandright_update($term){
  $new_paths = _leftandright_get_new_paths($term);
  // Work out how many we need to change/add/remove.
  $paths_to_add = array();
  $paths_to_remove = array();
  foreach($new_paths as $new_path){
    if(!in_array($new_path, $term->paths)){
      $paths_to_add[] = $new_path;
    }
  }
  foreach($term->paths as $old_path){
    if(!in_array($old_path, $new_paths)){
      $paths_to_remove[] = $old_path;
    }
  }
  $paths_to_change = array();
  while($old_path = next($paths_to_remove)){
    $new_path = next($paths_to_add);
    $paths_to_change[] = array(
      'old' => $old_path,
      'new' => $new_path,
      'tid' => $term->tid,
      'vid' => $term->vid
    );
  }
  while($new_path = next($paths_to_add)){
    $paths_to_change[] = array(
      'old' => FALSE,
      'new' => $new_path,
      'tid' => $term->tid,
      'vid' => $term->vid
    );
  }
  _leftandright_move_paths($paths_to_change);
}

/**
 * Helper function to alter a path.
 */
function _leftandright_move_paths($paths){
  foreach($paths as $path){
    if(isset($path['old']) && $path['old'] && isset($path['new']) && $path['new']){
      $temp_table = db_query_temporary('SELECT tid, vid, depth, CONCAT(:new_path, SUBSTRING(path, :old_path_length)) as path FROM {taxonomy_path} WHERE vid = :vid AND path LIKE :old_path', array(
        ':new_path' => $path['new'],
        ':old_path' => $path['old'],
        ':vid' => $path['vid'],
        ':old_path_length' => mb_strlen($path['old']) + 1
      ));
      // Delete the data from the 
    }
  }
  // We create a temporary table, inserting the new data into
  if(count($paths_to_add) == count($paths_to_remove)){
    // Spot on - easy.
    for($i = 0; $i < count($paths_to_add); $i++){
      _leftandright_move_paths($term->vid, array(
        $paths_to_remove[$i] => $paths_to_add[$i]
      ));
    }
  }elseif(count($paths_to_add) > count($paths_to_remove)){
    // Extra to add, a little tricky
    // Create a temporary table first with the data to copy.  We'll use the first
    // path from the term.
    db_query('CREATE TEMPORARY TABLE leftandright_temp AS SELECT tid, vid, depth, SUBSTR(path, :path_start) AS path FROM {taxonomy_path} WHERE vid = :vid AND path LIKE :path', array(
      ':path_start' => mb_strlen($term->paths[0]) + 1,
      ':vid' => $term->vid,
      ':path' => $term->paths[0]
    ));
    $i = 0;
    for(; $i < count($paths_to_remove); $i++){
      _leftandright_move_path($term->vid, $paths_to_remove[$i], $paths_to_add[$i]);
    }
    // Add the extra
    for(; $i < count($paths_to_add); $i++){
      db_query('INSERT INTO {taxonomy_path} SELECT tid, vid, depth, CONCAT(:new_path, path) FROM leftandright_temp', array(
        ':new_path' => $paths_to_add[$i]
      ));
    }
    // Drop the table once we're done with it.
    db_drop_table('leftandright_temp');
  }else{
    // Extra to remove - easy!
    $i = 0;
    for(; $i < count($paths_to_add); $i++){
      _leftandright_move_path($term->vid, $paths_to_remove[$i], $paths_to_add[$i]);
    }
    // Remove the extra
    for(; $i < count($paths_to_remove); $i++){
      db_delete('taxonomy_path')->condition('vid', $term->vid)->condition('path', $paths_to_remove[$i] . '%', 'LIKE')->execute();
    }
  }
  db_query('UPDATE {taxonomy_path} SET path = CONCAT(:new_path, SUBSTR(path, :path_start)) WHERE path LIKE :old_path AND vid = :vid', array(
    ':vid' => $vid,
    ':new_path' => $new_path,
    ':path_start' => mb_strlen($old_path) + 1,
    ':old_path' => $old_path . '%'
  ));
}

/**
 * Helper function to save a term and its paths
 */
function _leftandright_save_term($term){
  // First delete the term's paths
  db_delete('taxonomy_path')->condition('tid', $term->tid)->execute();
  $query = db_insert('taxonomy_path')->fields(array(
    'tid',
    'vid',
    'depth',
    'path'
  ));
  foreach($term->paths as $path){
    $query->values(array(
      'tid' => $term->tid,
      'vid' => $term->vid,
      'depth' => mb_strlen($path) - 1,
      'path' => $path
    ));
  }
  $query->execute();
}

/**
 * Load terms
 */
function leftandright_load($terms){
  $result = db_query('SELECT tid, path FROM {taxonomy_path} WHERE tid IN (:tids)', array(
    ':tids' => array_keys($terms)
  ));
  foreach($result as $record){
    if(!isset($terms[$record->tid]->paths)){
      $terms[$record->tid]->paths = array();
    }
    $terms[$record->tid]->paths[] = $record->path;
  }
  return $terms;
}

/**
 * Delete a term
 */
function leftandright_delete($term){
  // Get this term's paths
  if(isset($term->paths)){
    foreach($term->paths as $path){
      db_delete('taxonomy_path')->condition('vid', $term->vid)->condition('path', "$path%", 'LIKE')->execute();
    }
  }
}