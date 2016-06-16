<?php
/**
 * @file
 * viewthign.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ecoint_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ecoint_csv_export';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'EcoInt-CSV-export';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Taxon 2 (field_int_collection) */
  $handler->display->display_options['relationships']['field_int_collection_value']['id'] = 'field_int_collection_value';
  $handler->display->display_options['relationships']['field_int_collection_value']['table'] = 'field_data_field_int_collection';
  $handler->display->display_options['relationships']['field_int_collection_value']['field'] = 'field_int_collection_value';
  $handler->display->display_options['relationships']['field_int_collection_value']['delta'] = '-1';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['label'] = 'term from Species1';
  /* Relationship: Content: Interaction Type (field_interaction_type) */
  $handler->display->display_options['relationships']['field_interaction_type_tid']['id'] = 'field_interaction_type_tid';
  $handler->display->display_options['relationships']['field_interaction_type_tid']['table'] = 'field_data_field_interaction_type';
  $handler->display->display_options['relationships']['field_interaction_type_tid']['field'] = 'field_interaction_type_tid';
  $handler->display->display_options['relationships']['field_interaction_type_tid']['required'] = TRUE;
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['id'] = 'field_taxonomic_name_tid_1';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['label'] = 'Species2';
  /* Relationship: Content: Specimen 1 (field_specimen_1) */
  $handler->display->display_options['relationships']['field_specimen_1_nid']['id'] = 'field_specimen_1_nid';
  $handler->display->display_options['relationships']['field_specimen_1_nid']['table'] = 'field_data_field_specimen_1';
  $handler->display->display_options['relationships']['field_specimen_1_nid']['field'] = 'field_specimen_1_nid';
  $handler->display->display_options['relationships']['field_specimen_1_nid']['delta'] = '-1';
  /* Relationship: Field collection item: Specimen (field_specimen_2) */
  $handler->display->display_options['relationships']['field_specimen_2_nid']['id'] = 'field_specimen_2_nid';
  $handler->display->display_options['relationships']['field_specimen_2_nid']['table'] = 'field_data_field_specimen_2';
  $handler->display->display_options['relationships']['field_specimen_2_nid']['field'] = 'field_specimen_2_nid';
  $handler->display->display_options['relationships']['field_specimen_2_nid']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['relationships']['field_specimen_2_nid']['delta'] = '-1';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_2']['id'] = 'field_taxonomic_name_tid_2';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_2']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_2']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_2']['relationship'] = 'field_specimen_1_nid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_2']['label'] = 'spec1_taxon';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_3']['id'] = 'field_taxonomic_name_tid_3';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_3']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_3']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_3']['relationship'] = 'field_specimen_2_nid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_3']['label'] = 'spec2_taxon';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'InteractionID';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = 'InteractionURL';
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_2']['id'] = 'uuid_2';
  $handler->display->display_options['fields']['uuid_2']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_2']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_2']['relationship'] = 'field_taxonomic_name_tid_2';
  $handler->display->display_options['fields']['uuid_2']['label'] = '';
  $handler->display->display_options['fields']['uuid_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uuid_2']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['uuid']['label'] = 'Species1UUID';
  $handler->display->display_options['fields']['uuid']['empty'] = '[uuid_2]';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'field_taxonomic_name_tid_2';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = 'Species1Name';
  $handler->display->display_options['fields']['field_taxonomic_name']['empty'] = '[name]';
  $handler->display->display_options['fields']['field_taxonomic_name']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Taxonomy term: Authority */
  $handler->display->display_options['fields']['field_textformattedauthority_1']['id'] = 'field_textformattedauthority_1';
  $handler->display->display_options['fields']['field_textformattedauthority_1']['table'] = 'field_data_field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_1']['field'] = 'field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_1']['relationship'] = 'field_taxonomic_name_tid_2';
  $handler->display->display_options['fields']['field_textformattedauthority_1']['label'] = '';
  $handler->display->display_options['fields']['field_textformattedauthority_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_textformattedauthority_1']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Authority */
  $handler->display->display_options['fields']['field_textformattedauthority']['id'] = 'field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority']['table'] = 'field_data_field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority']['field'] = 'field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['field_textformattedauthority']['label'] = 'Species1Authority';
  $handler->display->display_options['fields']['field_textformattedauthority']['empty'] = '[field_textformattedauthority_1]';
  /* Field: Content: Life Cycle Stage */
  $handler->display->display_options['fields']['field_life_cycle_stage']['id'] = 'field_life_cycle_stage';
  $handler->display->display_options['fields']['field_life_cycle_stage']['table'] = 'field_data_field_life_cycle_stage';
  $handler->display->display_options['fields']['field_life_cycle_stage']['field'] = 'field_life_cycle_stage';
  $handler->display->display_options['fields']['field_life_cycle_stage']['label'] = 'Species1LifeCycleStage';
  /* Field: Content: Organism Part */
  $handler->display->display_options['fields']['field_organism_part']['id'] = 'field_organism_part';
  $handler->display->display_options['fields']['field_organism_part']['table'] = 'field_data_field_organism_part';
  $handler->display->display_options['fields']['field_organism_part']['field'] = 'field_organism_part';
  $handler->display->display_options['fields']['field_organism_part']['label'] = 'Species1OrganismPart';
  /* Field: Content: Species Status */
  $handler->display->display_options['fields']['field_species_status']['id'] = 'field_species_status';
  $handler->display->display_options['fields']['field_species_status']['table'] = 'field_data_field_species_status';
  $handler->display->display_options['fields']['field_species_status']['field'] = 'field_species_status';
  $handler->display->display_options['fields']['field_species_status']['label'] = 'Species1Status';
  /* Field: Content: Interaction Type */
  $handler->display->display_options['fields']['field_interaction_type']['id'] = 'field_interaction_type';
  $handler->display->display_options['fields']['field_interaction_type']['table'] = 'field_data_field_interaction_type';
  $handler->display->display_options['fields']['field_interaction_type']['field'] = 'field_interaction_type';
  $handler->display->display_options['fields']['field_interaction_type']['label'] = 'InteractionType';
  /* Field: Taxonomy term: Ontology Term URL */
  $handler->display->display_options['fields']['field_ontology_url']['id'] = 'field_ontology_url';
  $handler->display->display_options['fields']['field_ontology_url']['table'] = 'field_data_field_ontology_url';
  $handler->display->display_options['fields']['field_ontology_url']['field'] = 'field_ontology_url';
  $handler->display->display_options['fields']['field_ontology_url']['relationship'] = 'field_interaction_type_tid';
  $handler->display->display_options['fields']['field_ontology_url']['label'] = 'InteractionOntologyURL';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_3']['id'] = 'uuid_3';
  $handler->display->display_options['fields']['uuid_3']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_3']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_3']['relationship'] = 'field_taxonomic_name_tid_3';
  $handler->display->display_options['fields']['uuid_3']['label'] = '';
  $handler->display->display_options['fields']['uuid_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uuid_3']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['relationship'] = 'field_taxonomic_name_tid_1';
  $handler->display->display_options['fields']['uuid_1']['label'] = 'Species2UUID';
  $handler->display->display_options['fields']['uuid_1']['empty'] = '[uuid_3]';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'field_taxonomic_name_tid_3';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name_1']['id'] = 'field_taxonomic_name_1';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['label'] = 'Species2Name';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['empty'] = '[name_1]';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_taxonomic_name_1']['delta_offset'] = '0';
  /* Field: Taxonomy term: Authority */
  $handler->display->display_options['fields']['field_textformattedauthority_3']['id'] = 'field_textformattedauthority_3';
  $handler->display->display_options['fields']['field_textformattedauthority_3']['table'] = 'field_data_field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_3']['field'] = 'field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_3']['relationship'] = 'field_taxonomic_name_tid_3';
  $handler->display->display_options['fields']['field_textformattedauthority_3']['label'] = '';
  $handler->display->display_options['fields']['field_textformattedauthority_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_textformattedauthority_3']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Authority */
  $handler->display->display_options['fields']['field_textformattedauthority_2']['id'] = 'field_textformattedauthority_2';
  $handler->display->display_options['fields']['field_textformattedauthority_2']['table'] = 'field_data_field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_2']['field'] = 'field_textformattedauthority';
  $handler->display->display_options['fields']['field_textformattedauthority_2']['relationship'] = 'field_taxonomic_name_tid_1';
  $handler->display->display_options['fields']['field_textformattedauthority_2']['label'] = 'Species2Authority';
  $handler->display->display_options['fields']['field_textformattedauthority_2']['empty'] = '[field_textformattedauthority_3]';
  /* Field: Field collection item: Life Cycle Stage */
  $handler->display->display_options['fields']['field_life_cycle_stage_2']['id'] = 'field_life_cycle_stage_2';
  $handler->display->display_options['fields']['field_life_cycle_stage_2']['table'] = 'field_data_field_life_cycle_stage_2';
  $handler->display->display_options['fields']['field_life_cycle_stage_2']['field'] = 'field_life_cycle_stage_2';
  $handler->display->display_options['fields']['field_life_cycle_stage_2']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['fields']['field_life_cycle_stage_2']['label'] = 'Species2LifeCycleStage';
  /* Field: Field collection item: Organism Part */
  $handler->display->display_options['fields']['field_organism_part_2']['id'] = 'field_organism_part_2';
  $handler->display->display_options['fields']['field_organism_part_2']['table'] = 'field_data_field_organism_part_2';
  $handler->display->display_options['fields']['field_organism_part_2']['field'] = 'field_organism_part_2';
  $handler->display->display_options['fields']['field_organism_part_2']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['fields']['field_organism_part_2']['label'] = 'Species2OrganismPart';
  /* Field: Field collection item: Species Status */
  $handler->display->display_options['fields']['field_species_status_2']['id'] = 'field_species_status_2';
  $handler->display->display_options['fields']['field_species_status_2']['table'] = 'field_data_field_species_status_2';
  $handler->display->display_options['fields']['field_species_status_2']['field'] = 'field_species_status_2';
  $handler->display->display_options['fields']['field_species_status_2']['relationship'] = 'field_int_collection_value';
  $handler->display->display_options['fields']['field_species_status_2']['label'] = 'Species2Status';
  /* Field: Content: Importance */
  $handler->display->display_options['fields']['field_importance']['id'] = 'field_importance';
  $handler->display->display_options['fields']['field_importance']['table'] = 'field_data_field_importance';
  $handler->display->display_options['fields']['field_importance']['field'] = 'field_importance';
  /* Field: Content: Interaction Record Type */
  $handler->display->display_options['fields']['field_interaction_record_type']['id'] = 'field_interaction_record_type';
  $handler->display->display_options['fields']['field_interaction_record_type']['table'] = 'field_data_field_interaction_record_type';
  $handler->display->display_options['fields']['field_interaction_record_type']['field'] = 'field_interaction_record_type';
  $handler->display->display_options['fields']['field_interaction_record_type']['label'] = 'InteractionRecordType';
  /* Field: Field: Reference */
  $handler->display->display_options['fields']['field_reference']['id'] = 'field_reference';
  $handler->display->display_options['fields']['field_reference']['table'] = 'field_data_field_reference';
  $handler->display->display_options['fields']['field_reference']['field'] = 'field_reference';
  /* Field: Content: Reference Page */
  $handler->display->display_options['fields']['field_reference_page']['id'] = 'field_reference_page';
  $handler->display->display_options['fields']['field_reference_page']['table'] = 'field_data_field_reference_page';
  $handler->display->display_options['fields']['field_reference_page']['field'] = 'field_reference_page';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Notes';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'ecological_interactions' => 'ecological_interactions',
  );

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'interactions/export';
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $translatables['ecoint_csv_export'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('field collection item from field_int_collection'),
    t('term from Species1'),
    t('term from field_interaction_type'),
    t('Species2'),
    t('field_specimen_1'),
    t('field_specimen_2'),
    t('spec1_taxon'),
    t('spec2_taxon'),
    t('InteractionID'),
    t('InteractionURL'),
    t('Species1UUID'),
    t('[uuid_2]'),
    t('Species1Name'),
    t('[name]'),
    t('Species1Authority'),
    t('[field_textformattedauthority_1]'),
    t('Species1LifeCycleStage'),
    t('Species1OrganismPart'),
    t('Species1Status'),
    t('InteractionType'),
    t('InteractionOntologyURL'),
    t('Species2UUID'),
    t('[uuid_3]'),
    t('Species2Name'),
    t('[name_1]'),
    t('Species2Authority'),
    t('[field_textformattedauthority_3]'),
    t('Species2LifeCycleStage'),
    t('Species2OrganismPart'),
    t('Species2Status'),
    t('Importance'),
    t('InteractionRecordType'),
    t('Reference'),
    t('Reference Page'),
    t('Notes'),
    t('Data export'),
  );
  $export['ecoint_csv_export'] = $view;

  return $export;
}
