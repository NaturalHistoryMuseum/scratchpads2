<?php

function scratchpads_simple_taxonomy_page_views_default_views(){
  $views = array();
  $view = new view();
  $view->name = 'simple_taxonomy_page_images';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'file_managed';
  $view->human_name = 'Simple taxonomy page images';
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
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '8';
  $handler->display->display_options['style_options']['fill_single_line'] = 1;
  $handler->display->display_options['row_plugin'] = 'file';
  $handler->display->display_options['row_options']['view_mode'] = 'file_styles_square_thumbnail';
  $handler->display->display_options['row_options']['links'] = 1;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filename']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filename']['link_to_file'] = 1;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
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
  $handler->display->display_options['fields']['uri']['link_to_file'] = 1;
  $handler->display->display_options['fields']['uri']['file_download_path'] = 0;
  /* Contextual filter: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['not'] = 0;
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image'
  );
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['simple_taxonomy_page_images'] = array(
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
    t('Path'),
    t('All'),
    t('Block')
  );
  $views[$view->name] = $view;
  return $views;
}