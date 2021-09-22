<?php

/**
 * Defines a list of data integrity checks, and the frequency with which they
 * should be run.  The scratchpads_data_integrity module will then run each
 * check, returning any errors to the user and to the Scratchpads team
 *
 * This function should return a keyed array or arrays where the key is the name
 * of a function that should be run to check data integrity.  All
 * 
 * return array(
 *   'scratchpads_data_integrity_no_terms_without_parents' => array(
 *     'description' => translated description, t('DESCRIPTION'),
 *     'frequency' => time in seconds between checks, defaults to 86400,
 *     'file' => file that the function can be found in, relative to the 
 *       module's folder.
 *     'mail' => Boolean to state whether the error should be mailed. Defaults
 *       to FALSE.
 *   );
 * );
 */
function hook_scratchpads_data_integrity(){
  return array();
}

/**
 * Allows another module to alter the data checks list.
 */
function hook_scratchpads_data_integrity_alter(&$data){}