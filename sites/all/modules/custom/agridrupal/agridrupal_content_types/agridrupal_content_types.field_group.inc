<?php
/**
 * @file
 * agridrupal_content_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function agridrupal_content_types_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abstract|node|agdlios|form';
  $field_group->group_name = 'group_abstract';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abstract',
    'weight' => '4',
    'children' => array(
      0 => 'field_ag_abstract',
      1 => 'field_ag_langtext',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_abstract|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abstract|node|agperiodical|form';
  $field_group->group_name = 'group_abstract';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abstract',
    'weight' => '3',
    'children' => array(
      0 => 'field_ag_abstract',
      1 => 'field_ag_langtext',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-abstract field-group-field_group_multiple_container',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_abstract|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_access|node|agdlios|default';
  $field_group->group_name = 'group_access';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Access and copyright',
    'weight' => '15',
    'children' => array(
      0 => 'field_ag_access',
      1 => 'field_ag_acces',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-access field-group-hidden',
      ),
    ),
  );
  $export['group_access|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_access|node|agdlios|form';
  $field_group->group_name = 'group_access';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Access and copyright',
    'weight' => '21',
    'children' => array(
      0 => 'field_ag_access',
      1 => 'field_ag_acces',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Access and copyright',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-access field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_access|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_access|node|agdlios|teaser';
  $field_group->group_name = 'group_access';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Access and copyright',
    'weight' => '21',
    'children' => array(
      0 => 'field_ag_acces',
      1 => 'field_ag_access',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-access field-group-hidden',
      ),
    ),
  );
  $export['group_access|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_access|node|agperiodical|form';
  $field_group->group_name = 'group_access';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Access and copyright',
    'weight' => '10',
    'children' => array(
      0 => 'field_ag_acces',
      1 => 'field_ag_access',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-access field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_access|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_affiliations|node|agprofile|form';
  $field_group->group_name = 'group_affiliations';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => ' Affiliations / career',
    'weight' => '10',
    'children' => array(
      0 => 'field_ag_position',
      1 => 'field_ag_duties',
      2 => 'field_ag_period',
      3 => 'field_ag_institution',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => ' Affiliations / career',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => ' group-affiliations field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'div',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_affiliations|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_alttitle|node|agdlios|form';
  $field_group->group_name = 'group_alttitle';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Alternative title',
    'weight' => '3',
    'children' => array(
      0 => 'field_ag_subtitle',
      1 => 'field_ag_langsubt',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Alternative title',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-alttitle field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'fieldset',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_alttitle|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_alttitle|node|agperiodical|form';
  $field_group->group_name = 'group_alttitle';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Alternative title',
    'weight' => '2',
    'children' => array(
      0 => 'field_ag_subtitle',
      1 => 'field_ag_langsubt',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-alttitle field-group-field_group_multiple_container',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_alttitle|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_areas|node|aginstitution|default';
  $field_group->group_name = 'group_areas';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'aginstitution';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Area(s) of activity',
    'weight' => '2',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-areas field-group-fieldset',
      ),
    ),
  );
  $export['group_areas|node|aginstitution|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_areas|node|aginstitution|form';
  $field_group->group_name = 'group_areas';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'aginstitution';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Area(s) of activity',
    'weight' => '7',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-areas field-group-fieldset',
      ),
    ),
  );
  $export['group_areas|node|aginstitution|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_areas|node|aginstitution|teaser';
  $field_group->group_name = 'group_areas';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'aginstitution';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Area(s) of activity',
    'weight' => '2',
    'children' => array(
      0 => 'field_ag_agris',
      1 => 'field_ag_agrovoc',
    ),
    'format_type' => 'no_style',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-areas field-group-no_style',
      ),
    ),
  );
  $export['group_areas|node|aginstitution|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_authors|node|agdlios|full';
  $field_group->group_name = 'group_authors';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Authors',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_author',
      1 => 'field_ag_corauthor',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-authors field-group-fieldset',
      ),
    ),
  );
  $export['group_authors|node|agdlios|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_author|node|agdlios|form';
  $field_group->group_name = 'group_author';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal Author',
    'weight' => '10',
    'children' => array(
      0 => 'field_ag_author',
      1 => 'field_ag_role',
      2 => 'field_ag_auth_order',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Personal Author',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-author field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'fieldset',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_author|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_author|node|agperiodical|form';
  $field_group->group_name = 'group_author';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal Author',
    'weight' => '7',
    'children' => array(
      0 => 'field_ag_author',
      1 => 'field_ag_role',
      2 => 'field_ag_auth_order',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-author field-group-field_group_multiple_container',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_author|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_availability|node|agdlios|full';
  $field_group->group_name = 'group_availability';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Availability',
    'weight' => '3',
    'children' => array(
      0 => 'upload',
      1 => 'field_ag_access',
      2 => 'field_ag_acces',
      3 => 'field_ag_phlocation',
      4 => 'field_ag_locator',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-availability field-group-fieldset',
      ),
    ),
  );
  $export['group_availability|node|agdlios|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_book|node|agdlios|default';
  $field_group->group_name = 'group_book';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Book',
    'weight' => '8',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => ' group-book field-group-hidden',
      ),
    ),
  );
  $export['group_book|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_book|node|agdlios|form';
  $field_group->group_name = 'group_book';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Book',
    'weight' => '13',
    'children' => array(
      0 => 'field_ag_book',
      1 => 'field_ag_booknumber',
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
  $export['group_book|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_book|node|agdlios|teaser';
  $field_group->group_name = 'group_book';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Book',
    'weight' => '13',
    'children' => array(
      0 => 'field_ag_book',
      1 => 'field_ag_booknumber',
    ),
    'format_type' => 'simple',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-book field-group-simple',
      ),
    ),
  );
  $export['group_book|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_career|node|agprofile|default';
  $field_group->group_name = 'group_career';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Career',
    'weight' => '7',
    'children' => array(
      0 => 'field_ag_position',
      1 => 'field_ag_duties',
      2 => 'field_ag_institution',
      3 => 'field_ag_period',
    ),
    'format_type' => 'field_group_multiple_table',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-career field-group-field_group_multiple_table',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_career|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact|node|aginstitution|form';
  $field_group->group_name = 'group_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'aginstitution';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact information',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_telephone',
      1 => 'field_ag_address',
      2 => 'field_ag_email',
      3 => 'field_ag_country',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-contact field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_contact|node|aginstitution|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact|node|agprofile|default';
  $field_group->group_name = 'group_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact information',
    'weight' => '4',
    'children' => array(
      0 => 'field_ag_telephone',
      1 => 'field_ag_address',
      2 => 'field_ag_email',
      3 => 'field_ag_country',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Contact information',
      'instance_settings' => array(
        'classes' => ' group-contact field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_contact|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact|node|agprofile|form';
  $field_group->group_name = 'group_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact information',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_telephone',
      1 => 'field_ag_address',
      2 => 'field_ag_email',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-contact field-group-fieldset',
      ),
    ),
  );
  $export['group_contact|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact|node|agprofile|teaser';
  $field_group->group_name = 'group_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact information',
    'weight' => '2',
    'children' => array(
      0 => 'field_ag_address',
      1 => 'field_ag_country',
      2 => 'field_ag_email',
      3 => 'field_ag_telephone',
      4 => 'field_ag_address',
      5 => 'field_ag_country',
      6 => 'field_ag_email',
      7 => 'field_ag_telephone',
    ),
    'format_type' => 'no_style',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-contact field-group-no_style',
      ),
    ),
  );
  $export['group_contact|node|agprofile|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_corporateauthor|node|agdlios|form';
  $field_group->group_name = 'group_corporateauthor';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Corporate author',
    'weight' => '11',
    'children' => array(
      0 => 'field_ag_corauthor',
      1 => 'field_ag_rolecorp',
      2 => 'field_ag_orderc',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Corporate author',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-corporateauthor field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'fieldset',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_corporateauthor|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_corporateauthor|node|agperiodical|form';
  $field_group->group_name = 'group_corporateauthor';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Corporate author',
    'weight' => '8',
    'children' => array(
      0 => 'field_ag_corauthor',
      1 => 'field_ag_rolecorp',
      2 => 'field_ag_orderc',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-corporateauthor field-group-field_group_multiple_container',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_corporateauthor|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_degree|node|agprofile|form';
  $field_group->group_name = 'group_degree';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Studies',
    'weight' => '9',
    'children' => array(
      0 => 'field_ag_degree',
      1 => 'field_ag_study_field',
      2 => 'field_ag_degree_institution',
      3 => 'field_ag_year',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Studies',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-degree field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_degree|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|agrssjob|default';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agrssjob';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '-3',
    'children' => array(
      0 => 'field_ag_country',
      1 => 'field_ag_deadline',
      2 => 'field_ag_employer',
      3 => 'field_ag_location',
      4 => 'field_ag_country',
      5 => 'field_ag_deadline',
      6 => 'field_ag_institution',
      7 => 'field_ag_location',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-details field-group-fieldset',
      ),
    ),
  );
  $export['group_details|node|agrssjob|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|agrssjob|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agrssjob';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '-3',
    'children' => array(
      0 => 'field_ag_country',
      1 => 'field_ag_deadline',
      2 => 'field_ag_employer',
      3 => 'field_ag_location',
      4 => 'field_ag_country',
      5 => 'field_ag_deadline',
      6 => 'field_ag_institution',
      7 => 'field_ag_location',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-details field-group-fieldset',
      ),
    ),
  );
  $export['group_details|node|agrssjob|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|agrssjob|teaser';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agrssjob';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '-3',
    'children' => array(
      0 => 'field_ag_country',
      1 => 'field_ag_deadline',
      2 => 'field_ag_employer',
      3 => 'field_ag_location',
      4 => 'field_ag_country',
      5 => 'field_ag_deadline',
      6 => 'field_ag_institution',
      7 => 'field_ag_location',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-details field-group-fieldset',
      ),
    ),
  );
  $export['group_details|node|agrssjob|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edition|node|agdlios|default';
  $field_group->group_name = 'group_edition';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Date and language',
    'weight' => '7',
    'children' => array(
      0 => 'field_ag_language',
      1 => 'field_ag_datepub',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-edition field-group-fieldset',
      ),
    ),
  );
  $export['group_edition|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edition|node|agdlios|form';
  $field_group->group_name = 'group_edition';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Date and Language',
    'weight' => '9',
    'children' => array(
      0 => 'field_ag_language',
      1 => 'field_ag_datepub',
      2 => 'field_ag_chkdatepub',
      3 => 'field_ag_aproxdatepub',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Date and Language',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-edition field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_edition|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edition|node|agdlios|teaser';
  $field_group->group_name = 'group_edition';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Date and language',
    'weight' => '10',
    'children' => array(
      0 => 'field_ag_aproxdatepub',
      1 => 'field_ag_chkdatepub',
      2 => 'field_ag_datepub',
      3 => 'field_ag_language',
    ),
    'format_type' => 'no_style',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-edition field-group-no_style',
      ),
    ),
  );
  $export['group_edition|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edition|node|agperiodical|form';
  $field_group->group_name = 'group_edition';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Date and Language',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_datepub',
      1 => 'field_ag_aproxdatepub',
      2 => 'field_ag_chkdatepub',
      3 => 'field_ag_language',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-edition field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_edition|node|agperiodical|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_expertise|node|agprofile|default';
  $field_group->group_name = 'group_expertise';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Expertise',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_degree',
      1 => 'field_ag_study_field',
      2 => 'field_ag_degree_institution',
      3 => 'field_ag_year',
      4 => 'field_ag_agris',
      5 => 'field_ag_agrovoc',
      6 => 'field_ag_working_language',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-expertise field-group-fieldset',
      ),
    ),
  );
  $export['group_expertise|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_expertise|node|agprofile|form';
  $field_group->group_name = 'group_expertise';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Expertise',
    'weight' => '7',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_working_language',
      2 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-expertise field-group-fieldset',
      ),
    ),
  );
  $export['group_expertise|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_expertise|node|agprofile|teaser';
  $field_group->group_name = 'group_expertise';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Expertise',
    'weight' => '6',
    'children' => array(
      0 => 'field_ag_agris',
      1 => 'field_ag_agrovoc',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-expertise field-group-hidden',
      ),
    ),
  );
  $export['group_expertise|node|agprofile|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_journal|node|agdlios|default';
  $field_group->group_name = 'group_journal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Journal ',
    'weight' => '10',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => ' group-journal field-group-hidden',
      ),
    ),
  );
  $export['group_journal|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_journal|node|agdlios|form';
  $field_group->group_name = 'group_journal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Journal',
    'weight' => '16',
    'children' => array(
      0 => 'field_ag_journal',
      1 => 'field_ag_volume',
      2 => 'field_ag_number',
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
  $export['group_journal|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_journal|node|agdlios|teaser';
  $field_group->group_name = 'group_journal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Journal ',
    'weight' => '15',
    'children' => array(
      0 => 'field_ag_journal',
      1 => 'field_ag_number',
      2 => 'field_ag_volume',
    ),
    'format_type' => 'no_style',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-journal field-group-no_style',
      ),
    ),
  );
  $export['group_journal|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|agdlios|default';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '16',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => ' group-location field-group-hidden',
      ),
    ),
  );
  $export['group_location|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|agdlios|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '23',
    'children' => array(
      0 => 'field_ag_phlocation',
      1 => 'field_ag_locator',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-location field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'fieldset',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_location|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|agdlios|teaser';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '23',
    'children' => array(
      0 => 'field_ag_locator',
      1 => 'field_ag_phlocation',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-location field-group-hidden',
      ),
    ),
  );
  $export['group_location|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_more_profiles|node|agprofile|form';
  $field_group->group_name = 'group_more_profiles';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'More profiles',
    'weight' => '11',
    'children' => array(
      0 => 'field_ag_personal_website',
      1 => 'group_social',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-more-profiles field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_more_profiles|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_others|node|agdlios|default';
  $field_group->group_name = 'group_others';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Other information',
    'weight' => '17',
    'children' => array(
      0 => 'field_ag_pagination',
      1 => 'field_ag_form',
      2 => 'field_ag_notes',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-others field-group-hidden',
      ),
    ),
  );
  $export['group_others|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_others|node|agdlios|form';
  $field_group->group_name = 'group_others';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Other information',
    'weight' => '24',
    'children' => array(
      0 => 'field_ag_pagination',
      1 => 'field_ag_form',
      2 => 'field_ag_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Other information',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => ' group-others field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_others|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_others|node|agdlios|teaser';
  $field_group->group_name = 'group_others';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Other information',
    'weight' => '24',
    'children' => array(
      0 => 'field_ag_form',
      1 => 'field_ag_notes',
      2 => 'field_ag_pagination',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-others field-group-fieldset',
      ),
    ),
  );
  $export['group_others|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_profiles|node|agprofile|default';
  $field_group->group_name = 'group_profiles';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Other profiles',
    'weight' => '8',
    'children' => array(
      0 => 'field_ag_personal_website',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-profiles field-group-fieldset',
      ),
    ),
  );
  $export['group_profiles|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publications|node|agprofile|default';
  $field_group->group_name = 'group_publications';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Publications',
    'weight' => '10',
    'children' => array(
      0 => 'field_ag_pub_title',
      1 => 'field_ag_url_of_publication',
    ),
    'format_type' => 'field_group_multiple_table',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-publications field-group-field_group_multiple_table',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_publications|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication|node|agprofile|form';
  $field_group->group_name = 'group_publication';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Publications',
    'weight' => '12',
    'children' => array(
      0 => 'field_ag_pub_title',
      1 => 'field_ag_url_of_publication',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Publications',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => ' group-publication field-group-field_group_multiple_container',
        'description' => '',
        'parent_format_type' => 'div',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_publication|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publisher|node|agdlios|form';
  $field_group->group_name = 'group_publisher';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Publisher informations',
    'weight' => '12',
    'children' => array(
      0 => 'field_ag_publisher',
      1 => 'field_ag_publishplace',
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
  $export['group_publisher|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social_networks|node|agprofile|default';
  $field_group->group_name = 'group_social_networks';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Social channels and professional networks',
    'weight' => '9',
    'children' => array(
      0 => 'field_ag_network',
      1 => 'field_ag_profile_url',
      2 => 'field_ag_network_id',
    ),
    'format_type' => 'field_group_multiple_table',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'parent_format_type' => 'div',
        'classes' => ' group-social-networks field-group-field_group_multiple_table',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_social_networks|node|agprofile|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|agprofile|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agprofile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_more_profiles';
  $field_group->data = array(
    'label' => 'Social channels and professional networks',
    'weight' => '14',
    'children' => array(
      0 => 'field_ag_network',
      1 => 'field_ag_profile_url',
      2 => 'field_ag_network_id',
    ),
    'format_type' => 'field_group_multiple_container',
    'format_settings' => array(
      'label' => 'Social channels and professional networks',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => ' group-social field-group-field_group_multiple_container',
        'description' => 'Here you can provide your IDs and profile URLs in various social channels and international initiatives. In the future, this will help connect your various profiles in different communities and link to your publications',
        'parent_format_type' => 'div',
        'show_label' => '1',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_social|node|agprofile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_source|node|agdlios|full';
  $field_group->group_name = 'group_source';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Source',
    'weight' => '17',
    'children' => array(
      0 => 'field_ag_link_to_jissue',
      1 => 'field_ag_source',
      2 => 'field_ag_journal_issue',
      3 => 'field_ag_confname',
      4 => 'field_ag_book',
      5 => 'field_ag_journal',
      6 => 'field_ag_volume',
      7 => 'field_ag_number',
      8 => 'field_pages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Source',
      'instance_settings' => array(
        'classes' => ' group-source field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_source|node|agdlios|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_topics|node|agdlios|full';
  $field_group->group_name = 'group_topics';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Topic(s)',
    'weight' => '15',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-topics field-group-fieldset',
      ),
    ),
  );
  $export['group_topics|node|agdlios|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vocabulary|node|agdlios|default';
  $field_group->group_name = 'group_vocabulary';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vocabulary',
    'weight' => '14',
    'children' => array(
      0 => 'field_ag_agris',
      1 => 'field_ag_agrovoc',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-vocabulary field-group-fieldset',
      ),
    ),
  );
  $export['group_vocabulary|node|agdlios|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vocabulary|node|agdlios|form';
  $field_group->group_name = 'group_vocabulary';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vocabulary',
    'weight' => '20',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Vocabulary',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-vocabulary field-group-fieldset',
        'description' => '<B>PLEASE INSERT AGROVOC TERMS SEPARATED BY COMMA.<BR>
ATTENTION:WHEN YOU SAVE AN AGROVOC TERM COULD BE A NON-DESCRIPTOR TERM, THE VOCABULARY COULD SUBSTITUTE YOUR TERM WITH HIS RELATED DESCRIPTOR.</B></BR></BR>
<B>HERE YOU CAN FIND THE COMPLETE <a href="/Agridrupal7.x.7.9/agrislist" class="simple-dialog" title="AGRIS LIST">AGRIS LIST</a> WITH DESCRIPTIONS.</B>

',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_vocabulary|node|agdlios|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vocabulary|node|agdlios|teaser';
  $field_group->group_name = 'group_vocabulary';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agdlios';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vocabulary',
    'weight' => '20',
    'children' => array(
      0 => 'field_ag_agris',
      1 => 'field_ag_agrovoc',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => ' group-vocabulary field-group-fieldset',
      ),
    ),
  );
  $export['group_vocabulary|node|agdlios|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vocabulary|node|agperiodical|form';
  $field_group->group_name = 'group_vocabulary';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'agperiodical';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vocabulary',
    'weight' => '9',
    'children' => array(
      0 => 'field_ag_agrovoc',
      1 => 'field_ag_agris',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-vocabulary field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_vocabulary|node|agperiodical|form'] = $field_group;

  return $export;
}
