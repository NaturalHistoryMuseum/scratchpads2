<?php

/**
 * Get the previous term to one being inserted or updated.  This is a simple
 * helper function that makes calculating left and right values a little easier.
 */
function leftandright_previous_term($term){
  // Here we need to get the siblings of a term.  We'll get all the children
  // of the parent of this term, ordered correctly, and parse through.  If the
  // first term is the term passed, then we return the parent.
  if(current($term->parent)){
    // We have a parent set, we'll use taxonomy_get_children
    $children = taxonomy_get_children(current($term->parent), $term->vid);
    $previous_term = current($term->parent) ? taxonomy_term_load(current($term->parent)) : FALSE;
  }else{
    $children = taxonomy_get_tree($term->vid, 0, 1);
    $previous_term = FALSE;
  }
  foreach($children as $child){
    if($child->tid == $term->tid){return $previous_term;}
    $previous_term = $child;
  }
}

/**
 * Get next sibling
 */
function leftandright_get_next_sibling($term){
  // Get the parent from the taxonomy_term_hierarchy table first.
  $parent = db_select('taxonomy_term_hierarchy', 't')->fields('t', array(
    'parent'
  ))->condition('tid', $term->tid)->execute()->fetchField();
  $query = db_select('taxonomy_term_data', 't')->fields('t');
  $query->join('taxonomy_term_hierarchy', 'h', 't.tid = h.tid');
  $results = $query->condition('vid', $term->vid)->condition('parent', $parent)->orderBy('t.weight')->orderBy('t.name')->orderBy('t.tid', 'DESC')->fields('t')->execute();
  $found_me = FALSE;
  foreach($results as $sibling){
    if($found_me){return $sibling;}
    if($sibling->tid == $term->tid){
      $found_me = TRUE;
    }
  }
  return FALSE;
}

/**
 * Save the term
 */
function leftandright_save_term($term){
  // Get the current left and right for this term - if they're not set, then
  // saving is simply punching a hole.  If they are set, then we need to close
  // a hole, and open one!
  $current_landr = db_select('taxonomy_leftandright', 'l')->fields('l', array(
    'lft',
    'rgt'
  ))->condition('tid', $term->tid)->execute()->fetch();
  // If we don't already have a left and right, then saving is easy.
  if(!$current_landr){
    // Punch the hole!
    db_query('UPDATE {taxonomy_leftandright} SET lft = lft+2 WHERE lft >= :lft', array(
      ':lft' => $term->lft
    ));
    db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt+2 WHERE rgt >= :lft', array(
      ':lft' => $term->lft
    ));
    // Do the insert
    db_insert('taxonomy_leftandright')->fields(array(
      'lft' => $term->lft,
      'rgt' => $term->rgt,
      'vid' => $term->vid,
      'tid' => $term->tid,
      'depth' => $term->depth
    ))->execute();
  }else{
    // Do nothing if the lft and rgt match
    if($term->lft == $current_landr->lft && $term->rgt == $current_landr->rgt){return;}
  }
}

/**
 * Do an insert
 */
function leftandright_insert($term){
  // We need to get the previous sibling and get the right value for it. If
  // there is no previous sibling, we get the left value of the parent.  If
  // there is no parent, then this is the first term at the root, and receives
  // the left value of 1.
  module_load_include('functions.inc', 'leftandright');
  // Set the left, right and depth.
  leftandright_set_left_and_depth($term);
  // Set the right
  $term->rgt = $term->lft + 1;
  // We should now save the term in to the table.
  leftandright_save_term($term);
}

/**
 * Set the leftandright for a term
 */
function leftandright_set_left_and_depth(&$term){
  // Get the previous term
  $previous_term = leftandright_previous_term($term);
  if($previous_term){
    // We have a previous term, we need to check if this is the parent, else
    // it's a sibling
    $previous_term_landr = db_select('taxonomy_leftandright', 'l')->fields('l', array(
      'lft',
      'rgt',
      'depth'
    ))->condition('tid', $previous_term->tid)->execute()->fetch();
    if(!$previous_term_landr){return leftandright_insert_error($previous_term);}
    if($previous_term->tid == current($term->parent)){
      // Previous is the parent, left is parent left + 1.
      $term->lft = $previous_term_landr->lft + 1;
      $term->depth = $previous_term_landr->depth + 1;
    }else{
      // Previous is a sibling, left is sibling right +1.
      $term->lft = $previous_term_landr->rgt + 1;
      // Depth is same as sibling
      $term->depth = $previous_term_landr->depth;
    }
  }else{
    // No previous term, we're the first one!
    $term->lft = 1;
    $term->depth = 0;
  }
}

