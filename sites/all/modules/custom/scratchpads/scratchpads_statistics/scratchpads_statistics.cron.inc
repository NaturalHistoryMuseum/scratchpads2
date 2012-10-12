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
      if(in_array($entity, array(
        'comment',
        'user'
      ))){
        $bundle = $entity;
      }
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
    $name = $user->name;
    if(@isset($user->field_user_family_name[LANGUAGE_NONE][0]['value']) && isset($user->field_user_given_names[LANGUAGE_NONE][0]['value'])){
      $name = trim($user->field_user_given_names[LANGUAGE_NONE][0]['value'] . ' ' . $user->field_user_family_name[LANGUAGE_NONE][0]['value']);
    }
    $query = db_merge('scratchpads_statistics_user')->key(array(
      'email' => $user->mail
    ))->fields(array(
      'name' => $name,
      'email' => $user->mail
    ))->execute();
  }
  // Ensure we have a comprehensive list of terms
  db_query('INSERT INTO {scratchpads_statistics_term} (term) SELECT DISTINCT LOWER(name) FROM {taxonomy_term_data} WHERE name NOT IN (SELECT name FROM {scratchpads_statistics_term})');
  //
  // Create the data.
  //
  // Get the time to use.
  $timer = time();
  $col = db_select('scratchpads_statistics_data_temp', 's')->fields('s', array(
    'captured'
  ))->range(0, 1)->orderBy('captured')->execute()->fetchCol();
  if($col){
    $time = array_pop($col);
  }else{
    $time = $timer;
  }
  foreach($info as $entity => $entity_info){
    foreach($entity_info['bundles'] as $bundle => $bundle_info){
      try{
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $entity);
        $query->addMetaData('account', user_load(1));
        $query->addTag('scratchpads_statistics');
        switch($entity){
          case 'country':
          case 'eolapi':
          case 'og_membership_type':
          case 'og_membership':
          case 'group':
          case 'taxonomy_vocabulary':
          case 'taxonomy_term':
            // We don't bother with these entities.
            break 2;
          case 'comment':
          case 'user':
            // These don't support bundle queries.
            $bundle = $entity;
            break;
          default:
            $query->entityCondition('bundle', $bundle);
            break;
        }
        // Get the entity/bundle id.
        $col = db_select('scratchpads_statistics_entity_bundle', 'e')->fields('e', array(
          'id'
        ))->condition('entity', $entity)->condition('bundle', $bundle)->execute()->fetchCol();
        $id = array_pop($col);
        // Check we haven't already done this entity/bundle.
        $count = db_select('scratchpads_statistics_data_temp', 's')->condition('entity_bundle', $id)->countQuery()->execute()->fetchCol();
        if($count[0]){
          break;
        }
        // Order the query by the entity_id;
        $query->entityOrderBy('entity_id', 'ASC');
        $query->execute();
        $counts = array();
        $previous_entity_id = FALSE;
        if(isset($query->ordered_results)){
          foreach($query->ordered_results as $result){
            $tid = isset($result->ss_term_id) ? $result->ss_term_id : 0;
            // Number created
            if(!isset($counts[$result->ss_user_id])){
              $counts[$result->ss_user_id] = array(
                0 => array(
                  'number_created' => 0,
                  'number_edited' => 0,
                  'number_views' => 0
                )
              );
            }
            if(!isset($counts[$result->ss_user_id][$tid])){
              $counts[$result->ss_user_id][$tid] = array(
                'number_created' => 0,
                'number_edited' => 0,
                'number_views' => 0
              );
            }
            if($tid){
              $counts[$result->ss_user_id][$tid]['number_created']++;
              $counts[$result->ss_user_id][$tid]['number_edited']++;
            }
            if($result->entity_id != $previous_entity_id){
              $previous_entity_id = $result->entity_id;
              $counts[$result->ss_user_id][0]['number_created']++;
              $counts[$result->ss_user_id][0]['number_edited']++;
            }
            // Number edited (most likely the same as created).
            if(isset($result->ss_revision_user_id) && $result->ss_revision_user_id != $result->ss_user_id){
              $counts[$result->ss_revision_user_id][$tid]['number_edited']++;
            }
            // Number of views
            if(isset($result->totalcount)){
              $counts[$result->ss_user_id][$tid]['number_views'] = $result->totalcount;
            }
          }
          $query = db_insert('scratchpads_statistics_data_temp')->fields(array(
            'user',
            'term',
            'site',
            'entity_bundle',
            'number_created',
            'number_edited',
            'number_views',
            'captured'
          ));
          foreach($counts as $user_id => $values){
            foreach($values as $term_id => $count){
              $query->values(array(
                'user' => $user_id,
                'term' => $term_id,
                'site' => 1,
                'entity_bundle' => $id,
                'number_created' => $count['number_created'],
                'number_edited' => $count['number_edited'],
                'number_views' => $count['number_views'],
                'captured' => $time
              ));
            }
          }
          $query->execute();
          // Run for no more than sixty seconds.
          if(time() > $timer + 60){return;}
        }
      }
      catch(Exception $e){
        watchdog('scratchpads_statistics', 'Failed to calculate data for @entity/@bundle', array(
          '@entity' => $entity,
          '@bundle' => $bundle
        ));
      }
    }
  }
  // Finally, move the temp data to the permanent data table.
  db_query('INSERT INTO {scratchpads_statistics_data} SELECT * FROM {scratchpads_statistics_data_temp}');
  db_delete('scratchpads_statistics_data_temp')->execute();
}