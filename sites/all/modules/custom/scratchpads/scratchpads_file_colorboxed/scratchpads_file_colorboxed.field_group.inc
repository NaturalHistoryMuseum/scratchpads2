<?php

/**
 * @file
 * sfc.field_group.inc
 */
/**
 * Implements hook_field_group_info().
 */
function sfc_field_group_info(){
  $export = array();
  foreach(array(
    'image',
    'audio',
    'video'
  ) as $bundle){
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_file_group|file|' . $bundle . '|default';
    $field_group->group_name = 'group_file_group';
    $field_group->entity_type = 'file';
    $field_group->bundle = $bundle;
    $field_group->mode = 'default';
    $field_group->parent_name = '';
    $field_group->data = array(
      'label' => 'Group',
      'weight' => '1',
      'children' => array(
        0 => 'field_taxonomic_name',
        1 => 'field_cc_licence',
        2 => 'field_imaging_technique',
        3 => 'field_keywords',
        4 => 'field_preparation_technique',
        5 => 'field_description',
        6 => 'field_creator'
      ),
      'format_type' => 'field_group_stripy_div',
      'format_settings' => array(
        'label' => 'Group',
        'instance_settings' => array(
          'display_mode' => ''
        ),
        'formatter' => 'open'
      )
    );
    $export['group_file_group|file|' . $bundle . '|default'] = $field_group;
  }
  return $export;
}