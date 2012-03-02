<?php
/**
 * @file
 * contac.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function scratchpads_contact_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'modal_forms_contact';
  $strongarm->value = 1;
  $export['modal_forms_contact'] = $strongarm;

  return $export;
}
