<?php
/**
 * @file
 * emonocot_spm.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function emonocot_spm_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_misc|node|spm|form';
  $field_group->group_name = 'group_spm_em_misc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '5',
    'children' => array(
      0 => 'field_spm_em_cultivation',
      1 => 'field_spm_em_cult_ref',
      2 => 'field_spm_em_phenology',
      3 => 'field_spm_em_phen_ref',
      4 => 'field_spm_em_pollination',
      5 => 'field_spm_em_poll_ref',
      6 => 'field_spm_em_anatomy',
      7 => 'field_spm_em_anatomy_ref',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_spm_em_misc|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_taxonomy|node|spm|form';
  $field_group->group_name = 'group_spm_em_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '6',
    'children' => array(
      0 => 'field_spm_em_taxonomic_notes',
      1 => 'field_spm_em_tn_ref',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_spm_em_taxonomy|node|spm|form'] = $field_group;

  return $export;
}
