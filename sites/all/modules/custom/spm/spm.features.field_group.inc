<?php

/**
 * Implementation of hook_field_group_default_field_groups().
 */
function spm_field_group_default_field_groups() {
  $field_groups = array();

  // Exported field_group: 'node-spm-form-group_conservation'
  $field_groups['node-spm-form-group_conservation'] = array(
    'group_name' => 'group_conservation',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Conservation',
    'weight' => '0',
    'children' => array(
      '0' => 'field_conservation_status',
      '1' => 'field_legislation',
      '2' => 'field_management',
      '3' => 'field_procedures',
      '4' => 'field_threats',
      '5' => 'field_trends',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_description'
  $field_groups['node-spm-form-group_description'] = array(
    'group_name' => 'group_description',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Description',
    'weight' => '1',
    'children' => array(
      '0' => 'field_behavious',
      '1' => 'field_cytology',
      '2' => 'field_diagnostic_description',
      '3' => 'field_genetics',
      '4' => 'field_growth',
      '5' => 'field_look_alikes',
      '6' => 'field_molecular_biology',
      '7' => 'field_morphology',
      '8' => 'field_physiology',
      '9' => 'field_size',
      '10' => 'field_taxon_biology',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_eco_and_distro'
  $field_groups['node-spm-form-group_eco_and_distro'] = array(
    'group_name' => 'group_eco_and_distro',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Ecology and Distribution',
    'weight' => '3',
    'children' => array(
      '0' => 'field_evolution',
      '1' => 'field_phylogeny',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_evol_and_systematics'
  $field_groups['node-spm-form-group_evol_and_systematics'] = array(
    'group_name' => 'group_evol_and_systematics',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Evolution and Systematics',
    'weight' => '2',
    'children' => array(
      '0' => 'field_associations',
      '1' => 'field_cyclicity',
      '2' => 'field_dispersal',
      '3' => 'field_distribution',
      '4' => 'field_ecology',
      '5' => 'field_habitat',
      '6' => 'field_life_cycle',
      '7' => 'field_life_expectancy',
      '8' => 'field_migration',
      '9' => 'field_trophic_strategy',
      '10' => 'field_population_biology',
      '11' => 'field_reproduction',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_overview'
  $field_groups['node-spm-form-group_overview'] = array(
    'group_name' => 'group_overview',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Overview',
    'weight' => '-1',
    'children' => array(
      '0' => 'field_general_description',
      '1' => 'field_biology',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_relevance'
  $field_groups['node-spm-form-group_relevance'] = array(
    'group_name' => 'group_relevance',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => 'group_vertical_tabs',
    'label' => 'Relevance',
    'weight' => '4',
    'children' => array(
      '0' => 'field_diseases',
      '1' => 'field_risk_statement',
      '2' => 'field_uses',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-spm-form-group_vertical_tabs'
  $field_groups['node-spm-form-group_vertical_tabs'] = array(
    'group_name' => 'group_vertical_tabs',
    'entity_type' => 'node',
    'bundle' => 'spm',
    'mode' => 'form',
    'parent_name' => '',
    'label' => 'Vertical tabs',
    'weight' => '2',
    'children' => array(
      '0' => 'group_overview',
      '1' => 'group_conservation',
      '2' => 'group_description',
      '3' => 'group_eco_and_distro',
      '4' => 'group_evol_and_systematics',
      '5' => 'group_relevance',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  return $field_groups;
}
