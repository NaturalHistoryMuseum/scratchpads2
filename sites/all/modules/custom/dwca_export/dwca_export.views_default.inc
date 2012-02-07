<?php

/**
 * Implementation of hook_views_default_views().
 *
 * @see dwca_export_views_api()
 */
function dwca_export_views_default_views() {
	static $view_functions = array('classification', 'specimen');

	$views = array();

	foreach ($view_functions as $function_postfix) {
		$function = '_view_' . $function_postfix;
		$view = $function();
		$views[$view->name] = $view;
		watchdog("dwca_export", "hook_views_default_views() : view '".$view->name."'");
	}
	return $views;
}

/**
 * Classification Export View - exported from views module
 */
function _view_classification() {

	$view = new view;
	$view->name = 'dwca_export_classification';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'taxonomy_term_data';
	$view->human_name = 'Classification Export';
	$view->core = 7;
	$view->api_version = '3.0-alpha1';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

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
	/* Relationship: relParent */
	$handler->display->display_options['relationships']['parent']['id'] = 'parent';
	$handler->display->display_options['relationships']['parent']['table'] = 'taxonomy_term_hierarchy';
	$handler->display->display_options['relationships']['parent']['field'] = 'parent';
	$handler->display->display_options['relationships']['parent']['ui_name'] = 'relParent';
	$handler->display->display_options['relationships']['parent']['required'] = 0;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid_2']['id'] = 'tid_2';
	$handler->display->display_options['fields']['tid_2']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid_2']['field'] = 'tid';
	$handler->display->display_options['fields']['tid_2']['label'] = 'ID';
	$handler->display->display_options['fields']['tid_2']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid_2']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid_2']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid_2']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid_2']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid_2']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid_2']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid_2']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid_2']['separator'] = '';
	$handler->display->display_options['fields']['tid_2']['format_plural'] = 0;
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
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid_3']['id'] = 'tid_3';
	$handler->display->display_options['fields']['tid_3']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid_3']['field'] = 'tid';
	$handler->display->display_options['fields']['tid_3']['relationship'] = 'parent';
	$handler->display->display_options['fields']['tid_3']['label'] = 'parentNameUsageId';
	$handler->display->display_options['fields']['tid_3']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid_3']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid_3']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid_3']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid_3']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid_3']['hide_empty'] = 1;
	$handler->display->display_options['fields']['tid_3']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid_3']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid_3']['separator'] = '';
	$handler->display->display_options['fields']['tid_3']['format_plural'] = 0;
	/* Field: Taxonomy term: Name */
	$handler->display->display_options['fields']['name_4']['id'] = 'name_4';
	$handler->display->display_options['fields']['name_4']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['name_4']['field'] = 'name';
	$handler->display->display_options['fields']['name_4']['relationship'] = 'parent';
	$handler->display->display_options['fields']['name_4']['label'] = 'parentNameUsage';
	$handler->display->display_options['fields']['name_4']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name_4']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name_4']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name_4']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name_4']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name_4']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name_4']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name_4']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['name_4']['link_to_taxonomy'] = 0;
	/* Field: Taxonomy vocabulary: Name */
	$handler->display->display_options['fields']['name']['id'] = 'name';
	$handler->display->display_options['fields']['name']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['fields']['name']['field'] = 'name';
	$handler->display->display_options['fields']['name']['label'] = 'datasetName';
	$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
	/* Sort criterion: Taxonomy term: Term ID */
	$handler->display->display_options['sorts']['tid']['id'] = 'tid';
	$handler->display->display_options['sorts']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['sorts']['tid']['field'] = 'tid';
	/* Filter criterion: Taxonomy vocabulary: Machine name */
	$handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
	$handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
	$handler->display->display_options['filters']['machine_name']['value'] = array(
	  'dwca_export_test_taxonomy' => 'dwca_export_test_taxonomy',
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
	$handler->display->display_options['style_options']['trim'] = 0;
	$handler->display->display_options['style_options']['replace_newlines'] = 1;
	$handler->display->display_options['style_options']['newline_replacement'] = '\\r';
	$handler->display->display_options['style_options']['header'] = 1;
	$handler->display->display_options['path'] = 'dwca_export/classification/csv';

	return $view;
}

function _view_specimen() {
  $view = new view;
  $view->name = 'view_test_specimen_export';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'view_test_specimen_export';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Content: Taxonomy terms on node */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['required'] = 1;
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'dwca_export_test_taxonomy' => 'dwca_export_test_taxonomy',
    'forums' => 0,
    'tags' => 0,
  );
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['tid']['format_plural'] = 0;
  /* Field: Content: Type status */
  $handler->display->display_options['fields']['field_type_status_1']['id'] = 'field_type_status_1';
  $handler->display->display_options['fields']['field_type_status_1']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status_1']['field'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_type_status_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_type_status_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_type_status_1']['field_api_classes'] = 0;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_institution_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_institution_code']['field_api_classes'] = 0;
  /* Field: Content: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_collection_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_collection_code']['field_api_classes'] = 0;
  /* Field: Content: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_catalogue_number']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_catalogue_number']['field_api_classes'] = 0;
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation',
  );

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'dwca_typesandspecimen';
  
}