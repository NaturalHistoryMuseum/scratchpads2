<?php

/**
 * Implementation of hook_views_default_views().
 *
 * @see dwca_export_views_api()
 */
function dwca_export_views_default_views(){
  global $base_url;
  $default_licence = variable_get('creative_commons_block_licence_type', CC_BY);
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
  $handler->display->display_options['use_more_always'] = FALSE;
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
  /* Relationship: Field: Source (field_reference) */
  $handler->display->display_options['relationships']['field_reference_nid']['id'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['table'] = 'field_data_field_reference';
  $handler->display->display_options['relationships']['field_reference_nid']['field'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['delta'] = '-1';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'scientificName';
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = FALSE;
  /* Field: Taxonomy term: Unacceptability Reason */
  $handler->display->display_options['fields']['field_unacceptability_reason']['id'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['table'] = 'field_data_field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['field'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason']['label'] = 'taxonomicStatus';
  $handler->display->display_options['fields']['field_unacceptability_reason']['empty'] = '[field_unacceptability_reason]';
  $handler->display->display_options['fields']['field_unacceptability_reason']['type'] = 'taxon_status';
  /* Field: Taxonomy term: Rank */
  $handler->display->display_options['fields']['field_rank']['id'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['table'] = 'field_data_field_rank';
  $handler->display->display_options['fields']['field_rank']['field'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['label'] = 'taxonRank';
  /* Field: Taxonomy term: Authors */
  $handler->display->display_options['fields']['field_authors']['id'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['table'] = 'field_data_field_authors';
  $handler->display->display_options['fields']['field_authors']['field'] = 'field_authors';
  $handler->display->display_options['fields']['field_authors']['label'] = 'scientificNameAuthorship';
  /* Field: Biblio: Biblio Citation */
  $handler->display->display_options['fields']['citation']['id'] = 'citation';
  $handler->display->display_options['fields']['citation']['table'] = 'biblio';
  $handler->display->display_options['fields']['citation']['field'] = 'citation';
  $handler->display->display_options['fields']['citation']['relationship'] = 'field_reference_nid';
  $handler->display->display_options['fields']['citation']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['citation']['export_links'] = 0;
  $handler->display->display_options['fields']['citation']['file_attachments'] = 0;
  $handler->display->display_options['fields']['citation']['open_url_link'] = 0;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'field_reference_nid';
  $handler->display->display_options['fields']['path']['label'] = 'NamePublishedInID';
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['relationship'] = 'parent';
  $handler->display->display_options['fields']['uuid_1']['exclude'] = TRUE;
  /* Field: Taxonomy term: Unacceptability Reason */
  $handler->display->display_options['fields']['field_unacceptability_reason_5']['id'] = 'field_unacceptability_reason_5';
  $handler->display->display_options['fields']['field_unacceptability_reason_5']['table'] = 'field_data_field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_5']['field'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_5']['label'] = 'Parent';
  $handler->display->display_options['fields']['field_unacceptability_reason_5']['type'] = 'parent_formatter';
  /* Field: Taxonomy term: Unacceptability Reason */
  $handler->display->display_options['fields']['field_unacceptability_reason_2']['id'] = 'field_unacceptability_reason_2';
  $handler->display->display_options['fields']['field_unacceptability_reason_2']['table'] = 'field_data_field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_2']['field'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_2']['label'] = 'NomenclaturalStatus';
  $handler->display->display_options['fields']['field_unacceptability_reason_2']['type'] = 'name_status';
  /* Field: Taxonomy term: Unacceptability Reason */
  $handler->display->display_options['fields']['field_unacceptability_reason_3']['id'] = 'field_unacceptability_reason_3';
  $handler->display->display_options['fields']['field_unacceptability_reason_3']['table'] = 'field_data_field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_3']['field'] = 'field_unacceptability_reason';
  $handler->display->display_options['fields']['field_unacceptability_reason_3']['label'] = 'AssociatedAcceptedName';
  $handler->display->display_options['fields']['field_unacceptability_reason_3']['type'] = 'aan_formatter';
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
  /* Filter criterion: Taxonomy term: Unit name 1 (field_unit_name1) */
  $handler->display->display_options['filters']['field_unit_name1_value']['id'] = 'field_unit_name1_value';
  $handler->display->display_options['filters']['field_unit_name1_value']['table'] = 'field_data_field_unit_name1';
  $handler->display->display_options['filters']['field_unit_name1_value']['field'] = 'field_unit_name1_value';
  $handler->display->display_options['filters']['field_unit_name1_value']['operator'] = 'not empty';
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
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['path'] = 'dwca_export/classification/csv';
  $handler->display->display_options['use_batch'] = 'batch';
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
  $handler->display->display_options['use_more_always'] = FALSE;
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
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'field_taxonomic_name_tid';
  /* Field: Content: Node UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'node';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  /* Field: Biblio: Citation Key */
  $handler->display->display_options['fields']['biblio_citekey']['id'] = 'biblio_citekey';
  $handler->display->display_options['fields']['biblio_citekey']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_citekey']['field'] = 'biblio_citekey';
  $handler->display->display_options['fields']['biblio_citekey']['biblio_label'] = 1;
  /* Field: Biblio: DOI */
  $handler->display->display_options['fields']['biblio_doi']['id'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_doi']['field'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['biblio_doi']['alter']['text'] = 'doi:[biblio_doi]';
  $handler->display->display_options['fields']['biblio_doi']['biblio_label'] = 1;
  /* Field: Biblio: ISBN Number */
  $handler->display->display_options['fields']['biblio_isbn']['id'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_isbn']['field'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['biblio_isbn']['alter']['text'] = 'urn:ISBN:[biblio_isbn]';
  $handler->display->display_options['fields']['biblio_isbn']['biblio_label'] = 1;
  /* Field: Biblio: ISSN Number */
  $handler->display->display_options['fields']['biblio_issn']['id'] = 'biblio_issn';
  $handler->display->display_options['fields']['biblio_issn']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_issn']['field'] = 'biblio_issn';
  $handler->display->display_options['fields']['biblio_issn']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['biblio_issn']['alter']['text'] = 'urn:ISSN:[biblio_issn]';
  $handler->display->display_options['fields']['biblio_issn']['biblio_label'] = 1;
  /* Field: Biblio: Biblio Citation */
  $handler->display->display_options['fields']['citation']['id'] = 'citation';
  $handler->display->display_options['fields']['citation']['table'] = 'biblio';
  $handler->display->display_options['fields']['citation']['field'] = 'citation';
  $handler->display->display_options['fields']['citation']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['citation']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['citation']['style_name'] = 'chicago-author-date.csl';
  $handler->display->display_options['fields']['citation']['export_links'] = 0;
  $handler->display->display_options['fields']['citation']['file_attachments'] = 0;
  $handler->display->display_options['fields']['citation']['open_url_link'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = 'DC Source';
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Biblio: Notes */
  $handler->display->display_options['fields']['biblio_notes']['id'] = 'biblio_notes';
  $handler->display->display_options['fields']['biblio_notes']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_notes']['field'] = 'biblio_notes';
  /* Field: Biblio: Publication Language */
  $handler->display->display_options['fields']['biblio_lang']['id'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_lang']['field'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['biblio_label'] = 1;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_2']['id'] = 'uuid_2';
  $handler->display->display_options['fields']['uuid_2']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_2']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_2']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['uuid_2']['label'] = 'Type DwC';
  $handler->display->display_options['fields']['uuid_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uuid_2']['alter']['text'] = 'original';
  $handler->display->display_options['fields']['uuid_2']['empty'] = 'publication';
  /* Field: Biblio: Date Published */
  $handler->display->display_options['fields']['biblio_date']['id'] = 'biblio_date';
  $handler->display->display_options['fields']['biblio_date']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_date']['field'] = 'biblio_date';
  $handler->display->display_options['fields']['biblio_date']['biblio_label'] = 1;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'c';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'custom';
  $handler->display->display_options['fields']['changed']['custom_date_format'] = 'c';
  /* Field: Biblio: Type of Publication */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'biblio_types';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'DC Related';
  /* Field: Taxonomy term: Reference */
  $handler->display->display_options['fields']['field_reference']['id'] = 'field_reference';
  $handler->display->display_options['fields']['field_reference']['table'] = 'field_data_field_reference';
  $handler->display->display_options['fields']['field_reference']['field'] = 'field_reference';
  $handler->display->display_options['fields']['field_reference']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['field_reference']['label'] = 'DC Source';
  $handler->display->display_options['fields']['field_reference']['type'] = 'node_reference_path';
  $handler->display->display_options['fields']['field_reference']['settings'] = array(
    'alias' => 1,
    'absolute' => 1
  );
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
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name:delta) */
  $handler->display->display_options['filters']['delta']['id'] = 'delta';
  $handler->display->display_options['filters']['delta']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['delta']['field'] = 'delta';
  $handler->display->display_options['filters']['delta']['operator'] = '<';
  $handler->display->display_options['filters']['delta']['value']['value'] = '1';
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
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['path'] = 'dwca-export/reference/csv';
  $handler->display->display_options['sitename_title'] = 0;
  $handler->display->display_options['use_batch'] = 'batch';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // IMAGE
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_image';
  $view->description = 'DwC-A image extension for a Taxon';
  $view->tag = 'default';
  $view->base_table = 'file_managed';
  $view->human_name = 'DwC-A Export Image';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Images2';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  /* Relationship: File Usage: Content */
  $handler->display->display_options['relationships']['file_to_node']['id'] = 'file_to_node';
  $handler->display->display_options['relationships']['file_to_node']['table'] = 'file_usage';
  $handler->display->display_options['relationships']['file_to_node']['field'] = 'file_to_node';
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['id'] = 'field_taxonomic_name_tid_1';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['relationship'] = 'file_to_node';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid_1']['label'] = 'term from content from field_taxonomic_name';
  /* Relationship: File: User who uploaded */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'file_managed';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: File: File UUID */
  $handler->display->display_options['fields']['uuid_2']['id'] = 'uuid_2';
  $handler->display->display_options['fields']['uuid_2']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uuid_2']['field'] = 'uuid';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['uuid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uuid']['hide_empty'] = TRUE;
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['relationship'] = 'field_taxonomic_name_tid_1';
  $handler->display->display_options['fields']['uuid_1']['label'] = 'Taxonomy term UUID from content';
  $handler->display->display_options['fields']['uuid_1']['alter']['text'] = '[uuid]';
  $handler->display->display_options['fields']['uuid_1']['empty'] = '[uuid]';
  $handler->display->display_options['fields']['uuid_1']['hide_alter_empty'] = FALSE;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: Mime type */
  $handler->display->display_options['fields']['filemime']['id'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filemime']['field'] = 'filemime';
  /* Field: File: Licence */
  $handler->display->display_options['fields']['field_cc_licence']['id'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['table'] = 'field_data_field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['field'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['type'] = 'creative_commons_url';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  /* Field: User: User UUID */
  $handler->display->display_options['fields']['uuid_3']['id'] = 'uuid_3';
  $handler->display->display_options['fields']['uuid_3']['table'] = 'users';
  $handler->display->display_options['fields']['uuid_3']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_3']['relationship'] = 'uid';
  /* Field: File: DCMI file type */
  $handler->display->display_options['fields']['dcmitype']['id'] = 'dcmitype';
  $handler->display->display_options['fields']['dcmitype']['table'] = 'file_managed';
  $handler->display->display_options['fields']['dcmitype']['field'] = 'dcmitype';
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['text'] = $base_url . '/file/[fid]';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
  );
  /* Field: File: Licence */
  $handler->display->display_options['fields']['field_cc_licence_1']['id'] = 'field_cc_licence_1';
  $handler->display->display_options['fields']['field_cc_licence_1']['table'] = 'field_data_field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence_1']['field'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence_1']['label'] = 'Rights Statement';
  $handler->display->display_options['fields']['field_cc_licence_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cc_licence_1']['alter']['text'] = '&nbsp;';
  $handler->display->display_options['fields']['field_cc_licence_1']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_cc_licence_1']['empty'] = '';
  $handler->display->display_options['fields']['field_cc_licence_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_cc_licence_1']['type'] = 'creative_commons_url';
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'audio' => 'audio',
    'image' => 'image',
    'video' => 'video'
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri']['id'] = 'uri';
  $handler->display->display_options['filters']['uri']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri']['field'] = 'uri';
  $handler->display->display_options['filters']['uri']['operator'] = 'not';
  $handler->display->display_options['filters']['uri']['value'] = '/eolapi/';
  $handler->display->display_options['filters']['uri']['group'] = 1;
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['operator'] = 'regular_expression';
  $handler->display->display_options['filters']['filename']['value'] = '\\.(xml|nex|phy|nhx)$';
  $handler->display->display_options['filters']['filename']['group'] = 2;
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri_1']['id'] = 'uri_1';
  $handler->display->display_options['filters']['uri_1']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri_1']['field'] = 'uri';
  $handler->display->display_options['filters']['uri_1']['operator'] = 'not';
  $handler->display->display_options['filters']['uri_1']['value'] = '/eolapi/';
  $handler->display->display_options['filters']['uri_1']['group'] = 2;
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
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['path'] = 'dwca-export/image/page';
  $handler->display->display_options['use_batch'] = 'batch';
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
  $handler->display->display_options['use_more_always'] = FALSE;
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
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['required'] = TRUE;
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Field: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  /* Field: Content: Type status */
  $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['relationship'] = 'reverse_field_taxonomic_name_node';
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['relationship'] = 'reverse_field_taxonomic_name_node';
  /* Field: Content: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['relationship'] = 'reverse_field_taxonomic_name_node';
  /* Field: Content: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['relationship'] = 'reverse_field_taxonomic_name_node';
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['label'] = 'Latitude';
  $handler->display->display_options['fields']['field_map']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_map']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text_lat';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map_1']['id'] = 'field_map_1';
  $handler->display->display_options['fields']['field_map_1']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map_1']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map_1']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map_1']['label'] = 'Longitude';
  $handler->display->display_options['fields']['field_map_1']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_map_1']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map_1']['type'] = 'gm3_text_lon';
  $handler->display->display_options['fields']['field_map_1']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
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
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['path'] = 'dwca_export/specimen/csv';
  $views[$view->name] = $view;
  //----------------------------------------------------------------------------
  // COMMENTS
  //----------------------------------------------------------------------------
  $view = new view();
  $view->name = 'dwca_export_comment';
  $view->description = 'DwC-A comments extension';
  $view->tag = 'default';
  $view->base_table = 'comment';
  $view->human_name = 'DwC-A Export Comments';
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
  /* Relationship: Comment: Content */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 1;
  /* Relationship: Comment: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'comment';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 1;
  /* Relationship: Comment: Parent comment */
  $handler->display->display_options['relationships']['pid']['id'] = 'pid';
  $handler->display->display_options['relationships']['pid']['table'] = 'comment';
  $handler->display->display_options['relationships']['pid']['field'] = 'pid';
  $handler->display->display_options['relationships']['pid']['required'] = 0;
  /* Field: Comment: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'comment';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['format_plural'] = 0;
  /* Field: Comment: ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'comment';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['text'] = $base_url . '/comment/[nid]#comment-[cid]';
  $handler->display->display_options['fields']['cid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['cid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cid']['link_to_comment'] = 0;
  $handler->display->display_options['fields']['cid']['link_to_node'] = 0;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'nid';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['path']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['path']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['path']['alter']['external'] = 0;
  $handler->display->display_options['fields']['path']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['path']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['path']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['path']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['path']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['path']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['path']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['path']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['path']['alter']['html'] = 0;
  $handler->display->display_options['fields']['path']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['path']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['path']['hide_empty'] = 0;
  $handler->display->display_options['fields']['path']['empty_zero'] = 0;
  $handler->display->display_options['fields']['path']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['path']['absolute'] = 1;
  /* Field: Comment: Nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'comment';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid_1']['separator'] = '';
  $handler->display->display_options['fields']['nid_1']['format_plural'] = 0;
  /* Field: Comment: ID */
  $handler->display->display_options['fields']['cid_1']['id'] = 'cid_1';
  $handler->display->display_options['fields']['cid_1']['table'] = 'comment';
  $handler->display->display_options['fields']['cid_1']['field'] = 'cid';
  $handler->display->display_options['fields']['cid_1']['relationship'] = 'pid';
  $handler->display->display_options['fields']['cid_1']['label'] = 'ParentID';
  $handler->display->display_options['fields']['cid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['cid_1']['alter']['text'] = $base_url . '/comment/[nid_1]#-comment[cid_1]';
  $handler->display->display_options['fields']['cid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['cid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cid_1']['empty'] = '[path]';
  $handler->display->display_options['fields']['cid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cid_1']['link_to_comment'] = 1;
  $handler->display->display_options['fields']['cid_1']['link_to_node'] = 0;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  $handler->display->display_options['fields']['mail']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['external'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mail']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['mail']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['mail']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mail']['empty_zero'] = 0;
  $handler->display->display_options['fields']['mail']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Field: Comment: Title */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'comment';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = '';
  $handler->display->display_options['fields']['subject']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['external'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['html'] = 0;
  $handler->display->display_options['fields']['subject']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['subject']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['subject']['hide_empty'] = 0;
  $handler->display->display_options['fields']['subject']['empty_zero'] = 0;
  $handler->display->display_options['fields']['subject']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['subject']['link_to_comment'] = 0;
  $handler->display->display_options['fields']['subject']['link_to_node'] = 0;
  /* Field: Comment: Comment */
  $handler->display->display_options['fields']['comment_body']['id'] = 'comment_body';
  $handler->display->display_options['fields']['comment_body']['table'] = 'field_data_comment_body';
  $handler->display->display_options['fields']['comment_body']['field'] = 'comment_body';
  $handler->display->display_options['fields']['comment_body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['nl2br'] = 1;
  $handler->display->display_options['fields']['comment_body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['comment_body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['comment_body']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['comment_body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['comment_body']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['comment_body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['comment_body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['comment_body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['comment_body']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['comment_body']['field_api_classes'] = 0;
  /* Field: Comment: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'comment';
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
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: Comment: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'comment';
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
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  /* Sort criterion: Comment: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'comment';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Comment: Approved */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'comment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
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
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['path'] = 'dwca_export/comment/csv';
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
  $view->disabled = FALSE;
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
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
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
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => '1'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca_export/eol_agents/csv';
  $handler->display->display_options['use_batch'] = 'batch';
  ;
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
    'display_convex_hull' => '0'
  );
  $handler->display->display_options['fields']['field_spm_map']['group_rows'] = 0;
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
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '\\r';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca-export/distribution/csv';
  $handler->display->display_options['use_batch'] = 'batch';
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
  /* Relationship: Taxonomy term: Vernacular names (field_vernacular_name_collection) */
  $handler->display->display_options['relationships']['field_vernacular_name_collection_value']['id'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['relationships']['field_vernacular_name_collection_value']['table'] = 'field_data_field_vernacular_name_collection';
  $handler->display->display_options['relationships']['field_vernacular_name_collection_value']['field'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['relationships']['field_vernacular_name_collection_value']['delta'] = '-1';
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
  /* Field: Field collection item: Vernacular name */
  $handler->display->display_options['fields']['field_vernacular_name']['id'] = 'field_vernacular_name';
  $handler->display->display_options['fields']['field_vernacular_name']['table'] = 'field_data_field_vernacular_name';
  $handler->display->display_options['fields']['field_vernacular_name']['field'] = 'field_vernacular_name';
  $handler->display->display_options['fields']['field_vernacular_name']['relationship'] = 'field_vernacular_name_collection_value';
  /* Field: Field collection item: Language */
  $handler->display->display_options['fields']['field_language']['id'] = 'field_language';
  $handler->display->display_options['fields']['field_language']['table'] = 'field_data_field_language';
  $handler->display->display_options['fields']['field_language']['field'] = 'field_language';
  $handler->display->display_options['fields']['field_language']['relationship'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['fields']['field_language']['settings'] = array(
    'format' => array(
      'iso' => 'iso',
      'name' => 0,
      'name_native' => 0
    )
  );
  /* Field: Field collection item: Area from which the vernacular name usage originates */
  $handler->display->display_options['fields']['field_vernacular_locality']['id'] = 'field_vernacular_locality';
  $handler->display->display_options['fields']['field_vernacular_locality']['table'] = 'field_data_field_vernacular_locality';
  $handler->display->display_options['fields']['field_vernacular_locality']['field'] = 'field_vernacular_locality';
  $handler->display->display_options['fields']['field_vernacular_locality']['relationship'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['fields']['field_vernacular_locality']['label'] = 'Locality';
  /* Field: Field collection item: Vernacular name remarks */
  $handler->display->display_options['fields']['field_vernacular_name_remarks']['id'] = 'field_vernacular_name_remarks';
  $handler->display->display_options['fields']['field_vernacular_name_remarks']['table'] = 'field_data_field_vernacular_name_remarks';
  $handler->display->display_options['fields']['field_vernacular_name_remarks']['field'] = 'field_vernacular_name_remarks';
  $handler->display->display_options['fields']['field_vernacular_name_remarks']['relationship'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['fields']['field_vernacular_name_remarks']['label'] = 'remarks';
  /* Filter criterion: Field collection item: Vernacular name (field_vernacular_name) */
  $handler->display->display_options['filters']['field_vernacular_name_value']['id'] = 'field_vernacular_name_value';
  $handler->display->display_options['filters']['field_vernacular_name_value']['table'] = 'field_data_field_vernacular_name';
  $handler->display->display_options['filters']['field_vernacular_name_value']['field'] = 'field_vernacular_name_value';
  $handler->display->display_options['filters']['field_vernacular_name_value']['relationship'] = 'field_vernacular_name_collection_value';
  $handler->display->display_options['filters']['field_vernacular_name_value']['operator'] = 'not empty';
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
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['path'] = 'dwca_export/vernacular_names/csv';
  $handler->display->display_options['use_batch'] = 'batch';
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
  //Array to translate our field names into DwC/SPM accepted terms
  module_load_include('module', 'creative_commons');
  $default_licence = creative_commons_display(variable_get('creative_commons_block_licence_type', CC_BY), variable_get('creative_commons_block_jurisdiction', ''), CC_URL);
  $default_cc_url = $default_licence['#markup'];
  $replacements = array(
    'general_description' => 'general',
    'behavious' => 'behaviour',
    'conservation_status' => 'conservation',
    'diagnostic_description' => 'diagnostic',
    'life_cycle' => 'lifecycle',
    'life_expectancy' => 'lifespan',
    'population_biology' => 'population',
    'uses' => 'use'
  );
  $type_uris = array(
    'general_description' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#GeneralDescription',
    'behavious' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Behaviour',
    'biology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Biology',
    'conservation_status' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#ConservationStatus',
    'legislation' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Legislation',
    'management' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Management',
    'procedures' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Procedures',
    'threats' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Threats',
    'trends' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Trends',
    'diagnostic_description' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#DiagnosticDescription',
    'cytology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Cytology',
    'genetics' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Genetics',
    'growth' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Growth',
    'look_alikes' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#LookAlikes',
    'molecular_biology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#MolecularBiology',
    'morphology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Morphology',
    'physiology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Physiology',
    'size' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Size',
    'taxon_biology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#TaxonBiology',
    'evolution' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Evolution',
    'phylogeny' => '',
    'dispersal' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Dispersal',
    'associations' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Associations',
    'cyclicity' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Cyclicity',
    'distribution' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Distribution',
    'ecology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Ecology',
    'habitat' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Habitat',
    'life_cycle' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#LifeCycle',
    'life_expectancy' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#LifeExpectancy',
    'migration' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Migration',
    'trophic_strategy' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#TrophicStrategy',
    'population_biology' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#PopulationBiology',
    'reproduction' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Reproduction',
    'diseases' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Diseases',
    'risk_statement' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#RiskStatement',
    'uses' => 'http://rs.tdwg.org/ontology/voc/SPMInfoItems#Use',
    'spm_em_cultivation' => '',
    'spm_em_phenology' => '',
    'spm_em_pollination' => '',
    'spm_em+anatomy' => '',
    'spm_em_taxonomic_notes' => ''
  );
  $type_uri = '';
  if(isset($type_uris[$desc_type])){
    $type_uri = $type_uris[$desc_type];
  }
  $desc_type_std = $desc_type;
  if(isset($replacements[$desc_type])){
    $desc_type_std = $replacements[$desc_type];
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
  /* Field: Global: Custom text (Rights Statement) */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Rights';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '';
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
  /* Field: Content: Path (DC Reference) */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'field_spm_em_gd_ref_nid';
  $handler->display->display_options['fields']['path']['label'] = 'DC Reference';
  $handler->display->display_options['fields']['path']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['path']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['path']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['path']['alter']['external'] = 0;
  $handler->display->display_options['fields']['path']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['path']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['path']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['path']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['path']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['path']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['path']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['path']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['path']['alter']['html'] = 0;
  $handler->display->display_options['fields']['path']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['path']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['path']['hide_empty'] = 0;
  $handler->display->display_options['fields']['path']['empty_zero'] = 0;
  $handler->display->display_options['fields']['path']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['path']['absolute'] = 1;
  /* Field: Global: Custom text (DC Source) */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'DC Source';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Copyright Owner';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = variable_get('site_name');
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_2']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Language';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'eng';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_3']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_3']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_3']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_3']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_3']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_3']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_3']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'DescriptionTypeURI';
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = $type_uri;
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_4']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_4']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_4']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_4']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_4']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_4']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_4']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Format (MIME Type)';
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'text/html';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_5']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_5']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_5']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_5']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_5']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_5']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_5']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_5']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_6']['id'] = 'nothing_6';
  $handler->display->display_options['fields']['nothing_6']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_6']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_6']['label'] = 'DCMI Type';
  $handler->display->display_options['fields']['nothing_6']['alter']['text'] = 'http://purl.org/dc/dcmitype/Text';
  $handler->display->display_options['fields']['nothing_6']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_6']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_6']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_6']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_6']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_6']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_6']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_6']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_6']['hide_alter_empty'] = 0;
  /* Field: User: User UUID */
  $handler->display->display_options['fields']['uuid_4']['id'] = 'uuid_4';
  $handler->display->display_options['fields']['uuid_4']['table'] = 'users';
  $handler->display->display_options['fields']['uuid_4']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_4']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uuid_4']['label'] = 'User UUID (EoL Agent)';
  $handler->display->display_options['fields']['uuid_4']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid_4']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid_4']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid_4']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid_4']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid_4']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid_4']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid_4']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid_4']['hide_alter_empty'] = 1;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_7']['id'] = 'nothing_7';
  $handler->display->display_options['fields']['nothing_7']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_7']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_7']['label'] = 'Licence URL';
  $handler->display->display_options['fields']['nothing_7']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_7']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_7']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_7']['alter']['text'] = $default_cc_url;
  $handler->display->display_options['fields']['nothing_7']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_7']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_7']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_7']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_7']['hide_alter_empty'] = 0;
  /* Field: Content: Node UUID */
  $handler->display->display_options['fields']['uuid_3']['id'] = 'uuid_3';
  $handler->display->display_options['fields']['uuid_3']['table'] = 'node';
  $handler->display->display_options['fields']['uuid_3']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_3']['label'] = 'DC:identifier';
  $handler->display->display_options['fields']['uuid_3']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uuid_3']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uuid_3']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uuid_3']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uuid_3']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uuid_3']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uuid_3']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uuid_3']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uuid_3']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['uuid_3']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_8']['id'] = 'nothing_8';
  $handler->display->display_options['fields']['nothing_8']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_8']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_8']['alter']['text'] = '[uuid_3]#' . $desc_type_std;
  $handler->display->display_options['fields']['nothing_8']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_8']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_8']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_8']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_8']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_8']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_8']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_8']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_8']['hide_alter_empty'] = 0;
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
  /* Filter criterion: Content: Trophic strategy (field_trophic_strategy) */
  $handler->display->display_options['filters']['field_' . $desc_type . '_value']['id'] = 'field_' . $desc_type . '_value';
  $handler->display->display_options['filters']['field_' . $desc_type . '_value']['table'] = 'field_data_field_' . $desc_type . '';
  $handler->display->display_options['filters']['field_' . $desc_type . '_value']['field'] = 'field_' . $desc_type . '_value';
  $handler->display->display_options['filters']['field_' . $desc_type . '_value']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['field_' . $desc_type . '_value']['value'] = '2';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '<br/>';
  $handler->display->display_options['style_options']['header'] = 0;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['path'] = t('dwca-export/description-' . $desc_type . '/csv');
  $handler->display->display_options['use_batch'] = 'batch';
  return $view;
}
