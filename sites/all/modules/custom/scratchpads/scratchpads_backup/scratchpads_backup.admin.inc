<?php
define('SCRATCHPADS_BACKUP_REQUEST_FILE', 'public://scratchpads_backup_requested');

/**
 * Form for requesting/downloading a site backup.
 */
function scratchpads_backup_request_form($form, $form_state){
  $backup_running = file_exists(SCRATCHPADS_BACKUP_REQUEST_FILE);
  return array(
    'previous_backups' => array(
      '#type' => 'fieldset',
      '#title' => t('Previous backups'),
      '#collapsed' => TRUE,
      '#collapsible' => FALSE,
      'list' => array(
        '#theme' => 'item_list',
        '#items' => _scratchpads_backup_get_previous_backups()
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
        '#disabled' => $backup_running
      ),
      'backup_running' => array(
        '#markup' => $backup_running ? '<div class="messages status" style="margin-top:10px">' . t('A backup has already been request, please wait for that to complete before requesting another') . '</div>' : ''
      )
    )
  );
}

/**
 * Submit function for the above form.
 */
function scratchpads_backup_request_form_submit(&$form, &$form_state){
  file_unmanaged_save_data(time(), SCRATCHPADS_BACKUP_REQUEST_FILE, FILE_EXISTS_REPLACE);
  drupal_set_message('Backup requested');
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