<?php

/**
 * @file
 * darwincore.strongarm.inc
 */
/**
 * Implements hook_strongarm().
 */
function darwincore_strongarm(){
  $export = array();
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_specimen_observation';
  $strongarm->value = '[node:field_institution_code] - [node:field_collection_code] - [node:field_catalogue_number]';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_specimen_observation';
  $strongarm->value = 0;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_specimen_observation';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_location';
  $strongarm->value = 0;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_specimen_observation';
  $strongarm->value = 0;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_location';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_specimen_observation';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_location';
  $strongarm->value = '50';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_specimen_observation';
  $strongarm->value = '50';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_location';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_specimen_observation';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_location';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_location';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_specimen_observation';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_specimen_observation';
  $strongarm->value = '1';
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_location';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_specimen_observation';
  $strongarm->value = 1;
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_location';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision'
  );
  $export[$strongarm->name] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_specimen_observation';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision'
  );
  $export[$strongarm->name] = $strongarm;
  return $export;
}
