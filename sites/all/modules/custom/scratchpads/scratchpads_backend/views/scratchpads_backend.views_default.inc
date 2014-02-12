<?php
// FIXME - The following code may help add a thumbnail to the slickgrid view
// #3103.
// /* Field: File: Rendered */
// $handler->display->display_options['fields']['rendered']['id'] = 'rendered';
// $handler->display->display_options['fields']['rendered']['table'] = 'file_managed';
// $handler->display->display_options['fields']['rendered']['field'] = 'rendered';
// $handler->display->display_options['fields']['rendered']['file_view_mode'] = 'file_styles_thumbnail';
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
    // VBO Admin Node views
    $view = scratchpads_backend_views_get_admin_vbo_view('node', 'node', $node_type->orig_type, $node_type->name, 'admin/content', 'node/' . $node_type->orig_type);
    $views[$view->name] = $view;
  }
  // Build the file slickgrid editor views
  foreach(file_info_file_types() as $bundle => $file_info){
    $view = scratchpads_backend_views_get_view('file_managed', 'file', $bundle, $file_info['label'], 'admin/file/slickgrid');
    $views[$view->name] = $view;
  }
  // VBO  File views
  $view = new view();
  $view->name = 'admin_vbo_file';
  $view->description = '';
  $view->tag = 'admin-vbo';
  $view->base_table = 'file_managed';
  $view->human_name = 'Admin VBO file';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  // START //
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Admin VBO view';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
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
  /* Relationship: File: User who uploaded */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'file_managed';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Contributor';
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
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
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Contributor';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
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
  $handler->display->display_options['arguments']['type']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['type']['title'] = '%1 files';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['type']['limit'] = '0';
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
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['filename']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
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
  /* Filter criterion: File: Path */
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
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
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
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Modify Values',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'image::field_taxonomic_name' => 'image::field_taxonomic_name',
          'image::field_cc_licence' => 'image::field_cc_licence',
          'image::field_imaging_technique' => 'image::field_imaging_technique',
          'image::field_keywords' => 'image::field_keywords',
          'image::field_preparation_technique' => 'image::field_preparation_technique',
          'image::field_description' => 'image::field_description',
          'image::field_sticky' => 'image::field_sticky',
          'image::field_creator' => 'image::field_creator',
          'image::field_weight' => 'image::field_weight',
          'image::field_promote_to_home_page' => 'image::field_promote_to_home_page'
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Image';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Contributor';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
  /* Field: File Usage: Entity type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_usage';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid_2']['id'] = 'fid_2';
  $handler->display->display_options['fields']['fid_2']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_2']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_2']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR'
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $file_fields_array = field_info_instances('file', 'image');
  foreach($file_fields_array as $key => $file_field){
    $field_value = $key . '_value';
    if($file_field['widget']['module'] == 'text'){
      /* Filter criterion: */
      $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
      $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['operator'] = 'contains';
      $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
      $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
    }elseif(($file_field['widget']['module'] == 'options') && isset($file_field['display']['default']['module']) && ($file_field['display']['default']['module'] != 'taxonomy')){
      // We remove certain filters that do not need to be displayed
      if(($key != 'field_sticky') && ($key != 'field_weight') && ($key != 'field_promote_to_home_page')){
        /* Filter criterion: */
        $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
        $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
        $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
        $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
      }
    }elseif($file_field['widget']['module'] == 'creative_commons'){
      $license_array = creative_commons_get_licence_types();
      $group_array = array();
      $c = 1;
      foreach($license_array as $key => $license){
        $group_array[$c] = array(
          'title' => $license,
          'operator' => '=',
          'value' => array(
            'value' => $key,
            'min' => '',
            'max' => ''
          )
        );
        $c++;
      }
      /* Filter criterion: File: Licence (field_cc_licence) */
      $handler->display->display_options['filters']['field_cc_licence_licence']['id'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['table'] = 'field_data_field_cc_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['field'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['exposed'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator_id'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0
      );
      $handler->display->display_options['filters']['field_cc_licence_licence']['is_grouped'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['group_items'] = $group_array;
    }
  }
  /* Filter criterion: File: Imaging technique (field_imaging_technique) */
  $handler->display->display_options['filters']['field_imaging_technique_tid']['id'] = 'field_imaging_technique_tid';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['table'] = 'field_data_field_imaging_technique';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['field'] = 'field_imaging_technique_tid';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_imaging_technique_tid']['expose']['operator_id'] = 'field_imaging_technique_tid_op';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['expose']['label'] = 'Imaging technique';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['expose']['operator'] = 'field_imaging_technique_tid_op';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['expose']['identifier'] = 'field_imaging_technique_tid';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['field_imaging_technique_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_imaging_technique_tid']['vocabulary'] = 'imaging_technique';
  /* Filter criterion: File: Keywords (field_keywords) */
  $handler->display->display_options['filters']['field_keywords_tid']['id'] = 'field_keywords_tid';
  $handler->display->display_options['filters']['field_keywords_tid']['table'] = 'field_data_field_keywords';
  $handler->display->display_options['filters']['field_keywords_tid']['field'] = 'field_keywords_tid';
  $handler->display->display_options['filters']['field_keywords_tid']['value'] = '';
  $handler->display->display_options['filters']['field_keywords_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['operator_id'] = 'field_keywords_tid_op';
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['label'] = 'Keywords';
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['operator'] = 'field_keywords_tid_op';
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['identifier'] = 'field_keywords_tid';
  $handler->display->display_options['filters']['field_keywords_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['field_keywords_tid']['vocabulary'] = 'image_keywords';
  /* Filter criterion: File: Preparation technique (field_preparation_technique) */
  $handler->display->display_options['filters']['field_preparation_technique_tid']['id'] = 'field_preparation_technique_tid';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['table'] = 'field_data_field_preparation_technique';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['field'] = 'field_preparation_technique_tid';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['value'] = '';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['operator_id'] = 'field_preparation_technique_tid_op';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['label'] = 'Preparation technique';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['operator'] = 'field_preparation_technique_tid_op';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['identifier'] = 'field_preparation_technique_tid';
  $handler->display->display_options['filters']['field_preparation_technique_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['field_preparation_technique_tid']['vocabulary'] = 'preparation_technique';
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['filename']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
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
  /* Filter criterion: File: File ID */
  $handler->display->display_options['filters']['fid']['id'] = 'fid';
  $handler->display->display_options['filters']['fid']['table'] = 'file_managed';
  $handler->display->display_options['filters']['fid']['field'] = 'fid';
  $handler->display->display_options['filters']['fid']['group_type'] = 'count_distinct';
  $handler->display->display_options['filters']['fid']['operator'] = '>=';
  $handler->display->display_options['filters']['fid']['value']['value'] = '0';
  // Sorts handers by exposed filter label, if one exists
  $handler->display->display_options['filters'] = _scratchpads_backend_views_sort_by_label($handler->display->display_options['filters']);
  $handler->display->display_options['path'] = 'admin/file/image';
  /* Display: Audio */
  $handler = $view->new_display('page', 'Audio', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Audio files';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
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
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Modify Values',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          '_all_' => '_all_',
          'audio::field_taxonomic_name' => 'audio::field_taxonomic_name',
          'audio::field_cc_licence' => 'audio::field_cc_licence',
          'audio::field_description' => 'audio::field_description'
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Image';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Contributor';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
  /* Field: File Usage: Entity type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_usage';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid_2']['id'] = 'fid_2';
  $handler->display->display_options['fields']['fid_2']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_2']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_2']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR'
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'uid';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $file_fields_array = field_info_instances('file', 'audio');
  foreach($file_fields_array as $key => $file_field){
    $field_value = $key . '_value';
    if($file_field['widget']['module'] == 'text'){
      /* Filter criterion: */
      $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
      $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['operator'] = 'contains';
      $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
      $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
    }elseif($file_field['widget']['module'] == 'options'){
      // We remove certain filters that do not need to be displayed
      if(($key != 'field_sticky') && ($key != 'field_weight') && ($key != 'field_promote_to_home_page')){
        /* Filter criterion: */
        $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
        $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
        $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
        $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
      }
    }elseif($file_field['widget']['module'] == 'creative_commons'){
      $license_array = creative_commons_get_licence_types();
      $group_array = array();
      $c = 1;
      foreach($license_array as $key => $license){
        $group_array[$c] = array(
          'title' => $license,
          'operator' => '=',
          'value' => array(
            'value' => $key,
            'min' => '',
            'max' => ''
          )
        );
        $c++;
      }
      /* Filter criterion: File: Licence (field_cc_licence) */
      $handler->display->display_options['filters']['field_cc_licence_licence']['id'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['table'] = 'field_data_field_cc_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['field'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['exposed'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator_id'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0
      );
      $handler->display->display_options['filters']['field_cc_licence_licence']['is_grouped'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['group_items'] = $group_array;
    }
  }
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['filename']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'audio' => 'audio'
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
  /* Filter criterion: File: File ID */
  $handler->display->display_options['filters']['fid']['id'] = 'fid';
  $handler->display->display_options['filters']['fid']['table'] = 'file_managed';
  $handler->display->display_options['filters']['fid']['field'] = 'fid';
  $handler->display->display_options['filters']['fid']['group_type'] = 'count_distinct';
  $handler->display->display_options['filters']['fid']['operator'] = '>=';
  $handler->display->display_options['filters']['fid']['value']['value'] = '0';
  // Sorts handers by exposed filter label, if one exists
  $handler->display->display_options['filters'] = _scratchpads_backend_views_sort_by_label($handler->display->display_options['filters']);
  $handler->display->display_options['path'] = 'admin/file/audio';
  /* Display: Video */
  $handler = $view->new_display('page', 'Video', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Video files';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
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
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Modify Values',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'video::field_taxonomic_name' => 'video::field_taxonomic_name',
          'video::field_cc_licence' => 'video::field_cc_licence',
          'video::field_description' => 'video::field_description'
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => ''
    )
  );
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Image';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = 'Title';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Contributor';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Uploaded';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Edit */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['fid']['label'] = 'Operations';
  $handler->display->display_options['fields']['fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['fid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid']['alter']['path'] = 'file/[fid]/edit';
  /* Field: Delete */
  $handler->display->display_options['fields']['fid_1']['id'] = 'fid_1';
  $handler->display->display_options['fields']['fid_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_1']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_1']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['label'] = '';
  $handler->display->display_options['fields']['fid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['fid_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fid_1']['alter']['path'] = 'file/[fid_1]/delete';
  $handler->display->display_options['fields']['fid_1']['element_label_colon'] = FALSE;
  /* Field: File Usage: Entity type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_usage';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid_2']['id'] = 'fid_2';
  $handler->display->display_options['fields']['fid_2']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid_2']['field'] = 'fid';
  $handler->display->display_options['fields']['fid_2']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR'
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $file_fields_array = field_info_instances('file', 'video');
  foreach($file_fields_array as $key => $file_field){
    $field_value = $key . '_value';
    if($file_field['widget']['module'] == 'text'){
      /* Filter criterion: */
      $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
      $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
      $handler->display->display_options['filters'][$field_value]['operator'] = 'contains';
      $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
      $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
      $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
    }elseif($file_field['widget']['module'] == 'options'){
      // We remove certain filters that do not need to be displayed
      if(($key != 'field_sticky') && ($key != 'field_weight') && ($key != 'field_promote_to_home_page')){
        /* Filter criterion: */
        $handler->display->display_options['filters'][$field_value]['id'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['table'] = 'field_data_' . $key;
        $handler->display->display_options['filters'][$field_value]['field'] = $field_value;
        $handler->display->display_options['filters'][$field_value]['exposed'] = TRUE;
        $handler->display->display_options['filters'][$field_value]['expose']['operator_id'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['label'] = $file_field['label'];
        $handler->display->display_options['filters'][$field_value]['expose']['operator'] = $field_value . '_op';
        $handler->display->display_options['filters'][$field_value]['expose']['identifier'] = $field_value;
      }
    }elseif($file_field['widget']['module'] == 'creative_commons'){
      $license_array = creative_commons_get_licence_types();
      $group_array = array();
      $c = 1;
      foreach($license_array as $key => $license){
        $group_array[$c] = array(
          'title' => $license,
          'operator' => '=',
          'value' => array(
            'value' => $key,
            'min' => '',
            'max' => ''
          )
        );
        $c++;
      }
      /* Filter criterion: File: Licence (field_cc_licence) */
      $handler->display->display_options['filters']['field_cc_licence_licence']['id'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['table'] = 'field_data_field_cc_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['field'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['exposed'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator_id'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['operator'] = 'field_cc_licence_licence_op';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0
      );
      $handler->display->display_options['filters']['field_cc_licence_licence']['is_grouped'] = TRUE;
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['label'] = 'Licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['identifier'] = 'field_cc_licence_licence';
      $handler->display->display_options['filters']['field_cc_licence_licence']['group_info']['group_items'] = $group_array;
    }
  }
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['filename']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: File: Upload date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['group'] = 1;
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Upload date';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'video' => 'video'
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
  /* Filter criterion: File: File ID */
  $handler->display->display_options['filters']['fid']['id'] = 'fid';
  $handler->display->display_options['filters']['fid']['table'] = 'file_managed';
  $handler->display->display_options['filters']['fid']['field'] = 'fid';
  $handler->display->display_options['filters']['fid']['group_type'] = 'count_distinct';
  $handler->display->display_options['filters']['fid']['operator'] = '>=';
  $handler->display->display_options['filters']['fid']['value']['value'] = '0';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  // Sorts handers by exposed filter label, if one exists
  $handler->display->display_options['filters'] = _scratchpads_backend_views_sort_by_label($handler->display->display_options['filters']);
  $handler->display->display_options['path'] = 'admin/file/video';
  /// END  ///
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
  $view->disabled = FALSE;
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
      scratchpads_backend_views_add_field($handler, $entity_type, $entity_info['base table'], $entity_info['entity keys']['label'], FALSE, $default_group);
    }
  }
  // If groups, display this content type in a tab
  if($groups){
    usort($groups, 'scratchpads_backend_usort_comparison');
    foreach($groups as $group){
      // Does this group have fields
      if(!(isset($group->children[0]) && !is_string($group->children[0]))){
        if(count(array_intersect($group->children, array_keys($fields)))){
          $handler->display->display_options['style_options']['tabs'] .= $group->label . PHP_EOL;
          foreach($group->children as $field){
            if(array_key_exists($field, $fields)){
              $table = 'field_data_' . $field;
              $module = $fields[$field]['widget']['module'];
              scratchpads_backend_views_add_field($handler, $module, $table, $field, $fields[$field], $group->label);
              unset($fields[$field]);
            }
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
      scratchpads_backend_views_add_field($handler, $module, $table, $field, $fields[$field], $default_group, $fields[$field]);
    }
  }
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = $base_table;
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    $bundle => $bundle
  );
  /* Filter criterion: File: Path */
  if($entity_type == 'file'){
    $handler->display->display_options['filters']['uri']['id'] = 'uri';
    $handler->display->display_options['filters']['uri']['table'] = 'file_managed';
    $handler->display->display_options['filters']['uri']['field'] = 'uri';
    $handler->display->display_options['filters']['uri']['operator'] = 'not';
    $handler->display->display_options['filters']['uri']['value'] = '/eolapi/';
    // Sort by most recent at the top.
    $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['table'] = 'file_managed';
    $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  }else{
    // Sort by most recent at the top.
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'node';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  }
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = $base_path . '/' . $bundle;
  return $view;
}

/**
 * 
 * @param unknown_type $base_table
 * @param unknown_type $entity_type
 * @param unknown_type $bundle
 * @param unknown_type $label
 * @param unknown_type $base_path
 * @param unknown_type $add_link
 * @return view
 */
function scratchpads_backend_views_get_admin_vbo_view($base_table, $entity_type, $bundle, $label, $base_path, $add_link = null){
  $view = new view();
  $view->name = 'admin_vbo_node_' . $bundle;
  $view->description = '';
  $view->tag = 'admin-content-vbo';
  $view->base_table = $base_table;
  $view->human_name = "Admin VBO node - " . $bundle;
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = $label;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
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
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Relationship: Content: Identified by (field_identified_by) */
  $handler->display->display_options['relationships']['field_identified_by_uid']['id'] = 'field_identified_by_uid';
  $handler->display->display_options['relationships']['field_identified_by_uid']['table'] = 'field_data_field_identified_by';
  $handler->display->display_options['relationships']['field_identified_by_uid']['field'] = 'field_identified_by_uid';
  $handler->display->display_options['relationships']['field_identified_by_uid']['delta'] = '-1';
  /* Relationship: Content: Country (field_country) */
  $handler->display->display_options['relationships']['field_country_iso2']['id'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['table'] = 'field_data_field_country';
  $handler->display->display_options['relationships']['field_country_iso2']['field'] = 'field_country_iso2';
  /* Relationship: Content: Country (field_country) */
  $handler->display->display_options['relationships']['field_country_iso2_1']['id'] = 'field_country_iso2_1';
  $handler->display->display_options['relationships']['field_country_iso2_1']['table'] = 'field_data_field_country';
  $handler->display->display_options['relationships']['field_country_iso2_1']['field'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2_1']['relationship'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_country_iso2_1']['label'] = 'Country from Location';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
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
      ),
      'postpone_processing' => 0
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
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_unpromote_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_save_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0
    )
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Contributor';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['alter']['text'] = '[edit_node]&nbsp;[delete_node]';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  // Get  the available fields for this content type 
  $fields_array = field_info_instances($entity_type, $bundle);
  // sort the array to bring taxa to the top  
  $new_array = array();
  foreach($fields_array as $key => $field_details){
    if($field_details['widget']['module'] == 'scratchpads_taxonomic_name_field'){
      $new_array[$key] = $field_details;
    }
  }
  foreach($fields_array as $key => $field_details){
    if($field_details['widget']['module'] != 'scratchpads_taxonomic_name_field'){
      $new_array[$key] = $field_details;
    }
  }
  $fields_array = $new_array;
  foreach($fields_array as $key => $field_details){
    $value = $field_details['field_name'] . '_value';
    if(isset($field_details['display']['default']['module']) && ($field_details['display']['default']['module'] == 'node_reference') && ($field_details['field_name'] != 'field_location')){
      // do nothing
    }elseif($field_details['widget']['module'] == 'scratchpads_taxonomic_name_field'){
      // Create a filter for each vocabulary
      $vocab_objects = taxonomy_vocabulary_get_names();
      $count = 0;
      $tid = 'tid';
      $bio_vids = variable_get('biological_vids', array());
      foreach($vocab_objects as $key => $vocab){
        if(array_key_exists($vocab->vid, $bio_vids)){
          // Check to see whether they have any terms
          $query = db_select('taxonomy_term_data', 'ttd')->fields('ttd')->condition('vid', $vocab->vid, '=');
          $num_of_results = $query->countQuery()->execute()->fetchField();
          if($num_of_results > 0){
            if($count > 0){
              $tid = 'tid_' . $count;
            }
            /* Filter criterion: Content: Has taxonomy term */
            $handler->display->display_options['filters'][$tid]['id'] = $tid;
            $handler->display->display_options['filters'][$tid]['table'] = 'taxonomy_index';
            $handler->display->display_options['filters'][$tid]['field'] = 'tid';
            $handler->display->display_options['filters'][$tid]['value'] = '';
            $handler->display->display_options['filters'][$tid]['exposed'] = TRUE;
            $handler->display->display_options['filters'][$tid]['expose']['operator_id'] = $tid . '_op';
            $handler->display->display_options['filters'][$tid]['expose']['label'] = 'Taxonomy | ' . $vocab->name;
            $handler->display->display_options['filters'][$tid]['expose']['operator'] = $tid . '_op';
            $handler->display->display_options['filters'][$tid]['expose']['use_operator'] = TRUE;
            $handler->display->display_options['filters'][$tid]['expose']['identifier'] = $tid;
            $handler->display->display_options['filters'][$tid]['group'] = '1';
            $handler->display->display_options['filters'][$tid]['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
              5 => 0
            );
            $handler->display->display_options['filters'][$tid]['vocabulary'] = $vocab->machine_name;
            $handler->display->display_options['filters'][$tid]['error_message'] = FALSE;
            $count++;
          }
        }
      }
    }elseif($field_details['field_name'] == 'field_identified_by'){
      /* Filter criterion: User: Name */
      $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
      $handler->display->display_options['filters']['uid_1']['table'] = 'users';
      $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
      $handler->display->display_options['filters']['uid_1']['relationship'] = 'field_identified_by_uid';
      $handler->display->display_options['filters']['uid_1']['value'] = '';
      $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
      $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
      $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Identified By';
      $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
      $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
      $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0
      );
    }    // in this special case we add all location fields as exposed filters (with relationship) to the view
    // To do: Automatically add fields/relationships for node references
    elseif($field_details['field_name'] == 'field_location'){
      // loop though all location fields and add the handlers
      $location_fields_array = field_info_instances('node', 'location');
      foreach($location_fields_array as $key => $location_field){
        $value2 = $location_field['field_name'] . '_value';
        // Add text fields
        if($location_field['widget']['module'] == 'text'){
          /* Filter criterion: */
          $handler->display->display_options['filters'][$value2]['id'] = $value2;
          $handler->display->display_options['filters'][$value2]['table'] = 'field_data_' . $location_field['field_name'];
          $handler->display->display_options['filters'][$value2]['field'] = $value2;
          $handler->display->display_options['filters'][$value2]['operator'] = 'contains';
          $handler->display->display_options['filters'][$value2]['exposed'] = TRUE;
          $handler->display->display_options['filters'][$value2]['expose']['operator_id'] = $value2 . '_op';
          $handler->display->display_options['filters'][$value2]['expose']['use_operator'] = TRUE;
          $handler->display->display_options['filters'][$value2]['expose']['label'] = 'Location | ' . $location_field['label'];
          $handler->display->display_options['filters'][$value2]['expose']['operator'] = $value2 . '_op';
          $handler->display->display_options['filters'][$value2]['expose']['identifier'] = $value2;
          $handler->display->display_options['filters'][$value2]['relationship'] = 'field_location_nid';
          // Add option fields
        }elseif($location_field['widget']['module'] == 'options'){
          // We treat countries as a separate case, as it is a unique field type
          if($location_field['field_name'] == 'field_country'){
            /* Filter criterion: Countries: Name - list */
            $handler->display->display_options['filters'][$value2]['id'] = 'name_list';
            $handler->display->display_options['filters'][$value2]['table'] = 'countries_country';
            $handler->display->display_options['filters'][$value2]['field'] = 'name_list';
            $handler->display->display_options['filters'][$value2]['relationship'] = 'field_country_iso2_1';
            $handler->display->display_options['filters'][$value2]['exposed'] = TRUE;
            $handler->display->display_options['filters'][$value2]['expose']['operator_id'] = 'name_list_op';
            $handler->display->display_options['filters'][$value2]['expose']['label'] = 'Location | ' . $location_field['label'];
            $handler->display->display_options['filters'][$value2]['expose']['operator'] = 'name_list_op';
            $handler->display->display_options['filters'][$value2]['expose']['identifier'] = 'name_list';
            $handler->display->display_options['filters'][$value2]['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
              5 => 0
            );
            $handler->display->display_options['filters'][$value2]['configuration'] = '1';
            $handler->display->display_options['filters'][$value2]['filter'] = array(
              'enabled' => '1',
              'continents' => array()
            );
          }else{
            /* Filter criterion: */
            $handler->display->display_options['filters'][$value2]['id'] = $value2;
            $handler->display->display_options['filters'][$value2]['table'] = 'field_data_' . $location_field['field_name'];
            $handler->display->display_options['filters'][$value2]['field'] = $value2;
            $handler->display->display_options['filters'][$value2]['exposed'] = TRUE;
            $handler->display->display_options['filters'][$value2]['expose']['operator_id'] = $value2 . '_op';
            $handler->display->display_options['filters'][$value2]['expose']['label'] = 'Location | ' . $location_field['label'];
            $handler->display->display_options['filters'][$value2]['expose']['operator'] = $value2 . '_op';
            $handler->display->display_options['filters'][$value2]['expose']['identifier'] = $value2;
            $handler->display->display_options['filters'][$value2]['relationship'] = 'field_location_nid';
          }
        }elseif($location_field['widget']['module'] == 'number'){
          $handler->display->display_options['filters'][$value2]['id'] = $value2;
          $handler->display->display_options['filters'][$value2]['table'] = 'field_data_' . $location_field['field_name'];
          $handler->display->display_options['filters'][$value2]['field'] = $value2;
          $handler->display->display_options['filters'][$value2]['relationship'] = 'field_location_nid';
          $handler->display->display_options['filters'][$value2]['exposed'] = TRUE;
          $handler->display->display_options['filters'][$value2]['expose']['operator_id'] = $value2 . '_op';
          $handler->display->display_options['filters'][$value2]['expose']['label'] = 'Location | ' . $location_field['label'];
          $handler->display->display_options['filters'][$value2]['expose']['use_operator'] = TRUE;
          $handler->display->display_options['filters'][$value2]['expose']['operator'] = $value2 . '_op';
          $handler->display->display_options['filters'][$value2]['expose']['identifier'] = $value2;
          $handler->display->display_options['filters'][$value2]['expose']['remember_roles'] = array(
            2 => '2',
            1 => 0,
            3 => 0,
            4 => 0,
            5 => 0
          );
        }
      }
    }elseif($field_details['widget']['module'] == 'text'){
      /* Filter criterion: */
      $handler->display->display_options['filters'][$value]['id'] = $value;
      $handler->display->display_options['filters'][$value]['table'] = 'field_data_' . $field_details['field_name'];
      $handler->display->display_options['filters'][$value]['field'] = $value;
      $handler->display->display_options['filters'][$value]['operator'] = 'contains';
      $handler->display->display_options['filters'][$value]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$value]['expose']['operator_id'] = $value . '_op';
      $handler->display->display_options['filters'][$value]['expose']['label'] = $field_details['label'];
      $handler->display->display_options['filters'][$value]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$value]['expose']['operator'] = $value . '_op';
      $handler->display->display_options['filters'][$value]['expose']['identifier'] = $value;
      $handler->display->display_options['filters'][$value]['group'] = '1';
    }elseif($field_details['widget']['module'] == 'options'){
      // We treat countries as a separate case, as it is a unique field type
      if(isset($field_details['display']['default']['module']) && $field_details['display']['default']['module'] == 'countries'){
        /* Filter criterion: Countries: Name - list */
        $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
        $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
        $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
        $handler->display->display_options['filters']['name_list']['relationship'] = 'field_country_iso2';
        $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
        $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
        $handler->display->display_options['filters']['name_list']['expose']['label'] = 'Country';
        $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
        $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
        $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
          2 => '2',
          1 => 0,
          3 => 0,
          4 => 0,
          5 => 0
        );
        $handler->display->display_options['filters']['name_list']['configuration'] = '1';
        $handler->display->display_options['filters']['name_list']['filter'] = array(
          'enabled' => '1',
          'continents' => array()
        );
      }elseif(isset($field_details['display']['default']['module']) && $field_details['display']['default']['module'] == 'languagefield'){
        // do nothing  
      }else{
        /* Filter criterion: */
        $handler->display->display_options['filters'][$value]['id'] = $value;
        $handler->display->display_options['filters'][$value]['table'] = 'field_data_' . $field_details['field_name'];
        $handler->display->display_options['filters'][$value]['field'] = $value;
        $handler->display->display_options['filters'][$value]['exposed'] = TRUE;
        $handler->display->display_options['filters'][$value]['expose']['operator_id'] = $value . '_op';
        $handler->display->display_options['filters'][$value]['expose']['label'] = $field_details['label'];
        $handler->display->display_options['filters'][$value]['expose']['operator'] = $value . '_op';
        $handler->display->display_options['filters'][$value]['expose']['identifier'] = $value;
        $handler->display->display_options['filters'][$value]['group'] = '1';
      }
    }elseif($field_details['widget']['module'] == 'number'){
      $handler->display->display_options['filters'][$value]['id'] = $value;
      $handler->display->display_options['filters'][$value]['table'] = 'field_data_' . $field_details['field_name'];
      $handler->display->display_options['filters'][$value]['field'] = $value;
      $handler->display->display_options['filters'][$value]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$value]['expose']['operator_id'] = $value . '_op';
      $handler->display->display_options['filters'][$value]['expose']['label'] = $field_details['label'];
      $handler->display->display_options['filters'][$value]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$value]['expose']['operator'] = $value . '_op';
      $handler->display->display_options['filters'][$value]['expose']['identifier'] = $value;
      $handler->display->display_options['filters'][$value]['group'] = '1';
      $handler->display->display_options['filters'][$value]['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0
      );
    }
  }
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  if($bundle == 'biblio'){
    /* Filter criterion: Biblio: Type of Publication */
    $handler->display->display_options['filters']['tid_5']['id'] = 'tid_5';
    $handler->display->display_options['filters']['tid_5']['table'] = 'biblio_types';
    $handler->display->display_options['filters']['tid_5']['field'] = 'tid';
    $handler->display->display_options['filters']['tid_5']['exposed'] = TRUE;
    $handler->display->display_options['filters']['tid_5']['expose']['operator_id'] = 'tid_5_op';
    $handler->display->display_options['filters']['tid_5']['expose']['label'] = 'Type of Publication';
    $handler->display->display_options['filters']['tid_5']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['tid_5']['expose']['operator'] = 'tid_5_op';
    $handler->display->display_options['filters']['tid_5']['expose']['identifier'] = 'tid_5';
    $handler->display->display_options['filters']['tid_5']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['tid_5']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
    /* Filter criterion: Biblio: Year of Publication */
    $handler->display->display_options['filters']['biblio_year']['id'] = 'biblio_year';
    $handler->display->display_options['filters']['biblio_year']['table'] = 'biblio';
    $handler->display->display_options['filters']['biblio_year']['field'] = 'biblio_year';
    $handler->display->display_options['filters']['biblio_year']['exposed'] = TRUE;
    $handler->display->display_options['filters']['biblio_year']['expose']['operator_id'] = 'biblio_year_op';
    $handler->display->display_options['filters']['biblio_year']['expose']['label'] = 'Year of Publication';
    $handler->display->display_options['filters']['biblio_year']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['biblio_year']['expose']['operator'] = 'biblio_year_op';
    $handler->display->display_options['filters']['biblio_year']['expose']['identifier'] = 'biblio_year';
    $handler->display->display_options['filters']['biblio_year']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
    /* Filter criterion: Biblio: Author last name */
    $handler->display->display_options['filters']['lastname']['id'] = 'lastname';
    $handler->display->display_options['filters']['lastname']['table'] = 'biblio_contributor_data';
    $handler->display->display_options['filters']['lastname']['field'] = 'lastname';
    $handler->display->display_options['filters']['lastname']['exposed'] = TRUE;
    $handler->display->display_options['filters']['lastname']['expose']['operator_id'] = 'lastname_op';
    $handler->display->display_options['filters']['lastname']['expose']['label'] = 'Author';
    $handler->display->display_options['filters']['lastname']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['lastname']['expose']['operator'] = 'lastname_op';
    $handler->display->display_options['filters']['lastname']['expose']['identifier'] = 'lastname';
    $handler->display->display_options['filters']['lastname']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['lastname']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
    /* Filter criterion: Biblio: Secondary Title */
    $handler->display->display_options['filters']['biblio_secondary_title']['id'] = 'biblio_secondary_title';
    $handler->display->display_options['filters']['biblio_secondary_title']['table'] = 'biblio';
    $handler->display->display_options['filters']['biblio_secondary_title']['field'] = 'biblio_secondary_title';
    $handler->display->display_options['filters']['biblio_secondary_title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['operator_id'] = 'biblio_secondary_title_op';
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['label'] = 'Journal';
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['operator'] = 'biblio_secondary_title_op';
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['identifier'] = 'biblio_secondary_title';
    $handler->display->display_options['filters']['biblio_secondary_title']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
    /* Filter criterion: Biblio: Keyword */
    $handler->display->display_options['filters']['word']['id'] = 'word';
    $handler->display->display_options['filters']['word']['table'] = 'biblio_keyword_data';
    $handler->display->display_options['filters']['word']['field'] = 'word';
    $handler->display->display_options['filters']['word']['operator'] = 'word';
    $handler->display->display_options['filters']['word']['exposed'] = TRUE;
    $handler->display->display_options['filters']['word']['expose']['operator_id'] = 'word_op';
    $handler->display->display_options['filters']['word']['expose']['label'] = 'Keyword';
    $handler->display->display_options['filters']['word']['expose']['operator'] = 'word_op';
    $handler->display->display_options['filters']['word']['expose']['identifier'] = 'word';
    $handler->display->display_options['filters']['word']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
    /* Filter criterion: Biblio: Abstract */
    $handler->display->display_options['filters']['biblio_abst_e']['id'] = 'biblio_abst_e';
    $handler->display->display_options['filters']['biblio_abst_e']['table'] = 'biblio';
    $handler->display->display_options['filters']['biblio_abst_e']['field'] = 'biblio_abst_e';
    $handler->display->display_options['filters']['biblio_abst_e']['operator'] = 'contains';
    $handler->display->display_options['filters']['biblio_abst_e']['exposed'] = TRUE;
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['operator_id'] = 'biblio_abst_e_op';
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['label'] = 'Abstract';
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['operator'] = 'biblio_abst_e_op';
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['identifier'] = 'biblio_abst_e';
    $handler->display->display_options['filters']['biblio_abst_e']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0
    );
  }
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = '1';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['group'] = '1';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
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

