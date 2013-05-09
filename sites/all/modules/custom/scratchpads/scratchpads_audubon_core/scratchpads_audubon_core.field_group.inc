<?php
/**
 * @file
 * scratchpads_audubon_core.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function scratchpads_audubon_core_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_file_group|file|image|default';
  $field_group->group_name = 'group_file_group';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
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
      6 => 'field_creator',
      7 => 'field_capture_device',
      8 => 'field_continent_or_ocean',
      9 => 'field_copyright_owner',
      10 => 'field_copyright_statement',
      11 => 'field_country',
      12 => 'field_date_and_time_of_digitisat',
      13 => 'field_file_modified',
      14 => 'field_further_information_url',
      15 => 'field_image_processing',
      16 => 'field_imaging_preparation_techni',
      17 => 'field_map',
      18 => 'field_metadata_creator',
      19 => 'field_metadata_provider',
      20 => 'field_notes_comments',
      21 => 'field_original_date_and_time',
      22 => 'field_provider',
      23 => 'field_provider_id',
      24 => 'field_subject_orientation',
      25 => 'field_subject_part',
      26 => 'field_sublocation',
      27 => 'field_time_of_day',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'label' => 'Group',
      'instance_settings' => array(
        'display_mode' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_file_group|file|image|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_file_group|file|image|file_styles_colorbox';
  $field_group->group_name = 'group_file_group';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'file_styles_colorbox';
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
      6 => 'field_creator',
      7 => 'field_capture_device',
      8 => 'field_continent_or_ocean',
      9 => 'field_copyright_owner',
      10 => 'field_copyright_statement',
      11 => 'field_country',
      12 => 'field_date_and_time_of_digitisat',
      13 => 'field_file_modified',
      14 => 'field_further_information_url',
      15 => 'field_image_processing',
      16 => 'field_imaging_preparation_techni',
      17 => 'field_map',
      18 => 'field_metadata_creator',
      19 => 'field_metadata_provider',
      20 => 'field_notes_comments',
      21 => 'field_original_date_and_time',
      22 => 'field_provider',
      23 => 'field_provider_id',
      24 => 'field_subject_orientation',
      25 => 'field_subject_part',
      26 => 'field_sublocation',
      27 => 'field_time_of_day',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'label' => 'Group',
      'instance_settings' => array(
        'display_mode' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_file_group|file|image|file_styles_colorbox'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_img_basics|file|image|form';
  $field_group->group_name = 'group_img_basics';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basics',
    'weight' => '0',
    'children' => array(
      0 => 'field_imaging_technique',
      1 => 'field_keywords',
      2 => 'field_description',
      3 => 'field_capture_device',
      4 => 'field_image_processing',
      5 => 'field_provider_id',
      6 => 'filename',
      7 => 'preview',
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
  $export['group_img_basics|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_img_date_time|file|image|form';
  $field_group->group_name = 'group_img_date_time';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Date & Time',
    'weight' => '4',
    'children' => array(
      0 => 'field_date_and_time_of_digitisat',
      1 => 'field_file_modified',
      2 => 'field_original_date_and_time',
      3 => 'field_time_of_day',
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
  $export['group_img_date_time|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_img_location|file|image|form';
  $field_group->group_name = 'group_img_location';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '3',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_country',
      2 => 'field_map',
      3 => 'field_sublocation',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => 'This section is primarily designed to store some broad generic locality information. Greater control can be achieved by creating a Location and attaching this image, or creating a Specimen/Observation linked to a Location and attaching this image.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_img_location|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_img_misc|file|image|form';
  $field_group->group_name = 'group_img_misc';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '5',
    'children' => array(
      0 => 'field_smg_galleries',
      1 => 'field_further_information_url',
      2 => 'field_notes_comments',
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
  $export['group_img_misc|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_img_taxa|file|image|form';
  $field_group->group_name = 'group_img_taxa';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Taxa',
    'weight' => '2',
    'children' => array(
      0 => 'field_taxonomic_name',
      1 => 'field_subject_orientation',
      2 => 'field_subject_part',
      3 => 'field_imaging_preparation_techni',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Taxa',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => 'This section is primarily for recording minimal information about the taxon featured in this image. More comprehensive information can be managed by creating a Specimen/Observation record for the organism and attaching an image to that content.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_img_taxa|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rights|file|image|form';
  $field_group->group_name = 'group_rights';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Rights',
    'weight' => '1',
    'children' => array(
      0 => 'field_cc_licence',
      1 => 'field_creator',
      2 => 'field_copyright_owner',
      3 => 'field_copyright_statement',
      4 => 'field_metadata_creator',
      5 => 'field_metadata_provider',
      6 => 'field_provider',
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
  $export['group_rights|file|image|form'] = $field_group;

  return $export;
}
