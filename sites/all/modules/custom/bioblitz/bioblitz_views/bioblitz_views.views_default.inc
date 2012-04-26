<?php
/**
 * @file
 * bioblitz_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function bioblitz_views_views_default_views() {
  $export = array();

$view = new view;
$view->name = 'bioblitz_records';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'BioBlitz Records';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'BioBlitz Records';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = 'This map shows the five most recent observations uplaoded to this site.';
$handler->display->display_options['header']['area']['format'] = 'filtered_html';
$handler->display->display_options['header']['area']['tokenize'] = 0;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'You have not yet submitted any records!';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Relationship: Content: Location (field_location) */
$handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
$handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
$handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
$handler->display->display_options['relationships']['field_location_nid']['required'] = 1;
$handler->display->display_options['relationships']['field_location_nid']['delta'] = '0';
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
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
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['link_to_node'] = 0;
/* Field: Content: Map */
$handler->display->display_options['fields']['field_map']['id'] = 'field_map';
$handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
$handler->display->display_options['fields']['field_map']['field'] = 'field_map';
$handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
$handler->display->display_options['fields']['field_map']['label'] = '';
$handler->display->display_options['fields']['field_map']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_map']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_map']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_map']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_map']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_map']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_map']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_map']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_map']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
$handler->display->display_options['fields']['field_map']['settings'] = array(
  'display_convex_hull' => '0',
);
$handler->display->display_options['fields']['field_map']['field_api_classes'] = 0;
/* Field: Field: Taxonomic name */
$handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['field_api_classes'] = 0;
/* Field: Content: Media */
$handler->display->display_options['fields']['field_media']['id'] = 'field_media';
$handler->display->display_options['fields']['field_media']['table'] = 'field_data_field_media';
$handler->display->display_options['fields']['field_media']['field'] = 'field_media';
$handler->display->display_options['fields']['field_media']['label'] = '';
$handler->display->display_options['fields']['field_media']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['path'] = '/node/[nid]';
$handler->display->display_options['fields']['field_media']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_media']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_media']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_media']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_media']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_media']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_media']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_media']['type'] = 'styles_file_square_thumbnail';
$handler->display->display_options['fields']['field_media']['group_rows'] = 1;
$handler->display->display_options['fields']['field_media']['delta_limit'] = '3';
$handler->display->display_options['fields']['field_media']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_media']['delta_reversed'] = 0;
$handler->display->display_options['fields']['field_media']['delta_first_last'] = 0;
$handler->display->display_options['fields']['field_media']['field_api_classes'] = 0;
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Sort criterion: Content: Type */
$handler->display->display_options['sorts']['type']['id'] = 'type';
$handler->display->display_options['sorts']['type']['table'] = 'node';
$handler->display->display_options['sorts']['type']['field'] = 'type';
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'specimen_observation' => 'specimen_observation',
);

/* Display: recent */
$handler = $view->new_display('block', 'recent', 'block');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'gmap';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;

/* Display: taxon */
$handler = $view->new_display('block', 'taxon', 'block_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'gmap';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['header'] = FALSE;
$handler->display->display_options['defaults']['empty'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Field: Taxonomic name (field_taxonomic_name) */
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_action'] = 'default';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_type'] = 'taxonomy_tid';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['term_page'] = 1;
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['node'] = 0;
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['limit'] = 0;
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['field_taxonomic_name_tid']['not'] = 0;

/* Display: user_finds */
$handler = $view->new_display('page', 'user_finds', 'page_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'nid' => 'nid',
  'field_map' => 'field_map',
  'field_taxonomic_name' => 'field_taxonomic_name',
  'field_media' => 'field_media',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'nid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_map' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_taxonomic_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_media' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = 'This is a list of all the records you have submitted to this site.';
$handler->display->display_options['header']['area']['format'] = 'filtered_html';
$handler->display->display_options['header']['area']['tokenize'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
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
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['link_to_node'] = 0;
/* Field: Content: Map */
$handler->display->display_options['fields']['field_map']['id'] = 'field_map';
$handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
$handler->display->display_options['fields']['field_map']['field'] = 'field_map';
$handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
$handler->display->display_options['fields']['field_map']['label'] = '';
$handler->display->display_options['fields']['field_map']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_map']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_map']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_map']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_map']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_map']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_map']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_map']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_map']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_map']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
$handler->display->display_options['fields']['field_map']['settings'] = array(
  'display_convex_hull' => '0',
);
$handler->display->display_options['fields']['field_map']['field_api_classes'] = 0;
/* Field: Field: Taxonomic name */
$handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_taxonomic_name']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_taxonomic_name']['field_api_classes'] = 0;
/* Field: Content: Media */
$handler->display->display_options['fields']['field_media']['id'] = 'field_media';
$handler->display->display_options['fields']['field_media']['table'] = 'field_data_field_media';
$handler->display->display_options['fields']['field_media']['field'] = 'field_media';
$handler->display->display_options['fields']['field_media']['label'] = '';
$handler->display->display_options['fields']['field_media']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['path'] = '/node/[nid]';
$handler->display->display_options['fields']['field_media']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_media']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_media']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_media']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_media']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_media']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_media']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_media']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_media']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_media']['type'] = 'styles_file_square_thumbnail';
$handler->display->display_options['fields']['field_media']['group_rows'] = 1;
$handler->display->display_options['fields']['field_media']['delta_limit'] = '3';
$handler->display->display_options['fields']['field_media']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_media']['delta_reversed'] = 0;
$handler->display->display_options['fields']['field_media']['delta_first_last'] = 0;
$handler->display->display_options['fields']['field_media']['field_api_classes'] = 0;
/* Field: Content: Date collected */
$handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
$handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
$handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
$handler->display->display_options['fields']['field_date_collected']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_date_collected']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_date_collected']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_date_collected']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_date_collected']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_date_collected']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_date_collected']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_date_collected']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_date_collected']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_date_collected']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'value',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_date_collected']['field_api_classes'] = 0;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Author uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'node';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
$handler->display->display_options['path'] = 'bioblitz/user';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Your Records';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
  
  

  $export['bioblitz_records'] = $view;
  
  $view = new view;
$view->name = 'bioblitz_finds';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'taxonomy_term_data';
$view->human_name = 'BioBlitz Finds';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Species Found';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Relationship: Taxonomy term: Content using Taxonomic name */
$handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['id'] = 'reverse_field_taxonomic_name_node';
$handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['table'] = 'taxonomy_term_data';
$handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['field'] = 'reverse_field_taxonomic_name_node';
$handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['required'] = 1;
/* Field: Taxonomy term: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
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
$handler->display->display_options['fields']['name']['link_to_taxonomy'] = 1;
/* Filter criterion: Taxonomy vocabulary: Machine name */
$handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
$handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['value'] = array(
  'species_dictionary' => 'species_dictionary',
);
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_taxonomic_name_node';
$handler->display->display_options['filters']['type']['value'] = array(
  'specimen_observation' => 'specimen_observation',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'bioblitz-finds';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Species Found';
$handler->display->display_options['menu']['name'] = 'main-menu';

$xport['bioblitz_finds'] = $view;

  
  return $export;
}
