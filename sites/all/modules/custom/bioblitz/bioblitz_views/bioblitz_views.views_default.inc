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
  $handler->display->display_options['style_plugin'] = 'gmap';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'This map shows the five most recent observations uplaoded to this site.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = 1;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '0';
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
  $handler->display->display_options['fields']['field_media']['alter']['make_link'] = 0;
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
  /* Contextual filter: Content: Has taxonomy term ID (with depth) */
  $handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '10';
  $handler->display->display_options['arguments']['term_node_tid_depth']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['term_node_tid_depth']['set_breadcrumb'] = 0;
  $handler->display->display_options['arguments']['term_node_tid_depth']['use_taxonomy_term_path'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation',
  );

  /* Display: recent */
  $handler = $view->new_display('block', 'recent', 'block');

  /* Display: taxon */
  $handler = $view->new_display('block', 'taxon', 'block_1');
  $export['bioblitz_records'] = $view;

  return $export;
}
