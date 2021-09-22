<?php

function character_editor_views_default_views(){
  $views = array();
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
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '';
  $handler->display->display_options['style_plugin'] = 'character_editor';
  $handler->display->display_options['style_options']['enableColumnResize'] = 1;
  $handler->display->display_options['style_options']['enableColumnReorder'] = 1;
  $handler->display->display_options['style_options']['select_columns'] = 1;
  $handler->display->display_options['style_options']['asyncEditorLoading'] = 1;
  $handler->display->display_options['style_options']['forceFitColumns'] = 0;
  $handler->display->display_options['style_options']['pager'] = 0;
  $handler->display->display_options['style_options']['headerHeight'] = '42';
  $handler->display->display_options['style_options']['viewport_height'] = '500';
  $handler->display->display_options['style_options']['rowHeight'] = '30';
  $handler->display->display_options['style_options']['row_selection_checkbox'] = 0;
  $handler->display->display_options['style_options']['multi_edit'] = 0;
  $handler->display->display_options['style_options']['export_selected_rows'] = 0;
  $handler->display->display_options['style_options']['add'] = array(
    'character_editor_character/controlled' => 'character_editor_character/controlled',
    'character_editor_character/dna' => 'character_editor_character/dna',
    'character_editor_character/group' => 'character_editor_character/group',
    'character_editor_character/numeric' => 'character_editor_character/numeric',
    'character_editor_character/text' => 'character_editor_character/text'
  );
  $handler->display->display_options['style_options']['autoEdit'] = 1;
  /* Relationship: Taxonomy term: Character project using Taxa */
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['id'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['field'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_character_editor_project']['required'] = 1;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Classification';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = 1;
  $handler->display->display_options['fields']['name']['convert_spaces'] = 0;
  /* Contextual filter: Character project: Character project ID */
  $handler->display->display_options['arguments']['id']['id'] = 'id';
  $handler->display->display_options['arguments']['id']['table'] = 'character_editor_project';
  $handler->display->display_options['arguments']['id']['field'] = 'id';
  $handler->display->display_options['arguments']['id']['relationship'] = 'reverse_field_taxonomic_name_character_editor_project';
  $handler->display->display_options['arguments']['id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['id']['not'] = 0;
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
  return $views;
}