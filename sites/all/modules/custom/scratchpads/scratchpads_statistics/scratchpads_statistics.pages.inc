<?php
define('SCRATCHPADS_STATISTICS_POINT_SIZE', 5);

/**
 * Autocomplete callback
 * @return multitype:unknown
 */
function scratchpads_statistics_autocomplete($type, $string){
  $return = array();
  if(in_array($type, array(
    'site',
    'user',
    'term'
  ))){
    switch($type){
      case 'site':
        $condition = db_or()->condition('title', '%' . db_like($string) . '%', 'LIKE')->condition('url', '%' . db_like($string) . '%', 'LIKE');
        $table = 'scratchpads_statistics_site';
        $field_expression = "CONCAT(title, ' (', url, ')')";
        $order = 'title';
        break;
      case 'user':
        $condition = db_or()->condition('name', '%' . db_like($string) . '%', 'LIKE')->condition('email', db_like($string) . '%', 'LIKE');
        $table = 'scratchpads_statistics_user';
        $field_expression = 'name';
        $order = 'name';
        break;
      case 'term':
        $condition = db_and()->condition('term', '%' . db_like($string) . '%', 'LIKE');
        $table = 'scratchpads_statistics_term';
        $field_expression = 'term';
        $order = 'term';
        break;
    }
    $query = db_select($table, 't')->condition($condition)->fields('t')->orderBy($order)->range(0, 20);
    $query->addExpression($field_expression, 'matches');
    $results = $query->execute();
    foreach($results as $row){
      $return[$row->matches] = $row->matches;
    }
  }
  drupal_json_output($return);
}

/**
 * 
 * @return multitype:unknown
 */
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
  $categories = scratchpads_statistics_get_site_categories();
  foreach($results as $row){
    $row->categories = $categories;
    $stats[] = $row;
  }
  return $stats;
}

/**
 * FIXME Need to add CACHING.
 */
