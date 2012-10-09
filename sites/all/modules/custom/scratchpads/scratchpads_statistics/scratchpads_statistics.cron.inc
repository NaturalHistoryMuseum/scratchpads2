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
  db_query('INSERT INTO {scratchpads_statistics_term} (term) SELECT DISTINCT name FROM {taxonomy_term_data} WHERE name NOT IN (SELECT name FROM {scratchpads_statistics_term})');
  //
  // Create the data.
  //
  $counts = array();
  // Loop each entity
  foreach($info as $entity => $entity_info){
    // Loop each bundle
    foreach($entity_info['bundles'] as $bundle => $bundle_info){
      // Loop each user
      $results = db_select('users', 'u')->fields('u', array(
        'uid',
        'mail'
      ))->condition('uid', 1, '>')->execute();
      foreach($results as $row){
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
          if(@isset($counts[$entity][$bundle][$row->mail][$term->tid])){
            continue;
          }
          $counts[$entity][$bundle][$row->mail][$term->tid]['number_created'] = _scratchpads_statistics_cron_get_count($entity, $bundle, $row->uid, $term->tid);
        }
        $counts[$entity][$bundle][$row->mail][0]['number_created'] = _scratchpads_statistics_cron_get_count($entity, $bundle, $row->uid);
      }
    }
  }
  print_r($counts);
  echo "Finished";
}

/**
 * Helper function to do the counting.
 */
function _scratchpads_statistics_cron_get_count($entity, $bundle, $uid, $tid = FALSE){
  try{
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity);
    switch($entity){
      case 'comment':
      case 'taxonomy_term':
        $bundle = $entity;
        break;
      default:
        $query->entityCondition('bundle', $bundle);
        break;
    }
    $query->propertyCondition('uid', $uid);
    if($term){
      $query->
    }
    $query->count();
    return $query->execute();
  }
  catch(Exception $e){
    // Error no doubt as a result of us not being able to associate this
    // particular entity with a UID or similar
    ;
  }
}