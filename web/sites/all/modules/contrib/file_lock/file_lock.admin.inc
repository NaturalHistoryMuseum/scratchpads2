<?php

/**
 * @file
 * Administrative interface for file type configuration.
 */

/**
 * Administrative form for browsing files and performing operations on them.
 */
function file_lock_admin_config($form, &$form_state) {
  $form['lock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lock files'),
  );
  $form['lock']['lock_files'] = array(
    '#type' => 'submit',
    '#value' => t('Lock all existing files'),
    '#submit' => array('file_lock_lock_existing_files_form_submit'),
  );

  if (variable_get('file_lock_mode', 'none') == 'none') {
    $form['lock']['lock_files']['#value'] = t('Unlock all existing files');
  }

  $form['file_lock_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic file lock'),
    '#options' => array(
        'all' => t('Lock all new files'),
        'pattern' => t('Lock files by pattern'),
        'regex' => t('Lock files by regular expression'),
        'none' => t('None'),
    ),
    '#default_value' => variable_get('file_lock_mode', 'none'),
    '#description' => t('Choose how new files should automaticly be locked.'),
  );

  $form['file_lock_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern'),
    '#default_value' => variable_get('file_lock_pattern', '*'),
    '#description' => 'Only files with filename matching this pattern will be locked. Pattern is a shell wildcard pattern, more information: <a href="http://php.net/manual/function.fnmatch.php">fnmatch</a>',
    '#states' => array(
      'visible' => array(
        ':input[name="file_lock_mode"]' => array('value' => 'pattern'),
      ),
    ),
  );

  $form['file_lock_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Regular Expression'),
    '#default_value' => variable_get('file_lock_regex', '/.*/'),
    '#description' => 'Only files with filename matching this regex pattern will be locked. Pattern must be a Perl-style pattern for <a href="http://php.net/manual/function.preg-match.php">preg_match</a>',
    '#states' => array(
      'visible' => array(
        ':input[name="file_lock_mode"]' => array('value' => 'regex'),
      ),
    ),
  );

  /* TODO: by type, by field
   * add ^options 'type' => t('Lock files by filetype'),
   $form['file_lock_by_type'] = array(
     '#type' => 'checkbox',
     '#title' => t('lock files by type'),
     '#default_value' => variable_get('file_lock_by_type', 0),
     '#description' => t('Lock files from specific types'),
     '#states' => array(
       // Only show this field when the 'toggle_me' checkbox is enabled.
       'enabled' => array(
         ':input[name="file_lock_all"]' => array('checked' => FALSE),
         ':input[name="file_lock_by_pattern"]' => array('checked' => FALSE),
       ),
     ),
   );
  */
  $form['hook'] = array(
    '#type' => 'fieldset',
    '#title' => 'Hooks',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'collapsed' => array(
        ':input[name="file_lock_mode"]' => array('value' => 'none'),
      ),
      'disabled' => array(
        ':input[name="file_lock_mode"]' => array('value' => 'none'),
      ),
    ),
  );

  $form['hook']['file_lock_hook'] = array(
    '#type' => 'radios',
    '#title' => t('Select actions for automatic file locking'),
    '#options' => array(
      'all' => t('all file_save actions (insert and update)'),
      'insert' => t('file_insert: act on newly created files'),
      'update' => t('file_update: only act on updates for existing files'),
    ),
    '#default_value' => variable_get('file_lock_hook', 'insert'),
    '#description' => t('Select actions for automatic file locking.'),
  );


  return system_settings_form($form);
}

/**
 * Validate file_lock_admin_config form submissions.
 *
 * Check if any mode have been selected, and check if pattern is not empty.
 *
 */
function file_lock_admin_config_validate($form, &$form_state) {
  if ($form_state['values']['file_lock_mode'] == 'pattern' && $form_state['values']['file_lock_pattern'] == '') {
    form_set_error('file_lock_pattern', t('File pattern must not be empty'));
  }
  elseif ($form_state['values']['file_lock_mode'] == 'regex' && $form_state['values']['file_lock_regex'] == '') {
    form_set_error('file_lock_regex', t('RegEX pattern must not be empty.'));
  }
}

/**
 * Lock existing files.
 */
function file_lock_lock_existing_files_form_submit() {
  $batch = array(
    'title' => t('Locking existing files'),
    'operations' => array(
      array('_file_lock_lock_existing_files', array()),
    ),
    'finished' => '_file_lock_lock_existing_files_finished_callback',
    'file' => drupal_get_path('module', 'file_lock') . '/file_lock.admin.inc',
  );
  batch_set($batch);
}

function _file_lock_lock_existing_files(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_file'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT fid) FROM {file_managed}')->fetchField();
  }
  $limit = 5;
  $result = db_select('file_managed')
    ->fields('file_managed', array('fid'))
    ->condition('fid', $context['sandbox']['current_file'], '>')
    ->orderBy('fid')
    ->range(0, $limit)
    ->execute();
  foreach ($result as $row) {
    $file = file_load($row->fid);
    if (variable_get('file_lock_mode', 'none') == 'none') {
      file_lock_remove_lock($file);
    }
    else {
      file_lock_act_on($file);
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current_file'] = $file->fid;
    $context['message'] = check_plain($file->filename);
    $context['results'][] = $file->fid;
  }
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _file_lock_update_lock_existing_files_finished_callback($success, $results, $operations) {
  // The 'success' parameter means no fatal PHP errors were detected. All
  // other error management should be handled using 'results'.
  if ($success) {
    $message = format_plural(count($results), 'One file locked.', '@count files locked.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}
