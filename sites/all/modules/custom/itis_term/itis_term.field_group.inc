<?php

/**
 * @file
 * itis_term.field_group.inc
 */
/**
 * Implements hook_field_group_info().
 */
function itis_term_field_group_info(){
  $export = array();
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_extra_detail|field_collection_item|field_vernacular_name_collection|form';
  $field_group->group_name = 'group_extra_detail';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_vernacular_name_collection';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Extra detail',
    'weight' => '3',
    'children' => array(
      0 => 'field_vernacular_locality',
      1 => 'field_vernacular_name_remarks'
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Extra detail',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => ''
      ),
      'formatter' => 'collapsed'
    )
  );
  $export['group_extra_detail|field_collection_item|field_vernacular_name_collection|form'] = $field_group;
  return $export;
}