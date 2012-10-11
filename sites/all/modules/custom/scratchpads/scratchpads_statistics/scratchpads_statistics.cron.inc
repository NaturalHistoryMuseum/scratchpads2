<?php

function _scratchpads_statistics_cron(){
  // Check the last time we saved data for this site (should always be site 1),
  // and only progress if it was more than six days ago.
  $last_capture = 0;
  $col = db_select('scratchpads_statistics_data', 's')->fields('s', array(
    'captured'
  ))->orderBy('captured', 'DESC')->range(0, 1)->condition('site', 1)->execute()->fetchCol();
  if(count($col)){
    $last_capture = array_pop($col);
  }
  if($last_capture + 518400 > time()){return;}
  // Ensure we have a comprehensive list of entities and bundles.
  $info = entity_get_info();
  foreach(entity_get_info() as $entity => $entity_info){
    foreach($entity_info['bundles'] as $bundle => $bundle_info){
      $query = db_merge('scratchpads_statistics_entity_bundle')->key(array(
        'entity' => $entity,
        'bundle' => $bundle
      ))->fields(array(
        'entity' => $entity,
        'bundle' => $bundle,
        'name' => $bundle_info['label']
      ))->execute();
    }
  }
  // Ensure we have a comprehensive list of users
  $results = db_select('users', 'u')->fields('u', array(
    'uid'
  ))->condition('uid', 1, '>')->orderBy('uid')->execute();
  foreach($results as $row){
    $user = user_load($row->uid);
    $query = db_merge('scratchpads_statistics_user')->key(array(
      'email' => $user->mail
    ))->fields(array(
      'name' => @isset($user->field_user_family_name[LANGUAGE_NONE][0]['value']) ? trim($user->field_user_given_names[LANGUAGE_NONE][0]['value'] . ' ' . $user->field_user_family_name[LANGUAGE_NONE][0]['value']) : $user->name,
      'email' => $user->mail
    ))->execute();
  }
  // Ensure we have a comprehensive list of terms
  db_query('INSERT INTO {scratchpads_statistics_term} (term) SELECT DISTINCT LOWER(name) FROM {taxonomy_term_data} WHERE name NOT IN (SELECT name FROM {scratchpads_statistics_term})');
  //
  // Create the data.
  //
  // Set a timer to prevent us from taking too long on a single run.  We will
  // set the maximum execution time to 60 seconds.
  $timer = time();
  $counts = array();
  // We loop from most to least.  We most likely to have the following
  // # terms > # users > #entities > #bundles
  // Loop each Biological term.
  $vids = array();
  foreach(variable_get('biological_vids', array()) as $vid => $status){
    if($status){
      $vids[] = $vid;
    }
  }
  if(count($vids)){
    $terms = db_select('taxonomy_term_data', 't')->fields('t', array(
      'tid'
    ))->condition('vid', $vids)->execute();
  }
  foreach($terms as $term){
    // Load the actual term
    $term = taxonomy_term_load($term->tid);
    // Loop each user
    $results = db_select('users', 'u')->fields('u', array(
      'uid',
      'mail'
    ))->condition('uid', 1, '>')->execute();
    foreach($results as $row){
      // We complete for each term/user combination, so we only check here if we
      // have already completed this query.
      if(_scratchpads_statistics_not_already_collected($term, $row->mail)){
        // Loop each entity
        foreach($info as $entity => $entity_info){
          // Loop each bundle
          foreach($entity_info['bundles'] as $bundle => $bundle_info){
            if(!isset($position[$entity][$bundle][$row->uid][$term->tid])){
              _scratchpads_statistics_cron_set_count($entity, $bundle, $row->uid, $row->mail, $term);
            }
          }
          _scratchpads_statistics_cron_set_count($entity, $bundle, $row->uid, $row->mail);
        }
        // Stop if we've gone over time.
        if(time() > ($timer + 60)){
          echo "TIMED OUT";
          return;
        }
      }
    }
  }
  // Copy the data across
  db_query('INSERT INTO {scratchpads_statistics_data} SELECT * FROM {scratchpads_statistics_data_temp} WHERE number_created > 0');
  db_delete('scratchpads_statistics_data_temp')->execute();
}

