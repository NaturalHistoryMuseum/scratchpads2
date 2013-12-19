<?php

/**
 * @file
 * character_editor.features.field_group.inc
 */
/**
 * Implementation of hook_field_group_default_field_groups().
 */
function character_editor_field_group_default_field_groups(){
  $field_groups = array();
  // Exported field_group: 'character_editor_project-default_character_project-form-group_char_proj_metadata'
  $field_groups['character_editor_project-default_character_project-form-group_char_proj_metadata'] = array(
    'group_name' => 'group_char_proj_metadata',
    'entity_type' => 'character_editor_project',
    'bundle' => 'default_character_project',
    'mode' => 'form',
    'parent_name' => '',
    'label' => 'Metadata',
    'weight' => '0',
    'children' => array(
      0 => 'field_char_proj_description'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  // Exported field_group: 'character_editor_project-default_character_project-form-group_taxonomic_name'
  $field_groups['character_editor_project-default_character_project-form-group_taxonomic_name'] = array(
    'group_name' => 'group_taxonomic_name',
    'entity_type' => 'character_editor_project',
    'bundle' => 'default_character_project',
    'mode' => 'form',
    'parent_name' => '',
    'label' => 'Taxa',
    'weight' => '1',
    'children' => array(
      0 => 'field_taxonomic_name'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  return $field_groups;
}
