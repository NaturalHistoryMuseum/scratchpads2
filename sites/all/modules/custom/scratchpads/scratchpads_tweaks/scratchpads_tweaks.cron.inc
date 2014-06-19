<?php

/**
 * Cron helper function to keep the .module file small.
 */
function _scratchpads_tweaks_cron(){
  // Don't report if:
  // - URL begins with 's2.'
  // - URL begins with 'dev.', 'dev-', 'dev2-'
  // - URL contains 'training'
  // - URL contains 'taxon.name'
  // - URL contains 'myspecies.info' AND contains more than three '.' delimitted
  //   components
  if(substr($_SERVER['HTTP_HOST'], 0, 3) != 's2.' && substr($_SERVER['HTTP_HOST'], 0, 4) != 'dev.' && substr($_SERVER['HTTP_HOST'], 0, 4) != 'dev-' && substr($_SERVER['HTTP_HOST'], 0, 5) != 'dev2-' && strpos($_SERVER['HTTP_HOST'], 'training') === FALSE && strpos($_SERVER['HTTP_HOST'], 'taxon.name') === FALSE && !(strpos($_SERVER['HTTP_HOST'], 'myspecies.info') && count(explode('.', $_SERVER['HTTP_HOST'])) > 3)){
    // Once a day we'll report that we exist to a central repository which can
    // then build a list of all Scratchpads.
    if(variable_get('scratchpad_last_reported', 0) < time() - 86400){ // Seconds in a day
      // Send the update to our central server
      $central_servers = variable_get('scratchpad_central_servers', array(
        'http://stats.scratchpads.eu/'
      ));
      $result = db_query('SELECT COUNT(*) AS count, type FROM node GROUP BY type');
      $nodes = array();
      foreach($result as $row){
        $nodes[$row->type] = $row->count;
      }
      $users = db_query("SELECT COUNT(uid) AS users FROM users WHERE status = 1 AND uid > 1")->fetchField(); //Total users
      $users_login = db_query("SELECT COUNT(uid) AS users FROM users WHERE login != 0 AND uid > 1")->fetchField(); //Total users logged in
      $users_week = db_query("SELECT COUNT(uid) AS users FROM users WHERE login > UNIX_TIMESTAMP()-604800 AND uid > 1")->fetchField(); //Week users
      $users_month = db_query("SELECT COUNT(uid) AS users FROM users WHERE login > UNIX_TIMESTAMP()-2592000 AND uid > 1")->fetchField(); //Month users
      $results = db_query("SELECT mail FROM {users} u, {users_roles} ur WHERE u.uid = ur.uid AND rid = 5");
      $mails = array();
      foreach($results as $row){
        $mails[] = $row->mail;
      }
      $query = db_select('node_counter', 'n');
      $query->addExpression('SUM(totalcount)', 'sumtotalcount');
      $views = $query->execute()->fetchCol();
      $views = array_pop($views);
      $welcome_message = variable_get('front_page_welcome_message', array(
        'value' => ''
      ));
      $report_data = array(
        'site_url' => $_SERVER['HTTP_HOST'],
        'site_title' => variable_get('site_name', 'Scratchpad'),
        'site_mission' => $welcome_message['value'],
        'nodes' => $nodes,
        'users' => array(
          'login' => $users_login,
          'total' => $users,
          'week' => $users_week,
          'month' => $users_month
        ),
        'views' => $views,
        'maintainer_emails' => $mails,
        'last_login_time' => db_query('SELECT access FROM {users} WHERE uid > 1 ORDER BY access DESC LIMIT 1;')->fetchField(),
        'last_node_changed_time' => db_query('SELECT created FROM {node} ORDER BY created DESC LIMIT 1')->fetchField(),
        'profile' => variable_get('install_profile', 'scratchpad_profile')
      );
      foreach($central_servers as $central_server){
        exec('wget "' . $central_server . 'report_scratchpad?data=' . urlencode(serialize($report_data)) . '" -O /dev/null'); // Could run into GET limits here. Ho hum!
      }
      variable_set('scratchpad_last_reported', time());
    }
    if(variable_get('scratchpad_last_updated', 0) < time() - 86400 && !in_array("http://" . $_SERVER['HTTP_HOST'] . "/", variable_get('scratchpad_central_servers', array(
      'http://stats.scratchpads.eu/'
    )))){ // 86400 = seconds in a day
      // Also update the sites list for this site
      $central_servers = variable_get('scratchpad_central_servers', array(
        'http://stats.scratchpads.eu/'
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
  if(variable_get('standard_scratchpad', TRUE)){
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
  }
  // If we are using memcache, we truncate the cache tables to save space and to
  // also reduce database backup times.
  global $conf;
  if(false){
    if($conf['cache_default_class'] == 'MemCacheDrupal'){
      $results = db_query('SHOW TABLES');
      foreach($results as $row){
        foreach($row as $table_name){
          if(substr($table_name, 0, 5) == 'cache' && variable_get('cache_class_' . $table_name, FALSE) != 'DrupalDatabaseCache'){
            db_delete($table_name)->execute();
          }
        }
      }
    }
  }
  // Clean up the file_display table now that we have removed its primary key
  db_query('CREATE TEMPORARY TABLE file_display_temp AS SELECT DISTINCT * FROM {file_display}; TRUNCATE {file_display}; INSERT INTO {file_display} SELECT * FROM file_display_temp;');
  // Rebuild caches
  _scratchpads_tweaks_register_shutdown();
  // Mail the team if the site looks like it's not being used.
  $account = user_load(2);
  if($account && $account->mail != 'scratchpad@nhm.ac.uk'){
    $time = time();
    if(!$account->login && variable_get('install_time', 0) + 604800 < $time && $time - variable_get('scratchpads_tweaks_account_notify', 0) > 604800){
      variable_set('scratchpads_tweaks_account_notify', $time);
      _user_mail_notify('register_no_approval_required', $account);
    }
    // If the site has been around for over three months, we check that it is
    // active.  If it isn't, we notify ourselves to ping the owner.
    // 15778800
    $query = db_select('users');
    $query->addExpression('MAX(login)', 'lastlogin');
    $login = $query->execute()->fetchField();
    $norecentlogin = FALSE;
    if($login + 15778800 < $time){
      $norecentlogin = TRUE;
    }
    $login = $login ? format_date($login) : t('Never');
    if($time - variable_get('scratchpads_tweaks_site_active_check_time', 0) > 8000000){
      if((variable_get('install_time', 0) + 15778800 < $time && $norecentlogin) || (variable_get('install_time', 0) + 8000000 < $time && !variable_get('scratchpads_tweaks_site_active', FALSE))){
        // Get a count of the number of nodes, if we're less than 5, we ping a
        // message
        if(($num_nodes = db_select('node')->condition('type', 'biblio', '!=')->countQuery()->execute()->fetchField()) < 10 || $norecentlogin){
          $query = db_select('node');
          $query->addExpression('MAX(changed)', 'lastedit');
          $lastedit = $query->execute()->fetchField();
          $lastedit = $lastedit ? format_date($lastedit) : t('Never');
          global $language;
          variable_set('scratchpads_tweaks_site_active_check_time', $time);
          /*drupal_mail('scratchpads_tweaks', 'nominate-for-deletion', 'Scratchpads Support Team <support@scratchpads.eu>', $language, array(
            'nodes' => $num_nodes,
            'login' => $login,
            'edit' => $lastedit
          ));*/
        }else{
          variable_set('scratchpads_tweaks_site_active', TRUE);
        }
      }
    }
  }
  // Delete files from the temporary directory (assuming the path is different
  // to the files folder) if they're overy one month old.
  $temp_folder = drupal_realpath('temporary://');
  if($temp_folder != '/tmp' && $temp_folder != drupal_realpath('public://') && $temp_folder != drupal_realpath('private://')){
    foreach(scandir($temp_folder) as $file){
      if(is_file("$temp_folder/$file") && filemtime("$temp_folder/$file") + 2419200 < time()){
        file_unmanaged_delete("$temp_folder/$file");
        db_delete('file_managed')->condition('uri', 'temporary://' . $file)->execute();
      }
    }
  }
  // Remove any "batch" objects older than one week (they have almost definitely
  // failed).
  db_delete('batch')->condition('timestamp', time() - 604800, '<')->execute();
}