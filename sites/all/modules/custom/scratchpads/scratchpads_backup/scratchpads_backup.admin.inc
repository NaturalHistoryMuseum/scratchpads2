<?php
/**
 * The following simple BASH script creates the backups for us, which is run
 * every minute from cron using the flock (file-lock) command
 * $ flock -n /tmp/create_scratchpads_backups -c /var/aegir/bin/create_scratchpads_backups
 */
// #!/bin/bash
// # Look for the request files
// for i in $(ls -1 /var/aegir/platforms/*/sites/*/files/scratchpads_backup_requested 2>/dev/null)
// do
// 	# Get the domain name from the path of the file
// 	DOMAIN=`echo $i|sed "s|/var/aegir/platforms/[^/]*/sites/||;s|/.*||"`
// 	# Move the file so that we know this backup has been requested.
// 	# Request the backup
// 	drush @$DOMAIN provision-backup
// 	rm $i
// done
define('SCRATCHPADS_BACKUP_REQUEST_FILE', 'public://scratchpads_backup_requested');

/**
 * Form for requesting/downloading a site backup.
 */
function scratchpads_backup_request_form($form, $form_state){
  // We don't do backups if the site is larger than 20GB.  Instead, we give a 
  // warning and ask them to email the Scratchpads team instead.
  $query = db_select('file_managed');
  $query->addExpression('SUM(filesize)', 'sum_filesize');
  $size = $query->execute()->fetchField();
  if($size && $size > 20000000000){
    return array(
      'previous_backups' => array(
        '#type' => 'fieldset',
        '#title' => t('Site too big'),
        '#collapsed' => TRUE,
        '#collapsible' => FALSE,
        'note' => array(
          '#markup' => '<p>' . t('Your site is too large to request a backup, please raise an issue on our issues tracker.') . '</p>'
        )
      )
    );
  }else{
    drupal_add_http_header('Refresh', '180');
    $backup_running = file_exists(SCRATCHPADS_BACKUP_REQUEST_FILE);
    $previous_backups = _scratchpads_backup_get_previous_backups();
    return array(
      'previous_backups' => array(
        '#type' => 'fieldset',
        '#title' => t('Previous backups'),
        '#collapsed' => TRUE,
        '#collapsible' => FALSE,
        'list' => array(
          '#theme' => 'item_list',
          '#items' => $previous_backups
        ),
        'note' => array(
          '#markup' => '<p>' . t('Note, backups older than one week will be automatically deleted.') . '</p>'
        )
      ),
      'request_backup' => array(
        '#type' => 'fieldset',
        '#title' => t('Request backup'),
        '#collapsed' => TRUE,
        '#collapsible' => FALSE,
        'generate_new_backup' => array(
          '#type' => 'submit',
          '#value' => t('Generate new backup'),
          '#disabled' => $backup_running || count($previous_backups) >= 5
        ),
        'backup_running' => array(
          '#markup' => $backup_running ? '<div class="messages status" style="margin-top:10px">' . t('A backup has already been request, please wait for that to complete before requesting another') . '</div>' : ''
        ),
        'max_backups' => array(
          '#markup' => count($previous_backups) >= 5 ? '<div class="messages status" style="margin-top:10px">' . t('You may only have upto five backups at any one time.  Please wait for your oldest backup to be deleted.') . '</div>' : ''
        )
      )
    );
  }
}

/**
 * Submit function for the above form.
 */
function scratchpads_backup_request_form_submit(&$form, &$form_state){
  file_unmanaged_save_data(time(), SCRATCHPADS_BACKUP_REQUEST_FILE, FILE_EXISTS_REPLACE);
  drupal_set_message('Backup requested.  Your backup will be available on this page once it has been created.  Please allow up to three hours for the backup to be created.');
}

/**
 * Simple helper function to get the sites domain name.
 */
function _scratchpads_backup_get_domain(){
  static $domain = FALSE;
  if(!$domain){
    global $base_url;
    $parsed_base_url = parse_url($base_url);
    $domain = $parsed_base_url['host'];
  }
  return $domain;
}

/**
 * Helper function to return a list of links to previously requested backups.
 */
function _scratchpads_backup_get_previous_backups(){
  $potential_files = array();
  foreach(scandir('/var/aegir/backups') as $potential_file){
    if(strpos($potential_file, _scratchpads_backup_get_domain()) === 0){
      $potential_files[] = l($potential_file, 'http://backup.scratchpads.eu/' . $potential_file);
    }
  }
  return $potential_files;
}