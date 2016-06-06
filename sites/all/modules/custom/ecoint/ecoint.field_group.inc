<?php
/**
 * @file
 * ecoint.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function ecoint_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interaction|node|ecological_interactions|default';
  $field_group->group_name = 'group_interaction';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ecological_interactions';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Interaction',
    'weight' => '5',
    'children' => array(
      0 => 'field_reference',
      1 => 'field_interaction_type',
      2 => 'field_interaction_record_type',
      3 => 'field_location_ref',
      4 => 'field_date',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-interaction field-group-fieldset',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_interaction|node|ecological_interactions|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_interaction|node|ecological_interactions|form';
  $field_group->group_name = 'group_interaction';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ecological_interactions';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Interaction',
    'weight' => '5',
    'children' => array(
      0 => 'body',
      1 => 'field_date',
      2 => 'field_interaction_record_type',
      3 => 'field_interaction_type',
      4 => 'field_location_ref',
      5 => 'field_reference',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-interaction field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_interaction|node|ecological_interactions|form'] = $field_group;


  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxon_1|node|ecological_interactions|default';
  $field_group->group_name = 'group_taxon_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ecological_interactions';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Taxon 1',
    'weight' => '0',
    'children' => array(
      0 => 'field_taxonomic_name',
      1 => 'field_species_status',
      2 => 'field_life_cycle_stage',
      3 => 'field_organism_part',
      4 => 'field_specimen_1',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-taxon-1 field-group-fieldset',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_taxon_1|node|ecological_interactions|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxon_1|node|ecological_interactions|form';
  $field_group->group_name = 'group_taxon_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ecological_interactions';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Taxon 1',
    'weight' => '4',
    'children' => array(
      0 => 'field_life_cycle_stage',
      1 => 'field_organism_part',
      2 => 'field_species_status',
      3 => 'field_specimen_1',
      4 => 'field_taxonomic_name',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-taxon-1 field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_taxon_1|node|ecological_interactions|form'] = $field_group;

  return $field_groups;
}
