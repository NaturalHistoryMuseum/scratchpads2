<?php

/**
 * @file
 * pensoft_publication.field_group.inc
 */
/**
 * Implements hook_field_group_info().
 */
function pensoft_publication_field_group_info(){
  $export = array();
  $field_group = new stdClass();
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
    'weight' => '11',
    'children' => array(
      0 => 'field_pensoft_acknowledgments'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Acknowledgments',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => ''
          )
        ),
        'display_mode' => ''
      )
    )
  );
  $export['group_pensoft_acknowledgments|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '12',
    'children' => array(
      0 => 'field_pensoft_acknowledgments'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_pensoft_acknowledgments|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '10',
    'children' => array(
      0 => 'field_pensoft_files',
      1 => 'field_publication_references',
      2 => 'field_publication_tables',
      3 => 'field_publication_figures'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible'
    )
  );
  $export['group_pensoft_files|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '11',
    'children' => array(
      0 => 'field_pensoft_files'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_pensoft_files|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_abs_keyws|publication|pensoft|form';
  $field_group->group_name = 'group_publication_abs_keyws';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abstract & Keywords',
    'weight' => '2',
    'children' => array(
      0 => 'field_publication_abstract',
      1 => 'field_publication_keywords'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_abs_keyws|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_abstract_keyw|publication|pensoft|default';
  $field_group->group_name = 'group_publication_abstract_keyw';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abstract & Keywords',
    'weight' => '2',
    'children' => array(
      0 => 'field_publication_abstract',
      1 => 'field_publication_keywords'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible'
    )
  );
  $export['group_publication_abstract_keyw|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_auth_contrib|publication|pensoft|default';
  $field_group->group_name = 'group_publication_auth_contrib';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Authors & Contributors',
    'weight' => '1',
    'children' => array(
      0 => 'field_publication_authors',
      1 => 'field_publication_contributors',
      2 => 'field_publication_submit_author',
      3 => 'field_publication_ling_editor',
      4 => 'field_publication_copy_editor',
      5 => 'field_publication_mentor'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Authors & Contributors',
      'instance_settings' => array(
        'display_mode' => '',
        'links' => array(
          'edit_link' => 0,
          'additional_link' => array(
            'url' => '',
            'title' => ''
          )
        )
      )
    )
  );
  $export['group_publication_auth_contrib|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_authors|publication|pensoft|form';
  $field_group->group_name = 'group_publication_authors';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Authors',
    'weight' => '1',
    'children' => array(
      0 => 'field_publication_authors'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_authors|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_contributors|publication|pensoft|form';
  $field_group->group_name = 'group_publication_contributors';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contributors',
    'weight' => '3',
    'children' => array(
      0 => 'field_publication_contributors'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_contributors|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_data_resources|publication|pensoft|form';
  $field_group->group_name = 'group_publication_data_resources';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Data Resources',
    'weight' => '6',
    'children' => array(
      0 => 'field_publication_data_resources'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_data_resources|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '9',
    'children' => array(
      0 => 'field_publication_discussion'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Discussion',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => ''
          )
        ),
        'display_mode' => ''
      )
    )
  );
  $export['group_publication_discussion|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '10',
    'children' => array(
      0 => 'field_publication_discussion'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_discussion|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_figures|publication|pensoft|form';
  $field_group->group_name = 'group_publication_figures';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Figures',
    'weight' => '14',
    'children' => array(
      0 => 'field_publication_figures'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_figures|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '3',
    'children' => array(
      0 => 'field_publication_introduction'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Introduction',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => ''
          )
        ),
        'display_mode' => ''
      )
    )
  );
  $export['group_publication_introduction|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '4',
    'children' => array(
      0 => 'field_publication_introduction'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_introduction|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '4',
    'children' => array(
      0 => 'field_publication_methods'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible'
    )
  );
  $export['group_publication_methods|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '5',
    'children' => array(
      0 => 'field_publication_methods'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_methods|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_references|publication|pensoft|form';
  $field_group->group_name = 'group_publication_references';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'References',
    'weight' => '13',
    'children' => array(
      0 => 'field_publication_references'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_references|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_resources|publication|pensoft|default';
  $field_group->group_name = 'group_publication_resources';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Data Resources',
    'weight' => '5',
    'children' => array(
      0 => 'field_publication_data_resources'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible'
    )
  );
  $export['group_publication_resources|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_results|publication|pensoft|default';
  $field_group->group_name = 'group_publication_results';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Results',
    'weight' => '6',
    'children' => array(
      0 => 'field_publication_results'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'formatter' => 'collapsible'
    )
  );
  $export['group_publication_results|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_results|publication|pensoft|form';
  $field_group->group_name = 'group_publication_results';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Results',
    'weight' => '7',
    'children' => array(
      0 => 'field_publication_results'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_results|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '8',
    'children' => array(
      0 => 'field_publication_systematics',
      1 => 'field_publication_treatments'
    ),
    'format_type' => 'publication',
    'format_settings' => array(
      'label' => 'Systematics',
      'instance_settings' => array(
        'links' => array(
          'edit_link' => 1,
          'additional_link' => array(
            'url' => '',
            'title' => ''
          )
        ),
        'display_mode' => ''
      )
    )
  );
  $export['group_publication_systematics|publication|pensoft|default'] = $field_group;
  $field_group = new stdClass();
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
    'weight' => '8',
    'children' => array(
      0 => 'field_publication_systematics'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_systematics|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_tables|publication|pensoft|form';
  $field_group->group_name = 'group_publication_tables';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tables',
    'weight' => '15',
    'children' => array(
      0 => 'field_publication_tables'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_tables|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_title_authors|publication|pensoft|form';
  $field_group->group_name = 'group_publication_title_authors';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Title',
    'weight' => '0',
    'children' => array(
      0 => 'field_publication_title',
      1 => 'title'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Title',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => ''
      ),
      'formatter' => 'closed'
    )
  );
  $export['group_publication_title_authors|publication|pensoft|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_treatments|publication|pensoft|form';
  $field_group->group_name = 'group_publication_treatments';
  $field_group->entity_type = 'publication';
  $field_group->bundle = 'pensoft';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Treatments',
    'weight' => '9',
    'children' => array(
      0 => 'field_publication_treatments'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_publication_treatments|publication|pensoft|form'] = $field_group;
  return $export;
}
