<?php

function views_oai_pmh_views_default_views() {

$view = new view();
$view->name = 'oai_biblio';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Biblio OAI-PMH';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'dc:title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Contextual filter: Content: Has taxonomy term ID */
$handler->display->display_options['arguments']['tid']['id'] = 'tid';
$handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
$handler->display->display_options['arguments']['tid']['field'] = 'tid';
$handler->display->display_options['arguments']['tid']['default_action'] = 'default';
$handler->display->display_options['arguments']['tid']['default_argument_type'] = 'php';
$handler->display->display_options['arguments']['tid']['default_argument_options']['code'] = 'return $_GET[\'set\'];';
$handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['tid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
$handler->display->display_options['arguments']['tid']['validate_options']['vocabularies'] = array(
  'vocabulary_10' => 'vocabulary_10',
);
$handler->display->display_options['arguments']['tid']['validate_options']['type'] = 'convert';
$handler->display->display_options['arguments']['tid']['validate']['fail'] = 'ignore';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'agdlios' => 'agdlios',
);

/* Display: OAI-PMH */
$handler = $view->new_display('views_oai_pmh', 'OAI-PMH', 'views_oai_pmh_biblio');
$handler->display->display_options['style_plugin'] = 'views_oai_pmh_dc';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'dc';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'dc:title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Document type */
$handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['label'] = 'dc:type';
$handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_type']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: Date of publication */
$handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['label'] = 'dc:date';
$handler->display->display_options['fields']['field_ag_datepub']['alter']['strip_tags'] = TRUE;
$handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_datepub']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
  'format_type' => 'iso_date',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
);
/* Field: Content: Journal */
$handler->display->display_options['fields']['field_ag_journal']['id'] = 'field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['table'] = 'field_data_field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['field'] = 'field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['relationship'] = 'field_test_value';
$handler->display->display_options['fields']['field_ag_journal']['label'] = 'dc:source';
$handler->display->display_options['fields']['field_ag_journal']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_journal']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_journal']['type'] = 'node_reference_plain';
/* Field: Content: Language */
$handler->display->display_options['fields']['field_ag_language']['id'] = 'field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['table'] = 'field_data_field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['field'] = 'field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['label'] = 'dc:language';
$handler->display->display_options['fields']['field_ag_language']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_language']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_language']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_language']['separator'] = ';';
/* Field: Content: Abstract */
$handler->display->display_options['fields']['field_ag_abstract']['id'] = 'field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['table'] = 'field_data_field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['field'] = 'field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['label'] = 'dc:description';
$handler->display->display_options['fields']['field_ag_abstract']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_abstract']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_abstract']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_ag_abstract']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_abstract']['separator'] = ';';
/* Field: Content: E_ISSN */
$handler->display->display_options['fields']['field_ag_eissn']['id'] = 'field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['table'] = 'field_data_field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['field'] = 'field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_eissn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_eissn']['hide_empty'] = TRUE;
/* Field: Content: ISSN */
$handler->display->display_options['fields']['field_ag_issn']['id'] = 'field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['table'] = 'field_data_field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['field'] = 'field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_issn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_issn']['hide_empty'] = TRUE;
/* Field: Content: ISBN */
$handler->display->display_options['fields']['field_ag_isbn']['id'] = 'field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['table'] = 'field_data_field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['field'] = 'field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_isbn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_isbn']['hide_empty'] = TRUE;
/* Field: Content: Form */
$handler->display->display_options['fields']['field_ag_form']['id'] = 'field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['table'] = 'field_data_field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['field'] = 'field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['label'] = 'dc:format';
$handler->display->display_options['fields']['field_ag_form']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_form']['hide_empty'] = TRUE;
/* Field: Content: Author(s) */
$handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['label'] = 'dc:creator';
$handler->display->display_options['fields']['field_ag_author']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_author']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_author']['separator'] = ';';
/* Field: Content: Corporate author(s) */
$handler->display->display_options['fields']['field_ag_corauthor']['id'] = 'field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['table'] = 'field_data_field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['field'] = 'field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['label'] = 'dc:creator';
$handler->display->display_options['fields']['field_ag_corauthor']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_corauthor']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_corauthor']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_corauthor']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_corauthor']['separator'] = ';';
/* Field: Content: Agris Subject Categories */
$handler->display->display_options['fields']['field_ag_agris']['id'] = 'field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['table'] = 'field_data_field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['field'] = 'field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['label'] = 'dc:subject';
$handler->display->display_options['fields']['field_ag_agris']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_agris']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_agris']['type'] = 'taxonomy_term_reference_plain';
$handler->display->display_options['fields']['field_ag_agris']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_agris']['separator'] = ';';
/* Field: Content: Agrovoc terms */
$handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['label'] = 'dc:subject';
$handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'taxonomy_term_reference_plain';
$handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_agrovoc']['separator'] = '; ';
/* Field: Content: Book */
$handler->display->display_options['fields']['field_ag_book']['id'] = 'field_ag_book';
$handler->display->display_options['fields']['field_ag_book']['table'] = 'field_data_field_ag_book';
$handler->display->display_options['fields']['field_ag_book']['field'] = 'field_ag_book';
$handler->display->display_options['fields']['field_ag_book']['relationship'] = 'field_test_value';
$handler->display->display_options['fields']['field_ag_book']['label'] = 'dc:source';
$handler->display->display_options['fields']['field_ag_book']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_book']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_book']['type'] = 'node_reference_plain';
/* Field: Content: Publisher */
$handler->display->display_options['fields']['field_ag_publisher']['id'] = 'field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['table'] = 'field_data_field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['field'] = 'field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['label'] = 'dc:publisher';
$handler->display->display_options['fields']['field_ag_publisher']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_publisher']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_publisher']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_publisher']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_publisher']['separator'] = ';';
$handler->display->display_options['path'] = 'oai';

