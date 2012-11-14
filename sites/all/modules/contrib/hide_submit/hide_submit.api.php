<?php


/**
 * Allows modules alter the behavior of the hide_submit settings.
 *
 * @return
 *   An associative array describing the data structure. Primary key is the
 *   name used internally by Views for the table(s) – usually the actual table
 *   name. The values for the key entries are described in detail below.
 */
function hook_hide_submit_alter($hide_submit_settings) {

  // IMPORTANT NOTE: given the way module_invoke_all and array_merge work you
  // should not modify and return the original array. Instead create a new one
  // and add only the keys you care about. If two modules implement this hook
  // and try to modify the same values then the default will be used instead.

  // Creates a random class between 1 and 10 for using 10 random images
  // in place of the submit button.
  $rand = rand(1, 10);
  $altered_settings['hide_submit']['hide_submit_hide_css'] = 'hide-submit-processing' . $rand;

  // Disable the module for my special form page.
  if (arg(0) == 'my-special-form') {
    $altered_settings['hide_submit']['hide_submit_status'] = FALSE;
  }

  return $altered_settings;
}
