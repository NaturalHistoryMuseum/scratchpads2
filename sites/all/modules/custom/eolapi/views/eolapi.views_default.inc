<?php

/**
 * Implementation of hook_views_default_views().
 */
function eolapi_views_default_views(){
  $views = array();
  // -- EOL Images -------------------------------------------------------------------------------------------------
  $view = new view();
  $view->name = 'eol_images';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eolapi';
  $view->context = 'species_media';
  $view->human_name = 'EOL Images';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'EOL Images';
  $handler->display->display_options['css_class'] = 'alpha omega';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'eolapi';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Encyclopaedia of Life data: Encyclopaedia of life data ID */
  $handler->display->display_options['fields']['eid']['id'] = 'eid';
  $handler->display->display_options['fields']['eid']['table'] = 'eolapi';
  $handler->display->display_options['fields']['eid']['field'] = 'eid';
  /* Sort criterion: Encyclopaedia of Life data: Rating */
  $handler->display->display_options['sorts']['rating']['id'] = 'rating';
  $handler->display->display_options['sorts']['rating']['table'] = 'eolapi';
  $handler->display->display_options['sorts']['rating']['field'] = 'rating';
  $handler->display->display_options['sorts']['rating']['order'] = 'DESC';
  /* Contextual filter: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['not'] = 0;
  /* Filter criterion: Encyclopaedia of Life data: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eolapi';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'stillimage' => 'stillimage'
  );
  /* Filter criterion: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['operator'] = 'not empty';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['vocabulary'] = 'tags';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['hierarchy'] = 1;
  /* Filter criterion: Encyclopaedia of Life data: Source */
  $handler->display->display_options['filters']['source']['id'] = 'source';
  $handler->display->display_options['filters']['source']['table'] = 'eolapi';
  $handler->display->display_options['filters']['source']['field'] = 'source';
  $handler->display->display_options['filters']['source']['value'] = 'EOL';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  // -- EOL Images -------------------------------------------------------------------------------------------------
  $view = new view();
  $view->name = 'eol_videos';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eolapi';
  $view->context = 'species_media';
  $view->human_name = 'EOL Videos';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'EOL Videos';
  $handler->display->display_options['css_class'] = 'alpha omega';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'eolapi';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Encyclopaedia of Life data: Encyclopaedia of life data ID */
  $handler->display->display_options['fields']['eid']['id'] = 'eid';
  $handler->display->display_options['fields']['eid']['table'] = 'eolapi';
  $handler->display->display_options['fields']['eid']['field'] = 'eid';
  /* Sort criterion: Encyclopaedia of Life data: Rating */
  $handler->display->display_options['sorts']['rating']['id'] = 'rating';
  $handler->display->display_options['sorts']['rating']['table'] = 'eolapi';
  $handler->display->display_options['sorts']['rating']['field'] = 'rating';
  $handler->display->display_options['sorts']['rating']['order'] = 'DESC';
  /* Contextual filter: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['not'] = 0;
  /* Filter criterion: Encyclopaedia of Life data: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eolapi';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'movingimage' => 'movingimage'
  );
  /* Filter criterion: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['operator'] = 'not empty';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['vocabulary'] = 'tags';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['hierarchy'] = 1;
  /* Filter criterion: Encyclopaedia of Life data: Source */
  $handler->display->display_options['filters']['source']['id'] = 'source';
  $handler->display->display_options['filters']['source']['table'] = 'eolapi';
  $handler->display->display_options['filters']['source']['field'] = 'source';
  $handler->display->display_options['filters']['source']['value'] = 'EOL';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  // -- EOL Images -------------------------------------------------------------------------------------------------
  $view = new view();
  $view->name = 'eol_text';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eolapi';
  $view->human_name = 'EOL Text';
  $view->context = 'species_descriptions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'EOL Text';
  $handler->display->display_options['css_class'] = 'alpha omega';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'eolapi';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Encyclopaedia of Life data: Encyclopaedia of life data ID */
  $handler->display->display_options['fields']['eid']['id'] = 'eid';
  $handler->display->display_options['fields']['eid']['table'] = 'eolapi';
  $handler->display->display_options['fields']['eid']['field'] = 'eid';
  /* Sort criterion: Encyclopaedia of Life data: Rating */
  $handler->display->display_options['sorts']['rating']['id'] = 'rating';
  $handler->display->display_options['sorts']['rating']['table'] = 'eolapi';
  $handler->display->display_options['sorts']['rating']['field'] = 'rating';
  $handler->display->display_options['sorts']['rating']['order'] = 'DESC';
  /* Contextual filter: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['eolapi_taxonomy_tid']['not'] = 0;
  /* Filter criterion: Encyclopaedia of Life data: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eolapi';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'text' => 'text'
  );
  /* Filter criterion: Encyclopaedia of Life data: Taxonomy field (eolapi_taxonomy) */
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['id'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['table'] = 'field_data_eolapi_taxonomy';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['field'] = 'eolapi_taxonomy_tid';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['operator'] = 'not empty';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['type'] = 'select';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['vocabulary'] = 'tags';
  $handler->display->display_options['filters']['eolapi_taxonomy_tid']['hierarchy'] = 1;
  /* Filter criterion: Encyclopaedia of Life data: Source */
  $handler->display->display_options['filters']['source']['id'] = 'source';
  $handler->display->display_options['filters']['source']['table'] = 'eolapi';
  $handler->display->display_options['filters']['source']['field'] = 'source';
  $handler->display->display_options['filters']['source']['value'] = 'EOL';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  return $views;
}