/**
 * Insert fuck up!
 */
function leftandright_insert_error($previous_term){
  // PANIC!
  $vocabulary = taxonomy_vocabulary_load($previous_term->vid);
  drupal_set_message(t('The vocabulary @vocabulary_name needs to be <a href="!rebuild_url">rebuilt</a> immediately', array(
    '@vocabulary_name' => $vocabulary->name,
    '!rebuild_url' => 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/rebuild'
  )));
  return watchdog('Left and right error', 'Missing left and right values for <a href="!term_link">@term_name</a>.', array(
    '!term_link' => url('taxonomy/term/' . $previous_term->tid),
    '@term_name' => $previous_term->name
  ));
}

/**
 * Do an update.  Note, this is a little more complicated due to the fact that
 * branch terms can be updated.
 */
function leftandright_update($term){
  // We only need to do this if we have actually moved the term.  We test for
  // that first.
  // Get what we would like the left to be  
  leftandright_set_left_and_depth($term);
  // We start by getting the left and right values of the term that we're moving
  // We then move that term (and its children) to a temporary table.  The gap
  // is then closed, and then we punch a hole, and move the terms back to where
  // they should be.
  $leftandright = db_select('taxonomy_leftandright', 'l')->fields('l', array(
    'lft',
    'rgt',
    'depth'
  ))->condition('tid', $term->tid)->execute()->fetch();
  // Now we can check to see if the already set lft value is the same as what
  // we want it to be.  If it is, we can simply return
  if($term->lft == $leftandright->lft){return;}
  $temporary_table = db_query_temporary('SELECT * FROM {taxonomy_leftandright} WHERE lft >= :lft AND rgt <= :rgt AND vid = :vid', array(
    ':vid' => $term->vid,
    ':lft' => $leftandright->lft,
    ':rgt' => $leftandright->rgt
  ));
  // Delete the entries from the leftandright table
  db_delete('taxonomy_leftandright')->condition('lft', $leftandright->lft, '>=')->condition('vid', $term->vid)->condition('rgt', $leftandright->rgt, '<=')->execute();
  // Close up the hole
  $reduce_by = ($leftandright->rgt - $leftandright->lft) + 1;
  db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt - :reduce_by WHERE vid = :vid AND rgt > :rgt', array(
    ':reduce_by' => $reduce_by,
    ':vid' => $term->vid,
    ':rgt' => $leftandright->rgt
  ));
  db_query('UPDATE {taxonomy_leftandright} SET lft = lft - :reduce_by WHERE vid = :vid AND lft > :rgt', array(
    ':reduce_by' => $reduce_by,
    ':vid' => $term->vid,
    ':rgt' => $leftandright->rgt
  ));
  // Open a hole for where the term is going
  // Unfortunately, we need to get the previous term's left again as they may 
  // have changed (depending on the direction of the move). 
  leftandright_set_left_and_depth($term);
  db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt + :increase_by WHERE vid = :vid AND rgt >= :lft', array(
    ':increase_by' => $reduce_by,
    ':vid' => $term->vid,
    ':lft' => $term->lft
  ));
  db_query('UPDATE {taxonomy_leftandright} SET lft = lft + :increase_by WHERE vid = :vid AND lft >= :lft', array(
    ':increase_by' => $reduce_by,
    ':vid' => $term->vid,
    ':lft' => $term->lft
  ));
  // Move the terms in the table back, updating the depth, lft and rgt values
  // as we do.
  db_query('INSERT INTO {taxonomy_leftandright} (lft, rgt, tid, vid, depth) SELECT lft - :alter_by, rgt - :alter_by, tid, vid, depth - :alter_depth_by FROM ' . $temporary_table, array(
    ':alter_by' => $leftandright->lft - $term->lft,
    ':alter_depth_by' => $leftandright->depth - $term->depth
  ));
}

/**
 * Delete the term.  Note, this works if deleting branches as this is called
 * multiple times.
 */
function leftandright_delete($term){
  // Get the left and right values first so that we can close up the table.
  $leftandright = db_select('taxonomy_leftandright', 'l')->fields('l', array(
    'lft',
    'rgt'
  ))->condition('tid', $term->tid)->execute()->fetch();
  db_delete('taxonomy_leftandright')->condition('tid', $term->tid)->execute();
  db_query('UPDATE {taxonomy_leftandright} SET lft = lft-2 WHERE lft > :lft', array(
    ':lft' => $leftandright->lft
  ));
  db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt-2 WHERE rgt > :lft', array(
    ':lft' => $leftandright->lft
  ));
}