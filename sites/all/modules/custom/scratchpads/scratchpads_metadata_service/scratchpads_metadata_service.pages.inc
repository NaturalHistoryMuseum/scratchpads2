<?php

/**
 * Callback to provide metadata as JSON about this Scratchpad.
 */
function scratchpads_metadata_service_json_page(){
  print json_encode(scratchpads_metadata_service_get_metadata());
}

/**
 * Callback to provide the metadata as XML.
 */
function scratchpads_metadata_service_xml_page(){}

/**
 * Helper function that builds the metadata about this Scratchpad.
 */
function scratchpads_metadata_service_get_metadata(){
  $entity_info = entity_get_info();
  $metadata = array();
  foreach($entity_info as $key => $entity_type){
    scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, $metadata);
    if(isset($metadata[$key])){
      $metadata[$key]['label'] = $entity_type['label'];
    }
  }
  scratchpads_metadata_service_set_metadata_for_all_users($metadata);
  // We now show data for each individual user. We're using the UUID module to
  // identify them.
  $results = db_select('users', 'u')->fields('u', array(
    'uid'
  ))->condition('uid', -1, '>')->execute();
  $metadata['user']['users'] = array();
  foreach($results as $row){
    $user = user_load($row->uid);
    if(isset($user->uuid)){
      // Woot
      $metadata['user']['users'][$user->uuid] = array();
      $metadata['user']['users'][$user->uuid]['metadata'] = array(
        'mail' => $user->mail,
        'name' => $user->name,
        'roles' => array_keys($user->roles)
      );
      foreach($entity_info as $key => $entity_type){
        if($key != 'user'){
          scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, $metadata['user']['users'][$user->uuid], $user->uid);
        }
      }
    }
  }
  return $metadata;
}

/**
 * Helper function for the above.
 * Add site wide user stuff
 */
function scratchpads_metadata_service_set_metadata_for_all_users(&$metadata){
  // User specific shit.
  $row = db_query("SELECT MAX(login) AS last_login FROM users")->fetch();
  $metadata['user']['last_login'] = $row->last_login;
  // Number of users active in the last...
  // 7 days
  $row = db_query("SELECT COUNT(*) AS num_in_7_days FROM users WHERE access > :access", array(
    ':access' => time() - 604800
  ))->fetch();
  $metadata['user']['active_in_last_7_days'] = $row->num_in_7_days;
  // 28 days
  $row = db_query("SELECT COUNT(*) AS num_in_28_days FROM users WHERE access > :access", array(
    ':access' => time() - 2419200
  ))->fetch();
  $metadata['user']['active_in_last_28_days'] = $row->num_in_28_days;
  // Also, we reduce the number of users by one, as we don't want to include the
  // anonymous user.
  $metadata['user']['count']--;
}

/**
 * Helper function for the above.
 * Adds information for each entity type.
 */
function scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, &$metadata, $uid = FALSE){
  $where = '';
  if($uid !== FALSE){
    $where = 'WHERE uid = :uid';
  }
  $matches = FALSE;
  preg_match('/[a-z_A-Z]+/', $entity_type['base table'], $matches);
  if($matches){
    $table_name = $matches[0];
    try{
      $row = db_query("SELECT COUNT(*) AS num_of_entity FROM $table_name $where", array(
        ':uid' => $uid
      ))->fetch();
      if(!$row->num_of_entity){return;}
      $metadata[$key]['count'] = $row->num_of_entity;
    }
    catch(Exception $e){
      // If this has failed, we have no "uid" column, so we simply continue.
    }
    $table_name = $matches[0];
    try{
      if($where){
        $where .= ' AND status > 0 ';
      }else{
        $where = ' WHERE status > 0 ';
      }
      $row = db_query("SELECT COUNT(*) AS num_of_entity FROM $table_name $where", array(
        ':uid' => $uid
      ))->fetch();
      if(!$row->num_of_entity){return;}
      $metadata[$key]['count_status'] = $row->num_of_entity;
    }
    catch(Exception $e){
      // If this has failed, we have no "uid" or "status" column, so we simply
      // continue.
    }
    try{
      $row = db_query("SELECT MAX(changed) AS entity_changed FROM $table_name $where", array(
        ':uid' => $uid
      ))->fetch();
      $metadata[$key]['last_changed'] = $row->entity_changed;
    }
    catch(Exception $e){
      // No changed column. Try the created column.
      try{
        $row = db_query("SELECT MAX(created) AS entity_changed FROM $table_name $where", array(
          ':uid' => $uid
        ))->fetch();
        $metadata[$key]['last_changed'] = $row->entity_changed;
      }
      catch(Exception $e){
        ; // Do nothing, no created/changed columns.
      }
    }
  }
}