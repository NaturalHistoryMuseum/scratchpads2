<?php

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
    if($previous_term->tid == current($term->parent)){
      // Previous is the parent, left is parent left + 1.
      $previous_term_landr = db_select('taxonomy_leftandright', 'l')->fields('l', array(
        'lft',
        'rgt'
      ))->condition('tid', $previous_term->tid)->execute()->fetch();
      if($previous_term_landr){
        $term->lft = $previous_term_landr->lft + 1;
      }else{
        return leftandright_insert_error($previous_term);
      }
    }else{
      // Previous is a sibling, left is sibling right +1.
      $previous_term_landr = db_select('taxonomy_leftandright', 'l')->fields('l', array(
        'lft',
        'rgt'
      ))->condition('tid', $previous_term->tid)->execute()->fetch();
      if($previous_term_landr){
        $term->lft = $previous_term_landr->rgt + 1;
      }else{
        return leftandright_insert_error($previous_term);
      }
    }
    $term->rgt = $term->lft + 1;
    // We should now save the term in to the table.
    leftandright_save_term($term);
  }
}

/**
 * Insert fuck up!
 */
function leftandright_insert_error($previous_term){
  // PANIC!
  $vocabulary = taxonomy_vocabulary_load($previous_term->vid);
  drupal_set_message(t('The vocabulary @vocabulary_name needs to be <a href="!rebuild_url">rebuilt</a> immediately', array('@vocabulary_name' => $vocabulary->name)));
  return watchdog('Left and right error', 'Missing left and right values for <a href="!term_link">@term_name</a>.', array(
    '!term_link' => url('taxonomy/term/' . $previous_term->tid),
    '@term_name' => $previous_term->name
  ));
}