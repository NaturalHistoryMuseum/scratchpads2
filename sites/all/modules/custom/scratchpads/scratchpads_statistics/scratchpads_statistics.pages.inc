<?php

function scratchpads_statistics_stats_page_json(){
  // Get the content, and build it.
  return _scratchpads_statistics_build_from_time(isset($_GET['since']) ? $_GET['since'] : 0);
}

function scratchpads_statistics_stats_page(){
  // No idea yet!
  return array();
}

/**
 * Build the stats for the required time period
 */
function _scratchpads_statistics_build_from_time($time){
  $stats = array();
  $query = db_select('scratchpads_statistics_data', 'd');
  $query->innerJoin('scratchpads_statistics_site', 's', 'd.site = s.id');
  $query->innerJoin('scratchpads_statistics_user', 'u', 'd.user = u.id');
  $query->leftJoin('scratchpads_statistics_term', 't', 'd.term = t.id');
  $query->innerJoin('scratchpads_statistics_entity_bundle', 'e', 'd.entity_bundle = e.id');
  $query->innerJoin('scratchpads_statistics_user', 'o', 's.owner = o.id');
  $query->fields('d', array(
    'number_created',
    'number_edited',
    'number_views',
    'captured'
  ));
  $query->fields('s', array(
    'title',
    'url',
    'created'
  ));
  $query->addField('o', 'name', 'owner_name');
  $query->addField('o', 'email', 'owner_email');
  $query->addField('o', 'country', 'owner_country');
  $query->addField('o', 'latitude', 'owner_latitude');
  $query->addField('o', 'longitude', 'owner_longitude');
  $query->fields('u', array(
    'name',
    'email',
    'country',
    'latitude',
    'longitude'
  ));
  $query->fields('t', array(
    'term'
  ));
  $query->fields('e', array(
    'entity',
    'bundle'
  ));
  $query->addField('e', 'name', 'bundle_label');
  $query->condition('s.id', 1)->condition('captured', $time, '>');
  $results = $query->execute();
  foreach($results as $row){
    $stats[] = $row;
  }
  return $stats;
}

/**
 * Simple configuration form.
 */
function scratchapds_statistics_config_page(){
  return system_settings_form(array(
    'scratchpads_statistics_hash' => array(
      '#title' => t('Hash'),
      '#type' => 'textfield',
      '#default_value' => variable_get('scratchpads_statistics_hash', ''),
      '#description' => t('Enter the hash required to access this site.')
    )
  ));
}