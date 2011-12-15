<?php
/**
 * @file
 * itis_term.features.field_group.inc
 */

/**
 * Implementation of hook_field_group_default_field_groups().
 */
function itis_term_field_group_default_field_groups() {
  $field_groups = array();

  // Exported field_group: 'taxonomy_term-itis_vocabulary-form-group_reference'
  $field_groups['taxonomy_term-itis_vocabulary-form-group_reference'] = array(
    'group_name' => 'group_reference',
    'entity_type' => 'taxonomy_term',
    'bundle' => 'itis_vocabulary',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
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
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );

  // Exported field_group: 'taxonomy_term-itis_vocabulary-form-group_scientific_name'
  $field_groups['taxonomy_term-itis_vocabulary-form-group_scientific_name'] = array(
    'group_name' => 'group_scientific_name',
    'entity_type' => 'taxonomy_term',
    'bundle' => 'itis_vocabulary',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
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
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );

  // Exported field_group: 'taxonomy_term-itis_vocabulary-form-group_usage'
  $field_groups['taxonomy_term-itis_vocabulary-form-group_usage'] = array(
    'group_name' => 'group_usage',
    'entity_type' => 'taxonomy_term',
    'bundle' => 'itis_vocabulary',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Usage',
    'weight' => '9',
    'children' => array(
      0 => 'field_associated_accepted_name',
      1 => 'field_unacceptability_reason',
      2 => 'field_usage',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );

  // Exported field_group: 'taxonomy_term-itis_vocabulary-form-group_vernacular_names'
  $field_groups['taxonomy_term-itis_vocabulary-form-group_vernacular_names'] = array(
    'group_name' => 'group_vernacular_names',
    'entity_type' => 'taxonomy_term',
    'bundle' => 'itis_vocabulary',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Vernacular names',
    'weight' => '11',
    'children' => array(
      0 => 'field_vernacular_names',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );

  // Exported field_group: 'taxonomy_term-itis_vocabulary-form-group_vertical_tabs'
  $field_groups['taxonomy_term-itis_vocabulary-form-group_vertical_tabs'] = array(
    'group_name' => 'group_vertical_tabs',
    'entity_type' => 'taxonomy_term',
    'bundle' => 'itis_vocabulary',
    'mode' => 'form',
    'parent_name' => '',
    'label' => 'Vertical tabs',
    'weight' => '8',
    'children' => array(
      0 => 'group_reference',
      1 => 'group_scientific_name',
      2 => 'group_usage',
      3 => 'group_vernacular_names',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => NULL,
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  return $field_groups;
}
