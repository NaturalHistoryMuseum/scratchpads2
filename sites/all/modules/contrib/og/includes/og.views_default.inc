<?php

/**
 * Implement hook_views_default_views().
 */
function og_views_default_views() {
  /*
   * View 'og_list'
   */
  $view = new view;
  $view->name = 'og_list';
  $view->description = 'Show active groups that are nodes';
  $view->tag = 'group';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Groups list';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'align' => '',
      'separator' => '',
    ),
    'created' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Relationship: Group: Node group */
  $handler->display->display_options['relationships']['og_rel']['id'] = 'og_rel';
  $handler->display->display_options['relationships']['og_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['og_rel']['field'] = 'og_rel';
  $handler->display->display_options['relationships']['og_rel']['required'] = 0;
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Group: Group date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'og';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'og_rel';
  $handler->display->display_options['fields']['created']['label'] = 'Since';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  /* Filter: Group: Group's state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'og';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'og_rel';
  $handler->display->display_options['filters']['state']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Group: Group\'s state';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'group-list';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'group-list/feed';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
  );
  $translatables['og_list'] = array(
    t('Defaults'),
    t('Groups list'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('group'),
    t('Title'),
    t('Since'),
    t('Group: Group\'s state'),
    t('Page'),
    t('Feed'),
  );
  $views[$view->name] = $view;

  /*
   * View 'og_members'
   */
 $view = new view;
  $view->name = 'og_members';
  $view->description = 'Newest group members';
  $view->tag = 'group';
  $view->base_table = 'users';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Sort criterion: Fields: group_audience (group_audience) - created */
  $handler->display->display_options['sorts']['group_audience_created']['id'] = 'group_audience_created';
  $handler->display->display_options['sorts']['group_audience_created']['table'] = 'field_data_group_audience';
  $handler->display->display_options['sorts']['group_audience_created']['field'] = 'group_audience_created';
  $handler->display->display_options['sorts']['group_audience_created']['order'] = 'DESC';
  /* Argument: Fields: group_audience (group_audience) - gid */
  $handler->display->display_options['arguments']['group_audience_gid']['id'] = 'group_audience_gid';
  $handler->display->display_options['arguments']['group_audience_gid']['table'] = 'field_data_group_audience';
  $handler->display->display_options['arguments']['group_audience_gid']['field'] = 'group_audience_gid';
  $handler->display->display_options['arguments']['group_audience_gid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['group_audience_gid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['group_audience_gid']['title'] = '%1 members';
  $handler->display->display_options['arguments']['group_audience_gid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['group_audience_gid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['group_audience_gid']['not'] = 0;
  /* Filter: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter: Fields: group_audience (group_audience) - state */
  $handler->display->display_options['filters']['group_audience_state']['id'] = 'group_audience_state';
  $handler->display->display_options['filters']['group_audience_state']['table'] = 'field_data_group_audience';
  $handler->display->display_options['filters']['group_audience_state']['field'] = 'group_audience_state';
  $handler->display->display_options['filters']['group_audience_state']['value'] = array(
    1 => '1',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');

  if (module_exists('views_content')) {
    /* Display: Content pane */
    $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
    $handler->display->display_options['pane_title'] = 'OG members';
    $handler->display->display_options['allow']['use_pager'] = 0;
    $handler->display->display_options['allow']['items_per_page'] = 0;
    $handler->display->display_options['allow']['offset'] = 0;
    $handler->display->display_options['allow']['link_to_view'] = 0;
    $handler->display->display_options['allow']['more_link'] = 0;
    $handler->display->display_options['allow']['path_override'] = 0;
    $handler->display->display_options['allow']['title_override'] = 'title_override';
    $handler->display->display_options['allow']['exposed_form'] = 0;
    $handler->display->display_options['allow']['fields_override'] = 0;
    $handler->display->display_options['argument_input'] = array(
      'group_audience_gid' => array(
        'type' => 'context',
        'context' => 'entity:group.gid',
        'context_optional' => 0,
        'panel' => '0',
        'fixed' => '',
        'label' => 'Fields: group_audience (group_audience) - gid',
      ),
    );
  }
  $translatables['og_members'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('All'),
    t('%1 members'),
    t('Block'),
    t('Content pane'),
    t('OG members'),
    t('View panes'),
  );
  $views[$view->name] = $view;

  /*
   * View 'og_nodes'
   */
  $view = new view;
  $view->name = 'og_nodes';
  $view->description = 'Show all nodes of a group.';
  $view->tag = 'group';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Empty text: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There is no content in this group.';
  /* Sort criterion: Node: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Argument: Fields: group_audience (group_audience) - gid */
  $handler->display->display_options['arguments']['group_audience_gid']['id'] = 'group_audience_gid';
  $handler->display->display_options['arguments']['group_audience_gid']['table'] = 'field_data_group_audience';
  $handler->display->display_options['arguments']['group_audience_gid']['field'] = 'group_audience_gid';
  $handler->display->display_options['arguments']['group_audience_gid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['group_audience_gid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['group_audience_gid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['group_audience_gid']['validate_fail'] = 'empty';
  $handler->display->display_options['arguments']['group_audience_gid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['group_audience_gid']['not'] = 0;
  /* Filter: Node: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  if (module_exists('views_content')) {
    /* Display: Content pane */
    $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
    $handler->display->display_options['pane_title'] = 'OG nodes';
    $handler->display->display_options['allow']['use_pager'] = 0;
    $handler->display->display_options['allow']['items_per_page'] = 0;
    $handler->display->display_options['allow']['offset'] = 0;
    $handler->display->display_options['allow']['link_to_view'] = 0;
    $handler->display->display_options['allow']['more_link'] = 0;
    $handler->display->display_options['allow']['path_override'] = 0;
    $handler->display->display_options['allow']['title_override'] = 'title_override';
    $handler->display->display_options['allow']['exposed_form'] = 0;
    $handler->display->display_options['allow']['fields_override'] = 0;
    $handler->display->display_options['argument_input'] = array(
      'group_audience_gid' => array(
        'type' => 'context',
        'context' => 'entity:group.gid',
        'context_optional' => 0,
        'panel' => '0',
        'fixed' => '',
        'label' => 'Fields: group_audience (group_audience) - gid',
      ),
    );
  }
  $translatables['og_nodes'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('There is no content in this group.'),
    t('All'),
    t('Content pane'),
    t('OG nodes'),
    t('View panes'),
  );

  $views[$view->name] = $view;

  return $views;
}