/* Display: OAI-PMH-AG */
$handler = $view->new_display('views_oai_pmh', 'OAI-PMH-AG', 'views_oai_pmh_ag');
$handler->display->display_options['style_plugin'] = 'views_oai_pmh_agrisap';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agrisap';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Content: Book (field_ag_book) */
$handler->display->display_options['relationships']['field_ag_book_nid']['id'] = 'field_ag_book_nid';
$handler->display->display_options['relationships']['field_ag_book_nid']['table'] = 'field_data_field_ag_book';
$handler->display->display_options['relationships']['field_ag_book_nid']['field'] = 'field_ag_book_nid';
$handler->display->display_options['relationships']['field_ag_book_nid']['delta'] = '-1';
/* Relationship: Content: Journal (field_ag_journal) */
$handler->display->display_options['relationships']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
$handler->display->display_options['relationships']['field_ag_journal_nid']['table'] = 'field_data_field_ag_journal';
$handler->display->display_options['relationships']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
$handler->display->display_options['relationships']['field_ag_journal_nid']['delta'] = '-1';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'dc:title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Subtitle */
$handler->display->display_options['fields']['field_ag_subtitle']['id'] = 'field_ag_subtitle';
$handler->display->display_options['fields']['field_ag_subtitle']['table'] = 'field_data_field_ag_subtitle';
$handler->display->display_options['fields']['field_ag_subtitle']['field'] = 'field_ag_subtitle';
$handler->display->display_options['fields']['field_ag_subtitle']['label'] = 'dcterms:alternative';
$handler->display->display_options['fields']['field_ag_subtitle']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_subtitle']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_subtitle']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_ag_subtitle']['delta_offset'] = '0';
/* Field: Content: Author(s) */
$handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
$handler->display->display_options['fields']['field_ag_author']['label'] = 'ags:creatorPersonal';
$handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_author']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_author']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_author']['separator'] = ';';
/* Field: Content: Conference */
$handler->display->display_options['fields']['field_ag_confname']['id'] = 'field_ag_confname';
$handler->display->display_options['fields']['field_ag_confname']['table'] = 'field_data_field_ag_confname';
$handler->display->display_options['fields']['field_ag_confname']['field'] = 'field_ag_confname';
$handler->display->display_options['fields']['field_ag_confname']['label'] = 'ags:creatorConference';
$handler->display->display_options['fields']['field_ag_confname']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_confname']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_confname']['type'] = 'node_reference_plain';
/* Field: Content: Corporate author(s) */
$handler->display->display_options['fields']['field_ag_corauthor']['id'] = 'field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['table'] = 'field_data_field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['field'] = 'field_ag_corauthor';
$handler->display->display_options['fields']['field_ag_corauthor']['label'] = 'ags:creatorCorporate';
$handler->display->display_options['fields']['field_ag_corauthor']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_corauthor']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_corauthor']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_corauthor']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_corauthor']['separator'] = ';';
/* Field: Content: Place of Publication */
$handler->display->display_options['fields']['field_ag_publishplace']['id'] = 'field_ag_publishplace';
$handler->display->display_options['fields']['field_ag_publishplace']['table'] = 'field_data_field_ag_publishplace';
$handler->display->display_options['fields']['field_ag_publishplace']['field'] = 'field_ag_publishplace';
$handler->display->display_options['fields']['field_ag_publishplace']['label'] = 'ags:publisherPlace';
$handler->display->display_options['fields']['field_ag_publishplace']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_publishplace']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_publishplace']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_ag_publishplace']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_publishplace']['separator'] = ';';
/* Field: Content: Publisher */
$handler->display->display_options['fields']['field_ag_publisher']['id'] = 'field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['table'] = 'field_data_field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['field'] = 'field_ag_publisher';
$handler->display->display_options['fields']['field_ag_publisher']['label'] = 'ags:publisherName';
$handler->display->display_options['fields']['field_ag_publisher']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_publisher']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_publisher']['type'] = 'node_reference_plain';
$handler->display->display_options['fields']['field_ag_publisher']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_publisher']['separator'] = ';';
/* Field: Content: Date of publication */
$handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
$handler->display->display_options['fields']['field_ag_datepub']['label'] = 'dcterms:dateIssued';
$handler->display->display_options['fields']['field_ag_datepub']['alter']['strip_tags'] = TRUE;
$handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_datepub']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
  'format_type' => 'iso_date',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
);
/* Field: Content: Agris Subject Categories */
$handler->display->display_options['fields']['field_ag_agris']['id'] = 'field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['table'] = 'field_data_field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['field'] = 'field_ag_agris';
$handler->display->display_options['fields']['field_ag_agris']['label'] = 'ags:subjectClassification';
$handler->display->display_options['fields']['field_ag_agris']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_agris']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_agris']['type'] = 'taxonomy_term_reference_plain';
$handler->display->display_options['fields']['field_ag_agris']['delta_offset'] = '0';
/* Field: Content: Agrovoc terms */
$handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc']['label'] = 'ags:subjectThesaurus';
$handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_agrovoc']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'taxonomy_term_reference_plain';
$handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
/* Field: Content: Agrovoc terms */
$handler->display->display_options['fields']['field_ag_agrovoc_1']['id'] = 'field_ag_agrovoc_1';
$handler->display->display_options['fields']['field_ag_agrovoc_1']['table'] = 'field_data_field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc_1']['field'] = 'field_ag_agrovoc';
$handler->display->display_options['fields']['field_ag_agrovoc_1']['label'] = 'ags:agrovoc_uri';
$handler->display->display_options['fields']['field_ag_agrovoc_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_agrovoc_1']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_agrovoc_1']['type'] = 'agrovocfield_term_reference_agrovoc_uri';
$handler->display->display_options['fields']['field_ag_agrovoc_1']['settings'] = array(
  'only_agid' => 1,
  'strip_html' => 0,
);
$handler->display->display_options['fields']['field_ag_agrovoc_1']['delta_offset'] = '0';
/* Field: Content: Edition */
$handler->display->display_options['fields']['field_ag_edition']['id'] = 'field_ag_edition';
$handler->display->display_options['fields']['field_ag_edition']['table'] = 'field_data_field_ag_edition';
$handler->display->display_options['fields']['field_ag_edition']['field'] = 'field_ag_edition';
$handler->display->display_options['fields']['field_ag_edition']['label'] = 'ags:descriptionEdition';
$handler->display->display_options['fields']['field_ag_edition']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_edition']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_edition']['type'] = 'text_plain';
/* Field: Content: Notes */
$handler->display->display_options['fields']['field_ag_notes']['id'] = 'field_ag_notes';
$handler->display->display_options['fields']['field_ag_notes']['table'] = 'field_data_field_ag_notes';
$handler->display->display_options['fields']['field_ag_notes']['field'] = 'field_ag_notes';
$handler->display->display_options['fields']['field_ag_notes']['label'] = 'ags:descriptionNotes';
$handler->display->display_options['fields']['field_ag_notes']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_notes']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_notes']['type'] = 'text_plain';
/* Field: Content: Abstract */
$handler->display->display_options['fields']['field_ag_abstract']['id'] = 'field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['table'] = 'field_data_field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['field'] = 'field_ag_abstract';
$handler->display->display_options['fields']['field_ag_abstract']['label'] = 'dcterms:abstract';
$handler->display->display_options['fields']['field_ag_abstract']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_abstract']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_abstract']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_ag_abstract']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_abstract']['separator'] = ';';
/* Field: Content: File attachments */
$handler->display->display_options['fields']['upload']['id'] = 'upload';
$handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
$handler->display->display_options['fields']['upload']['field'] = 'upload';
$handler->display->display_options['fields']['upload']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['upload']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
$handler->display->display_options['fields']['upload']['delta_offset'] = '0';
/* Field: Content: ISBN */
$handler->display->display_options['fields']['field_ag_isbn']['id'] = 'field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['table'] = 'field_data_field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['field'] = 'field_ag_isbn';
$handler->display->display_options['fields']['field_ag_isbn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_isbn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_isbn']['hide_empty'] = TRUE;
/* Field: Content: ISSN */
$handler->display->display_options['fields']['field_ag_issn']['id'] = 'field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['table'] = 'field_data_field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['field'] = 'field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_issn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_issn']['hide_empty'] = TRUE;
/* Field: Content: E_ISSN */
$handler->display->display_options['fields']['field_ag_eissn']['id'] = 'field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['table'] = 'field_data_field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['field'] = 'field_ag_eissn';
$handler->display->display_options['fields']['field_ag_eissn']['label'] = 'dc:identifier';
$handler->display->display_options['fields']['field_ag_eissn']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_eissn']['hide_empty'] = TRUE;
/* Field: Content: Document type */
$handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
$handler->display->display_options['fields']['field_ag_type']['label'] = 'dc:type';
$handler->display->display_options['fields']['field_ag_type']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: Extent */
$handler->display->display_options['fields']['field_ag_pagination']['id'] = 'field_ag_pagination';
$handler->display->display_options['fields']['field_ag_pagination']['table'] = 'field_data_field_ag_pagination';
$handler->display->display_options['fields']['field_ag_pagination']['field'] = 'field_ag_pagination';
$handler->display->display_options['fields']['field_ag_pagination']['label'] = 'dcterms:extent';
$handler->display->display_options['fields']['field_ag_pagination']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_pagination']['hide_empty'] = TRUE;
/* Field: Content: Form */
$handler->display->display_options['fields']['field_ag_form']['id'] = 'field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['table'] = 'field_data_field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['field'] = 'field_ag_form';
$handler->display->display_options['fields']['field_ag_form']['label'] = 'dcterms:medium';
$handler->display->display_options['fields']['field_ag_form']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_form']['hide_empty'] = TRUE;
/* Field: Content: Language */
$handler->display->display_options['fields']['field_ag_language']['id'] = 'field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['table'] = 'field_data_field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['field'] = 'field_ag_language';
$handler->display->display_options['fields']['field_ag_language']['label'] = 'dc:language';
$handler->display->display_options['fields']['field_ag_language']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_language']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_language']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_language']['separator'] = ';';
/* Field: Content: Physical Location */
$handler->display->display_options['fields']['field_ag_phlocation']['id'] = 'field_ag_phlocation';
$handler->display->display_options['fields']['field_ag_phlocation']['table'] = 'field_data_field_ag_phlocation';
$handler->display->display_options['fields']['field_ag_phlocation']['field'] = 'field_ag_phlocation';
$handler->display->display_options['fields']['field_ag_phlocation']['label'] = 'ags:availabilityLocation';
$handler->display->display_options['fields']['field_ag_phlocation']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_phlocation']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_phlocation']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_phlocation']['separator'] = ';';
/* Field: Content: Shelf Locator */
$handler->display->display_options['fields']['field_ag_locator']['id'] = 'field_ag_locator';
$handler->display->display_options['fields']['field_ag_locator']['table'] = 'field_data_field_ag_locator';
$handler->display->display_options['fields']['field_ag_locator']['field'] = 'field_ag_locator';
$handler->display->display_options['fields']['field_ag_locator']['label'] = 'ags:availabilityNumber';
$handler->display->display_options['fields']['field_ag_locator']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_locator']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_locator']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_ag_locator']['separator'] = ';';
/* Field: Content: Journal */
$handler->display->display_options['fields']['field_ag_journal']['id'] = 'field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['table'] = 'field_data_field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['field'] = 'field_ag_journal';
$handler->display->display_options['fields']['field_ag_journal']['relationship'] = 'field_journal_value';
$handler->display->display_options['fields']['field_ag_journal']['label'] = 'ags:citationTitle';
$handler->display->display_options['fields']['field_ag_journal']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_journal']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_journal']['type'] = 'node_reference_plain';
/* Field: Content: ISSN */
$handler->display->display_options['fields']['field_ag_issn_1']['id'] = 'field_ag_issn_1';
$handler->display->display_options['fields']['field_ag_issn_1']['table'] = 'field_data_field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn_1']['field'] = 'field_ag_issn';
$handler->display->display_options['fields']['field_ag_issn_1']['relationship'] = 'field_ag_journal_nid';
$handler->display->display_options['fields']['field_ag_issn_1']['label'] = 'ags:citationIdentifier';
$handler->display->display_options['fields']['field_ag_issn_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_issn_1']['hide_empty'] = TRUE;
/* Field: Content: Volume */
$handler->display->display_options['fields']['field_ag_volume']['id'] = 'field_ag_volume';
$handler->display->display_options['fields']['field_ag_volume']['table'] = 'field_data_field_ag_volume';
$handler->display->display_options['fields']['field_ag_volume']['field'] = 'field_ag_volume';
$handler->display->display_options['fields']['field_ag_volume']['label'] = 'ags:citationNumber';
$handler->display->display_options['fields']['field_ag_volume']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_ag_volume']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_volume']['hide_empty'] = TRUE;
/* Field: Content: Number */
$handler->display->display_options['fields']['field_ag_number']['id'] = 'field_ag_number';
$handler->display->display_options['fields']['field_ag_number']['table'] = 'field_data_field_ag_number';
$handler->display->display_options['fields']['field_ag_number']['field'] = 'field_ag_number';
$handler->display->display_options['fields']['field_ag_number']['label'] = 'ags:citationNumber';
$handler->display->display_options['fields']['field_ag_number']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_ag_number']['alter']['text'] = 'v.[field_ag_volume]/n.[field_ag_number]';
$handler->display->display_options['fields']['field_ag_number']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_number']['hide_empty'] = TRUE;
/* Field: Content: Book */
$handler->display->display_options['fields']['field_ag_book_1']['id'] = 'field_ag_book_1';
$handler->display->display_options['fields']['field_ag_book_1']['table'] = 'field_data_field_ag_book';
$handler->display->display_options['fields']['field_ag_book_1']['field'] = 'field_ag_book';
$handler->display->display_options['fields']['field_ag_book_1']['relationship'] = 'field_book_value';
$handler->display->display_options['fields']['field_ag_book_1']['label'] = 'dc:source';
$handler->display->display_options['fields']['field_ag_book_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_ag_book_1']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_ag_book_1']['type'] = 'node_reference_plain';
$handler->display->display_options['path'] = 'oai2';

  
  
 //anto new view ListSets
$view = new view();
$view->name = 'oai_listsets';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'taxonomy_term_data';
$view->human_name = 'OAI-PMH ListSets';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'test';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Taxonomy term: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'setSpec';
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['name']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
/* Field: Taxonomy term: Term description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = 'setName';
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['description']['hide_empty'] = TRUE;
/* Field: Taxonomy term: Term ID */
$handler->display->display_options['fields']['tid']['id'] = 'tid';
$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['tid']['field'] = 'tid';
$handler->display->display_options['fields']['tid']['label'] = 'setSpec';
$handler->display->display_options['fields']['tid']['exclude'] = TRUE;
$handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['tid']['hide_empty'] = TRUE;
/* Filter criterion: Taxonomy vocabulary: Machine name */
$handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
$handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['value'] = array(
  'vocabulary_10' => 'vocabulary_10',
);

/* Display: OAI-PMH */
$handler = $view->new_display('views_oai_pmh', 'OAI-PMH', 'views_oai_pmh_1');
$handler->display->display_options['style_plugin'] = 'views_oai_pmh_ls';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'ls';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'oai3';

 

  
return array('oai_biblio' => $view, 'oai_listsets' => $viewSet);



/* Display: Page */

  
  
}
