<?php

/**
 * @file
 * Drush integration for the redirect module.
 */

/**
 * Implementation of hook_drush_command().
 */
function redirect_drush_command() {
  $items['generate-redirects'] = array(
    'description' => 'Create redirects.',
    'drupal dependencies' => array('devel_generate'),
    'arguments' => array(
      'count' => 'Number of redirects to generate.',
    ),
    'options' => array(
      'delete' => 'Delete all redirects before generating new ones.',
    ),
  );

  return $items;
}

/**
 * Command callback. Generate a number of redirects.
 */
function drush_redirect_generate_redirects($count = NULL) {
  if (drush_generate_is_number($count) == FALSE) {
    return drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of redirects.'));
  }
  module_load_include('inc', 'redirect', 'redirect.generate');
  drush_generate_include_devel();
  redirect_run_unprogressive_batch('redirect_generate_redirects_batch_info', $count, drush_get_option('delete'));
}

/**
 * Perform an unprogressive batch process for CLI.
 */
function redirect_run_unprogressive_batch() {
  $batch = batch_get();
  if (!empty($batch)) {
    // If there is already something in the batch, don't run.
    return FALSE;
  }

  $args = func_get_args();
  $batch_callback = array_shift($args);

  if (!lock_acquire($batch_callback)) {
    return FALSE;
  }

  // Attempt to increase the execution time.
  drupal_set_time_limit(240);

  // Build the batch array.
  $batch = call_user_func_array($batch_callback, $args);
  batch_set($batch);

  // We need to manually set the progressive variable again.
  // @todo Remove when http://drupal.org/node/638712 is fixed.
  $batch =& batch_get();
  $batch['progressive'] = FALSE;

  // Run the batch process.
  batch_process();

  lock_release($batch_callback);
  return TRUE;
}
