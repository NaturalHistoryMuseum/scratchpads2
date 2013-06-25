<?php
/**
 * @file
 * scratchpad_relations.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function scratchpad_relations_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ecological_relationships|taxonomy_term|testclassification|form';
  $field_group->group_name = 'group_ecological_relationships';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'testclassification';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Ecological relationships',
    'weight' => '13',
    'children' => array(
      0 => 'field_ecological_relationship',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ecological_relationships|taxonomy_term|testclassification|form'] = $field_group;

  return $export;
}
