<?php
// Set the internal encoding first, before we do anything else!
mb_internal_encoding('UTF-8');

/**
 * Helper function to convert an integer to a UTF-8 string.  Note, we will never
 * need to go in the reverse to this function, so this is nice and simple!
 */
function _leftandright_int_to_utf8_char($index){
  return mb_convert_encoding('&#x' . dechex($index) . ';', 'UTF-8', 'HTML-ENTITIES');
}

/**
 * Insert a term
 */
function leftandright_insert($term){
  // Get the parent term(s), get the child terms and work out where this term
  // is in that
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
    $result = $query->execute();
    $siblings = array();
    foreach($result as $sibling){
      $siblings[] = $sibling->tid;
    }
    // Load the terms.
    $siblings = taxonomy_term_load_multiple($siblings);
    $index = 0;
    foreach($siblings as $sibling){
      if(!isset($sibling->paths) || !in_array($parent_term->paths[0] . _leftandright_int_to_utf8_char($index), $sibling->paths)){
        // The path has changed for this term.  We need to pull out all of the
        // paths based on this parent, and then save.  This should leave other
        // paths that are not based on this terms parent unaffected.
        // Last char.  This will be the same for all paths for this term.
        $last_char = _leftandright_int_to_utf8_char($index);
        if(isset($sibling->paths)){
          // We already have paths. This is not a new term.
          $new_term = FALSE;
        }else{
          $sibling->paths = array();
          $new_term = TRUE;
        }
        foreach($parent_term->paths as $parent_path){
          if(!$new_term){
            $parent_str_length = mb_strlen($parent_path);
            foreach($sibling->paths as $key => $path){
              // If this path is like the parent path, then we remove it and add
              // the new one with the new ending!
              if(mb_substr($path, 0, $parent_str_length) == $parent_path){
                unset($sibling->paths[$key]);
              }
            }
          }
          $sibling->paths[] = $parent_path . $last_char;
        }
        // Save the terms new paths
        _leftandright_save_term($sibling);
      }
      // Increase the index for the next sibling.
      $index++;
    }
  }
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
      'depth' => mb_strlen($path),
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
    if(!isset($terms[$record->tid]->path)){
      $terms[$record->tid]->paths = array();
    }
    $terms[$record->tid]->paths[] = $record->path;
  }
}

/**
 * Delete a term
 */
function leftandright_delete($term){}