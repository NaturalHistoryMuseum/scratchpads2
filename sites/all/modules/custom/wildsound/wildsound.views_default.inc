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
  $view = new view();
  $view->name = 'biblio_uses_specimens';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'biblio_uses_specimens';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_taxonomic_name' => 'field_taxonomic_name',
    'field_recording' => 'field_recording'
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
    ),
    'field_recording' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['content'] = '<h2>Specimens used in this work</h2>';
  /* Relationship: Content: Specimen (field_specimen) - reverse */
  $handler->display->display_options['relationships']['reverse_field_specimen_node']['id'] = 'reverse_field_specimen_node';
  $handler->display->display_options['relationships']['reverse_field_specimen_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_specimen_node']['field'] = 'reverse_field_specimen_node';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = '';
  $handler->display->display_options['fields']['field_taxonomic_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: COUNT(DISTINCT Content: Recording) */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['relationship'] = 'reverse_field_specimen_node';
  $handler->display->display_options['fields']['field_recording']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['field_recording']['label'] = '';
  $handler->display->display_options['fields']['field_recording']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_recording']['alter']['path'] = 'node/[nid]#recordings';
  $handler->display->display_options['fields']['field_recording']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_recording']['separator'] = ', ';
  $handler->display->display_options['fields']['field_recording']['suffix'] = ' recording(s)';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Cited In  (field_cited_in_) */
  $handler->display->display_options['arguments']['field_cited_in__nid']['id'] = 'field_cited_in__nid';
  $handler->display->display_options['arguments']['field_cited_in__nid']['table'] = 'field_data_field_cited_in_';
  $handler->display->display_options['arguments']['field_cited_in__nid']['field'] = 'field_cited_in__nid';
  $handler->display->display_options['arguments']['field_cited_in__nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cited_in__nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cited_in__nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cited_in__nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cited_in__nid']['summary_options']['items_per_page'] = '25';
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
  $views[] = $view;
  $view = new view();
  $view->name = 'contributors';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Contributors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Contributors';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'name_1',
      'rendered' => 0,
      'rendered_strip' => 0
    )
  );
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = array(
    'description' => 'description',
    'name_1' => 'name_1',
    'field_user_given_names' => 'field_user_given_names',
    'field_user_family_name' => 'field_user_family_name',
    'picture' => 'field_user_family_name',
    'field_project_bio' => 'field_project_bio'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'description' => array(
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_user_given_names' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_user_family_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'picture' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    ),
    'field_project_bio' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  /* Relationship: User: Projects (field_projects) */
  $handler->display->display_options['relationships']['field_projects_tid']['id'] = 'field_projects_tid';
  $handler->display->display_options['relationships']['field_projects_tid']['table'] = 'field_data_field_projects';
  $handler->display->display_options['relationships']['field_projects_tid']['field'] = 'field_projects_tid';
  $handler->display->display_options['relationships']['field_projects_tid']['required'] = TRUE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['relationship'] = 'field_projects_tid';
  $handler->display->display_options['fields']['description']['exclude'] = TRUE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'field_projects_tid';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['text'] = '<h2><span style="text-align:left;">[name_1]</span></h2>
[description]';
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  /* Field: User: Other/given name(s)  */
  $handler->display->display_options['fields']['field_user_given_names']['id'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['exclude'] = TRUE;
  /* Field: User: Family name */
  $handler->display->display_options['fields']['field_user_family_name']['id'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['table'] = 'field_data_field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['field'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['label'] = '';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['text'] = '<strong>[field_user_given_names] [field_user_family_name]</strong>';
  $handler->display->display_options['fields']['field_user_family_name']['element_label_colon'] = FALSE;
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['picture']['alter']['text'] = '<br/>[picture]';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['link_photo_to_profile'] = FALSE;
  $handler->display->display_options['fields']['picture']['image_style'] = 'square_thumbnail';
  /* Field: User: Project Bio */
  $handler->display->display_options['fields']['field_project_bio']['id'] = 'field_project_bio';
  $handler->display->display_options['fields']['field_project_bio']['table'] = 'field_data_field_project_bio';
  $handler->display->display_options['fields']['field_project_bio']['field'] = 'field_project_bio';
  $handler->display->display_options['fields']['field_project_bio']['label'] = '';
  $handler->display->display_options['fields']['field_project_bio']['element_label_colon'] = FALSE;
  /* Sort criterion: Taxonomy term: Weight (field_project_weight) */
  $handler->display->display_options['sorts']['field_project_weight_value']['id'] = 'field_project_weight_value';
  $handler->display->display_options['sorts']['field_project_weight_value']['table'] = 'field_data_field_project_weight';
  $handler->display->display_options['sorts']['field_project_weight_value']['field'] = 'field_project_weight_value';
  $handler->display->display_options['sorts']['field_project_weight_value']['relationship'] = 'field_projects_tid';
  /* Sort criterion: User: Weight (field_bio_weight) */
  $handler->display->display_options['sorts']['field_bio_weight_value']['id'] = 'field_bio_weight_value';
  $handler->display->display_options['sorts']['field_bio_weight_value']['table'] = 'field_data_field_bio_weight';
  $handler->display->display_options['sorts']['field_bio_weight_value']['field'] = 'field_bio_weight_value';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'contributors';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Contributors';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'latest_projects';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'latest_projects';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'alpha grid-4';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Taxonomy term: Content using Project */
  $handler->display->display_options['relationships']['reverse_field_project_node']['id'] = 'reverse_field_project_node';
  $handler->display->display_options['relationships']['reverse_field_project_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_project_node']['field'] = 'reverse_field_project_node';
  $handler->display->display_options['relationships']['reverse_field_project_node']['required'] = TRUE;
  /* Field: COUNT(Taxonomy term: Name) */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['group_type'] = 'count';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'project' => 'project'
  );
  /* Filter criterion: Taxonomy term: Weight (field_project_weight) */
  $handler->display->display_options['filters']['field_project_weight_value']['id'] = 'field_project_weight_value';
  $handler->display->display_options['filters']['field_project_weight_value']['table'] = 'field_data_field_project_weight';
  $handler->display->display_options['filters']['field_project_weight_value']['field'] = 'field_project_weight_value';
  $handler->display->display_options['filters']['field_project_weight_value']['operator'] = '>';
  $handler->display->display_options['filters']['field_project_weight_value']['value']['value'] = '0';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'reverse_field_project_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Latest Projects';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content using Project */
  $handler->display->display_options['relationships']['reverse_field_project_node']['id'] = 'reverse_field_project_node';
  $handler->display->display_options['relationships']['reverse_field_project_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_project_node']['field'] = 'reverse_field_project_node';
  $handler->display->display_options['relationships']['reverse_field_project_node']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Title) */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'reverse_field_project_node';
  $handler->display->display_options['fields']['title']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<em>Recordings: [title]</em><br/>&nbsp;<br/>';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $views[] = $view;
  $view = new view();
  $view->name = 'map_taxa';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'map-taxa';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recordings Map';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Taxonomy term: Content using Species */
  $handler->display->display_options['relationships']['reverse_field_species_node']['id'] = 'reverse_field_species_node';
  $handler->display->display_options['relationships']['reverse_field_species_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_species_node']['field'] = 'reverse_field_species_node';
  $handler->display->display_options['relationships']['reverse_field_species_node']['required'] = TRUE;
  /* Relationship: Content: Specimen (field_specimen) */
  $handler->display->display_options['relationships']['field_specimen_nid']['id'] = 'field_specimen_nid';
  $handler->display->display_options['relationships']['field_specimen_nid']['table'] = 'field_data_field_specimen';
  $handler->display->display_options['relationships']['field_specimen_nid']['field'] = 'field_specimen_nid';
  $handler->display->display_options['relationships']['field_specimen_nid']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['relationships']['field_specimen_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_specimen_nid']['delta'] = '-1';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['relationship'] = 'field_specimen_nid';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Species';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_specimen_nid';
  $handler->display->display_options['fields']['title']['label'] = 'Specimen';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Location';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_2']['id'] = 'title_2';
  $handler->display->display_options['fields']['title_2']['table'] = 'node';
  $handler->display->display_options['fields']['title_2']['field'] = 'title';
  $handler->display->display_options['fields']['title_2']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['fields']['title_2']['label'] = 'Recording';
  $handler->display->display_options['fields']['title_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_2']['alter']['text'] = '[title_2]';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['fields']['field_recording']['label'] = '';
  $handler->display->display_options['fields']['field_recording']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '300',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'orthoptera' => 'orthoptera'
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Has taxonomy terms (with depth) */
  $handler->display->display_options['filters']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['filters']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['filters']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['filters']['term_node_tid_depth']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['filters']['term_node_tid_depth']['value'] = '';
  $handler->display->display_options['filters']['term_node_tid_depth']['exposed'] = TRUE;
  $handler->display->display_options['filters']['term_node_tid_depth']['expose']['operator_id'] = 'term_node_tid_depth_op';
  $handler->display->display_options['filters']['term_node_tid_depth']['expose']['label'] = 'Filter by taxon';
  $handler->display->display_options['filters']['term_node_tid_depth']['expose']['operator'] = 'term_node_tid_depth_op';
  $handler->display->display_options['filters']['term_node_tid_depth']['expose']['identifier'] = 'term_node_tid_depth';
  $handler->display->display_options['filters']['term_node_tid_depth']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['term_node_tid_depth']['vocabulary'] = 'orthoptera';
  $handler->display->display_options['filters']['term_node_tid_depth']['depth'] = '10';
  /* Filter criterion: Content: Project (field_project) */
  $handler->display->display_options['filters']['field_project_tid']['id'] = 'field_project_tid';
  $handler->display->display_options['filters']['field_project_tid']['table'] = 'field_data_field_project';
  $handler->display->display_options['filters']['field_project_tid']['field'] = 'field_project_tid';
  $handler->display->display_options['filters']['field_project_tid']['relationship'] = 'reverse_field_species_node';
  $handler->display->display_options['filters']['field_project_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_project_tid']['expose']['operator_id'] = 'field_project_tid_op';
  $handler->display->display_options['filters']['field_project_tid']['expose']['label'] = 'Project';
  $handler->display->display_options['filters']['field_project_tid']['expose']['operator'] = 'field_project_tid_op';
  $handler->display->display_options['filters']['field_project_tid']['expose']['identifier'] = 'field_project_tid';
  $handler->display->display_options['filters']['field_project_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  $handler->display->display_options['filters']['field_project_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_project_tid']['vocabulary'] = 'project';
  $handler->display->display_options['filters']['field_project_tid']['hierarchy'] = 1;
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'map';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Map';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'recordings_of_specimens';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'recordings of specimens';
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
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_recording' => 'field_recording'
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
    'field_recording' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['header']['area_text_custom']['content'] = '<span id="recordings"></span>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['label'] = '';
  $handler->display->display_options['fields']['field_recording']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '300',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Specimen (field_specimen) */
  $handler->display->display_options['arguments']['field_specimen_nid']['id'] = 'field_specimen_nid';
  $handler->display->display_options['arguments']['field_specimen_nid']['table'] = 'field_data_field_specimen';
  $handler->display->display_options['arguments']['field_specimen_nid']['field'] = 'field_specimen_nid';
  $handler->display->display_options['arguments']['field_specimen_nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_specimen_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_specimen_nid']['default_argument_options']['argument'] = '11800';
  $handler->display->display_options['arguments']['field_specimen_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_specimen_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_specimen_nid']['summary_options']['items_per_page'] = '25';
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
    'recording' => 'recording'
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'specimens_from_location';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'specimens from location';
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
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = '';
  $handler->display->display_options['fields']['field_taxonomic_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Content: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['label'] = '';
  $handler->display->display_options['fields']['field_date_collected']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_date_collected']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => ''
  );
  $handler->display->display_options['fields']['field_date_collected']['format_date_sql'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Location (field_location) */
  $handler->display->display_options['arguments']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['arguments']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['arguments']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['arguments']['field_location_nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_location_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_location_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_location_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_location_nid']['summary_options']['items_per_page'] = '25';
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
  $views[] = $view;
  $view = new view();
  $view->name = 'specimen_location_link';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'specimen_location_link';
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
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['label'] = '';
  $handler->display->display_options['fields']['field_location']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'taxa';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Taxa';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Taxa';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'orthoptera' => 'orthoptera'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'taxa';
  $views[] = $view;
  $view = new view();
  $view->name = 'check_copyright';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-copyright';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records with no copyright holder';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Published */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'node';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  $handler->display->display_options['sorts']['status']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Copyright Holder (field_copyright_holder) */
  $handler->display->display_options['filters']['field_copyright_holder_value']['id'] = 'field_copyright_holder_value';
  $handler->display->display_options['filters']['field_copyright_holder_value']['table'] = 'field_data_field_copyright_holder';
  $handler->display->display_options['filters']['field_copyright_holder_value']['field'] = 'field_copyright_holder_value';
  $handler->display->display_options['filters']['field_copyright_holder_value']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-copyright';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Records missing copyright holder';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'check_no_audio';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-no-audio';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records with no audio file';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Relationship: Content: Recording (field_recording:fid) */
  $handler->display->display_options['relationships']['field_recording_fid']['id'] = 'field_recording_fid';
  $handler->display->display_options['relationships']['field_recording_fid']['table'] = 'field_data_field_recording';
  $handler->display->display_options['relationships']['field_recording_fid']['field'] = 'field_recording_fid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Published */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'node';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  $handler->display->display_options['sorts']['status']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: File: File UUID */
  $handler->display->display_options['filters']['uuid']['id'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['table'] = 'file_managed';
  $handler->display->display_options['filters']['uuid']['field'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['relationship'] = 'field_recording_fid';
  $handler->display->display_options['filters']['uuid']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-no-audio';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Records with no audio';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'check_ready_to_publish';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-ready-to-publish';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records likely to be ready for publishing';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'status' => 'status'
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
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Relationship: Content: Recording (field_recording:fid) */
  $handler->display->display_options['relationships']['field_recording_fid']['id'] = 'field_recording_fid';
  $handler->display->display_options['relationships']['field_recording_fid']['table'] = 'field_data_field_recording';
  $handler->display->display_options['relationships']['field_recording_fid']['field'] = 'field_recording_fid';
  $handler->display->display_options['relationships']['field_recording_fid']['required'] = TRUE;
  /* Relationship: Content: Species (field_species) */
  $handler->display->display_options['relationships']['field_species_tid']['id'] = 'field_species_tid';
  $handler->display->display_options['relationships']['field_species_tid']['table'] = 'field_data_field_species';
  $handler->display->display_options['relationships']['field_species_tid']['field'] = 'field_species_tid';
  $handler->display->display_options['relationships']['field_species_tid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: Taxonomy term: Taxonomy term UUID */
  $handler->display->display_options['filters']['uuid']['id'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['uuid']['field'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['relationship'] = 'field_species_tid';
  $handler->display->display_options['filters']['uuid']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['uuid']['value'] = '2';
  /* Filter criterion: Content: License (field_license) */
  $handler->display->display_options['filters']['field_license_licence']['id'] = 'field_license_licence';
  $handler->display->display_options['filters']['field_license_licence']['table'] = 'field_data_field_license';
  $handler->display->display_options['filters']['field_license_licence']['field'] = 'field_license_licence';
  $handler->display->display_options['filters']['field_license_licence']['operator'] = 'not empty';
  /* Filter criterion: Content: Copyright Holder (field_copyright_holder) */
  $handler->display->display_options['filters']['field_copyright_holder_value']['id'] = 'field_copyright_holder_value';
  $handler->display->display_options['filters']['field_copyright_holder_value']['table'] = 'field_data_field_copyright_holder';
  $handler->display->display_options['filters']['field_copyright_holder_value']['field'] = 'field_copyright_holder_value';
  $handler->display->display_options['filters']['field_copyright_holder_value']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['field_copyright_holder_value']['value'] = '2';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-ready-to-publish';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Ready to publish?';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'check_published_no_taxon';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check_published_no_taxon';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'check_published_no_taxon';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-published-no-taxon';
  $views[] = $view;
  $view = new view();
  $view->name = 'check_licence';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-licence';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records with no licence';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Published */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'node';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  $handler->display->display_options['sorts']['status']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: Content: License (field_license) */
  $handler->display->display_options['filters']['field_license_licence']['id'] = 'field_license_licence';
  $handler->display->display_options['filters']['field_license_licence']['table'] = 'field_data_field_license';
  $handler->display->display_options['filters']['field_license_licence']['field'] = 'field_license_licence';
  $handler->display->display_options['filters']['field_license_licence']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-licence';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Records missing licence';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'check_no_taxon';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-no-taxon';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records not associated with a taxon name';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Relationship: Content: Taxonomy terms on node */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'orthoptera' => 'orthoptera',
    'image_keywords' => 0,
    'imaging_technique' => 0,
    'preparation_technique' => 0
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Published */
  $handler->display->display_options['sorts']['status']['id'] = 'status';
  $handler->display->display_options['sorts']['status']['table'] = 'node';
  $handler->display->display_options['sorts']['status']['field'] = 'status';
  $handler->display->display_options['sorts']['status']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Species (field_species) */
  $handler->display->display_options['filters']['field_species_tid']['id'] = 'field_species_tid';
  $handler->display->display_options['filters']['field_species_tid']['table'] = 'field_data_field_species';
  $handler->display->display_options['filters']['field_species_tid']['field'] = 'field_species_tid';
  $handler->display->display_options['filters']['field_species_tid']['operator'] = 'empty';
  $handler->display->display_options['filters']['field_species_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_species_tid']['vocabulary'] = 'orthoptera';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'check-no-taxon';
  $views[] = $view;
  $view = new view();
  $view->name = 'integrity_nhm_no_specimen';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'integrity_nhm_no_specimen';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'no_specimen';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Original Metadata Image */
  $handler->display->display_options['fields']['field_original_metadata_image']['id'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['field'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_original_metadata_image']['type'] = 'styles_file_thumbnail';
  $handler->display->display_options['fields']['field_original_metadata_image']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '300',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Original Metadata Image (field_original_metadata_image:fid) */
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['id'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['field'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['order'] = 'DESC';
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
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Specimen (field_specimen) */
  $handler->display->display_options['filters']['field_specimen_nid']['id'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['table'] = 'field_data_field_specimen';
  $handler->display->display_options['filters']['field_specimen_nid']['field'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'integrity-nhm-no-specimen';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'NHM no specimen';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  $view = new view();
  $view->name = 'integrity_nhm_no_specimen';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'integrity_nhm_no_specimen';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'no_specimen';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Original Metadata Image */
  $handler->display->display_options['fields']['field_original_metadata_image']['id'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['field'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_original_metadata_image']['type'] = 'styles_file_thumbnail';
  $handler->display->display_options['fields']['field_original_metadata_image']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '300',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Original Metadata Image (field_original_metadata_image:fid) */
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['id'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['field'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['order'] = 'DESC';
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
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Specimen (field_specimen) */
  $handler->display->display_options['filters']['field_specimen_nid']['id'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['table'] = 'field_data_field_specimen';
  $handler->display->display_options['filters']['field_specimen_nid']['field'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'integrity-nhm-no-specimen';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'NHM no specimen';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $view = new view();
  $view->name = 'integrity_nhm_no_specimen';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'integrity_nhm_no_specimen';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'no_specimen';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title'
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Original Metadata Image */
  $handler->display->display_options['fields']['field_original_metadata_image']['id'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['field'] = 'field_original_metadata_image';
  $handler->display->display_options['fields']['field_original_metadata_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_original_metadata_image']['type'] = 'styles_file_thumbnail';
  $handler->display->display_options['fields']['field_original_metadata_image']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '300',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Original Metadata Image (field_original_metadata_image:fid) */
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['id'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['table'] = 'field_data_field_original_metadata_image';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['field'] = 'field_original_metadata_image_fid';
  $handler->display->display_options['sorts']['field_original_metadata_image_fid']['order'] = 'DESC';
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
    'recording' => 'recording'
  );
  /* Filter criterion: Content: Specimen (field_specimen) */
  $handler->display->display_options['filters']['field_specimen_nid']['id'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['table'] = 'field_data_field_specimen';
  $handler->display->display_options['filters']['field_specimen_nid']['field'] = 'field_specimen_nid';
  $handler->display->display_options['filters']['field_specimen_nid']['operator'] = 'empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'integrity-nhm-no-specimen';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'NHM no specimen';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[] = $view;
  return $views;
}