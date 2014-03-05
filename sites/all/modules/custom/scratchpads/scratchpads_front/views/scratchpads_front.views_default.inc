<?php

function scratchpads_front_views_default_views(){
  /**
   * Front page blocks
   */
  $view = new view();
  $view->name = 'front_page_blocks';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Scratchpads front page blocks';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Home page block';
  $handler->display->display_options['css_class'] = 'grid-4';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'By adding content to your site, links to the latest content will appear here.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* No results behavior: Global: View area */
  $handler->display->display_options['empty']['view']['id'] = 'view';
  $handler->display->display_options['empty']['view']['table'] = 'views';
  $handler->display->display_options['empty']['view']['field'] = 'view';
  $handler->display->display_options['empty']['view']['empty'] = TRUE;
  $handler->display->display_options['empty']['view']['view_to_insert'] = 'front_page_blocks:block_4';
  $handler->display->display_options['empty']['view']['inherit_arguments'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['created']['alter']['text'] = '[name] - [created]';
  $handler->display->display_options['fields']['created']['element_type'] = 'p';
  $handler->display->display_options['fields']['created']['element_class'] = 'description';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '100'
  );
  /* Sort criterion: Content: Promoted to front page */
  $handler->display->display_options['sorts']['promote']['id'] = 'promote';
  $handler->display->display_options['sorts']['promote']['table'] = 'node';
  $handler->display->display_options['sorts']['promote']['field'] = 'promote';
  $handler->display->display_options['sorts']['promote']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['type']['title'] = 'Recent %1s';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
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
  $handler->display->display_options['filters']['type']['operator'] = 'not in';
  $handler->display->display_options['filters']['type']['value'] = array(
    'spm' => 'spm'
  );
  /* Display: Block one */
  $handler = $view->new_display('block', 'Block one', 'block_1');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'alpha grid-4';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: View area */
  $handler->display->display_options['empty']['view']['id'] = 'view';
  $handler->display->display_options['empty']['view']['table'] = 'views';
  $handler->display->display_options['empty']['view']['field'] = 'view';
  $handler->display->display_options['empty']['view']['empty'] = TRUE;
  $handler->display->display_options['empty']['view']['view_to_insert'] = 'scratchpads_front_page_taxon_description:block_4';
  $handler->display->display_options['empty']['view']['inherit_arguments'] = TRUE;
  /* Display: Block two */
  $handler = $view->new_display('block', 'Block two', 'block_2');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: View area */
  $handler->display->display_options['empty']['view']['id'] = 'view';
  $handler->display->display_options['empty']['view']['table'] = 'views';
  $handler->display->display_options['empty']['view']['field'] = 'view';
  $handler->display->display_options['empty']['view']['empty'] = TRUE;
  $handler->display->display_options['empty']['view']['view_to_insert'] = 'scratchpads_front_page_taxon_description:block_4';
  $handler->display->display_options['empty']['view']['inherit_arguments'] = TRUE;
  /* Display: Block three */
  $handler = $view->new_display('block', 'Block three', 'block_3');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'omega grid-4';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: View area */
  $handler->display->display_options['empty']['view']['id'] = 'view';
  $handler->display->display_options['empty']['view']['table'] = 'views';
  $handler->display->display_options['empty']['view']['field'] = 'view';
  $handler->display->display_options['empty']['view']['empty'] = TRUE;
  $handler->display->display_options['empty']['view']['view_to_insert'] = 'scratchpads_front_page_taxon_description:block_4';
  $handler->display->display_options['empty']['view']['inherit_arguments'] = TRUE;
  $views['front_page_blocks'] = $view;
  /**
   * Front page taxon description block
   */
  $view = new view();
  $view->name = 'scratchpads_front_page_taxon_description';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Scratchpads front page taxon description';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Home page block';
  $handler->display->display_options['css_class'] = 'grid-4';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'By adding content to your site, links to the latest content will appear here.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* No results behavior: Global: View area */
  $handler->display->display_options['empty']['view']['id'] = 'view';
  $handler->display->display_options['empty']['view']['table'] = 'views';
  $handler->display->display_options['empty']['view']['field'] = 'view';
  $handler->display->display_options['empty']['view']['empty'] = TRUE;
  $handler->display->display_options['empty']['view']['view_to_insert'] = 'front_page_blocks:block_4';
  $handler->display->display_options['empty']['view']['inherit_arguments'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['created']['alter']['text'] = '[name] - [created]';
  $handler->display->display_options['fields']['created']['element_type'] = 'p';
  $handler->display->display_options['fields']['created']['element_class'] = 'description';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '100'
  );
  /* Sort criterion: Content: Promoted to front page */
  $handler->display->display_options['sorts']['promote']['id'] = 'promote';
  $handler->display->display_options['sorts']['promote']['table'] = 'node';
  $handler->display->display_options['sorts']['promote']['field'] = 'promote';
  $handler->display->display_options['sorts']['promote']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['type']['title'] = 'Recent %1s';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
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
  $handler->display->display_options['filters']['type']['operator'] = 'not in';
  $handler->display->display_options['filters']['type']['value'] = array(
    'spm' => 'spm'
  );
  /* Display: Block Taxon Description */
  $handler = $view->new_display('block', 'Block Taxon Description', 'block_4');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'omega grid-4';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'By adding content to your site, links to the latest content will appear here.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Taxonomy Reference */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['ui_name'] = 'Taxonomy Reference';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'taxonomy/term/[tid]/descriptions';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['created']['alter']['text'] = '[name] - [created]';
  $handler->display->display_options['fields']['created']['element_type'] = 'p';
  $handler->display->display_options['fields']['created']['element_class'] = 'description';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '100'
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'spm' => 'spm'
  );
  $views['scratchpads_front_page_taxon_description'] = $view;
  /**
   * Front page slideshow
   */
  $view = new view();
  $view->name = 'front_page_slideshow';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'file_managed';
  $view->human_name = 'Front page slideshow';
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
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slideshow';
  $handler->display->display_options['style_options']['slideshow_type'] = 'views_slideshow_cycle';
  $handler->display->display_options['style_options']['slideshow_skin'] = 'default';
  $handler->display->display_options['style_options']['skin_info'] = array(
    'class' => 'default',
    'name' => 'Default',
    'module' => 'views_slideshow',
    'path' => '',
    'stylesheets' => array()
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'rendered_entity' => 0,
    'rendered_entity_1' => 0,
    'filename' => 0
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'rendered_entity_1' => 'rendered_entity_1',
    'rendered_entity' => 0,
    'filename' => 0
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_hover'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['effect'] = 'none';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['transition_advanced'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['wait_for_image_load_timeout'] = '3000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<div class="view view-front-page-slideshow view-id-front_page_slideshow view-display-id-default"><div class="view-content"><div class="skin-default"><div class="views_slideshow_cycle_main views_slideshow_main" id="views_slideshow_cycle_main_front_page_slideshow-default"><div class="views-slideshow-cycle-main-frame views_slideshow_cycle_teaser_section" id="views_slideshow_cycle_teaser_section_front_page_slideshow-default" style="position: relative; width: 619px; height: 463px;"><div class="views-slideshow-cycle-main-frame-row views_slideshow_cycle_slide views_slideshow_slide views-row-4 views_slideshow_cycle_hidden views-row-even" id="views_slideshow_cycle_div_front_page_slideshow-default_3" style="position: absolute; top: 0px; left: 0px; display: none; z-index: 4; opacity: 0; width: 619px; height: 463px;"><div class="views-slideshow-cycle-main-frame-row-item views-row views-row-0 views-row-first views-row-odd"><div class="views-field views-field-rendered-entity"><div class="file file-image file-image-jpeg contextual-links-region"><div class="content"><span class="field-content"><span class="styles file-styles slideshow_large" id="styles-6-0"><img width="619" height="463" src="data:image/gif;base64,R0lGODlhAQABAIABANLS0gAAACH5BAEKAAEALAAAAAABAAEAAAICRAEAOw==" /></span></span></div></div></div><div class="views-field views-field-filename"><span class="field-content">' . t('Up to four random images from your site will appear here.') . '</span></div></div></div></div></div><div class="views-slideshow-controls-bottom clearfix"><div class="views-slideshow-pager-fields widget_pager widget_pager_bottom views_slideshow_pager_field views-slideshow-pager-field-processed" id="widget_pager_bottom_front_page_slideshow-default"><div class="views-slideshow-pager-field-item views_slideshow_pager_field_item views_slideshow_active_pager_field_item views-row-odd active" id="views_slideshow_pager_field_item_bottom_front_page_slideshow-default_0"><div class="views-field-rendered-entity"><div class="views-content-rendered-entity"><div class="file file-image file-image-jpeg contextual-links-region"><div class="content"><span class="styles file-styles slideshow_thumbnail"><img width="154" height="115" src="data:image/gif;base64,R0lGODlhAQABAIABANLS0gAAACH5BAEKAAEALAAAAAABAAEAAAICRAEAOw==" /></span></div></div></div></div></div><div class="views-slideshow-pager-field-item views_slideshow_pager_field_item views-row-even" id="views_slideshow_pager_field_item_bottom_front_page_slideshow-default_1"><div class="views-field-rendered-entity"><div class="views-content-rendered-entity"><div class="file file-image file-image-jpeg contextual-links-region"><div class="content"><span class="styles file-styles slideshow_thumbnail"><img width="154" height="115" src="data:image/gif;base64,R0lGODlhAQABAIABANLS0gAAACH5BAEKAAEALAAAAAABAAEAAAICRAEAOw==" /></span></div></div></div></div></div><div class="views-slideshow-pager-field-item views_slideshow_pager_field_item views-row-odd" id="views_slideshow_pager_field_item_bottom_front_page_slideshow-default_2"><div class="views-field-rendered-entity"><div class="views-content-rendered-entity"><div class="file file-image file-image-jpeg contextual-links-region"><div class="content"><span class="styles file-styles slideshow_thumbnail" id="styles-5-0"><img width="154" height="115" src="data:image/gif;base64,R0lGODlhAQABAIABANLS0gAAACH5BAEKAAEALAAAAAABAAEAAAICRAEAOw==" /></span></div></div></div></div></div><div class="views-slideshow-pager-field-item views_slideshow_pager_field_item views-row-even"><div class="views-field-rendered-entity"><div class="views-content-rendered-entity"><div class="file file-image file-image-jpeg contextual-links-region"><div class="content"><span class="styles file-styles slideshow_thumbnail" id="styles-7-0"><img width="154" height="115" title="Andrallus spinidens (Photograph by Tristan Bantock)" src="data:image/gif;base64,R0lGODlhAQABAIABANLS0gAAACH5BAEKAAEALAAAAAABAAEAAAICRAEAOw==" /></span></div></div></div></div></div></div></div></div></div></div>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_slideshow_large';
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'file_styles_slideshow_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['exclude'] = TRUE;
  $handler->display->display_options['fields']['filename']['alter']['max_length'] = '75';
  $handler->display->display_options['fields']['filename']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['filename']['element_wrapper_type'] = 'h3';
  /* Field: File: Creator */
  $handler->display->display_options['fields']['field_creator']['id'] = 'field_creator';
  $handler->display->display_options['fields']['field_creator']['table'] = 'field_data_field_creator';
  $handler->display->display_options['fields']['field_creator']['field'] = 'field_creator';
  $handler->display->display_options['fields']['field_creator']['label'] = '';
  $handler->display->display_options['fields']['field_creator']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_creator']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_creator']['alter']['text'] = '<span class=\'creator\'>Creator: [field_creator]</span>';
  $handler->display->display_options['fields']['field_creator']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_creator']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_creator']['empty'] = '<div></div>';
  $handler->display->display_options['fields']['field_creator']['hide_alter_empty'] = FALSE;
  /* Field: File: Licence */
  $handler->display->display_options['fields']['field_cc_licence_1']['id'] = 'field_cc_licence_1';
  $handler->display->display_options['fields']['field_cc_licence_1']['table'] = 'field_data_field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence_1']['field'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence_1']['label'] = '';
  $handler->display->display_options['fields']['field_cc_licence_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cc_licence_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cc_licence_1']['alter']['text'] = '<span class=\'licence\'>[field_cc_licence_1]</span>';
  $handler->display->display_options['fields']['field_cc_licence_1']['element_label_colon'] = FALSE;
  /* Field: Container */
  $handler->display->display_options['fields']['field_cc_licence']['id'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['table'] = 'field_data_field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['field'] = 'field_cc_licence';
  $handler->display->display_options['fields']['field_cc_licence']['ui_name'] = 'Container';
  $handler->display->display_options['fields']['field_cc_licence']['label'] = '';
  $handler->display->display_options['fields']['field_cc_licence']['alter']['alter_text'] = TRUE;
  // The display is altered dynamically according to variable values
  $title_only = "class='title_only'";
  $creator_display = '';
  $licence_display = '';
  $other_fields = '';
  if(variable_get('front_page_main_block_slideshow_text_creator', 0)){
    $creator_display = "[field_creator]";
    $title_only = '';
  }
  if(variable_get('front_page_main_block_slideshow_text_licence', 0)){
    $licence_display = "[field_cc_licence_1]";
    $title_only = '';
  }
  if($title_only == ''){
    $other_fields = '<div>' . $creator_display . ' ' . $licence_display . '</div>';
  }
  $handler->display->display_options['fields']['field_cc_licence']['alter']['text'] = '<div id="slideshow-transparency" ' . $title_only . '></div>
<div id="slideshow-content" ' . $title_only . '>
<h3>[filename]</h3>
' . $other_fields . '
</div>';
  $handler->display->display_options['fields']['field_cc_licence']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cc_licence']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_cc_licence']['element_wrapper_class'] = 'caption';
  /* Sort criterion: File: Promote to home page (field_promote_to_home_page) */
  $handler->display->display_options['sorts']['field_promote_to_home_page_value']['id'] = 'field_promote_to_home_page_value';
  $handler->display->display_options['sorts']['field_promote_to_home_page_value']['table'] = 'field_data_field_promote_to_home_page';
  $handler->display->display_options['sorts']['field_promote_to_home_page_value']['field'] = 'field_promote_to_home_page_value';
  $handler->display->display_options['sorts']['field_promote_to_home_page_value']['order'] = 'DESC';
  /* Sort criterion: File: Sticky at top of lists (field_sticky) */
  $handler->display->display_options['sorts']['field_sticky_value']['id'] = 'field_sticky_value';
  $handler->display->display_options['sorts']['field_sticky_value']['table'] = 'field_data_field_sticky';
  $handler->display->display_options['sorts']['field_sticky_value']['field'] = 'field_sticky_value';
  $handler->display->display_options['sorts']['field_sticky_value']['order'] = 'DESC';
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image'
  );
  /* Filter criterion: File: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'file_managed';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1'
  );
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri']['id'] = 'uri';
  $handler->display->display_options['filters']['uri']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri']['field'] = 'uri';
  $handler->display->display_options['filters']['uri']['operator'] = 'not';
  $handler->display->display_options['filters']['uri']['value'] = '/eolapi/';
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri_1']['id'] = 'uri_1';
  $handler->display->display_options['filters']['uri_1']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri_1']['field'] = 'uri';
  $handler->display->display_options['filters']['uri_1']['operator'] = 'not';
  $handler->display->display_options['filters']['uri_1']['value'] = '/pictures/';
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri_2']['id'] = 'uri_2';
  $handler->display->display_options['filters']['uri_2']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri_2']['field'] = 'uri';
  $handler->display->display_options['filters']['uri_2']['operator'] = 'not';
  $handler->display->display_options['filters']['uri_2']['value'] = '/signature/';
  /* Filter criterion: File: Path */
  $handler->display->display_options['filters']['uri_3']['id'] = 'uri_3';
  $handler->display->display_options['filters']['uri_3']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri_3']['field'] = 'uri';
  $handler->display->display_options['filters']['uri_3']['operator'] = 'contains';
  $handler->display->display_options['filters']['uri_3']['value'] = 'public';
  /* Display: Latest images */
  $handler = $view->new_display('block', 'Latest images', 'block_1');
  $views['front_page_slideshow'] = $view;
  /**
   * Front page main block
   */
  $view = new view();
  $view->name = 'front_page_content';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Front page content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Home page content';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = FALSE;
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
  /* Sort criterion: Content: Promoted to front page */
  $handler->display->display_options['sorts']['promote']['id'] = 'promote';
  $handler->display->display_options['sorts']['promote']['table'] = 'node';
  $handler->display->display_options['sorts']['promote']['field'] = 'promote';
  $handler->display->display_options['sorts']['promote']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['title_enable'] = 1;
  $handler->display->display_options['arguments']['type']['title'] = '%1';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_description'] = 'Front page content block';
  $views['front_page_content'] = $view;
  return $views;
}
