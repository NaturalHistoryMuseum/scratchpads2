<?php
/**
 * @file
 * museum_profile.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function museum_profile_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'museum_profiles';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Museum profiles';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Profile';
  $handler->display->display_options['css_class'] = 'table-valign-top';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Museum image */
  $handler->display->display_options['fields']['field_image_museum']['id'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['table'] = 'field_data_field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['field'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['label'] = '';
  $handler->display->display_options['fields']['field_image_museum']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image_museum']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_museum']['settings'] = array(
    'image_style' => 'square_thumbnail_large',
    'image_link' => '',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'museum-profile/[nid]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
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
    'museum_profile' => 'museum_profile',
  );

  /* Display: Museum profiles index page */
  $handler = $view->new_display('page', 'Museum profiles index page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Museum profiles';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_image_museum' => 'field_image_museum',
    'field_museum_inst_code' => 'field_image_museum',
    'nid' => 'field_image_museum',
    'title' => 'title',
    'body' => 'title',
    'field_collections_website' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_image_museum' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_museum_inst_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collections_website' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: View area */
  $handler->display->display_options['header']['view']['id'] = 'view';
  $handler->display->display_options['header']['view']['table'] = 'views';
  $handler->display->display_options['header']['view']['field'] = 'view';
  $handler->display->display_options['header']['view']['view_to_insert'] = 'museum_profiles:page_1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Museum image */
  $handler->display->display_options['fields']['field_image_museum']['id'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['table'] = 'field_data_field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['field'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['label'] = '';
  $handler->display->display_options['fields']['field_image_museum']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image_museum']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_museum']['settings'] = array(
    'image_style' => 'square_thumbnail_large',
    'image_link' => '',
  );
  /* Field: Content: Museum international institution code */
  $handler->display->display_options['fields']['field_museum_inst_code']['id'] = 'field_museum_inst_code';
  $handler->display->display_options['fields']['field_museum_inst_code']['table'] = 'field_data_field_museum_inst_code';
  $handler->display->display_options['fields']['field_museum_inst_code']['field'] = 'field_museum_inst_code';
  $handler->display->display_options['fields']['field_museum_inst_code']['label'] = '';
  $handler->display->display_options['fields']['field_museum_inst_code']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_museum_inst_code']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_museum_inst_code']['type'] = 'text_plain';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'museum-profiles/[nid]/[field_museum_inst_code]/profile';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Collections website */
  $handler->display->display_options['fields']['field_collections_website']['id'] = 'field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['table'] = 'field_data_field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['field'] = 'field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['label'] = '';
  $handler->display->display_options['fields']['field_collections_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_collections_website']['click_sort_column'] = 'url';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['breadcrumb'] = 'test';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'museum-profiles';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Museum profiles';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Profiles';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'museum-profile/[nid]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;

  /* Display: Intro page */
  $handler = $view->new_display('page', 'Intro page', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['value']['value'] = '184';
  $handler->display->display_options['path'] = 'museum-profiles/intro';

  /* Display: Museum page */
  $handler = $view->new_display('page', 'Museum page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'field_image_museum' => 'field_image_museum',
    'nid' => 'field_image_museum',
    'body' => 'body',
    'field_size_of_uk_algal_collectio' => 'body',
    'field_collections_website' => 'body',
    'field_date_of_earliest_specimen' => 'body',
    'field_main_collecting_period' => 'body',
    'field_notes_coll_digit' => 'body',
    'field_significant_collectors_rep' => 'body',
    'nothing_1' => 'body',
    'field_significant_herbaria_incor' => 'body',
    'nothing' => 'body',
    'field_museum_location' => 'body',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_image_museum' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'field_size_of_uk_algal_collectio' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collections_website' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_date_of_earliest_specimen' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_main_collecting_period' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_notes_coll_digit' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_significant_collectors_rep' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_significant_herbaria_incor' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_museum_location' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '<a href="/museum-profiles">Return to museum profiles list</a>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Museum image */
  $handler->display->display_options['fields']['field_image_museum']['id'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['table'] = 'field_data_field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['field'] = 'field_image_museum';
  $handler->display->display_options['fields']['field_image_museum']['label'] = '';
  $handler->display->display_options['fields']['field_image_museum']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image_museum']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_museum']['settings'] = array(
    'image_style' => 'square_thumbnail_large',
    'image_link' => '',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content: Approzimate size of UK algal collection */
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['id'] = 'field_size_of_uk_algal_collectio';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['table'] = 'field_data_field_size_of_uk_algal_collectio';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['field'] = 'field_size_of_uk_algal_collectio';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['label'] = '';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['alter']['text'] = 'Approximate size of UK algal collection: [field_size_of_uk_algal_collectio]';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['element_type'] = '0';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['empty'] = 'Approzimate size of UK algal collection: n/a';
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_size_of_uk_algal_collectio']['settings'] = array(
    'thousand_separator' => ',',
    'prefix_suffix' => 0,
  );
  /* Field: Content: Collections website */
  $handler->display->display_options['fields']['field_collections_website']['id'] = 'field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['table'] = 'field_data_field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['field'] = 'field_collections_website';
  $handler->display->display_options['fields']['field_collections_website']['label'] = '';
  $handler->display->display_options['fields']['field_collections_website']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_collections_website']['alter']['text'] = 'Collections website: [field_collections_website]';
  $handler->display->display_options['fields']['field_collections_website']['element_type'] = '0';
  $handler->display->display_options['fields']['field_collections_website']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_collections_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_collections_website']['click_sort_column'] = 'url';
  /* Field: Content: Date of earliest specimen */
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['id'] = 'field_date_of_earliest_specimen';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['table'] = 'field_data_field_date_of_earliest_specimen';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['field'] = 'field_date_of_earliest_specimen';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['label'] = '';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['alter']['text'] = 'Date of earliest specimen: [field_date_of_earliest_specimen]';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['element_type'] = '0';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['empty'] = 'Date of earliest specimen: n/a';
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_date_of_earliest_specimen']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Main collecting period */
  $handler->display->display_options['fields']['field_main_collecting_period']['id'] = 'field_main_collecting_period';
  $handler->display->display_options['fields']['field_main_collecting_period']['table'] = 'field_data_field_main_collecting_period';
  $handler->display->display_options['fields']['field_main_collecting_period']['field'] = 'field_main_collecting_period';
  $handler->display->display_options['fields']['field_main_collecting_period']['label'] = '';
  $handler->display->display_options['fields']['field_main_collecting_period']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_main_collecting_period']['alter']['text'] = 'Main collecting period: [field_main_collecting_period]';
  $handler->display->display_options['fields']['field_main_collecting_period']['element_type'] = '0';
  $handler->display->display_options['fields']['field_main_collecting_period']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_main_collecting_period']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_main_collecting_period']['empty'] = 'Main collecting period: n/a';
  /* Field: Content: Notes on collection digitization */
  $handler->display->display_options['fields']['field_notes_coll_digit']['id'] = 'field_notes_coll_digit';
  $handler->display->display_options['fields']['field_notes_coll_digit']['table'] = 'field_data_field_notes_coll_digit';
  $handler->display->display_options['fields']['field_notes_coll_digit']['field'] = 'field_notes_coll_digit';
  $handler->display->display_options['fields']['field_notes_coll_digit']['label'] = '';
  $handler->display->display_options['fields']['field_notes_coll_digit']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_notes_coll_digit']['alter']['text'] = 'Notes on collection digitization: [field_notes_coll_digit]';
  $handler->display->display_options['fields']['field_notes_coll_digit']['element_type'] = '0';
  $handler->display->display_options['fields']['field_notes_coll_digit']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_notes_coll_digit']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_notes_coll_digit']['empty'] = 'Notes on collection digitization: n/a';
  /* Field: Content: Significant collectors represented */
  $handler->display->display_options['fields']['field_significant_collectors_rep']['id'] = 'field_significant_collectors_rep';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['table'] = 'field_data_field_significant_collectors_rep';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['field'] = 'field_significant_collectors_rep';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['label'] = '';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_significant_collectors_rep']['alter']['text'] = 'Significant collectors represented: [field_significant_collectors_rep]';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_significant_collectors_rep']['empty'] = 'n/a';
  $handler->display->display_options['fields']['field_significant_collectors_rep']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_significant_collectors_rep']['delta_offset'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Significant collectors represented';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Significant collectors represented: [field_significant_collectors_rep]';
  $handler->display->display_options['fields']['nothing_1']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Content: Significant herbaria incorporated */
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['id'] = 'field_significant_herbaria_incor';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['table'] = 'field_data_field_significant_herbaria_incor';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['field'] = 'field_significant_herbaria_incor';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['label'] = '';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['alter']['text'] = 'Significant herbaria incorporated: [field_significant_herbaria_incor]';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['empty'] = 'n/a';
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_significant_herbaria_incor']['delta_offset'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Significant herbaria incorporated';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Significant herbaria incorporated: [field_significant_herbaria_incor]';
  $handler->display->display_options['fields']['nothing']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_label_type'] = 'span';
  /* Field: Content: Museum location */
  $handler->display->display_options['fields']['field_museum_location']['id'] = 'field_museum_location';
  $handler->display->display_options['fields']['field_museum_location']['table'] = 'field_data_field_museum_location';
  $handler->display->display_options['fields']['field_museum_location']['field'] = 'field_museum_location';
  $handler->display->display_options['fields']['field_museum_location']['label'] = '';
  $handler->display->display_options['fields']['field_museum_location']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_museum_location']['alter']['text'] = 'Museum location: [field_museum_location]';
  $handler->display->display_options['fields']['field_museum_location']['element_type'] = '0';
  $handler->display->display_options['fields']['field_museum_location']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_museum_location']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_museum_location']['empty'] = 'Museum location: n/a';
  $handler->display->display_options['fields']['field_museum_location']['settings'] = array(
    'node_reference_view_mode' => 'full',
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Museums';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['breadcrumb'] = 'Museums';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'museum_profile' => 'museum_profile',
  );
  $handler->display->display_options['path'] = 'museum-profiles/%/%/profile';
  $handler->display->display_options['menu']['title'] = 'Museum profile';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['museum_profiles'] = $view;

  $view = new view();
  $view->name = 'museum_specimen_dates';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Museum specimen dates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Museum specimen dates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_institution_code' => 'field_institution_code',
    'field_date_collected' => 'field_date_collected',
    'field_date_collected_1' => 'field_date_collected_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_institution_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_date_collected' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_date_collected_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['label'] = 'Museum';
  /* Field: Content: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['settings'] = array(
    'format_type' => 'year_only',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_date_collected']['format_date_sql'] = 1;
  /* Field: COUNT(Content: Date collected) */
  $handler->display->display_options['fields']['field_date_collected_1']['id'] = 'field_date_collected_1';
  $handler->display->display_options['fields']['field_date_collected_1']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected_1']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_date_collected_1']['label'] = 'Number of specimens listed on this site';
  /* Contextual filter: Content: Institution code (field_institution_code) */
  $handler->display->display_options['arguments']['field_institution_code_value']['id'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['arguments']['field_institution_code_value']['field'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_institution_code_value']['limit'] = '0';
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
    'specimen_observation' => 'specimen_observation',
  );
  /* Filter criterion: Content: Date collected -  start date (field_date_collected) */
  $handler->display->display_options['filters']['field_date_collected_value']['id'] = 'field_date_collected_value';
  $handler->display->display_options['filters']['field_date_collected_value']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['filters']['field_date_collected_value']['field'] = 'field_date_collected_value';
  $handler->display->display_options['filters']['field_date_collected_value']['operator'] = 'not empty';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'museum-specimen-dates';

  /* Display: All block */
  $handler = $view->new_display('block', 'All block', 'block_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_institution_code' => 'field_institution_code',
    'field_date_collected' => 'field_date_collected',
    'field_date_collected_1' => 'field_date_collected_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_institution_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_date_collected' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_date_collected_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Filtered block */
  $handler = $view->new_display('block', 'Filtered block', 'block_2');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Institution code (field_institution_code) */
  $handler->display->display_options['arguments']['field_institution_code_value']['id'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['arguments']['field_institution_code_value']['field'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_institution_code_value']['limit'] = '0';
  $handler->display->display_options['block_description'] = 'Museum specimens date';
  $export['museum_specimen_dates'] = $view;

  $view = new view();
  $view->name = 'museum_specimens';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Museum specimen count';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Museum specimen count';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_institution_code' => 'field_institution_code',
    'field_institution_code_1' => 'field_institution_code_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_institution_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_institution_code_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['label'] = 'Museum';
  $handler->display->display_options['fields']['field_institution_code']['element_type'] = '0';
  $handler->display->display_options['fields']['field_institution_code']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_institution_code']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_institution_code']['separator'] = ',';
  /* Field: COUNT(Content: Institution code) */
  $handler->display->display_options['fields']['field_institution_code_1']['id'] = 'field_institution_code_1';
  $handler->display->display_options['fields']['field_institution_code_1']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_institution_code_1']['label'] = 'Count';
  $handler->display->display_options['fields']['field_institution_code_1']['element_type'] = '0';
  $handler->display->display_options['fields']['field_institution_code_1']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_institution_code_1']['element_label_colon'] = FALSE;
  /* Contextual filter: Content: Institution code (field_institution_code) */
  $handler->display->display_options['arguments']['field_institution_code_value']['id'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['arguments']['field_institution_code_value']['field'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['exception']['value'] = '';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_institution_code_value']['limit'] = '0';
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
    'specimen_observation' => 'specimen_observation',
  );

  /* Display: Filtered count block */
  $handler = $view->new_display('block', 'Filtered count block', 'block');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Institution code (field_institution_code) */
  $handler->display->display_options['arguments']['field_institution_code_value']['id'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['arguments']['field_institution_code_value']['field'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_institution_code_value']['limit'] = '0';
  $handler->display->display_options['block_description'] = 'Specimen count block';

  /* Display: All count block */
  $handler = $view->new_display('block', 'All count block', 'block_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_institution_code' => 'field_institution_code',
    'field_institution_code_1' => 'field_institution_code_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_institution_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_institution_code_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['museum_specimens'] = $view;

  $view = new view();
  $view->name = 'museum_statistics_page';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Museum statistics page';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Museum statistics';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
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
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['value']['value'] = '188';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'museum-statistics';
  $export['museum_statistics_page'] = $view;

  return $export;
}
