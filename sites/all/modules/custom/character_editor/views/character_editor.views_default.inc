<?php

function character_editor_views_default_views(){
  $views = array();
  /*
   * The character editor view
   */
  $view = new view();
  $view->name = 'character_editor';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Character editor';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Character editor';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '';
  $handler->display->display_options['style_plugin'] = 'character_editor';
  $handler->display->display_options['style_options']['columns'] = array(
    'character_entity_field' => array(
      'tab' => '',
      'width' => '100',
      'filter' => 'characterTaxonFilter',
      'formatter' => 'characterCollapsibleFormatter'
    )
  );
  $handler->display->display_options['style_options']['tabs'] = '';
  $handler->display->display_options['style_options']['default_active_tab'] = '';
  $handler->display->display_options['style_options']['enableColumnResize'] = 1;
  $handler->display->display_options['style_options']['enableColumnReorder'] = 0;
  $handler->display->display_options['style_options']['select_columns'] = 0;
  $handler->display->display_options['style_options']['asyncEditorLoading'] = 1;
  $handler->display->display_options['style_options']['forceFitColumns'] = 0;
  $handler->display->display_options['style_options']['headerHeight'] = '42';
  $handler->display->display_options['style_options']['viewport_height'] = '500';
  $handler->display->display_options['style_options']['height_expand'] = 1;
  $handler->display->display_options['style_options']['rowHeight'] = '30';
  $handler->display->display_options['style_options']['add'] = array(
    'character_editor_character/controlled' => 'character_editor_character/controlled',
    'character_editor_character/dna' => 'character_editor_character/dna',
    'character_editor_character/computed' => 'character_editor_character/computed',
    'character_editor_character/numeric' => 'character_editor_character/numeric',
    'character_editor_character/text' => 'character_editor_character/text',
    'character_editor_character/group' => 'character_editor_character/group'
  );
  $handler->display->display_options['style_options']['autoEdit'] = 1;
  /* Relationship: Taxonomy term: Character project using Taxonomic name */
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['id'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['field'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['required'] = TRUE;
  /* Field: Character Editor: Taxon and Specimen */
  $handler->display->display_options['fields']['character_entity_field']['id'] = 'character_entity_field';
  $handler->display->display_options['fields']['character_entity_field']['table'] = 'character_editor';
  $handler->display->display_options['fields']['character_entity_field']['field'] = 'character_entity_field';
  $handler->display->display_options['fields']['character_entity_field']['element_label_colon'] = FALSE;
  /* Contextual filter: Character project: Character project ID */
  $handler->display->display_options['arguments']['id']['id'] = 'id';
  $handler->display->display_options['arguments']['id']['table'] = 'character_editor_project';
  $handler->display->display_options['arguments']['id']['field'] = 'id';
  $handler->display->display_options['arguments']['id']['relationship'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['arguments']['id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['id']['summary_options']['items_per_page'] = '25';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'character-editor';
  /* Display: XML */
  $handler = $view->new_display('views_data_export', 'XML', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view%timestamp-full.xml';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['path'] = 'character-editor/export/xml';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0
  );
  /* Display: TXT */
  $handler = $view->new_display('views_data_export', 'TXT', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_txt';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view%timestamp-full.txt';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'character-editor/export/txt';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 0
  );
  $views['character_editor'] = $view;
  /*
   * The specimen selection view (used from "insert from view")
   */
  $view = new view();
  $view->name = 'specimen_node_ref_selection';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'View used to select specimens in node reference fields';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
      $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_taxonomic_name' => 'field_taxonomic_name'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_taxonomic_name' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Field: Insert from view: Insert from view checkbox */
  $handler->display->display_options['fields']['insert_from_view_checkbox']['id'] = 'insert_from_view_checkbox';
  $handler->display->display_options['fields']['insert_from_view_checkbox']['table'] = 'insert_from_view';
  $handler->display->display_options['fields']['insert_from_view_checkbox']['field'] = 'insert_from_view_checkbox';
  $handler->display->display_options['fields']['insert_from_view_checkbox']['label'] = '';
  $handler->display->display_options['fields']['insert_from_view_checkbox']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Specimen';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Content: Type status */
  $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
  /* Field: Content: Basis of record */
  $handler->display->display_options['fields']['field_basis_of_record']['id'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['table'] = 'field_data_field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['field'] = 'field_basis_of_record';
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['type'] = 'node_reference_plain';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nid]:[title]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
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
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Specimen';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['value'] = '';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator_id'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['label'] = 'Taxonomic name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['operator'] = 'field_taxonomic_name_tid_op';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['identifier'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['vocabulary'] = 'pediculus';
  /* Filter criterion: Content: Type status (field_type_status) */
  $handler->display->display_options['filters']['field_type_status_value']['id'] = 'field_type_status_value';
  $handler->display->display_options['filters']['field_type_status_value']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['filters']['field_type_status_value']['field'] = 'field_type_status_value';
  $handler->display->display_options['filters']['field_type_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_type_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_type_status_value']['expose']['operator_id'] = 'field_type_status_value_op';
  $handler->display->display_options['filters']['field_type_status_value']['expose']['label'] = 'Type status';
  $handler->display->display_options['filters']['field_type_status_value']['expose']['operator'] = 'field_type_status_value_op';
  $handler->display->display_options['filters']['field_type_status_value']['expose']['identifier'] = 'field_type_status_value';
  $handler->display->display_options['filters']['field_type_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: Content: Basis of record (field_basis_of_record) */
  $handler->display->display_options['filters']['field_basis_of_record_value']['id'] = 'field_basis_of_record_value';
  $handler->display->display_options['filters']['field_basis_of_record_value']['table'] = 'field_data_field_basis_of_record';
  $handler->display->display_options['filters']['field_basis_of_record_value']['field'] = 'field_basis_of_record_value';
  $handler->display->display_options['filters']['field_basis_of_record_value']['group'] = 1;
  $handler->display->display_options['filters']['field_basis_of_record_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_basis_of_record_value']['expose']['operator_id'] = 'field_basis_of_record_value_op';
  $handler->display->display_options['filters']['field_basis_of_record_value']['expose']['label'] = 'Basis of record';
  $handler->display->display_options['filters']['field_basis_of_record_value']['expose']['operator'] = 'field_basis_of_record_value_op';
  $handler->display->display_options['filters']['field_basis_of_record_value']['expose']['identifier'] = 'field_basis_of_record_value';
  $handler->display->display_options['filters']['field_basis_of_record_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $views['specimen_node_ref_selection'] = $view;
  return $views;
}