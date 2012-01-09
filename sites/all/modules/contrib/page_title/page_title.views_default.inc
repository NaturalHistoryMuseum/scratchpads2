<?php


/**
 * @file
 * Default Page Title Views
 */


/**
 * Implementation hook_views_default_views().
 */
function page_title_views_default_views() {
  // This lets us locally cache the result - in case the function is called twice
  static $views;
  if (isset($views)) {
    return $views;
  }

  // No views cached, generate the default views array and return
  $views = array();
  $views += _page_title_views_default_views_list_node_page_titles();
  return $views;
}


/**
 * Local "internal" function as a wrapper aroudn the list_page_titles view
 */
function _page_title_views_default_views_list_node_page_titles() {
  $view = new view;
  $view->name = 'list_node_page_titles';
  $view->description = 'List all Nodes with their Page Titles';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'List Node Page Titles';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE;

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Page Title List';
  $handler->display->display_options['use_ajax'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer page titles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '5, 10, 25, 50';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'nid';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'page_title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Header: Header Text */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['ui_name'] = 'Header Text';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = 'Listed below are all the nodes on the site with their Page Title (if set).';
  /* Empty text: Empty Text */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['ui_name'] = 'Empty Text';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There is no content to list.';
  /* Field: Node: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Page Title: Title */
  $handler->display->display_options['fields']['page_title']['id'] = 'page_title';
  $handler->display->display_options['fields']['page_title']['table'] = 'page_title';
  $handler->display->display_options['fields']['page_title']['field'] = 'page_title';
  $handler->display->display_options['fields']['page_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['page_title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['page_title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['page_title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['page_title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['page_title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['page_title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['page_title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['page_title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['page_title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['page_title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['page_title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['page_title']['link_to_node'] = 0;
  $handler->display->display_options['fields']['page_title']['use_node_title'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  /* Filter: Node: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Node ID (NID) is:';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['single'] = TRUE;
  /* Filter: Node: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title Contains:';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['single'] = TRUE;
  /* Filter: Page Title: Title */
  $handler->display->display_options['filters']['page_title']['id'] = 'page_title';
  $handler->display->display_options['filters']['page_title']['table'] = 'page_title';
  $handler->display->display_options['filters']['page_title']['field'] = 'page_title';
  $handler->display->display_options['filters']['page_title']['operator'] = 'contains';
  $handler->display->display_options['filters']['page_title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['page_title']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['page_title']['expose']['label'] = '"Page Title" Contains:';
  $handler->display->display_options['filters']['page_title']['expose']['identifier'] = 'page_title';
  $handler->display->display_options['filters']['page_title']['expose']['single'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/page-title';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Page Title List';
  $handler->display->display_options['menu']['description'] = 'List all nodes with their Page Titles';
  $handler->display->display_options['menu']['weight'] = '0';
  $translatables['list_page_titles'] = array(
    t('Defaults'),
    t('Page Title List'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Listed below are all the nodes on the site with their Page Title (if set).'),
    t('There is no content to list.'),
    t('Nid'),
    t('Title'),
    t('Edit link'),
    t('Node ID (NID) is:'),
    t('Title Contains:'),
    t('"Page Title" Contains:'),
    t('Page'),
  );

  return array($view->name => $view);
}