/**
 * Get whether or not we should collect data for this term/mail combination
 */
function _scratchpads_statistics_not_already_collected($term, $email){
  $query = db_select('scratchpads_statistics_data_temp', 's');
  $query->innerJoin('scratchpads_statistics_user', 'u', 's.user = u.id');
  $query->innerJoin('scratchpads_statistics_term', 't', 's.term = t.id');
  $query->condition('t.term', strtolower($term->name));
  $query->condition('email', $email);
  $query = $query->countQuery();
  $results = $query->execute()->fetchCol();
  $results = array_pop($results);
  return !$results;
}

/**
 * Helper function to do the counting.
 */
function _scratchpads_statistics_cron_set_count($entity, $bundle, $uid, $mail, $term = FALSE){
  try{
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity);
    switch($entity){
      case 'countries':
        return;
      case 'file':
        // propertyCondition does not appear to work as expected on the
        // file entity type.  May need to look at the file_entity module to fix
        // this.
        //$query->propertyCondition('uid', $uid);
        $query->entityCondition('bundle', $bundle);
        break;
      case 'comment':
      case 'taxonomy_term':
      case 'user':
        $query->propertyCondition('uid', $uid);
        $bundle = $entity;
        break;
      default:
        $query->propertyCondition('uid', $uid);
        $query->entityCondition('bundle', $bundle);
        break;
    }
    $term_id = 0;
    if($term){
      $query->fieldCondition('field_taxonomic_name', 'tid', $term->tid);
      // Get the stats term id
      $term_id = db_select('scratchpads_statistics_term', 's')->fields('s', array(
        'id'
      ))->condition('term', $term->name)->execute()->fetchCol();
      $term_id = array_pop($term_id);
    }
    $query->count();
    $count = $query->execute();
    // Get the entity/bundle ID
    $entity_bundle_id = db_select('scratchpads_statistics_entity_bundle', 's')->fields('s', array(
      'id'
    ))->condition('entity', $entity)->condition('bundle', $bundle)->execute()->fetchCol();
    $entity_bundle_id = array_pop($entity_bundle_id);
    // Get the stats user id
    $user_id = db_select('scratchpads_statistics_user', 's')->fields('s', array(
      'id'
    ))->condition('email', $mail)->execute()->fetchCol();
    $user_id = array_pop($user_id);
    $number_views = 0;
    $number_edited = $count;
    if($entity == 'node' && !$term){
      // Get the number of views if this is a node and tid = 0
      $col = db_query("SELECT SUM(totalcount) FROM {node_counter} WHERE nid IN (SELECT nid FROM {node} WHERE type = :bundle AND uid = :uid)", array(
        ':bundle' => $bundle,
        ':uid' => $uid
      ))->fetchCol();
      $number_views = array_pop($col);
      // Get the number edited if this is a node and tid = 0
      $col = db_query("SELECT COUNT(*) FROM {node_revision} WHERE nid IN (SELECT nid FROM {node} WHERE type = :bundle) AND uid = :uid", array(
        ':bundle' => $bundle,
        ':uid' => $uid
      ))->fetchCol();
      $number_edited = array_pop($col);
    }
    static $time = FALSE;
    if(!$time){
      $results = db_select('scratchpads_statistics_data_temp', 's')->fields('s', array(
        'captured'
      ))->range(0, 1)->execute()->fetchCol();
      if($results){
        $time = array_pop($results);
      }else{
        $time = time();
      }
    }
    // Insert a row.
    db_insert('scratchpads_statistics_data_temp')->fields(array(
      'user' => $user_id,
      'term' => $term_id,
      'site' => 1,
      'entity_bundle' => $entity_bundle_id,
      'number_created' => $count,
      'number_edited' => $number_edited,
      'number_views' => $number_views,
      'captured' => $time
    ))->execute();
  }
  catch(Exception $e){
    // Error no doubt as a result of us not being able to associate this
    // particular entity with a UID or similar
    return;
  }
}