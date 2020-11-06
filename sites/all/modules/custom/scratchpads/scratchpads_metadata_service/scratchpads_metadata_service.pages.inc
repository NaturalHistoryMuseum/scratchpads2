<?php

/**
 * Callback to provide metadata as JSON about this Scratchpad.
 */
function scratchpads_metadata_service_json_page($authorised = FALSE){
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  // As a simple method of checking authorisation, we simply check that the
  // client has originated from "stats.scratchpads.org". This leaves us open to
  // hacks from our local server, but given that we do not allow posting of PHP,
  // this should not be possible.
  if($_SERVER['REMOTE_ADDR'] == gethostbyname('stats.scratchpads.org') || $_SERVER['REMOTE_ADDR'] == '127.0.0.1'){
    $authorised = TRUE;
  }
  print json_encode(scratchpads_metadata_service_get_metadata($authorised));
}

/**
 * Callback to provide the metadata as XML.
 */
function scratchpads_metadata_service_xml_page(){}

/**
 * Helper function that builds the metadata about this Scratchpad.
 */
function scratchpads_metadata_service_get_metadata($authorised = FALSE){
  $entity_info = entity_get_info();
  $metadata = array();
  foreach($entity_info as $key => $entity_type){
    scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, $metadata);
    if(isset($metadata['entities'][$key])){
      $metadata['entities'][$key]['label'] = $entity_type['label'];
    }
  }
  scratchpads_metadata_service_set_metadata_for_all_users($metadata);
  // We now show data for each individual user. We're using the UUID module to
  // identify them.
  $results = db_select('users', 'u')->fields('u', array(
    'uid'
  ))->condition('uid', -1, '>')->execute();
  $metadata['entities']['user']['users'] = array();
  foreach($results as $row){
    $user = user_load($row->uid);
    if(isset($user->uuid)){
      // Woot
      $metadata['entities']['user']['users'][$user->uuid] = array();
      $metadata['entities']['user']['users'][$user->uuid]['metadata'] = array(
        'mail' => $authorised ? $user->mail : '',
        'name' => $user->name,
        'roles' => array_keys($user->roles)
      );
      foreach($entity_info as $key => $entity_type){
        if($key != 'user'){
          scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, $metadata['entities']['user']['users'][$user->uuid], $user->uid);
        }
      }
    }
  }
  $metadata['install_profile'] = variable_get('install_profile', 'scratchpad_2');
  return $metadata;
}

/**
 * Helper function for the above.
 * Add site wide user stuff
 */
function scratchpads_metadata_service_set_metadata_for_all_users(&$metadata){
  // User specific shit.
  $row = db_query("SELECT MAX(login) AS last_login FROM users")->fetch();
  $metadata['entities']['user']['last_login'] = $row->last_login;
  // Number of users active in the last...
  // 7 days
  $row = db_query("SELECT COUNT(*) AS num_in_7_days FROM users WHERE access > :access", array(
    ':access' => time() - 604800
  ))->fetch();
  $metadata['entities']['user']['active_in_last_7_days'] = $row->num_in_7_days;
  // 28 days
  $row = db_query("SELECT COUNT(*) AS num_in_28_days FROM users WHERE access > :access", array(
    ':access' => time() - 2419200
  ))->fetch();
  $metadata['entities']['user']['active_in_last_28_days'] = $row->num_in_28_days;
  // Also, we reduce the number of users by one, as we don't want to include the
  // anonymous user.
  $metadata['entities']['user']['count']--;
}

/**
 * Helper function for the above.
 * Adds information for each entity type.
 */
function scratchpads_metadata_service_set_metadata_for_entity($key, $entity_type, &$metadata, $uid = FALSE){
  $matches = FALSE;
  preg_match('/[a-z_A-Z]+/', $entity_type['base table'], $matches);
  if($matches){
    $table_name = $matches[0];
    if(db_table_exists($table_name)){
      $schema = drupal_get_schema($table_name);
      $where = '';
      if($uid !== FALSE){
        // Ensure this table has a "uid" column.
        if(!isset($schema['fields']['uid'])){return;}
        $where = 'WHERE uid = :uid';
      }
      $row = db_query("SELECT COUNT(*) AS num_of_entity FROM $table_name $where", array(
        ':uid' => $uid
      ))->fetch();
      if(!$row->num_of_entity){return;}
      $metadata['entities'][$key]['count'] = $row->num_of_entity;
      $table_name = $matches[0];
      if(isset($schema['fields']['status'])){
        if($where){
          $where .= ' AND status > 0 ';
        }else{
          $where = ' WHERE status > 0 ';
        }
        $row = db_query("SELECT COUNT(*) AS num_of_entity FROM $table_name $where", array(
          ':uid' => $uid
        ))->fetch();
        $metadata['entities'][$key]['count_status'] = $row->num_of_entity;
      }
      if(isset($schema['fields']['changed'])){
        $row = db_query("SELECT MAX(changed) AS entity_changed FROM $table_name $where", array(
          ':uid' => $uid
        ))->fetch();
        $metadata['entities'][$key]['last_changed'] = $row->entity_changed;
      }else{
        if(isset($schema['fields']['created'])){
          $row = db_query("SELECT MAX(created) AS entity_changed FROM $table_name $where", array(
            ':uid' => $uid
          ))->fetch();
          $metadata['entities'][$key]['last_changed'] = $row->entity_changed;
        }
      }
    }
  }
}
