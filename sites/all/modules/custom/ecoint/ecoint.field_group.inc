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
    'label' => '',
    'weight' => '1',
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
    'label' => '',
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
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_interaction|node|ecological_interactions|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_reference|hymenoptera';
  $field_group->group_name = 'group_reference';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'hymenoptera';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Reference',
    'weight' => '10',
    'children' => array(
      0 => 'field_authors',
      1 => 'field_page_number',
      2 => 'field_reference',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-reference field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_reference|hymenoptera'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_reference|phasmida_species_file_22092009';
  $field_group->group_name = 'group_reference';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'phasmida_species_file_22092009';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Reference',
    'weight' => '10',
    'children' => array(
      0 => 'field_authors',
      1 => 'field_page_number',
      2 => 'field_reference',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-reference field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_reference|phasmida_species_file_22092009'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_scientific_name|hymenoptera';
  $field_group->group_name = 'group_scientific_name';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'hymenoptera';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Scientific name',
    'weight' => '8',
    'children' => array(
      0 => 'field_rank',
      1 => 'field_unit_indicator1',
      2 => 'field_unit_indicator2',
      3 => 'field_unit_indicator3',
      4 => 'field_unit_indicator4',
      5 => 'field_unit_name1',
      6 => 'field_unit_name2',
      7 => 'field_unit_name3',
      8 => 'field_unit_name4',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-scientific-name field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_scientific_name|hymenoptera'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_scientific_name|phasmida_species_file_22092009';
  $field_group->group_name = 'group_scientific_name';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'phasmida_species_file_22092009';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Scientific name',
    'weight' => '8',
    'children' => array(
      0 => 'field_rank',
      1 => 'field_unit_indicator1',
      2 => 'field_unit_indicator2',
      3 => 'field_unit_indicator3',
      4 => 'field_unit_indicator4',
      5 => 'field_unit_name1',
      6 => 'field_unit_name2',
      7 => 'field_unit_name3',
      8 => 'field_unit_name4',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-scientific-name field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_scientific_name|phasmida_species_file_22092009'] = $field_group;

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
    'label' => '',
    'weight' => '0',
    'children' => array(
      0 => 'field_taxonomic_name',
      1 => 'field_species_status',
      2 => 'field_life_cycle_stage',
      3 => 'field_organism_part',
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
    'label' => '',
    'weight' => '4',
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
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_taxon_1|node|ecological_interactions|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_usage|hymenoptera';
  $field_group->group_name = 'group_usage';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'hymenoptera';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Usage',
    'weight' => '9',
    'children' => array(
      0 => 'field_aan_13',
      1 => 'field_unacceptability_reason',
      2 => 'field_usage',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-usage field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_usage|hymenoptera'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_usage|phasmida_species_file_22092009';
  $field_group->group_name = 'group_usage';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'phasmida_species_file_22092009';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Usage',
    'weight' => '9',
    'children' => array(
      0 => 'field_aan_14',
      1 => 'field_unacceptability_reason',
      2 => 'field_usage',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-usage field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_usage|phasmida_species_file_22092009'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vernacular_names|hymenoptera';
  $field_group->group_name = 'group_vernacular_names';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'hymenoptera';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Vernacular names',
    'weight' => '10',
    'children' => array(
      0 => 'field_vernacular_name_collection',
      1 => 'description',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-vernacular-names field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_vernacular_names|hymenoptera'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vernacular_names|phasmida_species_file_22092009';
  $field_group->group_name = 'group_vernacular_names';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'phasmida_species_file_22092009';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Vernacular names',
    'weight' => '10',
    'children' => array(
      0 => 'field_vernacular_name_collection',
      1 => 'description',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => ' group-vernacular-names field-group-htab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_vernacular_names|phasmida_species_file_22092009'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Reference');
  t('Scientific name');
  t('Usage');
  t('Vernacular names');

  return $field_groups;
}
