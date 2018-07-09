<?php
/**
 * @file
 * spm.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function spm_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_conservation|node|spm|default';
  $field_group->group_name = 'group_conservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '0',
    'children' => array(
      0 => 'field_conservation_status',
      1 => 'field_legislation',
      2 => 'field_management',
      3 => 'field_procedures',
      4 => 'field_threats',
      5 => 'field_trends',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_conservation|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_conservation|node|spm|form';
  $field_group->group_name = 'group_conservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Conservation',
    'weight' => '0',
    'children' => array(
      0 => 'field_conservation_status',
      1 => 'field_legislation',
      2 => 'field_management',
      3 => 'field_procedures',
      4 => 'field_threats',
      5 => 'field_trends',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_conservation|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_description|node|spm|default';
  $field_group->group_name = 'group_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '1',
    'children' => array(
      0 => 'field_behavious',
      1 => 'field_cytology',
      2 => 'field_diagnostic_description',
      3 => 'field_genetics',
      4 => 'field_growth',
      5 => 'field_look_alikes',
      6 => 'field_molecular_biology',
      7 => 'field_morphology',
      8 => 'field_physiology',
      9 => 'field_size',
      10 => 'field_taxon_biology',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_description|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_description|node|spm|form';
  $field_group->group_name = 'group_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Description',
    'weight' => '1',
    'children' => array(
      0 => 'field_behavious',
      1 => 'field_cytology',
      2 => 'field_diagnostic_description',
      3 => 'field_genetics',
      4 => 'field_growth',
      5 => 'field_look_alikes',
      6 => 'field_molecular_biology',
      7 => 'field_morphology',
      8 => 'field_physiology',
      9 => 'field_size',
      10 => 'field_taxon_biology',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_description|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_eco_and_distro|node|spm|default';
  $field_group->group_name = 'group_eco_and_distro';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '3',
    'children' => array(
      0 => 'field_associations',
      1 => 'field_cyclicity',
      2 => 'field_dispersal',
      3 => 'field_distribution',
      4 => 'field_ecology',
      5 => 'field_habitat',
      6 => 'field_life_cycle',
      7 => 'field_life_expectancy',
      8 => 'field_migration',
      9 => 'field_population_biology',
      10 => 'field_reproduction',
      11 => 'field_trophic_strategy',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_eco_and_distro|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_eco_and_distro|node|spm|form';
  $field_group->group_name = 'group_eco_and_distro';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Ecology and Distribution',
    'weight' => '3',
    'children' => array(
      0 => 'field_spm_map',
      1 => 'field_associations',
      2 => 'field_cyclicity',
      3 => 'field_dispersal',
      4 => 'field_distribution',
      5 => 'field_ecology',
      6 => 'field_habitat',
      7 => 'field_life_cycle',
      8 => 'field_life_expectancy',
      9 => 'field_migration',
      10 => 'field_trophic_strategy',
      11 => 'field_population_biology',
      12 => 'field_reproduction',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_eco_and_distro|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_evol_and_systematics|node|spm|default';
  $field_group->group_name = 'group_evol_and_systematics';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '2',
    'children' => array(
      0 => 'field_evolution',
      1 => 'field_phylogeny',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_evol_and_systematics|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_evol_and_systematics|node|spm|form';
  $field_group->group_name = 'group_evol_and_systematics';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Evolution and Systematics',
    'weight' => '2',
    'children' => array(
      0 => 'field_evolution',
      1 => 'field_phylogeny',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_evol_and_systematics|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overview|node|spm|default';
  $field_group->group_name = 'group_overview';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '-1',
    'children' => array(
      0 => 'field_biology',
      1 => 'field_general_description',
      2 => 'field_media',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_overview|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overview|node|spm|form';
  $field_group->group_name = 'group_overview';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Overview',
    'weight' => '-1',
    'children' => array(
      0 => 'field_general_description',
      1 => 'field_biology',
      2 => 'field_media',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_overview|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_relevance|node|spm|default';
  $field_group->group_name = 'group_relevance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '4',
    'children' => array(
      0 => 'field_diseases',
      1 => 'field_risk_statement',
      2 => 'field_uses',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_relevance|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_relevance|node|spm|form';
  $field_group->group_name = 'group_relevance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Relevance',
    'weight' => '4',
    'children' => array(
      0 => 'field_diseases',
      1 => 'field_risk_statement',
      2 => 'field_uses',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_relevance|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vertical_tabs|node|spm|form';
  $field_group->group_name = 'group_vertical_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vertical tabs',
    'weight' => '2',
    'children' => array(
      0 => 'group_overview',
      1 => 'group_conservation',
      2 => 'group_description',
      3 => 'group_eco_and_distro',
      4 => 'group_evol_and_systematics',
      5 => 'group_relevance',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_vertical_tabs|node|spm|form'] = $field_group;

  return $export;
}
