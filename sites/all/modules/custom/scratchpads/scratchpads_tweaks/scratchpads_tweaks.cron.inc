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
  // Note, we need to parse the scratchpads_tweaks info file ourselves as it
  // has its dependencies removed to allow required modules to be removed.
  if(variable_get('standard_scratchpad', TRUE)){
    // Ensure we do not send out update emails to our users.
    variable_del('update_notify_emails');
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
  // Disable the dblog module, as we occasionaly forget to disable it after
  // enabling it to debug issues with live sites.
  module_disable(array(
    'dblog'
  ));
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
  db_query('CREATE TABLE {file_display_temp} AS SELECT DISTINCT * FROM {file_display};');
  db_query('TRUNCATE {file_display};');
  db_query('INSERT INTO {file_display} SELECT * FROM {file_display_temp};');
  db_query('DROP TABLE {file_display_temp}');
  // Delete files from the temporary directory (assuming the path is different
  // to the files folder) if they're over one month old.
  $temp_folder = drupal_realpath('temporary://');
  if($temp_folder != '/tmp' && $temp_folder != drupal_realpath('public://') && $temp_folder != drupal_realpath('private://')){
    foreach(scandir($temp_folder) as $file){
      if(is_file("$temp_folder/$file") && filemtime("$temp_folder/$file") + 2419200 < time()){
        file_unmanaged_delete("$temp_folder/$file");
        db_delete('file_managed')->condition('uri', 'temporary://' . $file)->execute();
      }
    }
  }
  // Add a revision for a term that doesn't currently have one, which prevents
  // any new terms from being added
  if(($term = db_select('taxonomy_term_data', 't')->condition('revision_id', 0)->fields('t')->execute()->fetch()) != FALSE){
    unset($term->revision_id);
    $term->timestamp = time();
    $term->uid = 0;
    $term->log = t('Automatically created revision.');
    drupal_write_record('taxonomy_term_data_revision', $term);
    $term_update = array(
      'tid' => $term->tid,
      'revision_id' => $term->revision_id
    );
    drupal_write_record('taxonomy_term_data', $term_update, array(
      'tid'
    ));
  }
  // Remove any "batch" objects older than one week (they have almost definitely
  // failed).
  db_delete('batch')->condition('timestamp', time() - 604800, '<')->execute();
  // Rebuild caches
  _scratchpads_tweaks_register_shutdown();
}
