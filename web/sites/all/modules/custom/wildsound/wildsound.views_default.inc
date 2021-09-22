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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<p>Analyses are performed using the <a href="http://rug.mnhn.fr/seewave/">seewave</a> package for R (<a href="http://dx.doi.org/10.1080/09524622.2008.9753600">Sueur et al, 2008</a>) using the <a href="http://www.biovel.eu/">BioVeL</a> infrastructure.</p>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
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
  /* Sort criterion: Comment: ID */
  $handler->display->display_options['sorts']['cid']['id'] = 'cid';
  $handler->display->display_options['sorts']['cid']['table'] = 'comment';
  $handler->display->display_options['sorts']['cid']['field'] = 'cid';
  $handler->display->display_options['sorts']['cid']['order'] = 'DESC';
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
  $translatables['analyses'] = array(
    t('Master'),
    t('Analyses'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<p>Analyses are performed using the <a href="http://rug.mnhn.fr/seewave/">seewave</a> package for R (<a href="http://dx.doi.org/10.1080/09524622.2008.9753600">Sueur et al, 2008</a>) using the <a href="http://www.biovel.eu/">BioVeL</a> infrastructure.</p>'),
    t('Content'),
    t('term from field_type'),
    t('Taxonomic name'),
    t('Species: [field_taxonomic_name]<br/>'),
    t('Path'),
    t('Recording: <a href="[path]">[title]</a>'),
    t('Type'),
    t('Oscillogram'),
    t('Frequency Spectrum'),
    t('Spectrogram'),
    t('Dominant Frequency'),
    t('Acoustic Complexity Index'),
    t('Page')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'biblio_uses_specimens';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'biblio_uses_specimens';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Contextual filter: Content: Cited In (field_cited_in_) */
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
  $translatables['biblio_uses_specimens'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h2>Specimens used in this work</h2>'),
    t('field_specimen'),
    t('Nid'),
    t('All')
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Field: User: Other/given name(s) */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $handler->display->display_options['header']['area_text_custom']['content'] = '<h2 id="recordings">Recordings of this specimen</h2>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Recording name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '420',
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
  $translatables['recordings_of_specimens'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h2 id="recordings">Recordings of this specimen</h2>'),
    t('Recording name'),
    t('Recording'),
    t('All')
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records with no audio file';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = -1;
  $handler->display->display_options['cache']['output_lifespan'] = 300;
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
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
  $translatables['check_no_audio'] = array(
    t('Master'),
    t('Records with no audio file'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('file from field_recording'),
    t('Published'),
    t('Page')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'check_ready_to_publish';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'check-ready-to-publish';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Grabaciones sin asociación taxonómica';
  $handler->display->display_options['path'] = 'check-no-taxon';
  $translatables['check_no_taxon'] = array(
    t('Master'),
    t('Records not associated with a taxon name'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('term'),
    t('Published'),
    t('Page'),
    t('Grabaciones sin asociación taxonómica')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'integrity_nhm_no_specimen';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'integrity_nhm_no_specimen';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /*
   * Sort criterion: Content: Original Metadata Image
   * (field_original_metadata_image:fid)
   */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /*
   * Sort criterion: Content: Original Metadata Image
   * (field_original_metadata_image:fid)
   */
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
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /*
   * Sort criterion: Content: Original Metadata Image
   * (field_original_metadata_image:fid)
   */
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
  $view->name = 'biblio_uses_recordings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'biblio_uses_recordings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  $handler->display->display_options['header']['area_text_custom']['content'] = '<h2>Recordings used in this work</h2>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Recording name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Species */
  $handler->display->display_options['fields']['field_species']['id'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['table'] = 'field_data_field_species';
  $handler->display->display_options['fields']['field_species']['field'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['type'] = 'taxonomy_term_reference_link';
  $handler->display->display_options['fields']['field_species']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '420',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /*
   * Contextual filter: Content: Published Reference (field_published_reference)
   */
  $handler->display->display_options['arguments']['field_published_reference_nid']['id'] = 'field_published_reference_nid';
  $handler->display->display_options['arguments']['field_published_reference_nid']['table'] = 'field_data_field_published_reference';
  $handler->display->display_options['arguments']['field_published_reference_nid']['field'] = 'field_published_reference_nid';
  $handler->display->display_options['arguments']['field_published_reference_nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_published_reference_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_published_reference_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_published_reference_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_published_reference_nid']['summary_options']['items_per_page'] = '25';
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
  $translatables['biblio_uses_recordings'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h2>Recordings used in this work</h2>'),
    t('Recording name'),
    t('Species'),
    t('Recording'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'collection_citation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'collection_citation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Cite this Collection';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['wrapper_class'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area_1']['id'] = 'area_1';
  $handler->display->display_options['header']['area_1']['table'] = 'views';
  $handler->display->display_options['header']['area_1']['field'] = 'area';
  $handler->display->display_options['header']['area_1']['label'] = 'Title';
  $handler->display->display_options['header']['area_1']['content'] = '<h2>How to cite this collection</h2>';
  $handler->display->display_options['header']['area_1']['format'] = 'filtered_html';
  /* Header: Global: View area */
  $handler->display->display_options['header']['view']['id'] = 'view';
  $handler->display->display_options['header']['view']['table'] = 'views';
  $handler->display->display_options['header']['view']['field'] = 'view';
  $handler->display->display_options['header']['view']['view_to_insert'] = 'collection_citation_short:default';
  $handler->display->display_options['header']['view']['inherit_arguments'] = TRUE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<ul>
<li>Baker E, Price BW, Rycroft SD, Hill J, Smith VS. BioAcoustica: a free and open repository and analysis platform for bioacoustics. Database. 2015;2015:bav054. Available at: http://database.oxfordjournals.org/cgi/doi/10.1093/database/bav054.</li>
</ul>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Relationship: Content: Recordings (field_recordings) */
  $handler->display->display_options['relationships']['field_recordings_nid']['id'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['table'] = 'field_data_field_recordings';
  $handler->display->display_options['relationships']['field_recordings_nid']['field'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['delta'] = '-1';
  /* Relationship: Content: Project (field_project) */
  $handler->display->display_options['relationships']['field_project_tid']['id'] = 'field_project_tid';
  $handler->display->display_options['relationships']['field_project_tid']['table'] = 'field_data_field_project';
  $handler->display->display_options['relationships']['field_project_tid']['field'] = 'field_project_tid';
  $handler->display->display_options['relationships']['field_project_tid']['relationship'] = 'field_recordings_nid';
  /* Field: Taxonomy term: Project Citation */
  $handler->display->display_options['fields']['field_project_citation']['id'] = 'field_project_citation';
  $handler->display->display_options['fields']['field_project_citation']['table'] = 'field_data_field_project_citation';
  $handler->display->display_options['fields']['field_project_citation']['field'] = 'field_project_citation';
  $handler->display->display_options['fields']['field_project_citation']['relationship'] = 'field_project_tid';
  $handler->display->display_options['fields']['field_project_citation']['label'] = '';
  $handler->display->display_options['fields']['field_project_citation']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_project_citation']['alter']['text'] = '[field_project_citation]<br/>';
  $handler->display->display_options['fields']['field_project_citation']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_project_citation']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_project_citation']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_project_citation']['type'] = 'scratchpads_biblio_biblio_ref';
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  $translatables['collection_citation'] = array(
    t('Master'),
    t('Cite this Collection'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Title'),
    t('<h2>How to cite this collection</h2>'),
    t('<ul>
<li>Baker E, Price BW, Rycroft SD, Hill J, Smith VS. BioAcoustica: a free and open repository and analysis platform for bioacoustics. Database. 2015;2015:bav054. Available at: http://database.oxfordjournals.org/cgi/doi/10.1093/database/bav054.</li>
</ul>'),
    t('field_recordings'),
    t('term from field_project'),
    t('[field_project_citation]<br/>'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'collection_citation_short';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'collection_citation_short';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'views_delimited_list';
  $handler->display->display_options['style_options']['delimiter'] = '; ';
  $handler->display->display_options['style_options']['long_count'] = '3';
  $handler->display->display_options['style_options']['separator_long'] = 'delimiter';
  $handler->display->display_options['style_options']['prefix'] = 'This work makes use of the BioAcoustica framework (Baker et al, 2015) and contributed datasets described in ';
  $handler->display->display_options['style_options']['suffix'] = '.';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = 'Data used is listed at <a href="[path]">[path]</a>.';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['footer']['area']['tokenize'] = TRUE;
  /* Relationship: Content: Recordings (field_recordings) */
  $handler->display->display_options['relationships']['field_recordings_nid']['id'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['table'] = 'field_data_field_recordings';
  $handler->display->display_options['relationships']['field_recordings_nid']['field'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['delta'] = '-1';
  /* Relationship: Content: Project (field_project) */
  $handler->display->display_options['relationships']['field_project_tid']['id'] = 'field_project_tid';
  $handler->display->display_options['relationships']['field_project_tid']['table'] = 'field_data_field_project';
  $handler->display->display_options['relationships']['field_project_tid']['field'] = 'field_project_tid';
  $handler->display->display_options['relationships']['field_project_tid']['relationship'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_project_tid']['label'] = 'term_from_field_project';
  /* Relationship: Taxonomy term: Project Citation (field_project_citation) */
  $handler->display->display_options['relationships']['field_project_citation_nid']['id'] = 'field_project_citation_nid';
  $handler->display->display_options['relationships']['field_project_citation_nid']['table'] = 'field_data_field_project_citation';
  $handler->display->display_options['relationships']['field_project_citation_nid']['field'] = 'field_project_citation_nid';
  $handler->display->display_options['relationships']['field_project_citation_nid']['relationship'] = 'field_project_tid';
  $handler->display->display_options['relationships']['field_project_citation_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_project_citation_nid']['delta'] = '-1';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['relationship'] = 'field_project_citation_nid';
  $handler->display->display_options['fields']['biblio_authors']['label'] = '';
  $handler->display->display_options['fields']['biblio_authors']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['initialize_with'] = '';
  $handler->display->display_options['fields']['biblio_authors']['separator'] = ', ';
  $handler->display->display_options['fields']['biblio_authors']['sort_separator'] = ' ';
  $handler->display->display_options['fields']['biblio_authors']['name_order'] = 'last-first';
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['relationship'] = 'field_project_citation_nid';
  $handler->display->display_options['fields']['biblio_year']['label'] = '';
  $handler->display->display_options['fields']['biblio_year']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['biblio_year']['alter']['text'] = '[biblio_authors] ([biblio_year])';
  $handler->display->display_options['fields']['biblio_year']['alter']['path'] = '[path]';
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_year']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['biblio_year']['biblio_label'] = 0;
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  $translatables['collection_citation_short'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Data used is listed at <a href="[path]">[path]</a>.'),
    t('field_recordings'),
    t('term_from_field_project'),
    t('field_project_citation'),
    t('[biblio_authors] ([biblio_year])'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'collection_recordings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'collection_recordings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Recordings in this collection</h2>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Relationship: Content: Recordings (field_recordings) */
  $handler->display->display_options['relationships']['field_recordings_nid']['id'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['table'] = 'field_data_field_recordings';
  $handler->display->display_options['relationships']['field_recordings_nid']['field'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_recordings_nid']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_recordings_nid';
  $handler->display->display_options['fields']['title']['label'] = 'Recording';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Species */
  $handler->display->display_options['fields']['field_species']['id'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['table'] = 'field_data_field_species';
  $handler->display->display_options['fields']['field_species']['field'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['relationship'] = 'field_recordings_nid';
  $handler->display->display_options['fields']['field_species']['delta_offset'] = '0';
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  $translatables['collection_recordings'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h2>Recordings in this collection</h2>'),
    t('field_recordings'),
    t('Recording'),
    t('Species'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r';
  $view->description = '';
  $view->tag = 'default, r';
  $view->base_table = 'comment';
  $view->human_name = 'R annotations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Relationship: Comment: Content */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  /* Relationship: Comment: Type (field_type) */
  $handler->display->display_options['relationships']['field_type_tid']['id'] = 'field_type_tid';
  $handler->display->display_options['relationships']['field_type_tid']['table'] = 'field_data_field_type';
  $handler->display->display_options['relationships']['field_type_tid']['field'] = 'field_type_tid';
  /* Field: Comment: ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'comment';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['label'] = 'id';
  /* Field: Comment: Type */
  $handler->display->display_options['fields']['field_type']['id'] = 'field_type';
  $handler->display->display_options['fields']['field_type']['table'] = 'field_data_field_type';
  $handler->display->display_options['fields']['field_type']['field'] = 'field_type';
  $handler->display->display_options['fields']['field_type']['label'] = 'type';
  $handler->display->display_options['fields']['field_type']['type'] = 'taxonomy_term_reference_link';
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = 'taxon';
  $handler->display->display_options['fields']['field_taxonomic_name']['type'] = 'taxonomy_term_reference_link';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['tid']['label'] = 'taxon_id';
  /* Field: Comment: Start Time */
  $handler->display->display_options['fields']['field_start_time']['id'] = 'field_start_time';
  $handler->display->display_options['fields']['field_start_time']['table'] = 'field_data_field_start_time';
  $handler->display->display_options['fields']['field_start_time']['field'] = 'field_start_time';
  $handler->display->display_options['fields']['field_start_time']['label'] = 'start';
  /* Field: Comment: End Time */
  $handler->display->display_options['fields']['field_end_time']['id'] = 'field_end_time';
  $handler->display->display_options['fields']['field_end_time']['table'] = 'field_data_field_end_time';
  $handler->display->display_options['fields']['field_end_time']['field'] = 'field_end_time';
  $handler->display->display_options['fields']['field_end_time']['label'] = 'end';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['relationship'] = 'nid';
  $handler->display->display_options['fields']['field_recording']['label'] = 'file';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'file_url_plain';
  /* Field: Comment: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'comment';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'recording';
  $handler->display->display_options['fields']['nid']['separator'] = '';
  /* Field: Content: Original Tape Number */
  $handler->display->display_options['fields']['field_original_cd_nhm_tape_numbe']['id'] = 'field_original_cd_nhm_tape_numbe';
  $handler->display->display_options['fields']['field_original_cd_nhm_tape_numbe']['table'] = 'field_data_field_original_cd_nhm_tape_numbe';
  $handler->display->display_options['fields']['field_original_cd_nhm_tape_numbe']['field'] = 'field_original_cd_nhm_tape_numbe';
  $handler->display->display_options['fields']['field_original_cd_nhm_tape_numbe']['relationship'] = 'nid';
  $handler->display->display_options['fields']['field_original_cd_nhm_tape_numbe']['label'] = 'original_tape';
  /* Field: Comment: Author */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'comment';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'author';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Comment: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'comment';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created']['format_date_sql'] = 0;
  /* Sort criterion: Comment: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'comment';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
  /* Filter criterion: Taxonomy term: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: Taxonomy term: Name */
  $handler->display->display_options['filters']['name_1']['id'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['name_1']['field'] = 'name';
  $handler->display->display_options['filters']['name_1']['relationship'] = 'field_type_tid';
  $handler->display->display_options['filters']['name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator_id'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['name_1']['expose']['operator'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['identifier'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0
  );
  /* Filter criterion: Comment: End Time (field_end_time) */
  $handler->display->display_options['filters']['field_end_time_value']['id'] = 'field_end_time_value';
  $handler->display->display_options['filters']['field_end_time_value']['table'] = 'field_data_field_end_time';
  $handler->display->display_options['filters']['field_end_time_value']['field'] = 'field_end_time_value';
  $handler->display->display_options['filters']['field_end_time_value']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['field_end_time_value']['value'] = '0';
  /* Filter criterion: Comment: Start Time (field_start_time) */
  $handler->display->display_options['filters']['field_start_time_value']['id'] = 'field_start_time_value';
  $handler->display->display_options['filters']['field_start_time_value']['table'] = 'field_data_field_start_time';
  $handler->display->display_options['filters']['field_start_time_value']['field'] = 'field_start_time_value';
  $handler->display->display_options['filters']['field_start_time_value']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['field_start_time_value']['value'] = '0';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/annotations';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'R/annotations-page';
  $translatables['r'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Content'),
    t('term from field_taxonomic_name'),
    t('term from field_type'),
    t('id'),
    t('type'),
    t('taxon'),
    t('taxon_id'),
    t('.'),
    t(','),
    t('start'),
    t('end'),
    t('file'),
    t('recording'),
    t('original_tape'),
    t('author'),
    t('date'),
    t('Name'),
    t('Data export'),
    t('Page')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r_collections';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'R collections';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'R recording';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'views_delimited_list';
  $handler->display->display_options['style_options']['long_count'] = '3';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
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
    'collection' => 'collection'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/collections';
  $translatables['r_collections'] = array(
    t('Master'),
    t('R recording'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('id'),
    t('Title'),
    t('Data export')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r_recording';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'R recording';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'R recording';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Relationship: Content: Species (field_species) */
  $handler->display->display_options['relationships']['field_species_tid']['id'] = 'field_species_tid';
  $handler->display->display_options['relationships']['field_species_tid']['table'] = 'field_data_field_species';
  $handler->display->display_options['relationships']['field_species_tid']['field'] = 'field_species_tid';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Recording (field_recording:fid) */
  $handler->display->display_options['relationships']['field_recording_fid']['id'] = 'field_recording_fid';
  $handler->display->display_options['relationships']['field_recording_fid']['table'] = 'field_data_field_recording';
  $handler->display->display_options['relationships']['field_recording_fid']['field'] = 'field_recording_fid';
  $handler->display->display_options['relationships']['field_recording_fid']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'id';
  /* Field: Content: Species */
  $handler->display->display_options['fields']['field_species']['id'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['table'] = 'field_data_field_species';
  $handler->display->display_options['fields']['field_species']['field'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['label'] = 'taxon';
  $handler->display->display_options['fields']['field_species']['type'] = 'taxonomy_term_reference_link';
  $handler->display->display_options['fields']['field_species']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['label'] = 'file';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'file_url_plain';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'author';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created']['format_date_sql'] = 0;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_recording_fid';
  $handler->display->display_options['fields']['filesize']['label'] = 'size';
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize_1']['id'] = 'filesize_1';
  $handler->display->display_options['fields']['filesize_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize_1']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize_1']['relationship'] = 'field_recording_fid';
  $handler->display->display_options['fields']['filesize_1']['label'] = 'size_raw';
  $handler->display->display_options['fields']['filesize_1']['file_size_display'] = 'bytes';
  /* Field: File: Mime type */
  $handler->display->display_options['fields']['filemime']['id'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filemime']['field'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['relationship'] = 'field_recording_fid';
  $handler->display->display_options['fields']['filemime']['label'] = 'type';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID (with depth) */
  $handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'default';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '10';
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
  /* Filter criterion: Content: Recording (field_recording:fid) */
  $handler->display->display_options['filters']['field_recording_fid']['id'] = 'field_recording_fid';
  $handler->display->display_options['filters']['field_recording_fid']['table'] = 'field_data_field_recording';
  $handler->display->display_options['filters']['field_recording_fid']['field'] = 'field_recording_fid';
  $handler->display->display_options['filters']['field_recording_fid']['operator'] = 'not empty';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'R/recordings-page';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Taxonomy term: Name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['relationship'] = 'field_species_tid';
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'R/recordings';
  /* Display: Data export 2 */
  $handler = $view->new_display('views_data_export', 'Data export 2', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/recordings-depth';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'R/recordings/depth-page';
  $translatables['r_recording'] = array(
    t('Master'),
    t('R recording'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('term from field_species'),
    t('author'),
    t('file from field_recording'),
    t('id'),
    t('taxon'),
    t('file'),
    t('date'),
    t('size'),
    t('size_raw'),
    t('type'),
    t('All'),
    t('Page'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Data export'),
    t('Data export 2')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r_taxa';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'R taxa';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = 'id';
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'taxon';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Field: Taxonomy term: Unit name 1 */
  $handler->display->display_options['fields']['field_unit_name1']['id'] = 'field_unit_name1';
  $handler->display->display_options['fields']['field_unit_name1']['table'] = 'field_data_field_unit_name1';
  $handler->display->display_options['fields']['field_unit_name1']['field'] = 'field_unit_name1';
  /* Field: Taxonomy term: Unit name 2 */
  $handler->display->display_options['fields']['field_unit_name2']['id'] = 'field_unit_name2';
  $handler->display->display_options['fields']['field_unit_name2']['table'] = 'field_data_field_unit_name2';
  $handler->display->display_options['fields']['field_unit_name2']['field'] = 'field_unit_name2';
  /* Field: Taxonomy term: Unit name 3 */
  $handler->display->display_options['fields']['field_unit_name3']['id'] = 'field_unit_name3';
  $handler->display->display_options['fields']['field_unit_name3']['table'] = 'field_data_field_unit_name3';
  $handler->display->display_options['fields']['field_unit_name3']['field'] = 'field_unit_name3';
  /* Field: Taxonomy term: Unit name 4 */
  $handler->display->display_options['fields']['field_unit_name4']['id'] = 'field_unit_name4';
  $handler->display->display_options['fields']['field_unit_name4']['table'] = 'field_data_field_unit_name4';
  $handler->display->display_options['fields']['field_unit_name4']['field'] = 'field_unit_name4';
  /* Field: Taxonomy term: Rank */
  $handler->display->display_options['fields']['field_rank']['id'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['table'] = 'field_data_field_rank';
  $handler->display->display_options['fields']['field_rank']['field'] = 'field_rank';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'orthoptera' => 'orthoptera'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/taxa';
  $translatables['r_taxa'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('id'),
    t('.'),
    t('taxon'),
    t('Unit name 1'),
    t('Unit name 2'),
    t('Unit name 3'),
    t('Unit name 4'),
    t('Rank'),
    t('Data export')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'R types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Type';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'annotation_types' => 'annotation_types'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/types';
  $translatables['r_types'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Type'),
    t('Term ID'),
    t('.'),
    t(','),
    t('Data export')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'recording_in_collections';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'recording_in_collections';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
  /* Relationship: Content: Recordings (field_recordings) */
  $handler->display->display_options['relationships']['field_recordings_nid']['id'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['table'] = 'field_data_field_recordings';
  $handler->display->display_options['relationships']['field_recordings_nid']['field'] = 'field_recordings_nid';
  $handler->display->display_options['relationships']['field_recordings_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_recordings_nid']['delta'] = '-1';
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
  /* Contextual filter: Content: Recordings (field_recordings) */
  $handler->display->display_options['arguments']['field_recordings_nid']['id'] = 'field_recordings_nid';
  $handler->display->display_options['arguments']['field_recordings_nid']['table'] = 'field_data_field_recordings';
  $handler->display->display_options['arguments']['field_recordings_nid']['field'] = 'field_recordings_nid';
  $handler->display->display_options['arguments']['field_recordings_nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_recordings_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_recordings_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_recordings_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_recordings_nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  $translatables['recording_in_collections'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('field_recordings'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'recordings_from_location';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'recordings from location';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /*
                            * Edit this to true to make a default view disabled
                            * initially
                            */
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
    'field_taxonomic_name' => 'field_taxonomic_name',
    'field_date_collected' => 'field_date_collected'
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
    'field_date_collected' => array(
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
  $handler->display->display_options['header']['area_text_custom']['content'] = '<h2>Recordings made here</h2>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Recording name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Species */
  $handler->display->display_options['fields']['field_species']['id'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['table'] = 'field_data_field_species';
  $handler->display->display_options['fields']['field_species']['field'] = 'field_species';
  $handler->display->display_options['fields']['field_species']['type'] = 'taxonomy_term_reference_link';
  $handler->display->display_options['fields']['field_species']['delta_offset'] = '0';
  /* Field: Content: Recording */
  $handler->display->display_options['fields']['field_recording']['id'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['table'] = 'field_data_field_recording';
  $handler->display->display_options['fields']['field_recording']['field'] = 'field_recording';
  $handler->display->display_options['fields']['field_recording']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_recording']['type'] = 'mediaelement_audio';
  $handler->display->display_options['fields']['field_recording']['settings'] = array(
    'controls' => 1,
    'width' => '420',
    'height' => '30',
    'download_link' => 0,
    'download_text' => 'Download'
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Location (field_recording_location) */
  $handler->display->display_options['arguments']['field_recording_location_nid']['id'] = 'field_recording_location_nid';
  $handler->display->display_options['arguments']['field_recording_location_nid']['table'] = 'field_data_field_recording_location';
  $handler->display->display_options['arguments']['field_recording_location_nid']['field'] = 'field_recording_location_nid';
  $handler->display->display_options['arguments']['field_recording_location_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_recording_location_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_recording_location_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_recording_location_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_recording_location_nid']['summary_options']['items_per_page'] = '25';
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
  $translatables['recordings_from_location'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h2>Recordings made here</h2>'),
    t('No results found.'),
    t('Recording name'),
    t('Species'),
    t('Recording'),
    t('All')
  );
  $views[] = $view;
  $view = new view();
  $view->name = 'r_acoustic_traits';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'R/acoustic_traits';
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
      'field_call_type' => 'field_call_type',
      'field_sex_trait' => 'field_sex_trait',
      'field_temperature' => 'field_temperature',
      'field_trait' => 'field_trait',
      'field_value' => 'field_value',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
      'title' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
      'field_call_type' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
      'field_sex_trait' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
      'field_temperature' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
      'field_trait' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
      'field_value' => array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      ),
  );
  /* Relationship: Content: Bioacoustic Traits (field_bioacoustic_traits) */
  $handler->display->display_options['relationships']['field_bioacoustic_traits_value']['id'] = 'field_bioacoustic_traits_value';
  $handler->display->display_options['relationships']['field_bioacoustic_traits_value']['table'] = 'field_data_field_bioacoustic_traits';
  $handler->display->display_options['relationships']['field_bioacoustic_traits_value']['field'] = 'field_bioacoustic_traits_value';
  $handler->display->display_options['relationships']['field_bioacoustic_traits_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_bioacoustic_traits_value']['delta'] = '-1';
  /* Relationship: Field collection item: Trait (field_trait) */
  $handler->display->display_options['relationships']['field_trait_tid']['id'] = 'field_trait_tid';
  $handler->display->display_options['relationships']['field_trait_tid']['table'] = 'field_data_field_trait';
  $handler->display->display_options['relationships']['field_trait_tid']['field'] = 'field_trait_tid';
  $handler->display->display_options['relationships']['field_trait_tid']['relationship'] = 'field_bioacoustic_traits_value';
  $handler->display->display_options['relationships']['field_trait_tid']['required'] = TRUE;
  /* Relationship: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['relationships']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['relationship'] = 'field_bioacoustic_traits_value';
  $handler->display->display_options['fields']['item_id']['label'] = 'traitID';
  $handler->display->display_options['fields']['item_id']['separator'] = '';
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['fields']['tid']['label'] = 'taxonID';
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Field collection item: Trait */
  $handler->display->display_options['fields']['field_trait']['id'] = 'field_trait';
  $handler->display->display_options['fields']['field_trait']['table'] = 'field_data_field_trait';
  $handler->display->display_options['fields']['field_trait']['field'] = 'field_trait';
  $handler->display->display_options['fields']['field_trait']['relationship'] = 'field_bioacoustic_traits_value';
  /* Field: Taxonomy term: Ontology Link */
  $handler->display->display_options['fields']['field_ontology_link']['id'] = 'field_ontology_link';
  $handler->display->display_options['fields']['field_ontology_link']['table'] = 'field_data_field_ontology_link';
  $handler->display->display_options['fields']['field_ontology_link']['field'] = 'field_ontology_link';
  $handler->display->display_options['fields']['field_ontology_link']['relationship'] = 'field_trait_tid';
  /* Field: Field collection item: Value */
  $handler->display->display_options['fields']['field_value']['id'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['fields']['field_value']['field'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['relationship'] = 'field_bioacoustic_traits_value';
  /* Field: Field collection item: Call Type */
  $handler->display->display_options['fields']['field_call_type']['id'] = 'field_call_type';
  $handler->display->display_options['fields']['field_call_type']['table'] = 'field_data_field_call_type';
  $handler->display->display_options['fields']['field_call_type']['field'] = 'field_call_type';
  $handler->display->display_options['fields']['field_call_type']['relationship'] = 'field_bioacoustic_traits_value';
  /* Field: Field collection item: Sex */
  $handler->display->display_options['fields']['field_sex_trait']['id'] = 'field_sex_trait';
  $handler->display->display_options['fields']['field_sex_trait']['table'] = 'field_data_field_sex_trait';
  $handler->display->display_options['fields']['field_sex_trait']['field'] = 'field_sex_trait';
  $handler->display->display_options['fields']['field_sex_trait']['relationship'] = 'field_bioacoustic_traits_value';
  /* Field: Field collection item: Temperature */
  $handler->display->display_options['fields']['field_temperature']['id'] = 'field_temperature';
  $handler->display->display_options['fields']['field_temperature']['table'] = 'field_data_field_temperature';
  $handler->display->display_options['fields']['field_temperature']['field'] = 'field_temperature';
  $handler->display->display_options['fields']['field_temperature']['relationship'] = 'field_bioacoustic_traits_value';
  /* Field: Field: Reference */
  $handler->display->display_options['fields']['field_reference']['id'] = 'field_reference';
  $handler->display->display_options['fields']['field_reference']['table'] = 'field_data_field_reference';
  $handler->display->display_options['fields']['field_reference']['field'] = 'field_reference';
  /* Field: Field collection item: Cascade down */
  $handler->display->display_options['fields']['field_cascade_down']['id'] = 'field_cascade_down';
  $handler->display->display_options['fields']['field_cascade_down']['table'] = 'field_data_field_cascade_down';
  $handler->display->display_options['fields']['field_cascade_down']['field'] = 'field_cascade_down';
  $handler->display->display_options['fields']['field_cascade_down']['relationship'] = 'field_bioacoustic_traits_value';
  $handler->display->display_options['fields']['field_cascade_down']['label'] = 'Cascade';
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
      'bioacoustic_traits' => 'bioacoustic_traits',
  );
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'R/bioacoustic_traits';
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '250';
  $translatables['r_acoustic_traits'] = array(
      t('Master'),
      t('more'),
      t('Apply'),
      t('Reset'),
      t('Sort by'),
      t('Asc'),
      t('Desc'),
      t('Items per page'),
      t('- All -'),
      t('Offset'),
      t('« first'),
      t('‹ previous'),
      t('next ›'),
      t('last »'),
      t('field collection item from field_bioacoustic_traits'),
      t('term from field_trait'),
      t('term from field_taxonomic_name'),
      t('traitID'),
      t('.'),
      t('taxonID'),
      t('Taxonomic name'),
      t('Trait'),
      t('Ontology Link'),
      t('Value'),
      t('Call Type'),
      t('Sex'),
      t('Temperature'),
      t('Reference'),
      t('Cascade'),
      t('Data export'),
  );
  $views[] = $view;
  $view = new view();
$view->name = 'aao_orthoptera';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'aao/orthoptera';
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
/* Relationship: Content: Species (field_species) */
$handler->display->display_options['relationships']['field_species_tid']['id'] = 'field_species_tid';
$handler->display->display_options['relationships']['field_species_tid']['table'] = 'field_data_field_species';
$handler->display->display_options['relationships']['field_species_tid']['field'] = 'field_species_tid';
$handler->display->display_options['relationships']['field_species_tid']['required'] = TRUE;
/* Relationship: Taxonomy term: Associated accepted name (field_aan_4) */
$handler->display->display_options['relationships']['field_aan_4_tid']['id'] = 'field_aan_4_tid';
$handler->display->display_options['relationships']['field_aan_4_tid']['table'] = 'field_data_field_aan_4';
$handler->display->display_options['relationships']['field_aan_4_tid']['field'] = 'field_aan_4_tid';
$handler->display->display_options['relationships']['field_aan_4_tid']['relationship'] = 'field_species_tid';
/* Field: Taxonomy term: Unit name 1 */
$handler->display->display_options['fields']['field_unit_name1']['id'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['table'] = 'field_data_field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['field'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 2 */
$handler->display->display_options['fields']['field_unit_name2']['id'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['table'] = 'field_data_field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['field'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name2']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 3 */
$handler->display->display_options['fields']['field_unit_name3']['id'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['table'] = 'field_data_field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['field'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name3']['label'] = 'Taxa';
$handler->display->display_options['fields']['field_unit_name3']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3']['alter']['text'] = '[field_unit_name1] [field_unit_name3]';
$handler->display->display_options['fields']['field_unit_name3']['empty'] = '[field_unit_name1] [field_unit_name2]';
/* Field: Taxonomy term: Unit name 1 */
$handler->display->display_options['fields']['field_unit_name1_1']['id'] = 'field_unit_name1_1';
$handler->display->display_options['fields']['field_unit_name1_1']['table'] = 'field_data_field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1_1']['field'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name1_1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 2 */
$handler->display->display_options['fields']['field_unit_name2_1']['id'] = 'field_unit_name2_1';
$handler->display->display_options['fields']['field_unit_name2_1']['table'] = 'field_data_field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2_1']['field'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name2_1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 3 */
$handler->display->display_options['fields']['field_unit_name3_1']['id'] = 'field_unit_name3_1';
$handler->display->display_options['fields']['field_unit_name3_1']['table'] = 'field_data_field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3_1']['field'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name3_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3_1']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3_1']['alter']['text'] = '[field_unit_name1_1] [field_unit_name3_1]';
$handler->display->display_options['fields']['field_unit_name3_1']['alter']['trim_whitespace'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3_1']['empty'] = '[field_unit_name1_1] [field_unit_name2_1]';
$handler->display->display_options['fields']['field_unit_name3_1']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3_1']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_unit_name3_1']['settings'] = array(
  'trim_length' => '600',
);
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = 'SupertreeTaxon';
$handler->display->display_options['fields']['nothing']['alter']['text'] = '[field_unit_name3_1]';
$handler->display->display_options['fields']['nothing']['empty'] = '[field_unit_name3]';
$handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
/* Field: Taxonomy term: Term ID */
$handler->display->display_options['fields']['tid']['id'] = 'tid';
$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['tid']['field'] = 'tid';
$handler->display->display_options['fields']['tid']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['tid']['label'] = 'TaxonID';
$handler->display->display_options['fields']['tid']['exclude'] = TRUE;
$handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['tid']['separator'] = '';
/* Field: Taxonomy term: Term ID */
$handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
$handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['tid_1']['field'] = 'tid';
$handler->display->display_options['fields']['tid_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['tid_1']['label'] = 'taxon_id';
$handler->display->display_options['fields']['tid_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['tid_1']['empty'] = '[tid]';
$handler->display->display_options['fields']['tid_1']['empty_zero'] = TRUE;
$handler->display->display_options['fields']['tid_1']['hide_alter_empty'] = FALSE;
$handler->display->display_options['fields']['tid_1']['separator'] = '';
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Content: Has taxonomy term ID (with depth) */
$handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
$handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
$handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'default';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_options']['argument'] = '56';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '10';
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
  'recording' => 'recording',
);
/* Filter criterion: Taxonomy term: Rank (field_rank) */
$handler->display->display_options['filters']['field_rank_value']['id'] = 'field_rank_value';
$handler->display->display_options['filters']['field_rank_value']['table'] = 'field_data_field_rank';
$handler->display->display_options['filters']['field_rank_value']['field'] = 'field_rank_value';
$handler->display->display_options['filters']['field_rank_value']['relationship'] = 'field_species_tid';
$handler->display->display_options['filters']['field_rank_value']['value'] = array(
  'Species' => 'Species',
);

/* Display: Data export */
$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
$handler->display->display_options['style_options']['provide_file'] = 0;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['trim'] = 0;
$handler->display->display_options['style_options']['replace_newlines'] = 0;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['style_options']['keep_html'] = 0;
$handler->display->display_options['path'] = 'aao/orthoptera';
$translatables['aao_orthoptera'] = array(
  t('Master'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« first'),
  t('‹ previous'),
  t('next ›'),
  t('last »'),
  t('term from field_species'),
  t('term from field_aan_4'),
  t('Unit name 1'),
  t('Unit name 2'),
  t('Taxa'),
  t('[field_unit_name1] [field_unit_name3]'),
  t('[field_unit_name1] [field_unit_name2]'),
  t('Unit name 3'),
  t('[field_unit_name1_1] [field_unit_name3_1]'),
  t('[field_unit_name1_1] [field_unit_name2_1]'),
  t('SupertreeTaxon'),
  t('[field_unit_name3_1]'),
  t('[field_unit_name3]'),
  t('TaxonID'),
  t('.'),
  t('taxon_id'),
  t('[tid]'),
  t('All'),
  t('Data export'),
);
$views[] = $view;
$view = new view();
$view->name = 'aao_orthoptera_subspecies';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'aao/orthoptera/subspecies';
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
/* Relationship: Content: Species (field_species) */
$handler->display->display_options['relationships']['field_species_tid']['id'] = 'field_species_tid';
$handler->display->display_options['relationships']['field_species_tid']['table'] = 'field_data_field_species';
$handler->display->display_options['relationships']['field_species_tid']['field'] = 'field_species_tid';
$handler->display->display_options['relationships']['field_species_tid']['required'] = TRUE;
/* Relationship: Taxonomy term: Associated accepted name (field_aan_4) */
$handler->display->display_options['relationships']['field_aan_4_tid']['id'] = 'field_aan_4_tid';
$handler->display->display_options['relationships']['field_aan_4_tid']['table'] = 'field_data_field_aan_4';
$handler->display->display_options['relationships']['field_aan_4_tid']['field'] = 'field_aan_4_tid';
$handler->display->display_options['relationships']['field_aan_4_tid']['relationship'] = 'field_species_tid';
/* Field: Taxonomy term: Unit name 1 */
$handler->display->display_options['fields']['field_unit_name1']['id'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['table'] = 'field_data_field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['field'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 2 */
$handler->display->display_options['fields']['field_unit_name2']['id'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['table'] = 'field_data_field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['field'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name2']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name2']['alter']['trim_whitespace'] = TRUE;
/* Field: Taxonomy term: Unit name 3 */
$handler->display->display_options['fields']['field_unit_name3']['id'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['table'] = 'field_data_field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['field'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name3']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name3']['alter']['text'] = '[field_unit_name1] [field_unit_name3]';
$handler->display->display_options['fields']['field_unit_name3']['alter']['trim_whitespace'] = TRUE;
/* Field: Taxonomy term: Unit name 4 */
$handler->display->display_options['fields']['field_unit_name4']['id'] = 'field_unit_name4';
$handler->display->display_options['fields']['field_unit_name4']['table'] = 'field_data_field_unit_name4';
$handler->display->display_options['fields']['field_unit_name4']['field'] = 'field_unit_name4';
$handler->display->display_options['fields']['field_unit_name4']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['field_unit_name4']['label'] = 'Taxa';
$handler->display->display_options['fields']['field_unit_name4']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name4']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_unit_name4']['alter']['text'] = '[field_unit_name1] [field_unit_name3] [field_unit_name4]';
$handler->display->display_options['fields']['field_unit_name4']['empty'] = '[field_unit_name1] [field_unit_name2] [field_unit_name3]';
/* Field: Taxonomy term: Unit name 1 */
$handler->display->display_options['fields']['field_unit_name1_1']['id'] = 'field_unit_name1_1';
$handler->display->display_options['fields']['field_unit_name1_1']['table'] = 'field_data_field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1_1']['field'] = 'field_unit_name1';
$handler->display->display_options['fields']['field_unit_name1_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name1_1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 2 */
$handler->display->display_options['fields']['field_unit_name2_1']['id'] = 'field_unit_name2_1';
$handler->display->display_options['fields']['field_unit_name2_1']['table'] = 'field_data_field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2_1']['field'] = 'field_unit_name2';
$handler->display->display_options['fields']['field_unit_name2_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name2_1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 3 */
$handler->display->display_options['fields']['field_unit_name3_1']['id'] = 'field_unit_name3_1';
$handler->display->display_options['fields']['field_unit_name3_1']['table'] = 'field_data_field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3_1']['field'] = 'field_unit_name3';
$handler->display->display_options['fields']['field_unit_name3_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name3_1']['exclude'] = TRUE;
/* Field: Taxonomy term: Unit name 4 */
$handler->display->display_options['fields']['field_unit_name4_1']['id'] = 'field_unit_name4_1';
$handler->display->display_options['fields']['field_unit_name4_1']['table'] = 'field_data_field_unit_name4';
$handler->display->display_options['fields']['field_unit_name4_1']['field'] = 'field_unit_name4';
$handler->display->display_options['fields']['field_unit_name4_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['field_unit_name4_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_unit_name4_1']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_unit_name4_1']['alter']['text'] = '[field_unit_name1_1] [field_unit_name3_1] [field_unit_name4_1]';
$handler->display->display_options['fields']['field_unit_name4_1']['alter']['trim_whitespace'] = TRUE;
$handler->display->display_options['fields']['field_unit_name4_1']['empty'] = '[field_unit_name1_1] [field_unit_name2_1] [field_unit_name3_1]';
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = 'SupertreeTaxon';
$handler->display->display_options['fields']['nothing']['alter']['text'] = '[field_unit_name4_1]';
$handler->display->display_options['fields']['nothing']['empty'] = '[field_unit_name4]';
/* Field: Taxonomy term: Term ID */
$handler->display->display_options['fields']['tid']['id'] = 'tid';
$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['tid']['field'] = 'tid';
$handler->display->display_options['fields']['tid']['relationship'] = 'field_species_tid';
$handler->display->display_options['fields']['tid']['label'] = 'TaxonID';
$handler->display->display_options['fields']['tid']['exclude'] = TRUE;
$handler->display->display_options['fields']['tid']['separator'] = '';
/* Field: Taxonomy term: Term ID */
$handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
$handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['tid_1']['field'] = 'tid';
$handler->display->display_options['fields']['tid_1']['relationship'] = 'field_aan_4_tid';
$handler->display->display_options['fields']['tid_1']['label'] = 'taxon_id';
$handler->display->display_options['fields']['tid_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['tid_1']['empty'] = '[tid]';
$handler->display->display_options['fields']['tid_1']['empty_zero'] = TRUE;
$handler->display->display_options['fields']['tid_1']['separator'] = '';
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Content: Has taxonomy term ID (with depth) */
$handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
$handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
$handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'default';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_options']['argument'] = '56';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['term_node_tid_depth']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '10';
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
  'recording' => 'recording',
);
/* Filter criterion: Taxonomy term: Rank (field_rank) */
$handler->display->display_options['filters']['field_rank_value']['id'] = 'field_rank_value';
$handler->display->display_options['filters']['field_rank_value']['table'] = 'field_data_field_rank';
$handler->display->display_options['filters']['field_rank_value']['field'] = 'field_rank_value';
$handler->display->display_options['filters']['field_rank_value']['relationship'] = 'field_species_tid';
$handler->display->display_options['filters']['field_rank_value']['value'] = array(
  'Subspecies' => 'Subspecies',
);

/* Display: Data export */
$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
$handler->display->display_options['style_options']['provide_file'] = 0;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['trim'] = 0;
$handler->display->display_options['style_options']['replace_newlines'] = 0;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['style_options']['keep_html'] = 0;
$handler->display->display_options['path'] = 'aao/orthoptera/subspecies';
$translatables['aao_orthoptera_subspecies'] = array(
  t('Master'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« first'),
  t('‹ previous'),
  t('next ›'),
  t('last »'),
  t('term from field_species'),
  t('term from field_aan_4'),
  t('Unit name 1'),
  t('Unit name 2'),
  t('Unit name 3'),
  t('[field_unit_name1] [field_unit_name3]'),
  t('Taxa'),
  t('[field_unit_name1] [field_unit_name3] [field_unit_name4]'),
  t('[field_unit_name1] [field_unit_name2] [field_unit_name3]'),
  t('Unit name 4'),
  t('[field_unit_name1_1] [field_unit_name3_1] [field_unit_name4_1]'),
  t('[field_unit_name1_1] [field_unit_name2_1] [field_unit_name3_1]'),
  t('SupertreeTaxon'),
  t('[field_unit_name4_1]'),
  t('[field_unit_name4]'),
  t('TaxonID'),
  t('.'),
  t('taxon_id'),
  t('[tid]'),
  t('All'),
  t('Data export'),
);
$views[] = $view;
$view = new view();
$view->name = 'aao_silent_species';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'aao/silent_species';
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
/* Relationship: Content: Bioacoustic Traits (field_bioacoustic_traits) */
$handler->display->display_options['relationships']['field_bioacoustic_traits_value']['id'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['relationships']['field_bioacoustic_traits_value']['table'] = 'field_data_field_bioacoustic_traits';
$handler->display->display_options['relationships']['field_bioacoustic_traits_value']['field'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['relationships']['field_bioacoustic_traits_value']['required'] = TRUE;
$handler->display->display_options['relationships']['field_bioacoustic_traits_value']['delta'] = '-1';
/* Field: Field: Taxonomic name */
$handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
$handler->display->display_options['fields']['field_taxonomic_name']['label'] = 'Taxon';
$handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
/* Field: Field collection item: Trait */
$handler->display->display_options['fields']['field_trait']['id'] = 'field_trait';
$handler->display->display_options['fields']['field_trait']['table'] = 'field_data_field_trait';
$handler->display->display_options['fields']['field_trait']['field'] = 'field_trait';
$handler->display->display_options['fields']['field_trait']['relationship'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['fields']['field_trait']['exclude'] = TRUE;
/* Field: Field collection item: Value */
$handler->display->display_options['fields']['field_value']['id'] = 'field_value';
$handler->display->display_options['fields']['field_value']['table'] = 'field_data_field_value';
$handler->display->display_options['fields']['field_value']['field'] = 'field_value';
$handler->display->display_options['fields']['field_value']['relationship'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['fields']['field_value']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_value']['alter']['text'] = '[field_trait] ([field_value])';
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
  'bioacoustic_traits' => 'bioacoustic_traits',
);
/* Filter criterion: Field collection item: Trait (field_trait) */
$handler->display->display_options['filters']['field_trait_tid']['id'] = 'field_trait_tid';
$handler->display->display_options['filters']['field_trait_tid']['table'] = 'field_data_field_trait';
$handler->display->display_options['filters']['field_trait_tid']['field'] = 'field_trait_tid';
$handler->display->display_options['filters']['field_trait_tid']['relationship'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['filters']['field_trait_tid']['value'] = array(
  1860 => '1860',
);
$handler->display->display_options['filters']['field_trait_tid']['type'] = 'select';
$handler->display->display_options['filters']['field_trait_tid']['vocabulary'] = 'bioacoustics_ontology';
$handler->display->display_options['filters']['field_trait_tid']['hierarchy'] = 1;
/* Filter criterion: Field collection item: Value (field_value) */
$handler->display->display_options['filters']['field_value_value']['id'] = 'field_value_value';
$handler->display->display_options['filters']['field_value_value']['table'] = 'field_data_field_value';
$handler->display->display_options['filters']['field_value_value']['field'] = 'field_value_value';
$handler->display->display_options['filters']['field_value_value']['relationship'] = 'field_bioacoustic_traits_value';
$handler->display->display_options['filters']['field_value_value']['value'] = 'None';

/* Display: Data export */
$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
$handler->display->display_options['path'] = 'aao/silent_species';
$translatables['aao_silent_species'] = array(
  t('Master'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« first'),
  t('‹ previous'),
  t('next ›'),
  t('last »'),
  t('field collection item from field_bioacoustic_traits'),
  t('Taxon'),
  t('Trait'),
  t('Value'),
  t('[field_trait] ([field_value])'),
  t('Data export'),
);
$views[] = $view;
  return $views;
}
