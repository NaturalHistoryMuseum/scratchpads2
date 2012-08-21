<?php

/**
 * @file
 * emonocot_spm.field_group.inc
 */
/**
 * Implements hook_field_group_info().
 */
function emonocot_spm_field_group_info(){
  $export = array();
  $field_group = new stdClass();
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
      11 => 'field_spm_em_trends_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_conservation|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      11 => 'field_spm_em_beha_ref',
      12 => 'field_spm_em_cytol_ref',
      13 => 'field_spm_em_dd_ref',
      14 => 'field_spm_em_genetics_ref',
      15 => 'field_spm_em_growth_ref',
      16 => 'field_spm_em_la_ref',
      17 => 'field_spm_em_mb_ref',
      18 => 'field_spm_em_morph_ref',
      19 => 'field_sm_em_phys_ref',
      20 => 'field_spm_em_size_ref',
      21 => 'field_spm_em_tb_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_description|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      12 => 'field_spm_em_assoc',
      13 => 'field_spm_em_cyclicity_ref',
      14 => 'field_spm_em_dispersal_ref',
      15 => 'field_spm_em_distribution_ref',
      16 => 'field_spm_em_ecology_ref',
      17 => 'field_spm_em_habitat_ref',
      18 => 'field_spm_em_lc_ref',
      19 => 'field_spm_em_le_ref',
      20 => 'field_spm_em_migration_ref',
      21 => 'field_spm_em_ts_ref',
      22 => 'field_spm_em_popbio_ref',
      23 => 'field_spm_em_repro_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_eco_and_distro|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      2 => 'field_spm_em_phylo_ref',
      3 => 'field_spm_em_evo_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_evol_and_systematics|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      0 => 'field_biology',
      1 => 'field_general_description',
      2 => 'field_spm_em_gd_ref',
      3 => 'field_spm_em_bio_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_overview|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      5 => 'field_spm_em_uses_ref'
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_relevance|node|spm|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_misc|node|spm|form';
  $field_group->group_name = 'group_spm_em_misc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '5',
    'children' => array(
      0 => 'field_spm_em_cultivation',
      1 => 'field_spm_em_cult_ref',
      2 => 'field_spm_em_phenology',
      3 => 'field_spm_em_phen_ref',
      4 => 'field_spm_em_pollination',
      5 => 'field_spm_em_poll_ref',
      6 => 'field_spm_em_anatomy',
      7 => 'field_spm_em_anatomy_ref'
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
  $export['group_spm_em_misc|node|spm|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_taxonomy|node|spm|form';
  $field_group->group_name = 'group_spm_em_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '6',
    'children' => array(
      0 => 'field_spm_em_taxonomic_notes',
      1 => 'field_spm_em_tn_ref'
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
  $export['group_spm_em_taxonomy|node|spm|form'] = $field_group;
  $field_group = new stdClass();
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
      0 => 'group_spm_em_misc',
      1 => 'group_conservation',
      2 => 'group_description',
      3 => 'group_eco_and_distro',
      4 => 'group_evol_and_systematics',
      5 => 'group_overview',
      6 => 'group_relevance',
      7 => 'group_spm_em_taxonomy'
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_vertical_tabs|node|spm|form'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_conservation|node|spm|default';
  $field_group->group_name = 'group_conservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
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
      11 => 'field_spm_em_trends_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_conservation|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_description|node|spm|default';
  $field_group->group_name = 'group_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
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
      11 => 'field_spm_em_beha_ref',
      12 => 'field_spm_em_cytol_ref',
      13 => 'field_spm_em_dd_ref',
      14 => 'field_spm_em_genetics_ref',
      15 => 'field_spm_em_growth_ref',
      16 => 'field_spm_em_la_ref',
      17 => 'field_spm_em_mb_ref',
      18 => 'field_spm_em_morph_ref',
      19 => 'field_sm_em_phys_ref',
      20 => 'field_spm_em_size_ref',
      21 => 'field_spm_em_tb_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_description|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_eco_and_distro|node|spm|default';
  $field_group->group_name = 'group_eco_and_distro';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Ecology and Distribution',
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
      12 => 'field_spm_em_assoc',
      13 => 'field_spm_em_cyclicity_ref',
      14 => 'field_spm_em_dispersal_ref',
      15 => 'field_spm_em_distribution_ref',
      16 => 'field_spm_em_ecology_ref',
      17 => 'field_spm_em_habitat_ref',
      18 => 'field_spm_em_lc_ref',
      19 => 'field_spm_em_le_ref',
      20 => 'field_spm_em_migration_ref',
      21 => 'field_spm_em_ts_ref',
      22 => 'field_spm_em_popbio_ref',
      23 => 'field_spm_em_repro_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_eco_and_distro|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_evol_and_systematics|node|spm|default';
  $field_group->group_name = 'group_evol_and_systematics';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Evolution and Systematics',
    'weight' => '2',
    'children' => array(
      0 => 'field_evolution',
      1 => 'field_phylogeny',
      2 => 'field_spm_em_phylo_ref',
      3 => 'field_spm_em_evo_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_evol_and_systematics|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overview|node|spm|default';
  $field_group->group_name = 'group_overview';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Overview',
    'weight' => '-1',
    'children' => array(
      0 => 'field_biology',
      1 => 'field_general_description',
      2 => 'field_spm_em_gd_ref',
      3 => 'field_spm_em_bio_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_overview|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_relevance|node|spm|default';
  $field_group->group_name = 'group_relevance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
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
      5 => 'field_spm_em_uses_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'open',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_relevance|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_misc|node|spm|default';
  $field_group->group_name = 'group_spm_em_misc';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Misc',
    'weight' => '5',
    'children' => array(
      0 => 'field_spm_em_cultivation',
      1 => 'field_spm_em_cult_ref',
      2 => 'field_spm_em_phenology',
      3 => 'field_spm_em_phen_ref',
      4 => 'field_spm_em_pollination',
      5 => 'field_spm_em_poll_ref',
      6 => 'field_spm_em_anatomy',
      7 => 'field_spm_em_anatomy_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_spm_em_misc|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spm_em_taxonomy|node|spm|default';
  $field_group->group_name = 'group_spm_em_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_vertical_tabs';
  $field_group->data = array(
    'label' => 'Taxonomy',
    'weight' => '6',
    'children' => array(
      0 => 'field_spm_em_taxonomic_notes',
      1 => 'field_spm_em_tn_ref'
    ),
    'defaultat_type' => 'tab',
    'defaultat_settings' => array(
      'defaultatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1
      )
    )
  );
  $export['group_spm_em_taxonomy|node|spm|default'] = $field_group;
  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vertical_tabs|node|spm|default';
  $field_group->group_name = 'group_vertical_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'spm';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vertical tabs',
    'weight' => '2',
    'children' => array(
      0 => 'group_spm_em_misc',
      1 => 'group_conservation',
      2 => 'group_description',
      3 => 'group_eco_and_distro',
      4 => 'group_evol_and_systematics',
      5 => 'group_overview',
      6 => 'group_relevance',
      7 => 'group_spm_em_taxonomy'
    ),
    'defaultat_type' => 'tabs',
    'defaultat_settings' => array(
      'defaultatter' => '',
      'instance_settings' => array(
        'classes' => ''
      )
    )
  );
  $export['group_vertical_tabs|node|spm|default'] = $field_group;
  return $export;
}
