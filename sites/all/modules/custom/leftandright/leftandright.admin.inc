<?php

/**
 * Display form for adding and editing vocabularies.
 */
function leftandright_rebuild_tree_form($form, &$form_state, $edit = array()){
  // Check to see whether or not the tree needs rebuilding.
  $count_in_leftandright = db_query('SELECT COUNT(*) FROM {taxonomy_leftandright} WHERE vid = :vid', array(
    ':vid' => $edit->vid
  ))->fetchCol();
  $count_in_term_data = db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
    ':vid' => $edit->vid
  ))->fetchCol();
  if($count_in_leftandright == $count_in_term_data){
    // We have the same number of terms, rebuild MAY not be required.
    $message = '<p>' . t('A rebuild <strong>may not</strong> be required.  If you have been experiencing problems with display of taxonomic trees, then click rebuild (no harm will be done if a rebuild is not required).') . '</p>';
  }else{
    // Different number, rebuild definitely required.
    $message = '<p>' . t('A rebuild is definitely required.  You must click "Rebuild" below.') . '</p>';
  }
  return array(
    'information' => array(
      '#markup' => $message
    ),
    'vid' => array(
      '#type' => 'hidden',
      '#value' => $edit->vid
    ),
    'actions' => array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Rebuild')
      )
    )
  );
}

/**
 * Submit function for the above form
 */
function leftandright_rebuild_tree_form_submit($form, &$form_state){
  $batch = array(
    'title' => t('Rebuilding'),
    'operations' => array(
      array(
        'leftandright_rebuild_tree',
        array(
          $form_state['values']['vid']
        )
      ),
      array(
        'leftandright_rebuild_tree_merge',
        array(
          $form_state['values']['vid']
        )
      )
    ),
    'file' => drupal_get_path('module', 'leftandright') . '/leftandright.admin.inc'
  );
  batch_set($batch);
}

/**
 * Rebuild function called by batch!
 */
function leftandright_rebuild_tree($vid, &$context){
  if(empty($context['sandbox'])){
    // Ensure the process tables are empty    
    db_delete('leftandright_rebuild_right')->condition('vid', $vid)->execute();
    db_delete('leftandright_rebuild_left')->condition('vid', $vid)->execute();
    // Set the max and current location
    // Note, the max is twice the number of terms in the database so that we
    // can count setting the left and right as one point each!  
    $context['sandbox']['max'] = (2 * current(db_query('SELECT COUNT(*) FROM {taxonomy_term_data} WHERE vid = :vid', array(
      ':vid' => $vid
    ))->fetchCol()));
    $context['sandbox']['progress'] = 0;
    // Get the first term
    $query = db_select('taxonomy_term_data', 't')->fields('t');
    $query->join('taxonomy_term_hierarchy', 'h', 't.tid = h.tid');
    $term = $query->condition('vid', $vid)->condition('parent', 0)->orderBy('t.weight')->orderBy('t.name')->orderBy('t.tid', 'DESC')->fields('t')->execute()->fetch();
    $context['sandbox']['current_term'] = $term;
    // Set the start time to allow us to calculate a rough time remaining
    $context['sandbox']['starttime'] = time();
    // Set the depth to allow us to calculate later on
    $context['sandbox']['current_term']->depth = 0;
    $context['sandbox']['leftorright'] = 0;
    $context['sandbox']['parents'] = array();
    $context['finished'] = 0;
  }
  // We will do 1000 terms at a time, trying to reduce the number of breaks from
  // this code.  1000 terms also takes approximately 1 second to save, so gives
  // plenty of feedback to the user.
  $number_of_terms_processed = 0;
  while($number_of_terms_processed < 1000 && $context['sandbox']['progress'] < $context['sandbox']['max']){
    // Set the left for this term if it needs setting
    $context['sandbox']['leftorright']++;
    if(empty($context['sandbox']['current_term']->lft)){
      $context['sandbox']['current_term']->lft = $context['sandbox']['leftorright'];
      db_insert('leftandright_rebuild_left')->fields(array(
        'tid' => $context['sandbox']['current_term']->tid,
        'vid' => $context['sandbox']['current_term']->vid,
        'lft' => $context['sandbox']['current_term']->lft,
        'depth' => $context['sandbox']['current_term']->depth
      ))->execute();
      // Next term is either a child, or this term (if no children).
      // Get this terms children
      $children = taxonomy_get_children($context['sandbox']['current_term']->tid);
      if(count($children)){
        // We have children, we'll set the first one as the next term to handle
        // We also add to the parents array, so that we can parse back up it
        // later.
        $next_term = current($children);
        $next_term->depth = $context['sandbox']['current_term']->depth + 1;
        $context['sandbox']['parents'][] = $context['sandbox']['current_term'];
        $context['sandbox']['current_term'] = $next_term;
      }
    }else{
      // We already have the lft set for this term, lets set its right value
      $context['sandbox']['current_term']->rgt = $context['sandbox']['leftorright'];
      db_insert('leftandright_rebuild_right')->fields(array(
        'tid' => $context['sandbox']['current_term']->tid,
        'vid' => $context['sandbox']['current_term']->vid,
        'rgt' => $context['sandbox']['current_term']->rgt
      ))->execute();
      // Next term is either a sibling, or the parent of this term (or nothing
      // if we have finished).
      module_load_include('functions.inc', 'leftandright');
      $next_sibling = leftandright_get_next_sibling($context['sandbox']['current_term']);
      if($next_sibling){
        // We have a sibling, lets set it as the next term
        $next_sibling->depth = $context['sandbox']['current_term']->depth;
        $context['sandbox']['current_term'] = $next_sibling;
      }else{
        // No siblings (after this one), we'll set the parent as the next term
        if(count($context['sandbox']['parents'])){
          $context['sandbox']['current_term'] = array_pop($context['sandbox']['parents']);
        }
      }
    }
    $time_taken = time() - $context['sandbox']['starttime'];
    $interval_guess = ($context['sandbox']['max'] / $context['sandbox']['progress']) * (time() - $context['sandbox']['starttime']);
    //$interval_guess = ($context['sandbox']['max'] - $context['sandbox']['progress']) * ((time() - $context['sandbox']['starttime']/$context['sandbox']['progress']));
    $context['message'] = '<p>' . t('Assigned !progress of !total left and right values (Number of terms &times; 2)', array(
      '!progress' => $context['sandbox']['progress'] + 1,
      '!total' => $context['sandbox']['max']
    )) . '</p><p>' . t('Approximate time remaining: !time', array(
      '!time' => format_interval($interval_guess - $time_taken, 2)
    )) . '</p><p>' . t('Time taken: !timetaken', array(
      '!timetaken' => format_interval($time_taken)
    )) . '</p>';
    $number_of_terms_processed++;
    $context['sandbox']['progress']++;
  }
  if($context['sandbox']['progress'] != $context['sandbox']['max']){
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Merge the data into the main table.
 */
function leftandright_rebuild_tree_merge($vid, &$context){
  db_delete('taxonomy_leftandright')->condition('vid', $vid)->execute();
  db_query('INSERT INTO {taxonomy_leftandright} (lft, rgt, tid, vid, depth) SELECT lft, rgt, l.tid, l.vid, depth FROM {leftandright_rebuild_left} l INNER JOIN {leftandright_rebuild_right} r ON l.tid = r.tid AND l.vid = r.vid AND l.vid = :vid', array(
    ':vid' => $vid
  ));
  db_delete('leftandright_rebuild_right')->condition('vid', $vid)->execute();
  db_delete('leftandright_rebuild_left')->condition('vid', $vid)->execute();
  $context['finished'] = 1;
}