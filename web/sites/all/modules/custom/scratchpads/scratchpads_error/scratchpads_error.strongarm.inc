<?php

/**
 * @file
 * scratchpads_error.strongarm.inc
 */
/**
 * Implements hook_strongarm().
 */
function scratchpads_error_strongarm(){
  $export = array();
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_403';
  $strongarm->value = 'customerror/403';
  $export['site_403'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_404';
  $strongarm->value = 'customerror/404';
  $export['site_404'] = $strongarm;
  return $export;
}
