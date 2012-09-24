<?php

function scratchpads_backend_views_default_views(){
  $views = array();
  // Build the node slickgrid editor views
  $node_types = node_type_get_types();
  foreach($node_types as $node_type){
    // Biblio is a pile of crap so don't bother trying to add a grid
    if($node_type->orig_type != 'biblio'){
      $view = scratchpads_backend_views_get_view('node', 'node', $node_type->orig_type, $node_type->name, 'admin/content/slickgrid', 'node/' . $node_type->orig_type);
      $views[$view->name] = $view;
    }
  }
  // Build the file slickgrid editor views
  foreach(file_info_file_types() as $bundle => $file_info){
    $view = scratchpads_backend_views_get_view('file_managed', 'file', $bundle, $file_info['label'], 'admin/file/slickgrid');
    $views[$view->name] = $view;
  }
  // VBO Node & File views
  $view = new view();
  $view->name = 'admin_vbo_node';
  $view->description = '';
  $view->tag = 'admin-vbo';
  $view->base_table = 'node';
  $view->human_name = 'Admin VBO node';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Admin node';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'view_node' => 'view_node',
    'title_field' => 'title_field',
    'name' => 'name',
    'changed' => 'changed',
    'status' => 'status',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node'
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'view_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'language' => 'language',
          'status' => 'status',
          'promote' => 'promote',
          'sticky' => 'sticky',
          'log' => 'log',
          'revision' => 'revision',
          'comment' => 'comment'
        )
      )
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => ''
      )
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_unpromote_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_save_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Node: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['view_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['view_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['view_node']['hide_alter_empty'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 1;
  /* Field: Node: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['text'] = '[edit_node]&nbsp;[delete_node]';
  $handler->display->display_options['fields']['delete_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_node']['hide_alter_empty'] = 1;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['type']['title_enable'] = 1;
  $handler->display->display_options['arguments']['type']['title'] = '%1';
  $handler->display->display_options['arguments']['type']['breadcrumb_enable'] = 1;
  $handler->display->display_options['arguments']['type']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = '0';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = FALSE;
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['value'] = '';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator_id'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['label'] = 'Taxa';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['use_operator'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['identifier'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['vocabulary'] = 'dogs';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['error_message'] = 0;
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/%';
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'admin_vbo_file';
  $view->description = '';
  $view->tag = 'admin-vbo';
  $view->base_table = 'file_managed';
  $view->human_name = 'Admin VBO file';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Admin VBO view';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'rendered_entity' => 'rendered_entity',
    'filename' => 'filename',
    'filesize' => 'filesize',
    'name' => 'name',
    'timestamp' => 'timestamp',
    'fid' => 'fid',
    'fid_1' => 'fid'
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'filename' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'filesize' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'fid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'fid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: File: User who uploaded */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'file_managed';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'scheme' => 'public',
        'temporary' => 1
      )
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array()
      )
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => ''
      )
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filename']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filename']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filename']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['filename']['link_to_file'] = 1;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['filesize']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['filesize']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filesize']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filesize']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filesize']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filesize']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  $handler->display->display_options['fields']['fid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['fid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['fid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['fid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['fid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['fid']['link_to_file'] = 0;
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['fid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['fid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['fid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['fid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['fid_1']['link_to_file'] = 0;
  /* Sort criterion: File: Upload date */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Contextual filter: File: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'file_managed';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['type']['title_enable'] = 1;
  $handler->display->display_options['arguments']['type']['title'] = '%1 files';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['type']['glossary'] = 0;
  $handler->display->display_options['arguments']['type']['limit'] = '0';
  $handler->display->display_options['arguments']['type']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['type']['break_phrase'] = 0;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR'
  );
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'uid';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['required'] = 0;
  $handler->display->display_options['filters']['name']['expose']['multiple'] = FALSE;
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['multiple'] = FALSE;
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['value'] = '';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator_id'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['label'] = 'Taxa';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['use_operator'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['identifier'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['vocabulary'] = 'gadus';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['error_message'] = 0;
  /* Filter criterion: File Usage: Entity type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_usage';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'empty';
  $handler->display->display_options['filters']['type']['group'] = 2;
  /* Filter criterion: File Usage: Entity type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'file_usage';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['operator'] = '!=';
  $handler->display->display_options['filters']['type_1']['value'] = 'eolapi';
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: File path not like "temporary" */
  $handler->display->display_options['filters']['uri']['id'] = 'uri';
  $handler->display->display_options['filters']['uri']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uri']['field'] = 'uri';
  $handler->display->display_options['filters']['uri']['operator'] = 'not_starts';
  $handler->display->display_options['filters']['uri']['value'] = 'temporary';
  /* Display: Default */
  $handler = $view->new_display('page', 'Default', 'page');
  $handler->display->display_options['path'] = 'admin/file/%';
  /* Display: Images */
  $handler = $view->new_display('page', 'Images', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Image files';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'scheme' => 'public',
        'temporary' => 1
      )
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array()
      )
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => ''
      )
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Image';
  $handler->display->display_options['fields']['rendered_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filename']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filename']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filename']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['filename']['link_to_file'] = 1;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['filesize']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['filesize']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filesize']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filesize']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['filesize']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filesize']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filesize']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  $handler->display->display_options['fields']['fid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['fid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['fid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['fid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['fid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['fid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['fid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['fid']['link_to_file'] = 0;
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['fid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['fid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['fid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['fid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['fid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['fid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['fid_1']['link_to_file'] = 0;
  /* Field: File Usage: Entity type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_usage';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 1;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'uid';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['required'] = 0;
  $handler->display->display_options['filters']['name']['expose']['multiple'] = FALSE;
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['multiple'] = FALSE;
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['value'] = '';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator_id'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['label'] = 'Taxa';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['use_operator'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['identifier'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['vocabulary'] = 'gadus';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['error_message'] = 0;
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image'
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: File Usage: Entity type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'file_usage';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['operator'] = 'empty';
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: File Usage: Entity type */
  $handler->display->display_options['filters']['type_2']['id'] = 'type_2';
  $handler->display->display_options['filters']['type_2']['table'] = 'file_usage';
  $handler->display->display_options['filters']['type_2']['field'] = 'type';
  $handler->display->display_options['filters']['type_2']['operator'] = '!=';
  $handler->display->display_options['filters']['type_2']['value'] = 'eolapi';
  $handler->display->display_options['filters']['type_2']['group'] = 2;
  $handler->display->display_options['path'] = 'admin/file/image';
  $views[$view->name] = $view;
  return $views;
}

function scratchpads_backend_views_get_view($base_table, $entity_type, $bundle, $label, $base_path, $add_link = null){
  $view = new view();
  $view->name = 'slickgrid_admin_' . $bundle;
  $view->description = '';
  $view->tag = 'admin-' . $entity_type . '-slickgrid';
  $view->base_table = $base_table;
  $view->human_name = $label . ' grid editor';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = t('Edit !type', array(
    '!type' => $bundle
  ));
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  // Slickgrid options
  $handler->display->display_options['style_plugin'] = 'slickgrid';
  $handler->display->display_options['style_options']['grouping_field'] = '';
  $handler->display->display_options['style_options']['collapse_groups_by_default'] = 0;
  $handler->display->display_options['style_options']['tabs'] = '';
  $handler->display->display_options['style_options']['default_active_tab'] = '';
  $handler->display->display_options['style_options']['enableColumnResize'] = 1;
  $handler->display->display_options['style_options']['enableColumnReorder'] = 1;
  $handler->display->display_options['style_options']['select_columns'] = 1;
  $handler->display->display_options['style_options']['asyncEditorLoading'] = 1;
  $handler->display->display_options['style_options']['forceFitColumns'] = 1;
  $handler->display->display_options['style_options']['pager'] = 0;
  $handler->display->display_options['style_options']['headerHeight'] = '42';
  $handler->display->display_options['style_options']['viewport_height'] = '500';
  $handler->display->display_options['style_options']['rowHeight'] = '30';
  $handler->display->display_options['style_options']['row_selection_checkbox'] = 1;
  $handler->display->display_options['style_options']['multi_edit'] = 1;
  $handler->display->display_options['style_options']['delete'] = 1;
  $handler->display->display_options['style_options']['clone'] = 1;
  $handler->display->display_options['style_options']['export_selected_rows'] = 1;
  // Undo is only working with nodes.
  // TODO Get undo working with files
  if($entity_type == 'node'){
    $handler->display->display_options['style_options']['undo'] = 1;
  }else{
    $handler->display->display_options['style_options']['undo'] = 0;
  }
  if($add_link){
    $handler->display->display_options['style_options']['add'] = array(
      $add_link => $add_link
    );
  }
  $handler->display->display_options['style_options']['autoEdit'] = 1;
  // Add the fields
  $fields = field_info_instances($entity_type, $bundle);
  $groups = field_group_info_groups($entity_type, $bundle, 'form', true);
  $entity_info = entity_get_info($entity_type);
  $default_group = NULL;
  // Specimen observation has a title field but it is auto-populated so ignore it
  if($bundle != 'specimen_observation'){
    if(isset($entity_info['entity keys']['label'])){
      if($groups){
        $default_group = t('Default');
        $handler->display->display_options['style_options']['tabs'] .= $default_group . PHP_EOL;
      }
      scratchpads_backend_views_add_field($handler, $entity_type, $entity_info['base table'], $entity_info['entity keys']['label'], $entity_info['entity keys']['label'], $default_group);
    }
  }
  // If groups, display this content type in a tab
  if($groups){
    usort($groups, 'scratchpads_backend_usort_comparison');
    foreach($groups as $group){
      // Does this group have fields
      if(count(array_intersect($group->children, array_keys($fields)))){
        $handler->display->display_options['style_options']['tabs'] .= $group->label . PHP_EOL;
        foreach($group->children as $field){
          if(array_key_exists($field, $fields)){
            $table = 'field_data_' . $field;
            $module = $fields[$field]['widget']['module'];
            scratchpads_backend_views_add_field($handler, $module, $table, $field, $fields[$field]['label'], $group->label);
            unset($fields[$field]);
          }
        }
      }
    }
  }
  // Add fields (any group fields will have been added and removed from this array)
  if(count($fields)){
    foreach(array_keys($fields) as $field){
      $table = 'field_data_' . $field;
      $module = $fields[$field]['widget']['module'];
      scratchpads_backend_views_add_field($handler, $module, $table, $field, $fields[$field]['label'], $default_group);
    }
  }
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = $base_table;
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    $bundle => $bundle
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = $base_path . '/' . $bundle;
  return $view;
}

function scratchpads_backend_views_add_field(&$handler, $module, $table, $field_id, $field_label, $tab = null, $width = 120, $sortable = 1, $filter = 'textFilter'){
  // Ignore all GM3 fields
  if($module == 'gm3_field'){return false;}
  $handler->display->display_options['fields'][$field_id]['id'] = $field_id;
  $handler->display->display_options['fields'][$field_id]['table'] = $table;
  $handler->display->display_options['fields'][$field_id]['field'] = $field_id;
  $handler->display->display_options['fields'][$field_id]['label'] = $field_label;
  $handler->display->display_options['fields'][$field_id]['alter']['alter_text'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['make_link'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['absolute'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['external'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['nl2br'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields'][$field_id]['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields'][$field_id]['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['trim'] = 0;
  $handler->display->display_options['fields'][$field_id]['alter']['html'] = 0;
  $handler->display->display_options['fields'][$field_id]['element_type'] = '0';
  $handler->display->display_options['fields'][$field_id]['element_label_type'] = 'strong';
  $handler->display->display_options['fields'][$field_id]['element_label_colon'] = 1;
  $handler->display->display_options['fields'][$field_id]['element_default_classes'] = 0;
  $handler->display->display_options['fields'][$field_id]['hide_empty'] = 0;
  $handler->display->display_options['fields'][$field_id]['empty_zero'] = 0;
  $handler->display->display_options['fields'][$field_id]['hide_alter_empty'] = 1;
  $handler->display->display_options['fields'][$field_id]['field_api_classes'] = 0;
  $handler->display->display_options['style_options']['columns'][$field_id] = array(
    'width' => $width,
    'sortable' => $sortable,
    'filter' => $filter,
    'editor' => 'ModalForm'
  );
  if($tab){
    $handler->display->display_options['style_options']['columns'][$field_id]['tab'] = $tab;
  }
}
