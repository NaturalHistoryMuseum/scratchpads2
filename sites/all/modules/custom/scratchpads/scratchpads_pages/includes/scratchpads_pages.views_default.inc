<?php

function scratchpads_pages_views_default_views(){
  $views = array();
  $display_settings = scratchpads_pages_variable_get('scratchpads_pages_display');
  foreach(node_type_get_types() as $type => $node_info){
    if(!isset($display_settings[$type]) || $display_settings[$type] == 'view'){
      $view = new view();
      $view->name = 'scratchpads_' . $type . '_page';
      $view->description = '';
      $view->tag = 'scratchpads-page';
      $view->base_table = 'node';
      $view->human_name = 'Scratchpads ' . $node_info->name . ' page';
      $view->core = 7;
      $view->api_version = '3.0';
      $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
      /* Display: Master */
      $handler = $view->new_display('default', 'Master', 'default');
      $handler->display->display_options['title'] = $node_info->name . 's';
      $handler->display->display_options['access']['type'] = 'perm';
      $handler->display->display_options['cache']['type'] = 'none';
      $handler->display->display_options['query']['type'] = 'views_query';
      $handler->display->display_options['query']['options']['query_comment'] = FALSE;
      $handler->display->display_options['exposed_form']['type'] = 'basic';
      $handler->display->display_options['pager']['type'] = 'full';
      $handler->display->display_options['pager']['options']['items_per_page'] = '10';
      $handler->display->display_options['style_plugin'] = 'list';
      $handler->display->display_options['row_plugin'] = 'node';
      $handler->display->display_options['row_options']['links'] = FALSE;
      /* No results behavior: Global: Text area */
      $handler->display->display_options['empty']['area']['id'] = 'area';
      $handler->display->display_options['empty']['area']['table'] = 'views';
      $handler->display->display_options['empty']['area']['field'] = 'area';
      $handler->display->display_options['empty']['area']['label'] = 'Empty text';
      $handler->display->display_options['empty']['area']['empty'] = FALSE;
      $handler->display->display_options['empty']['area']['content'] = 'Sorry there is not yet any content of this type.';
      $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
      $handler->display->display_options['empty']['area']['tokenize'] = 0;
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
        $type => $type
      );
      /* Display: Page */
      $handler = $view->new_display('page', 'Page', 'page');
      $handler->display->display_options['path'] = $type;
      $handler->display->display_options['menu']['type'] = 'normal';
      $handler->display->display_options['menu']['title'] = $node_info->name . 's';
      $handler->display->display_options['menu']['name'] = PRIMARY_MENU_NAME;
      $handler->display->display_options['menu']['weight'] = 1;
      $views[$view->name] = $view;
    }
  }
  return $views;
}