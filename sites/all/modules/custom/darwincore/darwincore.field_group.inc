<?php
/**
 * @file
 * darwincore.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function darwincore_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_collection_display|node|specimen_observation|default';
  $field_group->group_name = 'group_collection_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '2',
    'children' => array(
      0 => 'field_collector',
      1 => 'field_collector_number',
      2 => 'field_date_collected',
      3 => 'field_notes',
      4 => 'field_number',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  // This code should be removed once all migrated sites have fixed their data.
  if(field_info_field('field_collector_copy')){
    array_unshift($field_group->data['children'], 'field_collector_copy');
  }
  $export['group_collection_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_collection|node|specimen_observation|form';
  $field_group->group_name = 'group_collection';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Collection',
    'weight' => '23',
    'children' => array(
      0 => 'field_collector',
      1 => 'field_collector_number',
      2 => 'field_date_collected',
      3 => 'field_notes',
      4 => 'field_number',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Collection',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  // This code should be removed once all migrated sites have fixed their data.
  if(field_info_field('field_collector_copy')){
    array_unshift($field_group->data['children'], 'field_collector_copy');
  }
  $export['group_collection|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_extra_fields|node|location|form';
  $field_group->group_name = 'group_extra_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Extra fields',
    'weight' => '24',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_country',
      2 => 'field_island_group',
      3 => 'field_island',
      4 => 'field_state_province',
      5 => 'field_county',
      6 => 'field_locality',
      7 => 'field_coordinate_system',
      8 => 'field_coordinate_uncertainty',
      9 => 'field_geodetic_datum',
      10 => 'field_georeference_protocol',
      11 => 'field_georeference_remarks',
      12 => 'field_max_depth',
      13 => 'field_max_elevation',
      14 => 'field_min_depth',
      15 => 'field_min_elevation',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Extra fields',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_extra_fields|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_display|node|location|default';
  $field_group->group_name = 'group_location_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'BLANK',
    'weight' => '10',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_coordinate_system',
      2 => 'field_coordinate_uncertainty',
      3 => 'field_country',
      4 => 'field_county',
      5 => 'field_geodetic_datum',
      6 => 'field_georeference_protocol',
      7 => 'field_georeference_remarks',
      8 => 'field_island_group',
      9 => 'field_island',
      10 => 'field_locality',
      12 => 'field_max_depth',
      13 => 'field_max_elevation',
      14 => 'field_min_depth',
      15 => 'field_min_elevation',
      16 => 'field_state_province',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_location_display|node|location|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_display|node|location|linked_node';
  $field_group->group_name = 'group_location_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'linked_node';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'BLANK',
    'weight' => '10',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_coordinate_system',
      2 => 'field_coordinate_uncertainty',
      3 => 'field_country',
      4 => 'field_county',
      5 => 'field_geodetic_datum',
      6 => 'field_georeference_protocol',
      7 => 'field_georeference_remarks',
      8 => 'field_island_group',
      9 => 'field_island',
      10 => 'field_locality',
      12 => 'field_max_depth',
      13 => 'field_max_elevation',
      14 => 'field_min_depth',
      15 => 'field_min_elevation',
      16 => 'field_state_province',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_location_display|node|location|linked_node'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|specimen_observation|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '25',
    'children' => array(
      0 => 'field_location',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_location|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_map|node|location|form';
  $field_group->group_name = 'group_map';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Map',
    'weight' => '23',
    'children' => array(
      0 => 'field_map',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Collection',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_map|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_misc_display|node|specimen_observation|default';
  $field_group->group_name = 'group_misc_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '3',
    'children' => array(
      0 => 'field_count',
      1 => 'field_genbank_number',
      2 => 'field_lifestage',
      3 => 'field_other_catalogue_numbers',
      4 => 'field_remarks',
      5 => 'field_sex',
      6 => 'field_media',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_misc_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_miscellaneous|node|specimen_observation|form';
  $field_group->group_name = 'group_miscellaneous';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Miscellaneous',
    'weight' => '24',
    'children' => array(
      0 => 'field_count',
      1 => 'field_genbank_number',
      2 => 'field_lifestage',
      3 => 'field_other_catalogue_numbers',
      4 => 'field_remarks',
      5 => 'field_sex',
      6 => 'field_media',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Miscellaneous',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_miscellaneous|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_required_display|node|specimen_observation|default';
  $field_group->group_name = 'group_required_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '0',
    'children' => array(
      0 => 'field_basis_of_record',
      1 => 'field_catalogue_number',
      2 => 'field_collection_code',
      3 => 'field_institution_code',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_required_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_required|node|specimen_observation|form';
  $field_group->group_name = 'group_required';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Required',
    'weight' => '21',
    'children' => array(
      0 => 'field_basis_of_record',
      1 => 'field_catalogue_number',
      2 => 'field_collection_code',
      3 => 'field_institution_code',
      4 => 'title'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Required',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_required|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|location|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '0',
    'children' => array(
      0 => 'group_extra_fields',
      1 => 'group_map',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_tabs|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|specimen_observation|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '0',
    'children' => array(
      0 => 'group_taxonomy',
      1 => 'group_required',
      2 => 'group_collection',
      3 => 'group_miscellaneous',
      4 => 'group_location',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_tabs|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxonomy_display|node|specimen_observation|default';
  $field_group->group_name = 'group_taxonomy_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '1',
    'children' => array(
      0 => 'field_date_identified',
      1 => 'field_identification_qualifier',
      2 => 'field_identified_by',
      3 => 'field_taxonomic_name',
      4 => 'field_type_status',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_taxonomy_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxonomy|node|specimen_observation|form';
  $field_group->group_name = 'group_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '23',
    'children' => array(
      0 => 'field_type_status',
      1 => 'field_taxonomic_name',
      2 => 'field_identification_qualifier',
      3 => 'field_identified_by',
      4 => 'field_date_identified',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Taxonomy',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_taxonomy|node|specimen_observation|form'] = $field_group;

  return $export;
}
