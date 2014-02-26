<?php

/**
 * @file
 * collections_profile.views_default.inc
 */
/**
 * Implements hook_views_default_views().
 */
function collections_profile_views_default_views(){
  $export = array();
  $view = new view();
  $view->name = 'collections_profiles';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Collections profiles';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Profile';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
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
  $handler->display->display_options['fields']['title']['alter']['path'] = 'collections_profiles/[nid]';
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
    'trim_length' => '600'
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
    'collections_profile' => 'collections_profile'
  );
  /* Display: Collections profiles index page */
  $handler = $view->new_display('page', 'Collections profiles index page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Collections profiles';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'table-valign-top';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_media' => 'field_media',
    'field_coll_institution_code' => 'field_media',
    'nid' => 'field_media',
    'title' => 'title',
    'body' => 'title',
    'field_coll_website' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_media' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_coll_institution_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_coll_website' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Sorry there is not yet any content of this type.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Media */
  $handler->display->display_options['fields']['field_media']['id'] = 'field_media';
  $handler->display->display_options['fields']['field_media']['table'] = 'field_data_field_media';
  $handler->display->display_options['fields']['field_media']['field'] = 'field_media';
  $handler->display->display_options['fields']['field_media']['label'] = '';
  $handler->display->display_options['fields']['field_media']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_media']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_media']['type'] = 'styles_file_square_thumbnail';
  $handler->display->display_options['fields']['field_media']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_media']['delta_offset'] = '0';
  /* Field: Content: Institution collection code */
  $handler->display->display_options['fields']['field_coll_institution_code']['id'] = 'field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['table'] = 'field_data_field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['field'] = 'field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['label'] = '';
  $handler->display->display_options['fields']['field_coll_institution_code']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_coll_institution_code']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_coll_institution_code']['type'] = 'text_plain';
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
  $handler->display->display_options['fields']['title']['alter']['path'] = 'collections_profile/[nid]/[field_coll_institution_code]/profile';
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
    'trim_length' => '600'
  );
  /* Field: Content: Collections website */
  $handler->display->display_options['fields']['field_coll_website']['id'] = 'field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['table'] = 'field_data_field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['field'] = 'field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['label'] = '';
  $handler->display->display_options['fields']['field_coll_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_coll_website']['click_sort_column'] = 'url';
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
  $handler->display->display_options['arguments']['nid']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['breadcrumb'] = 'test';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'collections_profile';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Collections profiles';
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
  $handler->display->display_options['fields']['title']['alter']['path'] = 'collections_profiles/[nid]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Display: Institutions page */
  $handler = $view->new_display('page', 'Institutions page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '<p><a href="/collections_profile">Return to collections profiles list</a></p>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Media */
  $handler->display->display_options['fields']['field_media']['id'] = 'field_media';
  $handler->display->display_options['fields']['field_media']['table'] = 'field_data_field_media';
  $handler->display->display_options['fields']['field_media']['field'] = 'field_media';
  $handler->display->display_options['fields']['field_media']['label'] = '';
  $handler->display->display_options['fields']['field_media']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_media']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_media']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_media']['type'] = 'styles_file_square_thumbnail';
  $handler->display->display_options['fields']['field_media']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_media']['delta_offset'] = '0';
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
  /* Field: Content: Main collecting period */
  $handler->display->display_options['fields']['field_coll_main_period']['id'] = 'field_coll_main_period';
  $handler->display->display_options['fields']['field_coll_main_period']['table'] = 'field_data_field_coll_main_period';
  $handler->display->display_options['fields']['field_coll_main_period']['field'] = 'field_coll_main_period';
  $handler->display->display_options['fields']['field_coll_main_period']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_main_period']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_main_period']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_main_period']['empty_zero'] = TRUE;
  /* Field: Content: Date of earliest specimen */
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['id'] = 'field_coll_earliest_spec_date';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['table'] = 'field_data_field_coll_earliest_spec_date';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['field'] = 'field_coll_earliest_spec_date';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['alter']['text'] = 'Date of earliest specimen: [field_coll_earliest_spec_date]';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => ''
  );
  $handler->display->display_options['fields']['field_coll_earliest_spec_date']['format_date_sql'] = 0;
  /* Field: Content: Significant collections incorporated */
  $handler->display->display_options['fields']['field_coll_significant']['id'] = 'field_coll_significant';
  $handler->display->display_options['fields']['field_coll_significant']['table'] = 'field_data_field_coll_significant';
  $handler->display->display_options['fields']['field_coll_significant']['field'] = 'field_coll_significant';
  $handler->display->display_options['fields']['field_coll_significant']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_significant']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_significant']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_significant']['empty_zero'] = TRUE;
  /* Field: Content: Notes on collection digitization */
  $handler->display->display_options['fields']['field_coll_digitization_notes']['id'] = 'field_coll_digitization_notes';
  $handler->display->display_options['fields']['field_coll_digitization_notes']['table'] = 'field_data_field_coll_digitization_notes';
  $handler->display->display_options['fields']['field_coll_digitization_notes']['field'] = 'field_coll_digitization_notes';
  $handler->display->display_options['fields']['field_coll_digitization_notes']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_digitization_notes']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_digitization_notes']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_digitization_notes']['empty_zero'] = TRUE;
  /* Field: Content: Collections website */
  $handler->display->display_options['fields']['field_coll_website']['id'] = 'field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['table'] = 'field_data_field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['field'] = 'field_coll_website';
  $handler->display->display_options['fields']['field_coll_website']['label'] = 'Website';
  $handler->display->display_options['fields']['field_coll_website']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_website']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_website']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_website']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_coll_website']['click_sort_column'] = 'url';
  /* Field: Content: Institution location */
  $handler->display->display_options['fields']['field_coll_institution_location']['id'] = 'field_coll_institution_location';
  $handler->display->display_options['fields']['field_coll_institution_location']['table'] = 'field_data_field_coll_institution_location';
  $handler->display->display_options['fields']['field_coll_institution_location']['field'] = 'field_coll_institution_location';
  $handler->display->display_options['fields']['field_coll_institution_location']['label'] = 'Location';
  $handler->display->display_options['fields']['field_coll_institution_location']['element_type'] = '0';
  $handler->display->display_options['fields']['field_coll_institution_location']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_coll_institution_location']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coll_institution_location']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_coll_institution_location']['settings'] = array(
    'node_reference_view_mode' => 'full'
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Collections';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['breadcrumb'] = 'Collections';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'collections_profile' => 'collections_profile'
  );
  $handler->display->display_options['path'] = 'collections_profile/%/%/profile';
  $handler->display->display_options['menu']['title'] = 'Collection profile';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  /* specimen statistics blocks */
  $export['collections_profiles'] = $view;
  $view = new view();
  $view->name = 'collections_specimen_statistics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Collections specimen statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
              $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Specimen count by date';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Institution collection code */
  $handler->display->display_options['fields']['field_coll_institution_code']['id'] = 'field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['table'] = 'field_data_field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['field'] = 'field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code']['label'] = 'Collection Institution';
  /* Field: COUNT(Content: Institution collection code) */
  $handler->display->display_options['fields']['field_coll_institution_code_1']['id'] = 'field_coll_institution_code_1';
  $handler->display->display_options['fields']['field_coll_institution_code_1']['table'] = 'field_data_field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code_1']['field'] = 'field_coll_institution_code';
  $handler->display->display_options['fields']['field_coll_institution_code_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_coll_institution_code_1']['label'] = 'Count';
  /* Field: Content: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => ''
  );
  $handler->display->display_options['fields']['field_date_collected']['format_date_sql'] = 0;
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
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Specimen count by institution */
  $handler = $view->new_display('block', 'Specimen count by institution', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Specimen count this institution';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  /* Field: COUNT(Content: Institution code) */
  $handler->display->display_options['fields']['field_institution_code_1']['id'] = 'field_institution_code_1';
  $handler->display->display_options['fields']['field_institution_code_1']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_institution_code_1']['label'] = 'Count';
  $handler->display->display_options['block_description'] = 'Specimen count by institution';
  /* Display: Specimen count by year */
  $handler = $view->new_display('block', 'Specimen count by year', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Specimen count by year';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  /* Field: Content: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['settings'] = array(
    'format_type' => 'year_only',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => ''
  );
  $handler->display->display_options['fields']['field_date_collected']['format_date_sql'] = 1;
  /* Field: COUNT(Content: Date collected) */
  $handler->display->display_options['fields']['field_date_collected_1']['id'] = 'field_date_collected_1';
  $handler->display->display_options['fields']['field_date_collected_1']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected_1']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_date_collected_1']['label'] = 'Count';
  $handler->display->display_options['block_description'] = 'Specimen count by year';
  /* Display: Count all specimens by Institution */
  $handler = $view->new_display('block', 'Count all specimens by Institution', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Specimen count all institutions';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  /* Field: COUNT(Content: Institution code) */
  $handler->display->display_options['fields']['field_institution_code_1']['id'] = 'field_institution_code_1';
  $handler->display->display_options['fields']['field_institution_code_1']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_institution_code_1']['label'] = 'Count';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Institution code (field_institution_code) */
  $handler->display->display_options['arguments']['field_institution_code_value']['id'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['arguments']['field_institution_code_value']['field'] = 'field_institution_code_value';
  $handler->display->display_options['arguments']['field_institution_code_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_institution_code_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_institution_code_value']['limit'] = '0';
  $handler->display->display_options['block_description'] = 'Specimen count all institutions';
  $export['collections_specimen_statistics'] = $view;
  return $export;
}
  