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
    'weight' => '6',
    'children' => array(
      0 => 'field_user_keywords',
      1 => 'field_comments',
      2 => 'field_related_links',
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
    'weight' => '132',
    'children' => array(
      0 => 'field_user_keywords',
      1 => 'field_comments',
      2 => 'field_related_links',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Additional information',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_additional_information|user|user|form'] = $field_group;

  $field_group = new stdClass();
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
    'weight' => '15',
    'children' => array(
      0 => 'group_tabs',
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
    'weight' => '129',
    'children' => array(
      0 => 'field_alternative_name',
      1 => 'field_standard_form',
      2 => 'field_orcid',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Name forms/Identifiers',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
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
      4 => 'field_standard_form',
      5 => 'field_orcid',
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
    'weight' => '5',
    'children' => array(
      0 => 'field_user_country',
      1 => 'field_gender',
      2 => 'field_birth_date',
      3 => 'field_death_date',
      4 => 'field_birth_place',
      5 => 'field_biography',
      6 => 'field_example_of_handwriting',
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
    'weight' => '131',
    'children' => array(
      0 => 'field_gender',
      1 => 'field_birth_date',
      2 => 'field_death_date',
      3 => 'field_birth_place',
      4 => 'field_biography',
      5 => 'field_example_of_handwriting',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Personal information',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
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
    'weight' => '130',
    'children' => array(
      0 => 'field_areas_of_professional_inte',
      1 => 'field_places_worked',
      2 => 'field_main_collaborators_co_auth',
      3 => 'field_known_for',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Professional information',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
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
  $field_group->parent_name = 'group_biography';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '16',
    'children' => array(
      0 => 'group_names_and_identifiers',
      1 => 'group_professional_information',
      2 => 'group_personal_information',
      3 => 'group_additional_information',
    ),
    'format_type' => 'tabs',
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
      4 => 'field_places_worked',
      5 => 'field_main_collaborators_co_auth',
      6 => 'field_known_for',
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
