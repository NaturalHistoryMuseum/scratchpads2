<?php

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
    db_query('UPDATE {taxonomy_leftandright} SET lft = lft+1 WHERE lft >= :lft', array(
      ':lft' => $term->lft
    ));
    db_query('UPDATE {taxonomy_leftandright} SET lft = lft+1 WHERE lft >= :rgt', array(
      ':rgt' => $term->rgt
    ));
    db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt+1 WHERE rgt >= :lft', array(
      ':lft' => $term->lft
    ));
    db_query('UPDATE {taxonomy_leftandright} SET rgt = rgt+1 WHERE rgt >= :rgt', array(
      ':rgt' => $term->rgt
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