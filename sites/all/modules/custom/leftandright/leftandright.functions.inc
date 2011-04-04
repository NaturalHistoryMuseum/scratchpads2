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
    
  }