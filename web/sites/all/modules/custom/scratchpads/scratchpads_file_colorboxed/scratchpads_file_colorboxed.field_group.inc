<?php

/**
 * @file
 * sfc.field_group.inc
 */
/**
 * Implements hook_field_group_info().
 */
function scratchpads_file_colorboxed_field_group_info(){
  $export = array();
  $children = array(
    0 => 'field_taxonomic_name',
    1 => 'field_cc_licence',
    2 => 'field_imaging_technique',
    3 => 'field_keywords',
    4 => 'field_preparation_technique',
    5 => 'field_description',
    6 => 'field_creator'
  );
  $image_children = $children;
  if(module_exists('scratchpads_audubon_core')){
    module_load_include('features.field.inc', 'scratchpads_audubon_core');
    $fields = scratchpads_audubon_core_field_default_fields();
    foreach($fields as $field){
      $image_children[] = $field['field_config']['field_name'];
    }
  }
  foreach(array(
    'default',
    'file_styles_colorbox'
  ) as $view_name){
    foreach(array(
      'image',
      'audio',
      'video'
    ) as $bundle){
      $field_group = new stdClass();
      $field_group->disabled = FALSE;
      $field_group->api_version = 1;
      $field_group->identifier = 'group_file_group|file|' . $bundle . '|' . $view_name;
      $field_group->group_name = 'group_file_group';
      $field_group->entity_type = 'file';
      $field_group->bundle = $bundle;
      $field_group->mode = $view_name;
      $field_group->parent_name = '';
      $field_group->data = array(
        'label' => 'Group',
        'weight' => '1',
        'children' => ($bundle == 'image' ? $image_children : $children),
        'format_type' => 'field_group_stripy_div',
        'format_settings' => array(
          'label' => 'Group',
          'instance_settings' => array(
            'display_mode' => ''
          ),
          'formatter' => 'open'
        )
      );
      $export['group_file_group|file|' . $bundle . '|' . $view_name] = $field_group;
    }
  }
  return $export;
}