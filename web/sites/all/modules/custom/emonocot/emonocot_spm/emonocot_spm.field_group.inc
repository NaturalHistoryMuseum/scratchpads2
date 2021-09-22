<?php
/**
 * @file
 * emonocot_spm.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function emonocot_spm_field_group_info() {
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
    'weight' => '3',
    'children' => array(
      0 => 'field_conservation_status',
      1 => 'field_legislation',
      2 => 'field_management',
      3 => 'field_procedures',
      4 => 'field_threats',
      5 => 'field_trends',
      6 => 'field_spm_em_cs_ref',
      7 => 'field_spm_em_leg_ref',
      8 => 'field_spm_em_manage_ref',
      9 => 'field_spm_em_proc_ref',
      10 => 'field_spm_em_threats_ref',
      11 => 'field_spm_em_trends_ref',
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
      6 => 'field_spm_em_cs_ref',
      7 => 'field_spm_em_leg_ref',
      8 => 'field_spm_em_manage_ref',
      9 => 'field_spm_em_proc_ref',
      10 => 'field_spm_em_threats_ref',
      11 => 'field_spm_em_trends_ref',
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
    'weight' => '4',
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
      11 => 'field_sm_em_phys_ref',
      12 => 'field_spm_em_beha_ref',
      13 => 'field_spm_em_cytol_ref',
      14 => 'field_spm_em_dd_ref',
      15 => 'field_spm_em_genetics_ref',
      16 => 'field_spm_em_growth_ref',
      17 => 'field_spm_em_la_ref',
      18 => 'field_spm_em_mb_ref',
      19 => 'field_spm_em_morph_ref',
      20 => 'field_spm_em_size_ref',
      21 => 'field_spm_em_tb_ref',
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
      11 => 'field_sm_em_phys_ref',
      12 => 'field_spm_em_beha_ref',
      13 => 'field_spm_em_cytol_ref',
      14 => 'field_spm_em_dd_ref',
      15 => 'field_spm_em_genetics_ref',
      16 => 'field_spm_em_growth_ref',
      17 => 'field_spm_em_la_ref',
      18 => 'field_spm_em_mb_ref',
      19 => 'field_spm_em_morph_ref',
      20 => 'field_spm_em_size_ref',
      21 => 'field_spm_em_tb_ref',
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
    'weight' => '6',
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
      10 => 'field_population_biology',
      11 => 'field_reproduction',
      12 => 'field_trophic_strategy',
      13 => 'field_spm_em_assoc',
      14 => 'field_spm_em_cyclicity_ref',
      15 => 'field_spm_em_dispersal_ref',
      16 => 'field_spm_em_distribution_ref',
      17 => 'field_spm_em_ecology_ref',
      18 => 'field_spm_em_habitat_ref',
      19 => 'field_spm_em_lc_ref',
      20 => 'field_spm_em_le_ref',
      21 => 'field_spm_em_migration_ref',
      22 => 'field_spm_em_popbio_ref',
      23 => 'field_spm_em_repro_ref',
      24 => 'field_spm_em_ts_ref',
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
      10 => 'field_population_biology',
      11 => 'field_reproduction',
      12 => 'field_trophic_strategy',
      13 => 'field_spm_em_assoc',
      14 => 'field_spm_em_cyclicity_ref',
      15 => 'field_spm_em_dispersal_ref',
      16 => 'field_spm_em_distribution_ref',
      17 => 'field_spm_em_ecology_ref',
      18 => 'field_spm_em_habitat_ref',
      19 => 'field_spm_em_lc_ref',
      20 => 'field_spm_em_le_ref',
      21 => 'field_spm_em_migration_ref',
      22 => 'field_spm_em_popbio_ref',
      23 => 'field_spm_em_repro_ref',
      24 => 'field_spm_em_ts_ref',
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
    'weight' => '5',
    'children' => array(
      0 => 'field_evolution',
      1 => 'field_phylogeny',
      2 => 'field_spm_em_evo_ref',
      3 => 'field_spm_em_phylo_ref',
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
      2 => 'field_spm_em_evo_ref',
      3 => 'field_spm_em_phylo_ref',
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
    'weight' => '2',
    'children' => array(
      0 => 'field_media',
      1 => 'field_biology',
      2 => 'field_general_description',
      3 => 'field_spm_em_bio_ref',
      4 => 'field_spm_em_gd_ref',
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
      0 => 'field_media',
      1 => 'field_biology',
      2 => 'field_general_description',
      3 => 'field_spm_em_bio_ref',
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
    'weight' => '8',
    'children' => array(
      0 => 'field_diseases',
      1 => 'field_risk_statement',
      2 => 'field_uses',
      3 => 'field_spm_em_diseases_ref',
      4 => 'field_spm_em_rs_ref',
      5 => 'field_spm_em_uses_ref',
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
      3 => 'field_spm_em_diseases_ref',
      4 => 'field_spm_em_rs_ref',
      5 => 'field_spm_em_uses_ref',
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
  $field_group->identifier = 'group_spm_em_misc|node|spm|default';
  $field_group->group_name = 'group_spm_em_misc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '10',
    'children' => array(
      0 => 'field_spm_em_anatomy',
      1 => 'field_spm_em_anatomy_ref',
      2 => 'field_spm_em_cultivation',
      3 => 'field_spm_em_phen_ref',
      4 => 'field_spm_em_phenology',
      5 => 'field_spm_em_poll_ref',
      6 => 'field_spm_em_pollination',
      7 => 'field_spm_em_cult_ref',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_spm_em_misc|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_misc|node|spm|form';
  $field_group->group_name = 'group_spm_em_misc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '5',
    'children' => array(
      0 => 'field_spm_em_anatomy',
      1 => 'field_spm_em_anatomy_ref',
      2 => 'field_spm_em_cultivation',
      3 => 'field_spm_em_phen_ref',
      4 => 'field_spm_em_phenology',
      5 => 'field_spm_em_poll_ref',
      6 => 'field_spm_em_pollination',
      7 => 'field_spm_em_cult_ref',
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
  $export['group_spm_em_misc|node|spm|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_taxonomy|node|spm|default';
  $field_group->group_name = 'group_spm_em_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '11',
    'children' => array(
      0 => 'field_spm_em_taxonomic_notes',
      1 => 'field_spm_em_tn_ref',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => NULL,
    ),
  );
  $export['group_spm_em_taxonomy|node|spm|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_taxonomy|node|spm|form';
  $field_group->group_name = 'group_spm_em_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '6',
    'children' => array(
      0 => 'field_spm_em_taxonomic_notes',
      1 => 'field_spm_em_tn_ref',
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
  $export['group_spm_em_taxonomy|node|spm|form'] = $field_group;

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
      0 => 'group_conservation',
      1 => 'group_description',
      2 => 'group_eco_and_distro',
      3 => 'group_evol_and_systematics',
      4 => 'group_overview',
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
