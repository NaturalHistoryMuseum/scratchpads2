<?php

/**
 * Standard data-int to check if any terms have lost their parent.
 */
function scratchpads_tweaks_check_all_terms_have_parents(){
  $not_in = db_select('taxonomy_term_hierarchy', 'th')->fields('th', array(
    'tid'
  ));
  $results = db_select('taxonomy_term_data', 't')->condition('tid', $not_in, 'NOT IN')->fields('t', array(
    'tid',
    'name'
  ))->execute();
  $messages = array();
  foreach($results as $row){
    if(!count($messages)){
      $messages[] = t('The following terms are missing parents:');
    }
    $messages[] = "{$row->name} ({$row->tid})";
  }
  return $messages;
}