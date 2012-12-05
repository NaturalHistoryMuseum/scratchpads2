<?php

function scratchpads_statistics_stats_page_json(){
  // Get the content, and build it.
  return _scratchpads_statistics_build_from_time(isset($_GET['since']) ? $_GET['since'] : 0);
}

function scratchpads_statistics_stats_page_login_json(){
  $stats = array();
  $query = db_select('scratchpads_statistics_login', 'l');
  $query->innerJoin('scratchpads_statistics_site', 's', 'l.site = s.id');
  $query->innerJoin('scratchpads_statistics_user', 'u', 'l.user = u.id');
  $query->innerJoin('scratchpads_statistics_user', 'o', 's.owner = o.id');
  $query->fields('l', array(
    'created',
    'access'
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
  $results = $query->execute();
  foreach($results as $row){
    $stats[] = $row;
  }
  return $stats;
}

function scratchpads_statistics_stats_page(){
  $content = array();
  // The following query will give us a total for each entity/bundle across all
  // of the sites.
  $query = db_select('scratchpads_statistics_data', 'd');
  $query->innerJoin('scratchpads_statistics_entity_bundle', 'e', 'e.id = d.entity_bundle');
  $query->groupBy('entity_bundle');
  $query->fields('e', array(
    'name'
  ));
  $query->addExpression('SUM(number_created)', 'sum_number_created');
  $query->orderBy('sum_number_created', 'DESC');
  $query->condition('captured', time() - 604800, '>');
  $results = $query->execute();
  $counts = array();
  $i = 0;
  $total = 0;
  foreach($results as $row){
    if($i > 8){
      if(!isset($counts['Other'])){
        $counts['Other'] = 0;
      }
      $counts['Other'] += $row->sum_number_created;
    }else{
      $counts[$row->name] = $row->sum_number_created;
    }
    $total += $row->sum_number_created;
    $i++;
  }
  $content[] = array(
    '#markup' => _scratchpads_statistics_chart(t('Content types'), t('Percent'), $counts, t('Scratchpads 2.0 Content (% of !total)', array(
      '!total' => $total
    )))
  );
  // Next we look for the number of
  return $content;
}

/**
 * Simple Google Charts function
 *
 * Returns the HTML wrapper that will contain the chart on a web page.
 */
function _scratchpads_statistics_chart($column_name, $value_name, $values, $title = '', $width = '400', $height = '300'){
  $id = md5(mt_rand());
  $id = "google_chart-$id";
  drupal_add_js('https://www.google.com/jsapi', array(
    'type' => 'external'
  ));
  drupal_add_js(drupal_get_path('module', 'scratchpads_statistics') . '/js/scratchpads_statistics.js');
  drupal_add_js(array(
    'scratchpads_statistics' => array(
      $id => array(
        'column_name' => $column_name,
        'value_name' => $value_name,
        'rows' => $values,
        'title' => $title,
        'width' => $width,
        'height' => $height
      )
    )
  ), array(
    'type' => 'setting'
  ));
  return '<div id="' . $id . '" class="google_chart"></div>';
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