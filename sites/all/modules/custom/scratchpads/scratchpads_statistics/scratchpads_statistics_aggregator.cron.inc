<?php

/**
 * Weekly cron
 *
 * Performs functions once a week. Non-essential tasks, like updating nodes/
 * organizations/scratchpads should be placed here.
 *
 * FIXME - Not sure what the "tag" is going to be for a "scratchpad", although
 * it is likely to be "scratchpad". This needs checking.
 */
function _scratchpads_statistics_aggregator_cron(){
  $gbif_registry = GBIFRegistry::singleton();
  // foreach($gbif_registry->getTechnicalInstallations('scratchpad') as
  // $technical_installation){
  foreach($gbif_registry->getTechnicalInstallations() as $ti){
    // Get this site from the list of sites.
    //$technical_installation = $technical_installation->getAsArray();
    $organization = $ti->getHostingOrganization();
    if($organization->getTitle() == "Scratchpads"){
      // We haven't actually registered an endpoint for each of the Scratchpads,
      // so for now will assume the location of the end point (this will only
      // be an issue when hosting of sites external to the NHM becomes more
      // prevalent).
      $url = $ti->getHomePage();
      // Get the SS site for this Scratchpad
      $query = db_select('scratchpads_statistics_data', 'd');
      $query->innerJoin('scratchpads_statistics_site', 's', 'd.site = s.id');
      $row = $query->fields('s')->condition('url', $url)->range(0, 1)->orderBy('captured', 'DESC')->execute()->fetch();
      $last_capture = 0;
      if($row){
        // We have harvested this site before, all we need is a most recent 
        // capture value.
        $last_capture = $row->capture;
      }
      // FIXME - Need to add the hash if we need to use it.
      $data = json_decode(file_get_contents($url . 'stats.json?since=' . $last_capture), TRUE);
      if($data){
        _scratchpads_statistics_aggregator_insert_data($data);
      }
    }
  }
}

/**
 * Insert the data into the database.
 */
function _scratchpads_statistics_aggregator_insert_data($data){
  foreach($data as $record){
    $user = _scratchpads_statistics_insert_item(array(
      'name' => $record['owner_name'],
      'email' => $record['owner_email']
    ), 'scratchpads_statistics_user', 'email');
    $site = _scratchpads_statistics_insert_item(array(
      'title' => $record['title'],
      'owner' => $user,
      'url' => $record['url'],
      'created' => $record['created']
    ), 'scratchpads_statistics_site', 'url');
    $term = _scratchpads_statistics_insert_item(array(
      'term' => $record['term']
    ), 'scratchpads_statistics_term', 'term');
    $entity_bundle = _scratchpads_statistics_insert_item(array(
      'entity' => $record['entity'],
      'bundle' => $record['bundle'],
      'label' => $record['label']
    ), 'scratchpads_statistics_entity_bundle', array(
      'entity',
      'bundle'
    ));
    db_insert('scratchpads_statistics_data')->fields(array(
      'user' => $user,
      'term' => $term,
      'site' => $site,
      'entity_bundle' => $entity_bundle,
      'number_created' => $record['number_created'],
      'number_edited' => $record['number_edited'],
      'number_views' => $record['number_views'],
      'captured' => $record['captured']
    ))->execute();
  }
}

/**
 * Create/Retrieve a user
 */
function _scratchpads_statistics_insert_item($item, $table, $key){
  // Keep a static cache of the users.
  $items = drupal_static(__FUNCTION__, array());
  $array_key = $key;
  if(is_array($array_key)){
    $array_key = implode("|", $array_key);
  }else{
    $key = array(
      $key
    );
  }
  if(@isset($users[$table][$item[$array_key]])){return $items[$table][$item[$array_key]];}
  drupal_write_record($table, $item, $key);
  $query = db_select($table, 's')->fields('s', array(
    'id'
  ));
  foreach($key as $key_item){
    $query->condition($key_item, $item[$key_item]);
  }
  $col = $query->execute()->fetchCol();
  $id = array_pop($col);
  $users[$table][$item[$array_key]] = $id;
  return $id;
}