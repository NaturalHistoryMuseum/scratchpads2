<?php

/**
 * @file
 * collections_profile.strongarm.inc
 */
/**
 * Implements hook_strongarm().
 */
function collections_profile_strongarm(){
  $export = array();
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_collections_profile';
  $strongarm->value = '0';
  $export['comment_anonymous_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_collections_profile';
  $strongarm->value = '1';
  $export['comment_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_collections_profile';
  $strongarm->value = 1;
  $export['comment_default_mode_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_collections_profile';
  $strongarm->value = '50';
  $export['comment_default_per_page_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_collections_profile';
  $strongarm->value = 1;
  $export['comment_form_location_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_collections_profile';
  $strongarm->value = '1';
  $export['comment_preview_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_collections_profile';
  $strongarm->value = 1;
  $export['comment_subject_field_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_year_only';
  $strongarm->value = 'Y';
  $export['date_format_year_only'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__collections_profile';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE
      ),
      'linked_node' => array(
        'custom_settings' => TRUE
      ),
      'full' => array(
        'custom_settings' => FALSE
      ),
      'rss' => array(
        'custom_settings' => FALSE
      ),
      'search_index' => array(
        'custom_settings' => FALSE
      ),
      'search_result' => array(
        'custom_settings' => FALSE
      ),
      'species' => array(
        'custom_settings' => FALSE
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE
      ),
      'token' => array(
        'custom_settings' => FALSE
      )
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0'
        )
      ),
      'display' => array()
    )
  );
  $export['field_bundle_settings_node__collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_collections_profile';
  $strongarm->value = array();
  $export['menu_options_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_collections_profile';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_collections_profile';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
    2 => 'revision'
  );
  $export['node_options_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_collections_profile';
  $strongarm->value = '1';
  $export['node_preview_collections_profile'] = $strongarm;
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_collections_profile';
  $strongarm->value = 0;
  $export['node_submitted_collections_profile'] = $strongarm;
  return $export;
}
