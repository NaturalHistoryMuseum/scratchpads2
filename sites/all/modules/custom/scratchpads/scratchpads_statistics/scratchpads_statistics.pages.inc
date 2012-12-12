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
    'url'
  ));
  $query->addField('s', 'created', 'site_created');
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
  $data = array(
    array(
      t('Content types'),
      t('Percent')
    )
  );
  foreach($counts as $title => $value){
    $data[] = array(
      $title,
      (integer)$value
    );
  }
  $content[] = array(
    '#theme' => 'simple_google_chart',
    '#type' => 'PieChart',
    '#data' => $data,
    '#title' => t('Content distribution (percent of !total)', array(
      '!total' => $total
    )),
    '#options' => array(
      'is3D' => TRUE,
      'height' => 400
    )
  );
  // Next we look for the number of users that have logged in in the past week,
  // and then the number of users that have logged in in the past month.
  //
  // Users
  //
  // Total
  $query = db_select('scratchpads_statistics_login', 'l')->countQuery();
  $result = $query->execute()->fetchAssoc();
  $total_users = (integer)array_pop($result);
  // Last year 31536000 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 31536000, '>')->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_year = (integer)array_pop($result);
  // Last month 2629740 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 2629740, '>')->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_month = (integer)array_pop($result);
  // Last week 604800 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 604800, '>')->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_week = (integer)array_pop($result);
  //
  // Sites
  //
  // Total
  $query = db_select('scratchpads_statistics_login', 'l');
  $query->addExpression('COUNT(DISTINCT site)');
  $result = $query->execute()->fetchAssoc();
  $total_users_site = (integer)array_pop($result);
  // Last year 31536000 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 31536000, '>');
  $query->addExpression('COUNT(DISTINCT site)');
  $result = $query->execute()->fetchAssoc();
  $last_year_site = (integer)array_pop($result);
  // Last month 2629740 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 2629740, '>');
  $query->addExpression('COUNT(DISTINCT site)');
  $result = $query->execute()->fetchAssoc();
  $last_month_site = (integer)array_pop($result);
  // Last week 604800 seconds
  $query = db_select('scratchpads_statistics_login', 'l')->condition('access', time() - 604800, '>');
  $query->addExpression('COUNT(DISTINCT site)');
  $result = $query->execute()->fetchAssoc();
  $last_week_site = (integer)array_pop($result);
  $content[] = array(
    '#theme' => 'simple_google_chart',
    '#type' => 'BarChart',
    '#title' => t('User activity'),
    '#data' => array(
      array(
        'Time period',
        'Active users',
        'Active sites'
      ),
      array(
        'Last week',
        $last_week,
        $last_week_site
      ),
      array(
        'Last month',
        $last_month,
        $last_month_site
      ),
      array(
        'Last year',
        $last_year,
        $last_year_site
      ),
      array(
        'Ever',
        $total_users,
        $total_users_site
      )
    )
  );
  return $content;
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