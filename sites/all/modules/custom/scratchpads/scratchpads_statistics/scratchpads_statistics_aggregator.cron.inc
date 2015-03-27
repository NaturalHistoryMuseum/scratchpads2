<?php
define('SCRATCHPAD_ORGANIZATION', '315b3c03-4a0a-424e-83a5-d25aa748e666');

/**
 * Weekly cron
 *
 * Performs functions once a week. Non-essential tasks, like updating nodes/
 * organizations/scratchpads should be placed here.
 *
 * FIXME - Not sure what the "tag" is going to be for a "scratchpad", although
 * it is likely to be "scratchpad". This needs checking.
 */
function _scratchpads_statistics_aggregator_cron($execution_time = 60){
  $gbif_registry = GBIFRegistry::singleton();
  // foreach($gbif_registry->getTechnicalInstallations('scratchpad') as
  // $technical_installation){
  // Start a timer so that we don't go over 60 seconds.
  timer_start('scratchpads_statistics_aggregator');
  $urls = array();
  foreach($gbif_registry->getTechnicalInstallations() as $ti){
    // Get this site from the list of sites.
    //$technical_installation = $technical_installation->getAsArray();
    $organization = $ti->getHostingOrganization();
    if($organization->getKey() == SCRATCHPAD_ORGANIZATION){
      // We haven't actually registered an endpoint for each of the Scratchpads,
      // so for now will assume the location of the end point (this will only
      // be an issue when hosting of sites external to the NHM becomes more
      // prevalent).
      $url = $ti->getHomePage();
      if(!strpos($url, '/s2.') && !strpos($url, 'taxon.name') && !strpos($url, 'training') && !strpos($url, 'sandbox') && !strpos($url, '/dev.') && !strpos($url, '/dev-')){
        $urls[$url] = $url;
      }
    }
  }
  // Random order the URLs so that we stand a chance of getting all of the sites
  // over a six day period.
  shuffle($urls);
  $worked_on_urls = array();
  foreach($urls as $url){
    $worked_on_urls[] = $url;
    // Whoa, we're taking too long, we should stop!
    if(timer_read('scratchpads_statistics_aggregator') > $execution_time * 1000){
      break;
    }
    // Get the greater captured value for this Scratchpad
    $query = db_select('scratchpads_statistics_data', 'd');
    $query->innerJoin('scratchpads_statistics_site', 's', 'd.site = s.id');
    $row = $query->fields('d', array(
      'captured'
    ))->condition('url', $url)->range(0, 1)->orderBy('captured', 'DESC')->execute()->fetch();
    $last_capture = 0;
    if($row){
      // We have harvested this site before, all we need is a most recent
      // capture value.
      $last_capture = $row->captured;
      // If the last_captured value is less than six days old, then we don't
      // need to go any further.
      if($last_capture > time() - 518400){
        continue;
      }
    }
    // Retrieve the login data for this site, and enter that in to the tables.
    $data = _scratchpads_statistics_aggregator_get_data($url . 'login.json');
    if($data && is_array($data) && count($data)){
      _scratchpads_statistics_aggregator_insert_login_data($data, $url);
      // Get the full statistics for this site, and tner that in to the tables.
      // FIXME - Need to add the hash/security if we need to use it.
      $data = _scratchpads_statistics_aggregator_get_data($url . 'stats.json?since=' . $last_capture);
      if($data){
        if(is_array($data) && count($data)){
          _scratchpads_statistics_aggregator_insert_data($data, $url);
        }
      }else{
        watchdog('scratchpads_statistics', 'Failed to get statistics data from the Scratchpads site at !url', array(
          '!url' => $url
        ), WATCHDOG_WARNING);
      }
    }else{
      watchdog('scratchpads_statistics', 'Failed to get login data from the Scratchpads site at !url', array(
        '!url' => $url
      ), WATCHDOG_WARNING);
    }
  }
  watchdog('scratchpads_statistics', 'Aggregated: !urls', array(
    '!urls' => implode(', ', $worked_on_urls)
  ), WATCHDOG_INFO);
}

/**
 * Simple drupal_http_request without following redirects as we can end up with
 * duplicate records if we do.
 */
function _scratchpads_statistics_aggregator_get_data($url){
  $request = drupal_http_request($url, array(
    'max_redirects' => 0,
    'timeout' => 5
  ));
  $data = FALSE;
  if($request->code == 200){
    $data = @json_decode($request->data);
  }
  return $data;
}

/**
 * Insert the login data into the database.
 */
