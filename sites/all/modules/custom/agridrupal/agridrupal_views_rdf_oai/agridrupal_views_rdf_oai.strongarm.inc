<?php
/**
 * @file
 * agridrupal_views_rdf_oai.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function agridrupal_views_rdf_oai_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agrepository';
  $strongarm->value = '0';
  $export['comment_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agrepository';
  $strongarm->value = 0;
  $export['comment_anonymous_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agrepository';
  $strongarm->value = '3';
  $export['comment_controls_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agrepository';
  $strongarm->value = 1;
  $export['comment_default_mode_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agrepository';
  $strongarm->value = '1';
  $export['comment_default_order_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agrepository';
  $strongarm->value = '50';
  $export['comment_default_per_page_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agrepository';
  $strongarm->value = '0';
  $export['comment_form_location_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agrepository';
  $strongarm->value = 2;
  $export['comment_preview_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agrepository';
  $strongarm->value = '1';
  $export['comment_subject_field_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agrepository';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '0',
        ),
        'comment_settings' => array(
          'weight' => '1',
        ),
        'menu' => array(
          'weight' => '-1',
        ),
        'path' => array(
          'weight' => '3',
        ),
        'attachments' => array(
          'weight' => '2',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agrepository';
  $strongarm->value = '0';
  $export['language_content_type_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agrepository';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agrepository'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agrepository';
  $strongarm->value = 1;
  $export['node_preview_agrepository'] = $strongarm;

  return $export;
}
