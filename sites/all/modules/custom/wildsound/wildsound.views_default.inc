<?php

function wildsound_views_default_views(){
  $views = array();
  $view = new view();
  $view->name = 'analyses';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'comment';
  $view->human_name = 'Analyses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Analyses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_taxonomic_name' => 'field_taxonomic_name',
    'field_type' => 'field_type',
    'field_oscilo' => 'field_oscilo',
    'field_spec' => 'field_spec',
    'field_spectro' => 'field_spectro',
    'field_dfreq_plot' => 'field_dfreq_plot',
    'field_aci' => 'field_aci',
    'view_comment' => 'view_comment',
    'title' => 'field_taxonomic_name'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_taxonomic_name' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_oscilo' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_spec' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_spectro' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_dfreq_plot' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_aci' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'view_comment' => array(
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
    )
  );
  /* Relationship: Comment: Content */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Relationship: Comment: Type (field_type) */
  $handler->display->display_options['relationships']['field_type_tid']['id'] = 'field_type_tid';
  $handler->display->display_options['relationships']['field_type_tid']['table'] = 'field_data_field_type';
  $handler->display->display_options['relationships']['field_type_tid']['field'] = 'field_type_tid';
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_taxonomic_name']['alter']['text'] = 'Species: [field_taxonomic_name]<br/>';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'nid';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = 'Recording: <a href="[path]">[title]</a>';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Comment: Type */
  $handler->display->display_options['fields']['field_type']['id'] = 'field_type';
  $handler->display->display_options['fields']['field_type']['table'] = 'field_data_field_type';
  $handler->display->display_options['fields']['field_type']['field'] = 'field_type';
  $handler->display->display_options['fields']['field_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Comment: Oscillogram */
  $handler->display->display_options['fields']['field_oscilo']['id'] = 'field_oscilo';
  $handler->display->display_options['fields']['field_oscilo']['table'] = 'field_data_field_oscilo';
  $handler->display->display_options['fields']['field_oscilo']['field'] = 'field_oscilo';
  $handler->display->display_options['fields']['field_oscilo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_oscilo']['type'] = 'styles_file_square_thumbnail';
  /* Field: Comment: Frequency Spectrum */
  $handler->display->display_options['fields']['field_spec']['id'] = 'field_spec';
  $handler->display->display_options['fields']['field_spec']['table'] = 'field_data_field_spec';
  $handler->display->display_options['fields']['field_spec']['field'] = 'field_spec';
  $handler->display->display_options['fields']['field_spec']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_spec']['type'] = 'styles_file_square_thumbnail';
  /* Field: Comment: Spectrogram */
  $handler->display->display_options['fields']['field_spectro']['id'] = 'field_spectro';
  $handler->display->display_options['fields']['field_spectro']['table'] = 'field_data_field_spectro';
  $handler->display->display_options['fields']['field_spectro']['field'] = 'field_spectro';
  $handler->display->display_options['fields']['field_spectro']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_spectro']['type'] = 'styles_file_square_thumbnail';
  /* Field: Comment: Dominant Frequency */
  $handler->display->display_options['fields']['field_dfreq_plot']['id'] = 'field_dfreq_plot';
  $handler->display->display_options['fields']['field_dfreq_plot']['table'] = 'field_data_field_dfreq_plot';
  $handler->display->display_options['fields']['field_dfreq_plot']['field'] = 'field_dfreq_plot';
  $handler->display->display_options['fields']['field_dfreq_plot']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_dfreq_plot']['type'] = 'styles_file_square_thumbnail';
  /* Field: Comment: Acoustic Complexity Index */
  $handler->display->display_options['fields']['field_aci']['id'] = 'field_aci';
  $handler->display->display_options['fields']['field_aci']['table'] = 'field_data_field_aci';
  $handler->display->display_options['fields']['field_aci']['field'] = 'field_aci';
  $handler->display->display_options['fields']['field_aci']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1
  );
  /* Field: Comment: View link */
  $handler->display->display_options['fields']['view_comment']['id'] = 'view_comment';
  $handler->display->display_options['fields']['view_comment']['table'] = 'comment';
  $handler->display->display_options['fields']['view_comment']['field'] = 'view_comment';
  $handler->display->display_options['fields']['view_comment']['label'] = '';
  $handler->display->display_options['fields']['view_comment']['element_label_colon'] = FALSE;
  /* Filter criterion: Comment: Approved */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'comment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_node']['id'] = 'status_node';
  $handler->display->display_options['filters']['status_node']['table'] = 'node';
  $handler->display->display_options['filters']['status_node']['field'] = 'status';
  $handler->display->display_options['filters']['status_node']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status_node']['value'] = 1;
  $handler->display->display_options['filters']['status_node']['group'] = 1;
  $handler->display->display_options['filters']['status_node']['expose']['operator'] = FALSE;
  /* Filter criterion: Taxonomy term: Analyze (field_analyze) */
  $handler->display->display_options['filters']['field_analyze_value']['id'] = 'field_analyze_value';
  $handler->display->display_options['filters']['field_analyze_value']['table'] = 'field_data_field_analyze';
  $handler->display->display_options['filters']['field_analyze_value']['field'] = 'field_analyze_value';
  $handler->display->display_options['filters']['field_analyze_value']['relationship'] = 'field_type_tid';
  $handler->display->display_options['filters']['field_analyze_value']['value'] = array(
    1 => '1'
  );
  /* Filter criterion: Comment: Acoustic Complexity Index (field_aci) */
  $handler->display->display_options['filters']['field_aci_value']['id'] = 'field_aci_value';
  $handler->display->display_options['filters']['field_aci_value']['table'] = 'field_data_field_aci';
  $handler->display->display_options['filters']['field_aci_value']['field'] = 'field_aci_value';
  $handler->display->display_options['filters']['field_aci_value']['operator'] = 'not empty';
  /* Filter criterion: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['value'] = '';
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
  $handler->display->display_options['filters']['field_taxonomic_name_tid']['vocabulary'] = 'orthoptera';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'analyses';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Analyses';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  return $views;
}