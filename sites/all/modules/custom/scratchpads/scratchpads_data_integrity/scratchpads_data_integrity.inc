<?php

/**
 * Standard data-int to check if any terms have lost their parent.
 */
function scratchpads_data_integrity_check_all_terms_have_parents(){
  $not_in = db_select('taxonomy_term_hierarchy', 'th')->fields('th', array(
    'tid'
  ));
  $results = db_select('taxonomy_term_data', 't')->condition('tid', $not_in, 'NOT IN')->fields('t', array(
    'tid',
    'name'
  ))->execute();
  $messages = array();
  foreach($results as $row){
    $messages[] = " - {$row->name} ({$row->tid})";
  }
  if(count($messages)){
    $messages = implode("\n", $messages);
    $messages = array(
      t('The following term(s) are missing a parent:'),
      $messages
    );
  }
  return $messages;
}

/**
 * Standard data-int to check if any vocabularies have a loop in them
 * 
 * Thanks to Alice for this code.
 */
function scratchpads_data_integrity_check_taxonomy_for_loops(){
  $result = db_select('taxonomy_term_hierarchy', 't')->fields('t')->execute()->fetchAllKeyed();
  $loops = array();
  $messages = array();
  $tid_in_loop = array();
  foreach($result as $tid => $parent){
    if(in_array($tid, $tid_in_loop)){
      continue;
    }
    $loop = scratchpads_data_integrity_check_taxonomy_for_loops_find_loop($tid, $result);
    if($loop){
      $tid_in_loop += $loop['tids'];
      $loop_str = implode(' => ', $loop['loop']);
      if(!in_array($loop_str, $loops)){
        $loops[] = $loop_str;
      }
    }
  }
  if(count($loops)){
    $messages = array(
      t('The following loop(s) in the taxonomy were found:'),
      implode("\n", $loops)
    );
  }
  return $messages;
}

function scratchpads_data_integrity_check_taxonomy_for_loops_find_loop($tid, $result, $stack = array()){
  $pos = array_search($tid, $stack);
  if($pos !== FALSE){
    $loop = scratchpads_data_integrity_check_taxonomy_for_loops_rotate_array(array_slice($stack, $pos));
    return array(
      'tids' => $stack,
      'loop' => $loop
    );
  }
  if(!isset($result[$tid])){return FALSE;}
  $stack[] = $tid;
  return scratchpads_data_integrity_check_taxonomy_for_loops_find_loop($result[$tid], $result, $stack);
}

function scratchpads_data_integrity_check_taxonomy_for_loops_rotate_array($a){
  $pos = array_search(min($a), $a);
  if($pos == 0){return $a;}
  return array_merge(array_slice($a, $pos), array_slice($a, 0, $pos));
}