function scratchpads_statistics_stats_page(){
  $content = array(
    '#prefix' => '<div class="scratchpads-statistics-wrapper">',
    '#suffix' => '</div>'
  );
  $site = FALSE;
  $user = FALSE;
  // Filter by a category, which will add a site query.  This can then be
  // overridden by the site query below.
  if(isset($_GET['category'])){
    $sites_with_category = db_select('scratchpads_statistics_site_site_type', 's')->fields('s', array(
      'site'
    ))->condition('site_type', $_GET['category'])->execute()->fetchCol();
    $site = db_or();
    foreach($sites_with_category as $site_with_category){
      $site->condition('d.site', $site_with_category);
    }
  }
  // Filter the data by this site.
  if(isset($_GET['site'])){
    $site = db_or();
    foreach($_GET['site'] as $site_id){
      $site->condition('d.site', $site_id);
    }
  }
  // Filter the data by a term.
  $term = db_or();
  if(isset($_GET['term'])){
    foreach($_GET['term'] as $term_id){
      $term->condition('d.term', $term_id);
    }
  }else{
    $term->condition('d.term', 0);
  }
  // Filter the data by a user.
  if(isset($_GET['user'])){
    $user = db_or();
    foreach($_GET['user'] as $user_id){
      $user->condition('d.user', $user_id);
    }
  }
  // Objects ------------------------------------------------------------------
  $query = db_select('scratchpads_statistics_data', 'd')->fields('d', array(
    'captured',
    'entity_bundle',
    'site'
  ))->condition($term);
  $query->groupBy('entity_bundle');
  $query->groupBy('captured');
  $query->orderBy('captured', 'ASC');
  $query->addExpression('SUM(d.number_created)', 'sum_created');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $results = $query->execute();
  $site_month_object_counts = array();
  foreach($results as $row){
    $site_month_object_counts[$row->site][$row->entity_bundle][date('Y', $row->captured) . "/" . date('n', $row->captured)] = $row->sum_created;
  }
  foreach($site_month_object_counts as $site_array){
    foreach($site_array as $data){
      foreach($data as $key => $value){
        if(!isset($month_counts[$key])){
          $month_counts[$key] = array(
            'objects' => 0,
            'edits' => 0,
            'views' => 0
          );
        }
        $month_counts[$key]['objects'] += $value;
      }
    }
  }
  // Edits --------------------------------------------------------------------
  $query = db_select('scratchpads_statistics_data', 'd')->fields('d', array(
    'captured',
    'entity_bundle',
    'site'
  ))->condition($term);
  $query->groupBy('entity_bundle');
  $query->groupBy('site');
  $query->groupBy('captured');
  $query->orderBy('captured', 'ASC');
  $query->addExpression('SUM(d.number_edited)', 'sum_created');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $results = $query->execute();
  $site_month_object_counts = array();
  foreach($results as $row){
    $site_month_object_counts[$row->site][$row->entity_bundle][date('Y', $row->captured) . "/" . date('n', $row->captured)] = $row->sum_created;
  }
  foreach($site_month_object_counts as $site_array){
    foreach($site_array as $data){
      foreach($data as $key => $value){
        if(!isset($month_counts[$key])){
          $month_counts[$key] = array(
            'objects' => 0,
            'edits' => 0,
            'views' => 0
          );
        }
        $month_counts[$key]['edits'] += $value;
      }
    }
  }
  // Views --------------------------------------------------------------------
  $query = db_select('scratchpads_statistics_data', 'd')->fields('d', array(
    'captured',
    'entity_bundle',
    'site'
  ))->condition($term);
  $query->groupBy('entity_bundle');
  $query->groupBy('captured');
  $query->orderBy('captured', 'ASC');
  $query->addExpression('SUM(d.number_views)', 'sum_created');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $results = $query->execute();
  $site_month_object_counts = array();
  foreach($results as $row){
    $site_month_object_counts[$row->site][$row->entity_bundle][date('Y', $row->captured) . "/" . date('n', $row->captured)] = $row->sum_created;
  }
  foreach($site_month_object_counts as $site_array){
    foreach($site_array as $data){
      foreach($data as $key => $value){
        if(!isset($month_counts[$key])){
          $month_counts[$key] = array(
            'objects' => 0,
            'edits' => 0,
            'views' => 0
          );
        }
        $month_counts[$key]['views'] += $value;
      }
    }
  }
  // We pop the last month if it has gone down SIGNIFICANTLY (>5%), as that
  // means we're probably missing some statistics
  $last_month_counts = end($month_counts);
  $previous_month_counts = prev($month_counts);
  reset($month_counts);
  if($last_month_counts['objects'] < (0.95 * $previous_month_counts['objects'])){
    array_pop($month_counts);
  }
  $data_objects = array(
    array(
      t('Year/Month'),
      t('Objects')
    )
  );
  $data_users = array(
    array(
      t('Year/Month'),
      t('Users')
    )
  );
  $data_views = array(
    array(
      t('Year/Month'),
      t('Views')
    )
  );
  foreach($month_counts as $key => $value){
    $data_objects[] = array(
      $key,
      $value['objects']
    );
    $data_users[] = array(
      $key,
      $value['users']
    );
    $data_views[] = array(
      $key,
      $value['views']
    );
  }
  if(isset($data_objects[2])){
    $content[] = array(
      '#theme' => 'simple_google_chart',
      '#type' => 'LineChart',
      '#data' => $data_objects,
      '#title' => t('Content'),
      '#subtitle' => t('The number of different objects in Scratchpads (incl. taxon descriptions, bibliographic entries, localities, specimens and images)'),
      '#options' => array(
        'pointSize' => SCRATCHPADS_STATISTICS_POINT_SIZE,
        'legend' => array(
          'position' => 'none'
        )
      )
    );
  }
  if(!$user){
    // Users -------------------------------------------------------------------
    $query = db_select('scratchpads_statistics_login', 'd')->condition('access', 0, '>')->fields('d', array(
      'created'
    ))->orderBy('created');
    if($site){
      $query->condition($site);
    }
    $createds = $query->execute()->fetchCol();
    $user_counts = array();
    foreach($createds as $created){
      if($created){
        if(!isset($user_counts[date('Y', $created) . "/" . date('n', $created)])){
          $user_counts[date('Y', $created) . "/" . date('n', $created)] = end($user_counts);
        }
        $user_counts[date('Y', $created) . "/" . date('n', $created)] += 1;
      }
    }
    $user_counts_table = array(
      array(
        t('Year/Month'),
        t('Users')
      )
    );
    foreach($user_counts as $month => $count){
      $user_counts_table[] = array(
        $month,
        $count
      );
    }
    $content[] = array(
      '#theme' => 'simple_google_chart',
      '#type' => 'LineChart',
      '#data' => $user_counts_table,
      '#title' => t('Active users'),
      '#subtitle' => t('Registered users logged-in at least once to their Scratchpad sites'),
      '#options' => array(
        'pointSize' => 2,
        'legend' => array(
          'position' => 'none'
        )
      )
    );
  }
  if(isset($data_views[2]) && $value['views']){
    $content[] = array(
      '#theme' => 'simple_google_chart',
      '#type' => 'LineChart',
      '#data' => $data_views,
      '#title' => t('Views'),
      '#subtitle' => t('The number of times Scratchpads objects have been viewed'),
      '#options' => array(
        'pointSize' => SCRATCHPADS_STATISTICS_POINT_SIZE,
        'legend' => array(
          'position' => 'none'
        )
      ),
      'note_about_views' => array(
        '#markup' => '<p class="scratchpads-statistics-note">' . t('The number of times content has been viewed. Views records are available from April 2013 onward.') . '</p>'
      )
    );
  }
  if(!$site && !$user){
    // Sites --------------------------------------------------------------------
    $query = db_select('scratchpads_statistics_site', 's')->fields('s', array(
      'created'
    ))->orderBy('created');
    $createds = $query->execute()->fetchCol();
    $site_counts = array();
    foreach($createds as $created){
      if($created){
        if(!isset($site_counts[date('Y', $created) . "/" . date('n', $created)])){
          $site_counts[date('Y', $created) . "/" . date('n', $created)] = end($site_counts);
        }
        $site_counts[date('Y', $created) . "/" . date('n', $created)] += 1;
      }
    }
    $site_counts_table = array(
      array(
        t('Year/Month'),
        t('Sites')
      )
    );
    foreach($site_counts as $month => $count){
      $site_counts_table[] = array(
        $month,
        $count
      );
    }
    $content[] = array(
      '#theme' => 'simple_google_chart',
      '#type' => 'LineChart',
      '#data' => $site_counts_table,
      '#title' => t('Sites'),
      '#subtitle' => t('The number of Scratchpad sites'),
      '#options' => array(
        'pointSize' => SCRATCHPADS_STATISTICS_POINT_SIZE,
        'legend' => array(
          'position' => 'none'
        )
      )
    );
  }
  // Bar chart of entities ----------------------------------------------------
  // The following query will give us a total for each entity/bundle across all
  // of the sites.
  // Create a temporary table first which contains the sites and the max 
  // captured value for each.
  $temp_query = db_select('scratchpads_statistics_data', 'd');
  $temp_query->addExpression('MAX(captured)', 'captured');
  $temp_query->fields('d', array(
    'site'
  ))->groupBy('site');
  if($site){
    $temp_query->condition($site);
  }
  if($user){
    $temp_query->condition($user);
  }
  $temp_table_name = db_query_temporary((string)$temp_query, $temp_query->getArguments());
  // Now join to that table.
  $query = db_select('scratchpads_statistics_data', 'd')->condition($term);
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $query->innerJoin($temp_table_name, 't', 't.site = d.site AND t.captured = d.captured');
  $query->innerJoin('scratchpads_statistics_entity_bundle', 'e', 'e.id = d.entity_bundle');
  $query->groupBy('entity_bundle');
  $query->fields('e', array(
    'name'
  ));
  $query->addExpression('SUM(number_created)', 'sum_number_created');
  $query->addExpression('SUM(number_edited)', 'sum_number_edited');
  $query->addExpression('SUM(number_views)', 'sum_number_views');
  $query->orderBy('sum_number_created', 'DESC');
  $results = $query->execute();
  $counts = array();
  $i = 0;
  $eight_count = PHP_INT_MAX;
  $data = array(
    array(
      t('Content types'),
      t('Objects'),
      //t('Edited'),
      t('Views')
    )
  );
  $max_value = FALSE;
  $all_views_zero = TRUE;
  foreach($results as $row){
    // PieChart stuff
    if($i >= 8 && $row->sum_number_created < $eight_count){
      if($i == 8){
        $eight_count = $row->sum_number_created;
        $counts[$row->name] = $row->sum_number_created;
      }else{
        if(!isset($counts['Other'])){
          $counts['Other'] = 0;
        }
        $counts['Other'] += $row->sum_number_created;
      }
    }else{
      $counts[$row->name] = $row->sum_number_created;
    }
    $i++;
    // BarChart stuff.
    if(!$max_value){
      $max_value = (integer)$row->sum_number_created + (integer)$row->sum_number_edited;
    }
    if(((integer)$row->sum_number_created + (integer)$row->sum_number_edited) / $max_value > 0.03){
      if((integer)$row->sum_number_views){
        $all_views_zero = FALSE;
      }
      $data[] = array(
        $row->name,
        (integer)$row->sum_number_created,
        //(integer)$row->sum_number_edited,
        (integer)$row->sum_number_views
      );
    }
  }
  if(isset($data[2])){
    if($all_views_zero){
      foreach(array_keys($data) as $key){
        // Unset the Views column
        unset($data[$key][2]);
      }
      $content[] = array(
        'graph' => array(
          '#theme' => 'simple_google_chart',
          '#type' => 'ColumnChart',
          '#data' => $data,
          '#title' => t('Objects'),
          '#options' => array(
            'bar' => array(
              'groupWidth' => '80%'
            ),
            'height' => '400',
            'vAxes' => array(
              array(
                'title' => 'Objects',
                'baseline' => 0
              )
            )
          )
        )
      );
    }else{
      $content[] = array(
        'graph' => array(
          '#theme' => 'simple_google_chart',
          '#type' => 'ColumnChart',
          '#data' => $data,
          '#title' => t('Objects and Views'),
          '#options' => array(
            'bar' => array(
              'groupWidth' => '80%'
            ),
            'height' => '400',
            'vAxes' => array(
              array(
                'title' => 'Objects',
                'baseline' => 0
              ),
              array(
                'title' => 'Views',
                'baseline' => 0
              )
            ),
            'series' => array(
              0 => array(
                'targetAxisIndex' => 0
              ),
              1 => array(
                'targetAxisIndex' => 1
              )
            )
          )
        ),
        'note_about_views' => array(
          '#markup' => '<p class="scratchpads-statistics-note">' . t('Note, view counts are only recorded for "Nodes".') . '</p>'
        )
      );
    }
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
  if(isset($data[2])){
    $content[] = array(
      '#theme' => 'simple_google_chart',
      '#type' => 'PieChart',
      '#data' => $data,
      '#title' => t('Distribution'),
      '#options' => array(
        'is3D' => TRUE,
        'height' => 400
      )
    );
  }
  // Next we look for the number of users that have logged in in the past week,
  // and then the number of users that have logged in in the past month.
  //
  // Users
  //
  // Total
  $query = db_select('scratchpads_statistics_login', 'd')->condition('access', 0, '>');
  if($site){
    $query->condition($site);
  }
  $query = $query->countQuery();
  $result = $query->execute()->fetchAssoc();
  $total_users = (integer)array_pop($result);
  // Last year 31536000 seconds
  $query = db_select('scratchpads_statistics_login', 'd')->condition('access', time() - 31536000, '>');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $query = $query->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_year = (integer)array_pop($result);
  // Last month 2629740 seconds
  $query = db_select('scratchpads_statistics_login', 'd')->condition('access', time() - 2629740, '>');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $query = $query->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_month = (integer)array_pop($result);
  // Last week 604800 seconds
  $query = db_select('scratchpads_statistics_login', 'd')->condition('access', time() - 604800, '>');
  if($site){
    $query->condition($site);
  }
  if($user){
    $query->condition($user);
  }
  $query = $query->countQuery();
  $result = $query->execute()->fetchAssoc();
  $last_week = (integer)array_pop($result);
  if(!$user){
    if($site){
      $options = array();
      if($total_users > 10){
        $options = array(
          'hAxis' => array(
            'logScale' => TRUE
          )
        );
      }
      $content[] = array(
        '#theme' => 'simple_google_chart',
        '#type' => 'BarChart',
        '#title' => t('Users'),
        '#options' => $options,
        '#data' => array(
          array(
            'Time period',
            'Active users'
          ),
          array(
            'Last week',
            $last_week
          ),
          array(
            'Last month',
            $last_month
          ),
          array(
            'Last year',
            $last_year
          ),
          array(
            'Ever',
            $total_users
          )
        )
      );
    }else{
      //
      // Sites
      //
      // Total
      $query = db_select('scratchpads_statistics_login', 'l')->condition('access', 0, '>');
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
      $options = array();
      if($total_users > 10){
        $options = array(
          'hAxis' => array(
            'logScale' => TRUE,
            'title' => t('Log')
          )
        );
      }
      $content[] = array(
        'graph' => array(
          '#theme' => 'simple_google_chart',
          '#type' => 'BarChart',
          '#title' => t('Number of Active Users/Sites'),
          '#options' => $options,
          '#data' => array(
            array(
              'Time period',
              'Users',
              'Sites'
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
        ),
        'note_about_views' => array(
          '#markup' => '<p class="scratchpads-statistics-note">' . t('A site is understood to be active if a registered user signs in to modify the site’s content. The action of signing in defines a user as active. Visitors browsing the site, i.e. without logging in, are not counted in these figures. These figures show that the dominant pattern of activity is for registered users to update their sites periodically.') . '</p>'
        )
      );
    }
  }
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
  $categories = scratchpads_statistics_get_site_categories();
  foreach($results as $row){
    $row->categories = $categories;
    $stats[] = $row;
  }
  return $stats;
}

/**
 * Simple configuration form.
 */
function scratchpads_statistics_config_page(){
  return system_settings_form(array(
    'scratchpads_statistics_hash' => array(
      '#title' => t('Hash'),
      '#type' => 'textfield',
      '#default_value' => variable_get('scratchpads_statistics_hash', ''),
      '#description' => t('Enter the hash required to access this site.')
    )
  ));
}
