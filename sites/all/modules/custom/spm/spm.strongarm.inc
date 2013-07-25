<?php

/**
 * @file
 * spm.strongarm.inc
 */
/**
 * Implements hook_strongarm().
 */
function spm_strongarm(){
  $export = array();
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_spm';
  $strongarm->value = 0;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_spm';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_spm';
  $strongarm->value = '50';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_spm';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_spm';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_spm';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_spm';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_spm';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision'
  );
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_spm';
  $strongarm->value = '[node:field_taxonomic_name]';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_spm';
  $strongarm->value = 0;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_spm';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  return $export;
}
