<?php

/**
 * Cron helper function to keep the .module file small.
 */
function _scratchpads_tweaks_cron(){
  // Don't report if we're a "dev" or D6 upgrade site.
  if(substr($_SERVER['HTTP_HOST'], 0, 3) != 's2.' && substr($_SERVER['HTTP_HOST'], 0, 4) != 'dev.' && strpos($_SERVER['HTTP_HOST'], 'training') === FALSE && strpos($_SERVER['HTTP_HOST'], 'taxon.name') === FALSE){
    // Once a day we'll report that we exist to a central repository which can
    // then build a list of all Scratchpads.
    if(variable_get('scratchpad_last_reported', 0) < time() - 86400){ // Seconds in a day
      // Send the update to our central server
      $central_servers = variable_get('scratchpad_central_servers', array(
        'http://dev.scratchpads.eu/'
      ));
      $result = db_query('SELECT COUNT(*) AS count, type FROM node GROUP BY type');
      $nodes = array();
      foreach($result as $row){
        $nodes[$row->type] = $row->count;
      }
      $users = db_query("SELECT COUNT(uid) AS users FROM users WHERE status = 1 AND uid > 1")->fetchField(); //Total users
      $users_login = db_query("SELECT COUNT(uid) AS users FROM users WHERE login != 0")->fetchField(); //Total users logged in
      $users_week = db_query("SELECT COUNT(uid) AS users FROM users WHERE login > UNIX_TIMESTAMP()-604800")->fetchField(); //Week users
      $users_month = db_query("SELECT COUNT(uid) AS users FROM users WHERE login > UNIX_TIMESTAMP()-2592000")->fetchField(); //Month users
      $results = db_query("SELECT mail FROM {users} u, {users_roles} ur WHERE u.uid = ur.uid AND rid = 5");
      foreach($results as $row){
        $mails[] = $row->mail;
      }
      $report_data = array(
        'site_url' => $_SERVER['HTTP_HOST'],
        'site_title' => variable_get('site_name', 'Scratchpad'),
        'site_mission' => variable_get('front_page_welcome_message', ''),
        'nodes' => $nodes,
        'users' => array(
          'login' => $users_login,
          'total' => $users,
          'week' => $users_week,
          'month' => $users_month
        ),
        'views' => 0,
        'maintainer_emails' => $mails,
        'last_login_time' => db_query('SELECT access FROM {users} WHERE uid > 1 ORDER BY access DESC LIMIT 1;')->fetchField(),
        'last_node_changed_time' => db_query('SELECT created FROM {node} ORDER BY created DESC LIMIT 1')->fetchField(),
        'profile' => variable_get('install_profile', 'scratchpad_profile')
      );
      foreach($central_servers as $central_server){
        file_get_contents($central_server . 'report_scratchpad/?data=' . urlencode(serialize($report_data))); // Could run into GET limits here. Ho hum!
      }
      variable_set('scratchpad_last_reported', time());
    }
    if(variable_get('scratchpad_last_updated', 0) < time() - 86400 && !in_array("http://" . $_SERVER['HTTP_HOST'] . "/", variable_get('scratchpad_central_servers', array(
      'http://dev.scratchpads.eu/'
    )))){ // 86400 = seconds in a day
      // Also update the sites list for this site
      $central_servers = variable_get('scratchpad_central_servers', array(
        'http://dev.scratchpads.eu/'
      ));
      $sites = array();
      foreach($central_servers as $central_server){
        $content = file_get_contents($central_server . 'scratchpads/serial');
        if($content){
          $sites = array_merge($sites, unserialize($content));
        }
      }
      variable_set('scratchpad_sites_list', $sites);
      variable_set('scratchpad_last_updated', time());
    }
  }
  // Note, we need to parse the scratchpads_tweaks info file ourselves as it
  // has its dependencies removed to allow required modules to be removed.
  $tweaks_info = drupal_parse_info_file(drupal_get_path('module', 'scratchpads_tweaks') . '/scratchpads_tweaks.info');
  $system_data = system_rebuild_module_data();
  $modules_to_enable = array();
  foreach($tweaks_info['dependencies'] as $module_name){
    if(!module_exists($module_name)){
      $row = db_select('system', 's')->condition('name', $module_name)->fields('s')->execute()->fetch();
      if(!$row || $row->schema_version == -1){
        // This module has never been installed, we install it.
        $modules_to_enable[] = $module_name;
      }else{
        // The module has been installed before, we need to check the module's
        // info file to check if it is a tool and could be disabled.
        if(!isset($system_data[$module_name]->info['tool'])){
          $modules_to_enable[] = $module_name;
        }
      }
    }
  }
  if(count($modules_to_enable)){
    module_enable($modules_to_enable);
    menu_rebuild();
  }
  // Delete certain messages from the watchdog as we know they can be safely
  // ignored.
  db_delete('watchdog')->condition('message', array(
    '%module module enabled.',
    '%module module installed.',
    "Action '%action' added.",
    'There is new syndicated content from %site.',
    'Flushed all files (%count) from static page cache.',
    '@actioning @module_name / @component.',
    '@action completed for @module_name / @component.',
    'Updated site meta data',
    'Fetched site meta data',
    'Expired %count stale files from static page cache.',
    'Cron run completed.',
    '"@openid" logged in as "@user"'
  ))->execute();
  // Rebuild caches
  _scratchpads_tweaks_register_shutdown();
}