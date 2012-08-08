<?php

/**
 * Implementation of hook_views_default_views().
 *
 * @see dwca_export_views_api()
 */
function dwca_export_views_default_views(){
  static $view_description_names = array(
    'general_description',
    'morphology',
    'behavious',
    'biology',
    'conservation_status',
    'legislation',
    'management',
    'procedures',
    'threats',
    'associations',
    'cyclicity',
    'diagnostic_description',
    'diseases',
    'dispersal',
    'distribution',
    'ecology',
    'evolution',
    'genetics',
    'growth',
    'habitat',
    'life_cycle',
    'life_expectancy',
    'look_alikes',
    'migration',
    'molecular_biology',
    'physiology',
    'population_biology',
    'reproduction',
    'risk_statement',
    'size',
    'taxon_biology',
    'trophic_strategy',
    'uses'
  );
  $views = array();
  //----------------------------------------------------------------------------
  // CLASSIFICATION
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_classification';
  $view->description = 'DwC-A core data for a Taxon';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'DwC-A Export Classification';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Classification Export View';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Taxonomy term: Parent term */
  $handler->display->display_options['relationships']['parent']['id'] = 'parent';
  $handler->display->display_options['relationships']['parent']['table'] = 'taxonomy_term_hierarchy';
  $handler->display->display_options['relationships']['parent']['field'] = 'parent';
  $handler->display->display_options['relationships']['parent']['required'] = 0;
  /* Relationship: Taxonomy term: Reference (field_reference) */
  $handler->display->display_options['relationships']['field_reference_nid']['id'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['table'] = 'field_data_field_reference';
  $handler->display->display_options['relationships']['field_reference_nid']['field'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['required'] = 0;
  $handler->display->display_options['relationships']['field_reference_nid']['delta'] = '-1';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'scientificName';
  $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['link_to_taxonomy'] = 0;
  /* Field: Taxonomy term: Vernacular names */
  $handler->display->display_options['fields']['field_vernacular_names']['id'] = 'field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['table'] = 'field_data_field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['field'] = 'field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['label'] = 'vernacularName';
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_vernacular_names']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Usage */
  $handler->display->display_options['fields']['field_usage']['id'] = 'field_usage';
  $handler->display->display_options['fields']['field_usage']['table'] = 'field_data_field_usage';
  $handler->display->display_options['fields']['field_usage']['field'] = 'field_usage';
  $handler->display->display_options['fields']['field_usage']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_usage']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_usage']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_usage']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_usage']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_usage']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_usage']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_usage']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_usage']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_usage']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_usage']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Unacceptability Reason */
  $handler->display->display_options['fields']['field_unacceptability_reason']['id'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['table'] = 'field_data_field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['field'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['label'] = 'taxonomicStatus';
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_unacceptability_reason']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_unacceptability_reason']['empty'] = '[field_usage]';
  $handler->display->display_options['fields']['field_unacceptability_reason']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_unacceptability_reason']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_unacceptability_reason']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Rank */
  $handler->display->display_options['fields']['field_rank']['id'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['table'] = 'field_data_field_rank';
  $handler->display->display_options['fields']['field_rank']['field'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['label'] = 'taxonRank';
  $handler->display->display_options['fields']['field_rank']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_rank']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_rank']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_rank']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_rank']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_rank']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_rank']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_rank']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_rank']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Authors */
  $handler->display->display_options['fields']['field_authors']['id'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['table'] = 'field_data_field_authors';
  $handler->display->display_options['fields']['field_authors']['field'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['label'] = 'scientificNameAuthorship';
  $handler->display->display_options['fields']['field_authors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_authors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_authors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_authors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_authors']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_authors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_authors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_authors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_authors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_authors']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Reference */
  /* Field: Biblio: Biblio Citation */
  $handler->display->display_options['fields']['citation']['id'] = 'citation';
  $handler->display->display_options['fields']['citation']['table'] = 'biblio';
  $handler->display->display_options['fields']['citation']['field'] = 'citation';
  $handler->display->display_options['fields']['citation']['relationship'] = 'field_reference_nid';
  $handler->display->display_options['fields']['citation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['external'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['citation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['citation']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['citation']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['citation']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['citation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['citation']['empty_zero'] = 0;
  $handler->display->display_options['fields']['citation']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['citation']['style_name'] = 'chicago-author-date.csl';
  $handler->display->display_options['fields']['citation']['export_links'] = 0;
  $handler->display->display_options['fields']['citation']['file_attachments'] = 0;
  $handler->display->display_options['fields']['citation']['open_url_link'] = 0;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['relationship'] = 'parent';
  $handler->display->display_options['fields']['uuid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid_1']['hide_alter_empty'] = 1;
  /* Sort criterion: Taxonomy term: Term ID */
  $handler->display->display_options['sorts']['tid']['id'] = 'tid';
  $handler->display->display_options['sorts']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['tid']['field'] = 'tid';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['operator'] = 'not in';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'image_keywords' => 'image_keywords',
    'imaging_technique' => 'imaging_technique',
    'preparation_technique' => 'preparation_technique'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'dwca_export/classification/page';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca_export/classification/csv';
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // REFERENCE
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_reference';
  $view->description = 'DwC-A reference extension for a Taxon';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'DwC-A Export Reference';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reference Export';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['required'] = 1;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Content: Node UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'node';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid_1']['hide_alter_empty'] = 1;
  /* Field: Biblio: Call Number */
  $handler->display->display_options['fields']['biblio_call_number']['id'] = 'biblio_call_number';
  $handler->display->display_options['fields']['biblio_call_number']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_call_number']['field'] = 'biblio_call_number';
  $handler->display->display_options['fields']['biblio_call_number']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_call_number']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_call_number']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_call_number']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_call_number']['biblio_label'] = 1;
  /* Field: Biblio: Citation Key */
  $handler->display->display_options['fields']['biblio_citekey']['id'] = 'biblio_citekey';
  $handler->display->display_options['fields']['biblio_citekey']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_citekey']['field'] = 'biblio_citekey';
  $handler->display->display_options['fields']['biblio_citekey']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_citekey']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_citekey']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_citekey']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_citekey']['biblio_label'] = 1;
  /* Field: Biblio: DOI */
  $handler->display->display_options['fields']['biblio_doi']['id'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_doi']['field'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['alter']['text'] = 'doi:[biblio_doi]';
  $handler->display->display_options['fields']['biblio_doi']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_doi']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_doi']['biblio_label'] = 1;
  /* Field: Biblio: ISBN Number */
  $handler->display->display_options['fields']['biblio_isbn']['id'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_isbn']['field'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['text'] = 'urn:ISBN:[biblio_isbn]';
  $handler->display->display_options['fields']['biblio_isbn']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_isbn']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_isbn']['biblio_label'] = 1;
  /* Field: Biblio: ISSN Number */
  $handler->display->display_options['fields']['biblio_issn']['id'] = 'biblio_issn';
  $handler->display->display_options['fields']['biblio_issn']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_issn']['field'] = 'biblio_issn';
  $handler->display->display_options['fields']['biblio_issn']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['alter']['text'] = 'urn:ISSN:[biblio_issn]';
  $handler->display->display_options['fields']['biblio_issn']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_issn']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_issn']['biblio_label'] = 1;
  /* Field: Biblio: Other Numbers */
  $handler->display->display_options['fields']['biblio_other_number']['id'] = 'biblio_other_number';
  $handler->display->display_options['fields']['biblio_other_number']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_other_number']['field'] = 'biblio_other_number';
  $handler->display->display_options['fields']['biblio_other_number']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_other_number']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_other_number']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_other_number']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_other_number']['biblio_label'] = 1;
  /* Field: Biblio: Biblio Citation */
  $handler->display->display_options['fields']['citation']['id'] = 'citation';
  $handler->display->display_options['fields']['citation']['table'] = 'biblio';
  $handler->display->display_options['fields']['citation']['field'] = 'citation';
  $handler->display->display_options['fields']['citation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['external'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['citation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['citation']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['citation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['citation']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['citation']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['citation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['citation']['empty_zero'] = 0;
  $handler->display->display_options['fields']['citation']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['citation']['style_name'] = 'chicago-author-date.csl';
  $handler->display->display_options['fields']['citation']['export_links'] = 0;
  $handler->display->display_options['fields']['citation']['file_attachments'] = 0;
  $handler->display->display_options['fields']['citation']['open_url_link'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
  /* Field: Biblio: Secondary Title */
  $handler->display->display_options['fields']['biblio_secondary_title']['id'] = 'biblio_secondary_title';
  $handler->display->display_options['fields']['biblio_secondary_title']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_secondary_title']['field'] = 'biblio_secondary_title';
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_secondary_title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_secondary_title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_secondary_title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_secondary_title']['biblio_label'] = 1;
  /* Field: Biblio: Notes */
  $handler->display->display_options['fields']['biblio_notes']['id'] = 'biblio_notes';
  $handler->display->display_options['fields']['biblio_notes']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_notes']['field'] = 'biblio_notes';
  $handler->display->display_options['fields']['biblio_notes']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_notes']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_notes']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_notes']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_notes']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_notes']['hide_alter_empty'] = 1;
  /* Field: Biblio: Keyword */
  $handler->display->display_options['fields']['word']['id'] = 'word';
  $handler->display->display_options['fields']['word']['table'] = 'biblio_keyword_data';
  $handler->display->display_options['fields']['word']['field'] = 'word';
  $handler->display->display_options['fields']['word']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['word']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['word']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['word']['alter']['external'] = 0;
  $handler->display->display_options['fields']['word']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['word']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['word']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['word']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['word']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['word']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['word']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['word']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['word']['alter']['html'] = 0;
  $handler->display->display_options['fields']['word']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['word']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['word']['hide_empty'] = 0;
  $handler->display->display_options['fields']['word']['empty_zero'] = 0;
  $handler->display->display_options['fields']['word']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['word']['link_to_keyword'] = 0;
  /* Field: Biblio: Publication Language */
  $handler->display->display_options['fields']['biblio_lang']['id'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_lang']['field'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_lang']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_lang']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_lang']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_lang']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_lang']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_lang']['biblio_label'] = 1;
  /* Field: Biblio: Type of Publication */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'biblio_types';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  /* Field: Biblio: Date Published */
  $handler->display->display_options['fields']['biblio_date']['id'] = 'biblio_date';
  $handler->display->display_options['fields']['biblio_date']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_date']['field'] = 'biblio_date';
  $handler->display->display_options['fields']['biblio_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_date']['biblio_label'] = 1;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'c';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['changed']['date_format'] = 'custom';
  $handler->display->display_options['fields']['changed']['custom_date_format'] = 'c';
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
    'biblio' => 'biblio'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'dwc-biblio-new';
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
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca-export/reference/csv';
  $handler->display->display_options['sitename_title'] = 0;
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // IMAGE
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_image';
  $view->description = 'DwC-A image extension for a Taxon';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'DwC-A Export Image';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['required'] = 1;
  /* Relationship: File Usage: File */
  $handler->display->display_options['relationships']['node_to_file']['id'] = 'node_to_file';
  $handler->display->display_options['relationships']['node_to_file']['table'] = 'file_usage';
  $handler->display->display_options['relationships']['node_to_file']['field'] = 'node_to_file';
  $handler->display->display_options['relationships']['node_to_file']['required'] = 1;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'contributor';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['relationship'] = 'node_to_file';
  $handler->display->display_options['fields']['uri']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uri']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uri']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uri']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uri']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uri']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uri']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uri']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uri']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['uri']['link_to_file'] = 0;
  $handler->display->display_options['fields']['uri']['file_download_path'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['relationship'] = 'node_to_file';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['filename']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filename']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filename']['empty'] = '[title]';
  $handler->display->display_options['fields']['filename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filename']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filename']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['filename']['link_to_file'] = 0;
  /* Field: File: Mime type */
  $handler->display->display_options['fields']['filemime']['id'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filemime']['field'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['relationship'] = 'node_to_file';
  $handler->display->display_options['fields']['filemime']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['filemime']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['filemime']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filemime']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filemime']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filemime']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filemime']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filemime']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filemime']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['filemime']['link_to_file'] = 0;
  $handler->display->display_options['fields']['filemime']['filemime_image'] = 0;
  /* Field: File: Licence */
  $handler->display->display_options['fields']['field_cc_licence']['id'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['table'] = 'field_data_field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['field'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['relationship'] = 'node_to_file';
  $handler->display->display_options['fields']['field_cc_licence']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['trim_whitespace'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_cc_licence']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_cc_licence']['field_api_classes'] = 0;
  /* Field: User: User UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'users';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uuid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid_1']['hide_alter_empty'] = 1;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'http://purl.org/dc/dcmitype/StillImage';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
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
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'node_to_file';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca-export/image/page';
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // SPECIMEN
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_specimen';
  $view->description = 'DwC-A specimen extension for a Taxon';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'DwC-A Export Specimen';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'test_specimen';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Taxonomy term: Content using Taxonomic name */
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['id'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['field'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['required'] = 1;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Content: Type status */
  $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['field_type_status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_type_status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_type_status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_type_status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_type_status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_type_status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_type_status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_type_status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_type_status']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_type_status']['field_api_classes'] = 0;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['field_institution_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['field_api_classes'] = 0;
  /* Field: Content: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['field_collection_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['field_api_classes'] = 0;
  /* Field: Content: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['field_api_classes'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'dwca_export/specimen/page';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca_export/specimen/csv';
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // EoL AGENTS
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_eol_agents';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'DwC-A Export EoL Agents';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: User UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'users';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: User: Family name */
  $handler->display->display_options['fields']['field_user_family_name']['id'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['table'] = 'field_data_field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['field'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['field_api_classes'] = 0;
  /* Field: User: Other/given name(s)  */
  $handler->display->display_options['fields']['field_user_given_names']['id'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['field_api_classes'] = 0;
  /* Field: User: Other/given name(s)  */
  $handler->display->display_options['fields']['field_user_given_names_1']['id'] = 'field_user_given_names_1';
  $handler->display->display_options['fields']['field_user_given_names_1']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names_1']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names_1']['label'] = 'Full Name';
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['text'] = '[field_user_given_names_1] [field_user_family_name]';
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_given_names_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_given_names_1']['field_api_classes'] = 0;
  /* Field: User: Institution */
  $handler->display->display_options['fields']['field_user_institution']['id'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['table'] = 'field_data_field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['field'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['field_api_classes'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['use_batch'] = '0';
  $handler->display->display_options['path'] = 'dwca_export/eol_agents/csv';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // DISTRIBUTION
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_distribution';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'DwC-A Distribution';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'DwC-A Distribution';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Relationship: Content: Taxonomy terms on node */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'cichorieae' => 0,
    'image_keywords' => 0,
    'imaging_technique' => 0,
    'preparation_technique' => 0
  );
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'term_node_tid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_spm_map']['id'] = 'field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['table'] = 'field_data_field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['field'] = 'field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['click_sort_column'] = 'region_id';
  $handler->display->display_options['fields']['field_spm_map']['type'] = 'gm3_text';
  $handler->display->display_options['fields']['field_spm_map']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  $handler->display->display_options['fields']['field_spm_map']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_spm_map']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['field_api_classes'] = 0;
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
  /* Filter criterion: Content: Distribution (field_distribution) */
  $handler->display->display_options['filters']['field_distribution_value']['id'] = 'field_distribution_value';
  $handler->display->display_options['filters']['field_distribution_value']['table'] = 'field_data_field_distribution';
  $handler->display->display_options['filters']['field_distribution_value']['field'] = 'field_distribution_value';
  $handler->display->display_options['filters']['field_distribution_value']['operator'] = 'not empty';
  /* Filter criterion: Taxonomy term: Term */
  $handler->display->display_options['filters']['tid']['id'] = 'tid';
  $handler->display->display_options['filters']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['tid']['field'] = 'tid';
  $handler->display->display_options['filters']['tid']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['tid']['operator'] = 'not empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'dwca-export/distribution/page';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca-export/distribution/csv';
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //--------------------------------------------------------------------------------
  // Vernacular Names
  //--------------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_vernacular_names';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'dwca_export_vernacular_names';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Taxonomy term: Vernacular names */
  $handler->display->display_options['fields']['field_vernacular_names']['id'] = 'field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['table'] = 'field_data_field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['field'] = 'field_vernacular_names';
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_vernacular_names']['group_rows'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_vernacular_names']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_vernacular_names']['field_api_classes'] = 0;
  /* Filter criterion: Taxonomy term: Vernacular names (field_vernacular_names) */
  $handler->display->display_options['filters']['field_vernacular_names_value']['id'] = 'field_vernacular_names_value';
  $handler->display->display_options['filters']['field_vernacular_names_value']['table'] = 'field_data_field_vernacular_names';
  $handler->display->display_options['filters']['field_vernacular_names_value']['field'] = 'field_vernacular_names_value';
  $handler->display->display_options['filters']['field_vernacular_names_value']['operator'] = 'not empty';
  /* Filter criterion: Taxonomy term: Vernacular names (field_vernacular_names) */
  $handler->display->display_options['filters']['field_vernacular_names_value_1']['id'] = 'field_vernacular_names_value_1';
  $handler->display->display_options['filters']['field_vernacular_names_value_1']['table'] = 'field_data_field_vernacular_names';
  $handler->display->display_options['filters']['field_vernacular_names_value_1']['field'] = 'field_vernacular_names_value';
  $handler->display->display_options['filters']['field_vernacular_names_value_1']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['field_vernacular_names_value_1']['value'] = '0';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca_export/vernacular_names/csv';
  $handler->display->display_options['use_batch'] = '0';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // DESCRIPTION VIEWS
  //----------------------------------------------------------------------------
  foreach($view_description_names as $desc_type){
    $view = _dwca_export_view_description($desc_type);
    $views[$view->name] = $view;
  }
  return $views;
}

function _dwca_export_view_description($desc_type){
  $desc_type_std = $desc_type;
  if($desc_type == 'general_description'){
    $desc_type_std = 'general';
  }
  $view = new view();
  $view->name = 'dwca_export_description_' . substr($desc_type, 0, 6);
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'DwC-A Description ' . $desc_type;
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'DwC-A Description ' . $desc_type;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Relationship: Content: Taxonomy terms on node */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'cichorieae' => 0,
    'image_keywords' => 0,
    'imaging_technique' => 0,
    'preparation_technique' => 0
  );
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'term_node_tid';
  $handler->display->display_options['fields']['uuid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid']['hide_alter_empty'] = 1;
  /* Field: Content: description */
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['id'] = t('field_' . $desc_type);
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['table'] = t('field_data_field_' . $desc_type);
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['field'] = t('field_' . $desc_type);
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['label'] = 'type';
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['alter_text'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['text'] = $desc_type_std;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['make_link'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['absolute'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['external'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['nl2br'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['more_link'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['trim'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['alter']['html'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['element_label_colon'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['element_default_classes'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['empty'] = $desc_type;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['hide_empty'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['empty_zero'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['hide_alter_empty'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type)]['field_api_classes'] = 0;
  /* Field: Content: description */
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['id'] = t('field_' . $desc_type . '_1');
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['table'] = t('field_data_field_' . $desc_type);
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['field'] = t('field_' . $desc_type);
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['label'] = 'description';
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['alter_text'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['make_link'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['absolute'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['external'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['nl2br'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['more_link'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['trim'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['alter']['html'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['element_label_colon'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['element_default_classes'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['hide_empty'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['empty_zero'] = 0;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['hide_alter_empty'] = 1;
  $handler->display->display_options['fields'][t('field_' . $desc_type . '_1')]['field_api_classes'] = 0;
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
    'spm' => 'spm'
  );
  /* Display: Page  */
  /*$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = t('dwca-export/description-' . $desc_type . '/page');*/
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['path'] = t('dwca-export/description-' . $desc_type . '/csv');
  $handler->display->display_options['use_batch'] = '0';
  return $view;
}