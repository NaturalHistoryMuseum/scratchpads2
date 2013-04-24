<?php
/**
 * @file
 * scratchpads_biography.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function scratchpads_biography_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_biography|user|user|form';
  $field_group->group_name = 'group_biography';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Biography',
    'weight' => '16',
    'children' => array(
      0 => 'field_born_date_',
      1 => 'field_died_date_',
      2 => 'field_institutions',
      3 => 'field_file_attachments',
      4 => 'field_gender',
      5 => 'group_born_place',
      6 => 'group_died_place',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Biography',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_biography|user|user|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_born_place|user|user|form';
  $field_group->group_name = 'group_born_place';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_biography';
  $field_group->data = array(
    'label' => 'Born (Place)',
    'weight' => '17',
    'children' => array(
      0 => 'field_born_country_',
      1 => 'field_born_island_',
      2 => 'field_born_island_group_',
      3 => 'field_born_state_province_',
      4 => 'field_born_county',
      5 => 'field_born_locality_',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Born (Place)',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_born_place|user|user|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_died_place|user|user|form';
  $field_group->group_name = 'group_died_place';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_biography';
  $field_group->data = array(
    'label' => 'Died (Place)',
    'weight' => '19',
    'children' => array(
      0 => 'field_died_country_',
      1 => 'field_died_island_',
      2 => 'field_died_island_group_',
      3 => 'field_died_state_province_',
      4 => 'field_died_county_',
      5 => 'field_died_locality_',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Died (Place)',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_died_place|user|user|form'] = $field_group;

  return $export;
}
