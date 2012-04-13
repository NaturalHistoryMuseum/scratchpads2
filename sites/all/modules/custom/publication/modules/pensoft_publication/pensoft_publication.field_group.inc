<?php
/**
 * @file
 * pensoft_publication.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function pensoft_publication_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pensoft_acknowledgments|publication|pensoft|default';
  $field_group->group_name = 'group_pensoft_acknowledgments';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Acknowledgments',
    'weight' => '19',
    'children' => array(
      0 => 'field_pensoft_acknowledgments',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Acknowledgments',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => '',
          ),
        ),
        'display_mode' => '',
      ),
    ),
  );
  $export['group_pensoft_acknowledgments|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pensoft_acknowledgments|publication|pensoft|form';
  $field_group->group_name = 'group_pensoft_acknowledgments';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Acknowledgments',
    'weight' => '18',
    'children' => array(
      0 => 'field_pensoft_acknowledgments',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_pensoft_acknowledgments|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pensoft_files|publication|pensoft|default';
  $field_group->group_name = 'group_pensoft_files';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supplementary files',
    'weight' => '21',
    'children' => array(
      0 => 'field_pensoft_files',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible',
    ),
  );
  $export['group_pensoft_files|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pensoft_files|publication|pensoft|form';
  $field_group->group_name = 'group_pensoft_files';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Supplementary files',
    'weight' => '15',
    'children' => array(
      0 => 'field_pensoft_files',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'label' => 'Supplementary files',
      'instance_settings' => array(
        'classes' => '',
        'display_mode' => '',
      ),
    ),
  );
  $export['group_pensoft_files|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_discussion|publication|pensoft|default';
  $field_group->group_name = 'group_publication_discussion';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Discussion',
    'weight' => '16',
    'children' => array(
      0 => 'field_publication_discussion',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Discussion',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => '',
          ),
        ),
        'display_mode' => '',
      ),
    ),
  );
  $export['group_publication_discussion|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_discussion|publication|pensoft|form';
  $field_group->group_name = 'group_publication_discussion';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Discussion',
    'weight' => '13',
    'children' => array(
      0 => 'field_publication_discussion',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_publication_discussion|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_introduction|publication|pensoft|default';
  $field_group->group_name = 'group_publication_introduction';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Introduction',
    'weight' => '17',
    'children' => array(
      0 => 'field_publication_introduction',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Introduction',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => '',
          ),
        ),
        'display_mode' => '',
      ),
    ),
  );
  $export['group_publication_introduction|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_introduction|publication|pensoft|form';
  $field_group->group_name = 'group_publication_introduction';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Introduction',
    'weight' => '2',
    'children' => array(
      0 => 'field_publication_introduction',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_publication_introduction|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_metadata|publication|pensoft|default';
  $field_group->group_name = 'group_publication_metadata';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '14',
    'children' => array(
      0 => 'field_publication_abstract',
      1 => 'field_publication_keywords',
      2 => 'field_publication_title',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => '',
          ),
        ),
        'display_mode' => '',
      ),
    ),
  );
  $export['group_publication_metadata|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_metadata|publication|pensoft|form';
  $field_group->group_name = 'group_publication_metadata';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '1',
    'children' => array(
      0 => 'field_publication_abstract',
      1 => 'field_publication_keywords',
      2 => 'field_publication_title',
      3 => 'title',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_publication_metadata|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_methods|publication|pensoft|default';
  $field_group->group_name = 'group_publication_methods';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Material & Methods',
    'weight' => '20',
    'children' => array(
      0 => 'field_publication_methods',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible',
    ),
  );
  $export['group_publication_methods|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_methods|publication|pensoft|form';
  $field_group->group_name = 'group_publication_methods';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Material & Methods',
    'weight' => '3',
    'children' => array(
      0 => 'field_publication_methods',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_publication_methods|publication|pensoft|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_systematics|publication|pensoft|default';
  $field_group->group_name = 'group_publication_systematics';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Systematics',
    'weight' => '18',
    'children' => array(
      0 => 'field_publication_treatments',
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Systematics',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => '',
          ),
        ),
        'display_mode' => '',
      ),
    ),
  );
  $export['group_publication_systematics|publication|pensoft|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_systematics|publication|pensoft|form';
  $field_group->group_name = 'group_publication_systematics';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Systematics',
    'weight' => '12',
    'children' => array(
      0 => 'field_publication_treatments',
    ),
    'format_type' => 'formflow',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_publication_systematics|publication|pensoft|form'] = $field_group;

  return $export;
}