/**
 * 
 * 
 * @param unknown_type $handler
 * @param unknown_type $module
 * @param unknown_type $table
 * @param unknown_type $field_id
 * @param unknown_type $tab
 * @param unknown_type $width
 * @param unknown_type $sortable
 * @param unknown_type $filter
 * @param unknown_type $field An instance of the field.
 * @return boolean
 */
function scratchpads_backend_views_add_field(&$handler, $module, $table, $field_id, $field = FALSE, $tab = null, $field = FALSE){
  // Ignore all GM3 fields
  if($module == 'gm3_field'){return false;}
  $handler->display->display_options['fields'][$field_id]['id'] = $field_id;
  $handler->display->display_options['fields'][$field_id]['table'] = $table;
  $handler->display->display_options['fields'][$field_id]['field'] = $field_id;
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
  if($field && @isset($field['widget']['type']) && $field['widget']['type'] == 'options_onoff'){
    $handler->display->display_options['fields'][$field_id]['type'] = 'list_key';
  }
  $handler->display->display_options['fields'][$field_id]['field_api_classes'] = 0;
  $handler->display->display_options['style_options']['columns'][$field_id] = array(
    'width' => 120,
    'sortable' => 1,
    'filter' => ($field && $field['widget']['module'] == 'media_multiselect' ? FALSE : 'textFilter'),
    'editor' => 'ModalForm'
  );
  if($tab){
    $handler->display->display_options['style_options']['columns'][$field_id]['tab'] = $tab;
  }
}
/*
 * Helper function to sort handers by label
 */
function _scratchpads_backend_views_sort_by_label($array_to_sort){
  $label_keyed = array();
  $other = array();
  foreach($array_to_sort as $key => $filter){
    if(isset($filter['expose'])){
      $label_keyed[$filter['expose']['label']] = array(
        $key => $filter
      );
    }else{
      $other[$key] = $filter;
    }
  }
  ksort($label_keyed);
  $final = array();
  foreach($label_keyed as $key => $element){
    $array_values = array_values($element);
    $final[key($element)] = array_shift($array_values);
  }
  return array_merge($final, $other);
}


