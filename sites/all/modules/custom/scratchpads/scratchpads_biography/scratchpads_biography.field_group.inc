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

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_additional_information|user|user|default';
  $field_group->group_name = 'group_additional_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional information',
    'weight' => '3',
    'children' => array(
      0 => 'field_comments',
      1 => 'field_related_links',
      2 => 'field_user_keywords',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_additional_information|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_additional_information|user|user|form';
  $field_group->group_name = 'group_additional_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Additional information',
    'weight' => '106',
    'children' => array(
      0 => 'field_comments',
      1 => 'field_related_links',
      2 => 'field_user_keywords',
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
  $export['group_additional_information|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_default|user|user|form';
  $field_group->group_name = 'group_default';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Default',
    'weight' => '102',
    'children' => array(
      0 => 'field_user_country',
      1 => 'field_homepage',
      2 => 'field_user_family_name',
      3 => 'field_user_given_names',
      4 => 'field_user_institution',
      5 => 'field_user_taxonomic_interest',
      6 => 'field_user_title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_default|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_names_and_identifiers|user|user|form';
  $field_group->group_name = 'group_names_and_identifiers';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Name forms/Identifiers',
    'weight' => '103',
    'children' => array(
      0 => 'field_alternative_name',
      1 => 'field_orcid',
      2 => 'field_standard_form',
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
  $export['group_names_and_identifiers|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_names|user|user|default';
  $field_group->group_name = 'group_names';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Name and Identifiers',
    'weight' => '0',
    'children' => array(
      0 => 'field_user_family_name',
      1 => 'field_user_given_names',
      2 => 'field_user_title',
      3 => 'field_alternative_name',
      4 => 'field_orcid',
      5 => 'field_standard_form',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Name and Identifiers',
      'instance_settings' => array(
        'display_mode' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_names|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal_information|user|user|default';
  $field_group->group_name = 'group_personal_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal information',
    'weight' => '2',
    'children' => array(
      0 => 'field_user_country',
      1 => 'field_biography',
      2 => 'field_birth_date',
      3 => 'field_birth_place',
      4 => 'field_death_date',
      5 => 'field_example_of_handwriting',
      6 => 'field_gender',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_personal_information|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal_information|user|user|form';
  $field_group->group_name = 'group_personal_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Personal information',
    'weight' => '105',
    'children' => array(
      0 => 'field_biography',
      1 => 'field_birth_date',
      2 => 'field_birth_place',
      3 => 'field_death_date',
      4 => 'field_example_of_handwriting',
      5 => 'field_gender',
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
  $export['group_personal_information|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_professional_information|user|user|form';
  $field_group->group_name = 'group_professional_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Professional information',
    'weight' => '104',
    'children' => array(
      0 => 'field_areas_of_professional_inte',
      1 => 'field_known_for',
      2 => 'field_main_collaborators_co_auth',
      3 => 'field_places_worked',
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
  $export['group_professional_information|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|user|user|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '9',
    'children' => array(
      0 => 'group_default',
      1 => 'group_additional_information',
      2 => 'group_names_and_identifiers',
      3 => 'group_personal_information',
      4 => 'group_professional_information',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_tabs|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_work|user|user|default';
  $field_group->group_name = 'group_work';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Professional information',
    'weight' => '1',
    'children' => array(
      0 => 'field_homepage',
      1 => 'field_user_institution',
      2 => 'field_user_taxonomic_interest',
      3 => 'field_areas_of_professional_inte',
      4 => 'field_known_for',
      5 => 'field_main_collaborators_co_auth',
      6 => 'field_places_worked',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Professional information',
      'instance_settings' => array(
        'display_mode' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_work|user|user|default'] = $field_group;

  return $export;
}
