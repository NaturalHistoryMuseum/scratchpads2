<?php

/**
 * Check we don't have orphaned terms in deleted vocabularies.  These do not
 * need to be reported, and can simply be deleted.
 */
function scratchpads_data_integrity_check_for_terms_in_deleted_vocabularies(){
  $subquery = db_select('taxonomy_vocabulary', 'v')->fields('v', array(
    'vid'
  ));
  $results = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid',
    'name'
  ))->condition('vid', $subquery, 'NOT IN')->execute();
  $tids = array();
  $messages = array();
  foreach($results as $row){
    taxonomy_term_delete($row->tid);
  }
  return array();
}

/**
 * Check for files that have been deleted from the filesystem, but not deleted
 * from the file_managed database table.
 */
function scratchpads_data_integrity_check_for_deleted_files(){
  $results = db_select('file_managed', 'f')->fields('f', array(
    'uri',
    'fid',
    'uid',
    'filesize',
    'filename'
  ))->execute();
  $messages = array();
  foreach($results as $row){
    $delete_file = FALSE;
    if(!file_exists($row->uri)){
      if(!$row->filesize || !$row->uid || !trim($row->uri) || substr($row->uri, 0, 12) == 'temporary://' || preg_match('/file[A-Za-z][A-Za-z][A-Za-z][A-Za-z][A-Za-z][A-Za-z]/', $row->filename)){
        // Simply delete the file if it is owned by anon or is a temporary file,
        // or is a file with no filename, or has zero filesize.
        file_delete(file_load($row->fid), TRUE);
      }else{
        // Alert if a user uploaded the file
        $messages[] = " - " . t('Missing') . ": {$row->fid}:{$row->uri}";
      }
    }
  }
  if(count($messages)){
    $messages = implode("\n", $messages);
    $messages = array(
      t('The following file(s) were found to be missing:'),
      $messages
    );
  }
  return $messages;
}

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
    // We add a parent for the terms that are missing one.
    $query = db_select('taxonomy_term_data', 't')->fields('t', array(
      'tid'
    ))->condition('tid', $not_in, 'NOT IN');
    $query->addExpression('0', 'parent');
    db_insert('taxonomy_term_hierarchy')->fields(array(
      'tid',
      'parent'
    ))->from($query)->execute();
    // And also report that they have been added
    $messages = implode("\n", $messages);
    $messages = array(
      t('The following term(s) were missing a parent and have been moved to the root of their classification:'),
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
        $loops[] = " - " . $loop_str;
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
