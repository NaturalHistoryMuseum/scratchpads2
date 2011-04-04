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
    //drupal_set_message(kprint_r($term, 1));
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
    $term->rgt = $term->lft + 1;
  }else{
    // No previous term, we're the first one!
    $term->lft = 1;
    $term->rgt = 2;
    $term->depth = 0;
  }
  // We should now save the term in to the table.
  leftandright_save_term($term);
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