function _scratchpads_statistics_aggregator_insert_login_data($data, $url){
  $query = db_insert('scratchpads_statistics_login')->fields(array(
    'site',
    'user',
    'created',
    'access'
  ));
  $site = FALSE;
  foreach($data as $record){
    // For sanity sake, we ensure that the URL in the data record matches the
    // URL we are expecting.
    if($url != $record['url']){
      continue;
    }
    if(!$site){
      // For one set of data, we always have the same site, so we can avoid
      // checking it for every record.
      $user = scratchpads_statistics_insert_item(array(
        'name' => $record['owner_name'],
        'email' => $record['owner_email'],
        'country' => $record['owner_country'],
        'latitude' => $record['owner_latitude'],
        'longitude' => $record['owner_longitude']
      ), 'scratchpads_statistics_user', 'email');
      $site = scratchpads_statistics_insert_item(array(
        'title' => $record['title'],
        'owner' => $user,
        'url' => $record['url'],
        'created' => $record['site_created']
      ), 'scratchpads_statistics_site', 'url');
      if(count($record['categories'])){
        $sites_types_map = array();
        db_delete('scratchpads_statistics_site_site_type')->condition('site', $site)->execute();
        $query2 = db_insert('scratchpads_statistics_site_site_type')->fields(array(
          'site',
          'site_type'
        ));
        foreach($record['categories'] as $category){
          $query2->values(array(
            'site_type' => scratchpads_statistics_insert_item(array(
              'type' => $category
            ), 'scratchpads_statistics_site_type', 'type'),
            'site' => $site
          ));
        }
        $query2->execute();
      }
    }
    $user = scratchpads_statistics_insert_item(array(
      'name' => $record['name'],
      'email' => $record['email'],
      'country' => $record['country'],
      'latitude' => $record['latitude'],
      'longitude' => $record['longitude']
    ), 'scratchpads_statistics_user', 'email');
    $query->values(array(
      'site' => $site,
      'user' => $user,
      'created' => $record['created'],
      'access' => $record['access']
    ));
  }
  // First we delete from scratchpads_statistics_login based on the site.
  db_delete('scratchpads_statistics_login')->condition('site', $site)->execute();
  $query->execute();
}

/**
 * Insert the data into the database.
 */
function _scratchpads_statistics_aggregator_insert_data($data, $url){
  $query = db_insert('scratchpads_statistics_data')->fields(array(
    'user',
    'term',
    'site',
    'entity_bundle',
    'number_created',
    'number_edited',
    'number_views',
    'captured'
  ));
  $site = FALSE;
  foreach($data as $record){
    // For sanity sake, we ensure that the URL in the data record matches the
    // URL we are expecting.
    if($url != $record['url']){
      continue;
    }
    if(!$site){
      // For one set of data, we always have the same site, so we can avoid
      // checking it for every record.
      $user = scratchpads_statistics_insert_item(array(
        'name' => $record['owner_name'],
        'email' => $record['owner_email'],
        'country' => $record['owner_country'],
        'latitude' => $record['owner_latitude'],
        'longitude' => $record['owner_longitude']
      ), 'scratchpads_statistics_user', 'email');
      $site = scratchpads_statistics_insert_item(array(
        'title' => $record['title'],
        'owner' => $user,
        'url' => $record['url'],
        'created' => $record['created']
      ), 'scratchpads_statistics_site', 'url');
      if(count($record['categories'])){
        $sites_types_map = array();
        db_delete('scratchpads_statistics_site_site_type')->condition('site', $site)->execute();
        $query2 = db_insert('scratchpads_statistics_site_site_type')->fields(array(
          'site',
          'site_type'
        ));
        foreach($record['categories'] as $category){
          $query2->values(array(
            'site_type' => scratchpads_statistics_insert_item(array(
              'type' => $category
            ), 'scratchpads_statistics_site_type', 'type'),
            'site' => $site
          ));
        }
        $query2->execute();
      }
    }
    $user = scratchpads_statistics_insert_item(array(
      'name' => $record['name'],
      'email' => $record['email'],
      'country' => $record['country'],
      'latitude' => $record['latitude'],
      'longitude' => $record['longitude']
    ), 'scratchpads_statistics_user', 'email');
    $term = 0;
    if($record['term']){
      $term = scratchpads_statistics_insert_item(array(
        'term' => $record['term']
      ), 'scratchpads_statistics_term', 'term');
    }
    $entity_bundle = scratchpads_statistics_insert_item(array(
      'entity' => $record['entity'],
      'bundle' => $record['bundle'],
      'name' => $record['bundle_label']
    ), 'scratchpads_statistics_entity_bundle', array(
      'entity',
      'bundle'
    ));
    $query->values(array(
      'user' => $user,
      'term' => $term,
      'site' => $site,
      'entity_bundle' => $entity_bundle,
      'number_created' => $record['number_created'],
      'number_edited' => $record['number_edited'],
      'number_views' => $record['number_views'],
      'captured' => isset($record['captured']) ? $record['captured'] : time()
    ));
  }
  $query->execute();
}
