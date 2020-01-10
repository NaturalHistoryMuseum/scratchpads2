<?php

function scratchpads_species_views_default_views(){
  /*********************************************************************************************
   * 
   * OVERVIEW PAGE
   * 
   ********************************************************************************************/
  $views = array();
  $view = new view();
  $view->name = 'species_nomenclature';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'species nomenclature';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->context = 'species_overview';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Nomenclature';
  $handler->display->display_options['css_class'] = 'alpha grid-4';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Parent */
  $handler->display->display_options['relationships']['parent']['id'] = 'parent';
  $handler->display->display_options['relationships']['parent']['table'] = 'taxonomy_term_hierarchy';
  $handler->display->display_options['relationships']['parent']['field'] = 'parent';
  $handler->display->display_options['relationships']['parent']['ui_name'] = 'Parent';
  $handler->display->display_options['relationships']['parent']['required'] = 0;
  /* Relationship: Grandparent */
  $handler->display->display_options['relationships']['parent_1']['id'] = 'parent_1';
  $handler->display->display_options['relationships']['parent_1']['table'] = 'taxonomy_term_hierarchy';
  $handler->display->display_options['relationships']['parent_1']['field'] = 'parent';
  $handler->display->display_options['relationships']['parent_1']['relationship'] = 'parent';
  $handler->display->display_options['relationships']['parent_1']['ui_name'] = 'Grandparent';
  $handler->display->display_options['relationships']['parent_1']['label'] = 'Grandparent';
  $handler->display->display_options['relationships']['parent_1']['required'] = 0;
  /* Relationship: Taxonomy term: Reference (field_reference) */
  $handler->display->display_options['relationships']['field_reference_nid']['id'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['table'] = 'field_data_field_reference';
  $handler->display->display_options['relationships']['field_reference_nid']['field'] = 'field_reference_nid';
  $handler->display->display_options['relationships']['field_reference_nid']['required'] = 0;
  $handler->display->display_options['relationships']['field_reference_nid']['delta'] = '-1';
  /* Field: Grandparent : TID */
  $handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
  $handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid_1']['field'] = 'tid';
  $handler->display->display_options['fields']['tid_1']['relationship'] = 'parent_1';
  $handler->display->display_options['fields']['tid_1']['ui_name'] = 'Grandparent : TID';
  $handler->display->display_options['fields']['tid_1']['label'] = '';
  $handler->display->display_options['fields']['tid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tid_1']['separator'] = '';
  $handler->display->display_options['fields']['tid_1']['format_plural'] = 0;
  /* Field: Taxonomy term: Rank */
  $handler->display->display_options['fields']['field_rank']['id'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['table'] = 'field_data_field_rank';
  $handler->display->display_options['fields']['field_rank']['field'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank']['relationship'] = 'parent_1';
  $handler->display->display_options['fields']['field_rank']['label'] = '';
  $handler->display->display_options['fields']['field_rank']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_rank']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_rank']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_rank']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_rank']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_rank']['element_type'] = 'strong';
  $handler->display->display_options['fields']['field_rank']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_rank']['element_wrapper_class'] = 'label';
  $handler->display->display_options['fields']['field_rank']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['field_rank']['empty'] = 'Unranked';
  $handler->display->display_options['fields']['field_rank']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_rank']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_rank']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_rank']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  $handler->display->display_options['fields']['name_2']['relationship'] = 'parent_1';
  $handler->display->display_options['fields']['name_2']['label'] = '';
  $handler->display->display_options['fields']['name_2']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['name_2']['alter']['text'] = '<strong>[field_rank]:</strong> <a href="/taxonomy/term/[tid_1]" title="[name_2]">[name_2]</a>';
  $handler->display->display_options['fields']['name_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_2']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name_2']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['name_2']['convert_spaces'] = 0;
  /* Field: Parent: TID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'parent';
  $handler->display->display_options['fields']['tid']['ui_name'] = 'Parent: TID';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  $handler->display->display_options['fields']['tid']['format_plural'] = 0;
  /* Field: Taxonomy term: Rank */
  $handler->display->display_options['fields']['field_rank_1']['id'] = 'field_rank_1';
  $handler->display->display_options['fields']['field_rank_1']['table'] = 'field_data_field_rank';
  $handler->display->display_options['fields']['field_rank_1']['field'] = 'field_rank';
  $handler->display->display_options['fields']['field_rank_1']['relationship'] = 'parent';
  $handler->display->display_options['fields']['field_rank_1']['label'] = '';
  $handler->display->display_options['fields']['field_rank_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_rank_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['text'] = '[field_rank_1]:';
  $handler->display->display_options['fields']['field_rank_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_rank_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_rank_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_rank_1']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['empty'] = 'Unranked';
  $handler->display->display_options['fields']['field_rank_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_rank_1']['field_api_classes'] = 0;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'parent';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['name_1']['alter']['text'] = '<strong>[field_rank_1]:</strong> <a href="/taxonomy/term/[tid]" title="[name_1]">[name_1]</a>';
  $handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name_1']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['name_1']['convert_spaces'] = 0;
  /* Contextual filter: Taxonomy term: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['count'] = FALSE;
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['tid']['validate_options']['transform'] = 0;
  $handler->display->display_options['arguments']['tid']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['tid']['not'] = 0;
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  /* Display: Attachment: Rendered Term */
  $handler = $view->new_display('attachment', 'Attachment: Rendered Term', 'attachment_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['displays'] = array(
    'block' => 'block',
    'default' => 0
  );
  $handler->display->display_options['attachment_position'] = 'after';
  /* Display: Attachment: Synonyms */
  $handler = $view->new_display('attachment', 'Attachment: Synonyms', 'attachment_1');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Parent */
  $handler->display->display_options['relationships']['parent']['id'] = 'parent';
  $handler->display->display_options['relationships']['parent']['table'] = 'taxonomy_term_hierarchy';
  $handler->display->display_options['relationships']['parent']['field'] = 'parent';
  $handler->display->display_options['relationships']['parent']['ui_name'] = 'Parent';
  $handler->display->display_options['relationships']['parent']['required'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['tid']['label'] = 'Synonyms';
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['tid']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 1;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 1;
  $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  $handler->display->display_options['fields']['tid']['format_plural'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Taxonomy term: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['relationship'] = 'parent';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['count'] = FALSE;
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['tid']['validate_options']['transform'] = 0;
  $handler->display->display_options['arguments']['tid']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['tid']['not'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Taxonomy term: Usage (field_usage) */
  $handler->display->display_options['filters']['field_usage_value']['id'] = 'field_usage_value';
  $handler->display->display_options['filters']['field_usage_value']['table'] = 'field_data_field_usage';
  $handler->display->display_options['filters']['field_usage_value']['field'] = 'field_usage_value';
  $handler->display->display_options['filters']['field_usage_value']['value'] = array(
    'invalid' => 'invalid',
    'not accepted' => 'not accepted'
  );
  $handler->display->display_options['filters']['field_usage_value']['group'] = 2;
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'block' => 'block'
  );
  $handler->display->display_options['attachment_position'] = 'after';
  /* Display: Attachment: Type specimens */
  $handler = $view->new_display('attachment', 'Attachment: Type specimens', 'attachment_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Type specimens';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_type_status' => 'field_type_status',
    'title' => 'title'
  );
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content using Taxonomic name */
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['id'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_field_taxonomic_name_node']['field'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Type status */
  $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['field_type_status']['label'] = '';
  $handler->display->display_options['fields']['field_type_status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_type_status']['alter']['text'] = '<strong>[field_type_status-value]:</strong>';
  $handler->display->display_options['fields']['field_type_status']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Contextual filter: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Filter criterion: Content: Type status (field_type_status) */
  $handler->display->display_options['filters']['field_type_status_value']['id'] = 'field_type_status_value';
  $handler->display->display_options['filters']['field_type_status_value']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['filters']['field_type_status_value']['field'] = 'field_type_status_value';
  $handler->display->display_options['filters']['field_type_status_value']['relationship'] = 'reverse_field_taxonomic_name_node';
  $handler->display->display_options['filters']['field_type_status_value']['operator'] = 'not empty';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'block' => 'block'
  );
  $handler->display->display_options['attachment_position'] = 'after';
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'species_slideshow';
  $view->description = '';
  $view->context = 'species_overview';
  $view->base_table = 'file_managed';
  $view->human_name = 'Species slideshow';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Media';
  $handler->display->display_options['css_class'] = 'omega grid-4';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slideshow';
  $handler->display->display_options['style_options']['slideshow_type'] = 'views_slideshow_cycle';
  $handler->display->display_options['style_options']['slideshow_skin'] = 'default';
  $handler->display->display_options['style_options']['skin_info'] = array(
    'class' => 'default',
    'name' => 'Default',
    'module' => 'views_slideshow',
    'path' => '',
    'stylesheets' => array()
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'rendered_entity' => 0,
    'rendered_entity_1' => 0,
    'filename' => 0
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'rendered_entity_1' => 'rendered_entity_1',
    'rendered_entity' => 0,
    'filename' => 0
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_hover'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['effect'] = 'none';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['transition_advanced'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['wait_for_image_load_timeout'] = '3000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'file_styles_species_slideshow_large';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 1;
  /* Field: File: Rendered File */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_file';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'file_styles_species_slideshow_thumbnail';
  $handler->display->display_options['fields']['rendered_entity_1']['bypass_access'] = 1;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['external'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['filename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['filename']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['filename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['filename']['empty_zero'] = 0;
  $handler->display->display_options['fields']['filename']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['filename']['link_to_file'] = 0;
  /* Sort criterion: File: Sticky at top of lists (field_sticky) */
  $handler->display->display_options['sorts']['field_sticky_value']['id'] = 'field_sticky_value';
  $handler->display->display_options['sorts']['field_sticky_value']['table'] = 'field_data_field_sticky';
  $handler->display->display_options['sorts']['field_sticky_value']['field'] = 'field_sticky_value';
  $handler->display->display_options['sorts']['field_sticky_value']['order'] = 'DESC';
  /* Sort criterion: File: Weight (field_weight) */
  $handler->display->display_options['sorts']['field_weight_value']['id'] = 'field_weight_value';
  $handler->display->display_options['sorts']['field_weight_value']['table'] = 'field_data_field_weight';
  $handler->display->display_options['sorts']['field_weight_value']['field'] = 'field_weight_value';
  /* Contextual filter: Field: Taxonomic name (field_taxonomic_name) */
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['field_taxonomic_name_tid']['not'] = 0;
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image'
  );
  /* Filter criterion: File: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'file_managed';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1'
  );
  /* Display: Latest images */
  $handler = $view->new_display('block', 'Latest images', 'block');
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'species_description';
  $view->description = '';
  $view->context = 'species_overview';
  $view->base_table = 'node';
  $view->human_name = 'Species description';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'SUMMARY';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: General description */
  $handler->display->display_options['fields']['field_general_description']['id'] = 'field_general_description';
  $handler->display->display_options['fields']['field_general_description']['table'] = 'field_data_field_general_description';
  $handler->display->display_options['fields']['field_general_description']['field'] = 'field_general_description';
  $handler->display->display_options['fields']['field_general_description']['label'] = '';
  $handler->display->display_options['fields']['field_general_description']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_general_description']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_general_description']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_general_description']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_general_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_general_description']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_general_description']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_general_description']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_general_description']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_general_description']['field_api_classes'] = 0;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
    'spm' => 'spm'
  );
  /* Filter criterion: Content: General description (field_general_description) */
  $handler->display->display_options['filters']['field_general_description_value']['id'] = 'field_general_description_value';
  $handler->display->display_options['filters']['field_general_description_value']['table'] = 'field_data_field_general_description';
  $handler->display->display_options['filters']['field_general_description_value']['field'] = 'field_general_description_value';
  $handler->display->display_options['filters']['field_general_description_value']['operator'] = 'not empty';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'species_overview_map';
  $view->description = '';
  $view->tag = 'default';
  $view->context = 'species_overview';
  $view->base_table = 'node';
  $view->human_name = 'Species overview map';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Distribution';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['label'] = 'Specimen-Location';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = 0;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_spm_map']['id'] = 'field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['table'] = 'field_data_field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['field'] = 'field_spm_map';
  $handler->display->display_options['fields']['field_spm_map']['label'] = '';
  $handler->display->display_options['fields']['field_spm_map']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_spm_map']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_spm_map']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['click_sort_column'] = 'region_id';
  $handler->display->display_options['fields']['field_spm_map']['type'] = 'gm3_text';
  $handler->display->display_options['fields']['field_spm_map']['settings'] = array(
    'display_convex_hull' => '0'
  );
  $handler->display->display_options['fields']['field_spm_map']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_spm_map']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_spm_map']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_spm_map']['field_api_classes'] = 0;
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['label'] = '';
  $handler->display->display_options['fields']['field_map']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_map']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_map']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_map']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_map']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_map']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_map']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_map']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_map']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_map']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => '0'
  );
  $handler->display->display_options['fields']['field_map']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Specimen/Observation';
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
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
    'specimen_observation' => 'specimen_observation',
    'spm' => 'spm'
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Map (field_spm_map:gm3_type) */
  $handler->display->display_options['filters']['field_spm_map_gm3_type']['id'] = 'field_spm_map_gm3_type';
  $handler->display->display_options['filters']['field_spm_map_gm3_type']['table'] = 'field_data_field_spm_map';
  $handler->display->display_options['filters']['field_spm_map_gm3_type']['field'] = 'field_spm_map_gm3_type';
  $handler->display->display_options['filters']['field_spm_map_gm3_type']['operator'] = 'not empty';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'species_overview_biblio';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Species overview biblio';
  $view->context = 'species_overview';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Literature';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega biblio';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'More';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['style_options']['columns'] = array(
    'biblio_authors' => 'biblio_authors',
    'title' => 'title',
    'biblio_year' => 'biblio_year'
  );
  $handler->display->display_options['style_options']['default'] = 'biblio_authors';
  $handler->display->display_options['style_options']['info'] = array(
    'biblio_authors' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
    'biblio_year' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
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
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_year']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_year']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_year']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_year']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_year']['biblio_label'] = 1;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
    'biblio' => 'biblio'
  );
  $views[$view->name] = $view;
  if(module_exists('darwincore')){
    $view = new view();
    $view->name = 'species_specimens_overview';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'node';
    $view->human_name = 'Species specimens overview';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->context = 'species_overview';
    $view->disabled = FALSE;
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Specimen count';
    $handler->display->display_options['css_class'] = 'grid-8 alpha omega';
    $handler->display->display_options['group_by'] = TRUE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '1';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['group_rendered'] = 1;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['hide_empty'] = 1;
    $handler->display->display_options['row_options']['default_field_elements'] = 1;
    /* Field: COUNT(DISTINCT Content: Nid) */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'taxonomy/term/!1/specimens';
    $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['prefix'] = 'There ';
    $handler->display->display_options['fields']['nid']['alter']['suffix'] = ' for this taxon.';
    $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['nid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['nid']['separator'] = '';
    $handler->display->display_options['fields']['nid']['format_plural'] = 1;
    $handler->display->display_options['fields']['nid']['format_plural_singular'] = 'is 1 specimen';
    $handler->display->display_options['fields']['nid']['format_plural_plural'] = 'are @count specimens';
    /* Contextual filter: Content: Has taxonomy term ID */
    $handler->display->display_options['arguments']['tid']['id'] = 'tid';
    $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
    $handler->display->display_options['arguments']['tid']['field'] = 'tid';
    $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
    $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
    $handler->display->display_options['arguments']['tid']['add_table'] = 0;
    $handler->display->display_options['arguments']['tid']['require_value'] = 0;
    $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
    $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
      'specimen_observation' => 'specimen_observation'
    );
    /* Display: Specimen count */
    $handler = $view->new_display('block', 'Specimen count', 'block');
    $handler->display->display_options['defaults']['empty'] = FALSE;
    /* Display: Paratype count */
    $handler = $view->new_display('attachment', 'Paratype count', 'attachment_1');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Paratype count';
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: COUNT(DISTINCT Content: Nid) */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'taxonomy/term/!1/specimens?field_type_status=paratype';
    $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['prefix'] = 'There ';
    $handler->display->display_options['fields']['nid']['alter']['suffix'] = ' for this taxon.';
    $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['nid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['nid']['separator'] = '';
    $handler->display->display_options['fields']['nid']['format_plural'] = 1;
    $handler->display->display_options['fields']['nid']['format_plural_singular'] = 'is 1 paratype';
    $handler->display->display_options['fields']['nid']['format_plural_plural'] = 'are @count paratypes';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
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
      'specimen_observation' => 'specimen_observation'
    );
    /* Filter criterion: Content: Type status (field_type_status) */
    $handler->display->display_options['filters']['field_type_status_value']['id'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['table'] = 'field_data_field_type_status';
    $handler->display->display_options['filters']['field_type_status_value']['field'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['value'] = array(
      'paratype' => 'paratype'
    );
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block'
    );
    $handler->display->display_options['attachment_position'] = 'after';
    /* Display: Syntype count */
    $handler = $view->new_display('attachment', 'Syntype count', 'attachment_3');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Syntype count';
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: COUNT(DISTINCT Content: Nid) */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'taxonomy/term/!1/specimens?field_type_status=syntype';
    $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['prefix'] = 'There ';
    $handler->display->display_options['fields']['nid']['alter']['suffix'] = ' for this taxon.';
    $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nid']['empty'] = '0';
    $handler->display->display_options['fields']['nid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['nid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['nid']['separator'] = '';
    $handler->display->display_options['fields']['nid']['format_plural'] = 1;
    $handler->display->display_options['fields']['nid']['format_plural_singular'] = 'is 1 syntype';
    $handler->display->display_options['fields']['nid']['format_plural_plural'] = 'are @count syntypes';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
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
      'specimen_observation' => 'specimen_observation'
    );
    /* Filter criterion: Content: Type status (field_type_status) */
    $handler->display->display_options['filters']['field_type_status_value']['id'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['table'] = 'field_data_field_type_status';
    $handler->display->display_options['filters']['field_type_status_value']['field'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['value'] = array(
      'syntype' => 'syntype'
    );
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block'
    );
    $handler->display->display_options['attachment_position'] = 'after';
    /* Display: Specimen */
    $handler = $view->new_display('attachment', 'Specimen', 'attachment_2');
    $handler->display->display_options['defaults']['group_by'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '1';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Content: Basis of record */
    $handler->display->display_options['fields']['field_basis_of_record']['id'] = 'field_basis_of_record';
    $handler->display->display_options['fields']['field_basis_of_record']['table'] = 'field_data_field_basis_of_record';
    $handler->display->display_options['fields']['field_basis_of_record']['field'] = 'field_basis_of_record';
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['external'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['alter']['html'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['element_type'] = '0';
    $handler->display->display_options['fields']['field_basis_of_record']['element_label_type'] = 'strong';
    $handler->display->display_options['fields']['field_basis_of_record']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['field_basis_of_record']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['field_basis_of_record']['hide_empty'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['empty_zero'] = 0;
    $handler->display->display_options['fields']['field_basis_of_record']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['field_basis_of_record']['field_api_classes'] = 0;
    /* Field: Content: Catalogue number */
    $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
    $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
    $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['external'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['alter']['html'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['element_type'] = '0';
    $handler->display->display_options['fields']['field_catalogue_number']['element_label_type'] = 'strong';
    $handler->display->display_options['fields']['field_catalogue_number']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['field_catalogue_number']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['field_catalogue_number']['hide_empty'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['empty_zero'] = 0;
    $handler->display->display_options['fields']['field_catalogue_number']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['field_catalogue_number']['field_api_classes'] = 0;
    /* Field: Content: Collection code */
    $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
    $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
    $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
    $handler->display->display_options['fields']['field_collection_code']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['external'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['field_collection_code']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['field_collection_code']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['alter']['html'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['element_type'] = '0';
    $handler->display->display_options['fields']['field_collection_code']['element_label_type'] = 'strong';
    $handler->display->display_options['fields']['field_collection_code']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['field_collection_code']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['field_collection_code']['hide_empty'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['empty_zero'] = 0;
    $handler->display->display_options['fields']['field_collection_code']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['field_collection_code']['field_api_classes'] = 0;
    /* Field: Content: Institution code */
    $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
    $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
    $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
    $handler->display->display_options['fields']['field_institution_code']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['external'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['field_institution_code']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['field_institution_code']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['alter']['html'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['element_type'] = '0';
    $handler->display->display_options['fields']['field_institution_code']['element_label_type'] = 'strong';
    $handler->display->display_options['fields']['field_institution_code']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['field_institution_code']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['field_institution_code']['hide_empty'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['empty_zero'] = 0;
    $handler->display->display_options['fields']['field_institution_code']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['field_institution_code']['field_api_classes'] = 0;
    /* Field: Content: Type status */
    $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
    $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
    $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
    $handler->display->display_options['fields']['field_type_status']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['external'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['field_type_status']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['field_type_status']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['field_type_status']['alter']['html'] = 0;
    $handler->display->display_options['fields']['field_type_status']['element_type'] = '0';
    $handler->display->display_options['fields']['field_type_status']['element_label_type'] = 'strong';
    $handler->display->display_options['fields']['field_type_status']['element_label_colon'] = 1;
    $handler->display->display_options['fields']['field_type_status']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['field_type_status']['hide_empty'] = 0;
    $handler->display->display_options['fields']['field_type_status']['empty_zero'] = 0;
    $handler->display->display_options['fields']['field_type_status']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['field_type_status']['field_api_classes'] = 0;
    $handler->display->display_options['defaults']['filters'] = FALSE;
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
      'specimen_observation' => 'specimen_observation'
    );
    /* Filter criterion: Content: Type status (field_type_status) */
    $handler->display->display_options['filters']['field_type_status_value']['id'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['table'] = 'field_data_field_type_status';
    $handler->display->display_options['filters']['field_type_status_value']['field'] = 'field_type_status_value';
    $handler->display->display_options['filters']['field_type_status_value']['value'] = array(
      'holotype' => 'holotype',
      'lectotype' => 'lectotype',
      'neotype' => 'neotype'
    );
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block',
      'block_1' => 0
    );
    $views[$view->name] = $view;
  }
  /*********************************************************************************************
   * 
   * BIBLIO PAGE
   * 
   ********************************************************************************************/
  $view = new view();
  $view->name = 'species_biblio';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Species biblio';
  $view->context = 'species_literature';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Literature';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega biblio';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'More';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['style_options']['columns'] = array(
    'biblio_authors' => 'biblio_authors',
    'title' => 'title',
    'biblio_year' => 'biblio_year'
  );
  $handler->display->display_options['style_options']['default'] = 'biblio_year';
  $handler->display->display_options['style_options']['info'] = array(
    'biblio_authors' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'biblio_year' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0
    )
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
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
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_year']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_year']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_year']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_year']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_year']['biblio_label'] = 1;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
    'biblio' => 'biblio'
  );
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  /*********************************************************************************************
   * 
   * MAPS PAGE
   * 
   ********************************************************************************************/
  $view = new view();
  $view->name = 'species_map';
  $view->description = '';
  $view->tag = 'default';
  $view->context = 'species_maps';
  $view->base_table = 'node';
  $view->human_name = 'Species map';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Distribution';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['label'] = '';
  $handler->display->display_options['fields']['field_map']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_map']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => FALSE,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Collector */
  $handler->display->display_options['fields']['field_collector']['id'] = 'field_collector';
  $handler->display->display_options['fields']['field_collector']['table'] = 'field_data_field_collector';
  $handler->display->display_options['fields']['field_collector']['field'] = 'field_collector';
  $handler->display->display_options['fields']['field_collector']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_collector']['delta_offset'] = '0';
  /* Field: Content: Collector number */
  $handler->display->display_options['fields']['field_collector_number']['id'] = 'field_collector_number';
  $handler->display->display_options['fields']['field_collector_number']['table'] = 'field_data_field_collector_number';
  $handler->display->display_options['fields']['field_collector_number']['field'] = 'field_collector_number';
  $handler->display->display_options['fields']['field_collector_number']['hide_empty'] = TRUE;
  /* Field: Content: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_date_collected']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Basis of record */
  $handler->display->display_options['fields']['field_basis_of_record']['id'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['table'] = 'field_data_field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['field'] = 'field_basis_of_record';
  /* Field: Content: Coordinate uncertainty in meters */
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['id'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['table'] = 'field_data_field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['field'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['label'] = 'Coordinate uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = TRUE;
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
    'specimen_observation' => 'specimen_observation',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;
  /*********************************************************************************************
   * 
   * SPECIMENS PAGE
   * 
   ********************************************************************************************/
  $view = new view();
  $view->name = 'species_specimen_observation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Species specimens';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->context = 'species_specimens';
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Specimens';
  $handler->display->display_options['css_class'] = 'alpha omega';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slickgrid';
  $handler->display->display_options['style_options']['grouping_field'] = '';
  $handler->display->display_options['style_options']['collapse_groups_by_default'] = 0;
  $handler->display->display_options['style_options']['enableColumnResize'] = 1;
  $handler->display->display_options['style_options']['enableColumnReorder'] = 1;
  $handler->display->display_options['style_options']['select_columns'] = 1;
  $handler->display->display_options['style_options']['asyncEditorLoading'] = 1;
  $handler->display->display_options['style_options']['forceFitColumns'] = 1;
  $handler->display->display_options['style_options']['pager'] = 0;
  $handler->display->display_options['style_options']['headerHeight'] = '42';
  $handler->display->display_options['style_options']['viewport_height'] = '500';
  $handler->display->display_options['style_options']['rowHeight'] = '30';
  $handler->display->display_options['style_options']['row_selection_checkbox'] = 0;
  $handler->display->display_options['style_options']['multi_edit'] = 0;
  $handler->display->display_options['style_options']['delete'] = 0;
  $handler->display->display_options['style_options']['clone'] = 0;
  $handler->display->display_options['style_options']['export_selected_rows'] = 0;
  $handler->display->display_options['style_options']['undo'] = 0;
  $handler->display->display_options['style_options']['add'] = '0';
  $handler->display->display_options['style_options']['autoEdit'] = 0;
  $handler->display->display_options['style_options']['columns'] = array();
  $handler->display->display_options['style_options']['tabs'] = '';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '0';
  $fields = field_info_instances('node', 'specimen_observation');
  $location_fields = field_info_instances('node', 'location');
  $groups = field_group_info_groups('node', 'specimen_observation', 'form', TRUE);
  usort($groups, 'scratchpads_species_usort_comparison');
  foreach($groups as $group){
    if(isset($group->label) && $group->identifier == 'group_location|node|specimen_observation|form'){
      $handler->display->display_options['style_options']['tabs'] .= $group->label . PHP_EOL;
      foreach($location_fields as $location_field){
        switch($location_field['field_name']){
          case 'field_map':
            $handler->display->display_options['fields']['field_map_latitude']['type'] = 'gm3_text_lat';
            $handler->display->display_options['fields']['field_map_latitude']['id'] = 'field_map_latitude';
            $handler->display->display_options['fields']['field_map_latitude']['table'] = 'field_data_' . $location_field['field_name'];
            $handler->display->display_options['fields']['field_map_latitude']['field'] = $location_field['field_name'];
            $handler->display->display_options['fields']['field_map_latitude']['relationship'] = 'field_location_nid';
            $handler->display->display_options['fields']['field_map_latitude']['label'] = t('Latitude');
            $handler->display->display_options['style_options']['columns']['field_map_latitude'] = array(
              'width' => 120,
              'sortable' => 1,
              'tab' => $group->label,
              'filter' => 'textFilter'
            );
            $handler->display->display_options['fields']['field_map_longitude']['type'] = 'gm3_text_lon';
            $handler->display->display_options['fields']['field_map_longitude']['id'] = 'field_map_longitude';
            $handler->display->display_options['fields']['field_map_longitude']['table'] = 'field_data_' . $location_field['field_name'];
            $handler->display->display_options['fields']['field_map_longitude']['field'] = $location_field['field_name'];
            $handler->display->display_options['fields']['field_map_longitude']['relationship'] = 'field_location_nid';
            $handler->display->display_options['fields']['field_map_longitude']['label'] = t('Longitude');
            $handler->display->display_options['style_options']['columns']['field_map_longitude'] = array(
              'width' => 120,
              'sortable' => 1,
              'tab' => $group->label,
              'filter' => 'textFilter'
            );
            break;
          default:
            $handler->display->display_options['fields'][$location_field['field_name']]['id'] = $location_field['field_name'];
            $handler->display->display_options['fields'][$location_field['field_name']]['table'] = 'field_data_' . $location_field['field_name'];
            $handler->display->display_options['fields'][$location_field['field_name']]['field'] = $location_field['field_name'];
            $handler->display->display_options['fields'][$location_field['field_name']]['relationship'] = 'field_location_nid';
            $handler->display->display_options['fields'][$location_field['field_name']]['label'] = $location_field['label'];
            $handler->display->display_options['style_options']['columns'][$location_field['field_name']] = array(
              'width' => 120,
              'sortable' => 1,
              'tab' => $group->label,
              'filter' => 'textFilter'
            );
        }
      }
    }else{
      if(!(isset($group->children[0]) && !is_string($group->children[0]))){
        if(count(array_intersect($group->children, array_keys($fields)))){
          $handler->display->display_options['style_options']['tabs'] .= $group->label . PHP_EOL;
          foreach($group->children as $field){
            if(array_key_exists($field, $fields)){
              $handler->display->display_options['fields'][$field]['id'] = $field;
              $handler->display->display_options['fields'][$field]['table'] = 'field_data_' . $field;
              $handler->display->display_options['fields'][$field]['field'] = $field;
              $handler->display->display_options['fields'][$field]['label'] = $fields[$field]['label'];
              $handler->display->display_options['fields'][$field]['alter']['alter_text'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['make_link'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['absolute'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['external'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['replace_spaces'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['trim_whitespace'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['nl2br'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['word_boundary'] = 1;
              $handler->display->display_options['fields'][$field]['alter']['ellipsis'] = 1;
              $handler->display->display_options['fields'][$field]['alter']['strip_tags'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['trim'] = 0;
              $handler->display->display_options['fields'][$field]['alter']['html'] = 0;
              $handler->display->display_options['fields'][$field]['element_type'] = '0';
              $handler->display->display_options['fields'][$field]['element_label_type'] = 'strong';
              $handler->display->display_options['fields'][$field]['element_label_colon'] = 1;
              $handler->display->display_options['fields'][$field]['element_default_classes'] = 0;
              $handler->display->display_options['fields'][$field]['hide_empty'] = 0;
              $handler->display->display_options['fields'][$field]['empty_zero'] = 0;
              $handler->display->display_options['fields'][$field]['hide_alter_empty'] = 1;
              $handler->display->display_options['fields'][$field]['field_api_classes'] = 0;
              $handler->display->display_options['style_options']['columns'][$field] = array(
                'width' => 120,
                'sortable' => 1,
                'tab' => $group->label,
                'filter' => 'textFilter'
              );
            }
          }
        }
      }
    }
  }
  /* Field: Node: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = 'Specimen';
  $handler->display->display_options['fields']['view_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['view_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['view_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['view_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['view_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['view_node']['text'] = 'View specimen';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  /* Display: XML Export */
  $handler = $view->new_display('views_data_export', 'XML Export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['path'] = 'taxonomy/term/%/xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'block' => 'block'
  );
  /* Display: CSV Export */
  $handler = $view->new_display('views_data_export', 'CSV Export', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'taxonomy/term/%/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'block' => 'block'
  );
  $views[$view->name] = $view;
  /*********************************************************************************************
   * 
   * DYNAMIC CONTENT TYPES
   * 
   ********************************************************************************************/
  // Create views for and content type selected to display on the species pages
  foreach(scratchpads_species_get_species_node_types() as $node_type => $name){
    $tab = scratchpads_species_variable_get('species_page_' . $node_type . '_tab');
    $view = new view();
    $view->name = 'species_' . $node_type;
    // Ensure we don't overwrite default views
    if(array_key_exists($view->name, $views)){
      continue;
    }
    $view->description = '';
    $view->tag = 'species-node-tab';
    $view->base_table = 'node';
    $view->human_name = 'Species ' . $name;
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
    $view->context = 'species_' . ($tab == 'own' ? $node_type : $tab);
    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = $name;
    $handler->display->display_options['css_class'] = 'alpha omega';
    $handler->display->display_options['use_ajax'] = TRUE;
    $handler->display->display_options['use_more'] = TRUE;
    $handler->display->display_options['use_more_always'] = TRUE;
    $handler->display->display_options['use_more_text'] = 'More';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'mini';
    $handler->display->display_options['pager']['options']['items_per_page'] = '10';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['pager']['options']['id'] = '0';
    $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['group_rendered'] = 1;
    $handler->display->display_options['row_plugin'] = 'entity';
    $handler->display->display_options['row_options']['view_mode'] = 'species';
    /* Sort criterion: Content: Sticky */
    $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
    $handler->display->display_options['sorts']['sticky']['table'] = 'node';
    $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
    $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
    if($node_type == 'spm'){
      /* Sort criterion: Weight: Weight */
      $handler->display->display_options['sorts']['weight']['id'] = 'weight';
      $handler->display->display_options['sorts']['weight']['table'] = 'weight_weights';
      $handler->display->display_options['sorts']['weight']['field'] = 'weight';
    }
    /* Sort criterion: Content: Post date */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'node';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
    /* Contextual filter: Content: Has taxonomy term ID */
    $handler->display->display_options['arguments']['tid']['id'] = 'tid';
    $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
    $handler->display->display_options['arguments']['tid']['field'] = 'tid';
    $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['tid']['title_enable'] = 1;
    $handler->display->display_options['arguments']['tid']['title'] = '<none>';
    $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
    $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['tid']['break_phrase'] = 1;
    $handler->display->display_options['arguments']['tid']['add_table'] = 0;
    $handler->display->display_options['arguments']['tid']['require_value'] = 0;
    $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
    $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
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
      $node_type => $node_type
    );
    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block');
    $views[$view->name] = $view;
    $view = new view();
    $view->name = 'species_media';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'file_managed';
    $view->human_name = 'Species media';
    $view->context = 'species_media';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE;
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Species media';
    $handler->display->display_options['css_class'] = 'view-inline-filters';
    $handler->display->display_options['use_ajax'] = TRUE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '16';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['pager']['options']['id'] = '0';
    $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['wrapper_class'] = 'media-gallery';
    $handler->display->display_options['row_plugin'] = 'file';
    $handler->display->display_options['row_options']['view_mode'] = 'file_styles_square_thumbnail';
    $handler->display->display_options['row_options']['links'] = 1;
    /* Sort criterion: File: Upload date */
    $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['table'] = 'file_managed';
    $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
    /* Contextual filter: Field: Taxonomic name (field_taxonomic_name) */
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['id'] = 'field_taxonomic_name_tid';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['table'] = 'field_data_field_taxonomic_name';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['field'] = 'field_taxonomic_name_tid';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_type'] = 'taxonomy_tid';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['term_page'] = 1;
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['node'] = 0;
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_options']['limit'] = 0;
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['break_phrase'] = 1;
    $handler->display->display_options['arguments']['field_taxonomic_name_tid']['not'] = 0;
    /* Filter criterion: File: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'file_managed';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['exposed'] = TRUE;
    $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
    $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
    $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
    $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
    $handler->display->display_options['filters']['type']['expose']['reduce'] = 0;
    /* Filter criterion: File: Status */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'file_managed';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = array(
      1 => '1'
    );
    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block');
    $handler->display->display_options['defaults']['css_class'] = FALSE;
    $handler->display->display_options['css_class'] = 'view-inline-filters grid-8 alpha omega';
    /* Display: Count images */
    $handler = $view->new_display('attachment', 'Count images', 'attachment_1');
    $handler->display->display_options['defaults']['group_by'] = FALSE;
    $handler->display->display_options['group_by'] = TRUE;
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['wrapper_class'] = 'media-gallery';
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['hide_empty'] = 0;
    $handler->display->display_options['row_options']['default_field_elements'] = 0;
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: File: File ID */
    $handler->display->display_options['fields']['fid']['id'] = 'fid';
    $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
    $handler->display->display_options['fields']['fid']['field'] = 'fid';
    $handler->display->display_options['fields']['fid']['group_type'] = 'count';
    $handler->display->display_options['fields']['fid']['label'] = '';
    $handler->display->display_options['fields']['fid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['text'] = 'Images ([fid])';
    $handler->display->display_options['fields']['fid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['path'] = '#type=image';
    $handler->display->display_options['fields']['fid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['link_class'] = 'view-filter';
    $handler->display->display_options['fields']['fid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['fid']['element_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['fid']['element_wrapper_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_default_classes'] = 0;
    $handler->display->display_options['fields']['fid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['fid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['fid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['fid']['separator'] = '';
    $handler->display->display_options['fields']['fid']['format_plural'] = 0;
    $handler->display->display_options['fields']['fid']['format_plural_singular'] = '1 image';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: File: Status */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'file_managed';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = array(
      1 => '1'
    );
    $handler->display->display_options['filters']['status']['group'] = 2;
    /* Filter criterion: File: Type */
    $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
    $handler->display->display_options['filters']['type_1']['table'] = 'file_managed';
    $handler->display->display_options['filters']['type_1']['field'] = 'type';
    $handler->display->display_options['filters']['type_1']['value'] = array(
      'image' => 'image'
    );
    $handler->display->display_options['filters']['type_1']['group'] = 2;
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block'
    );
    $handler->display->display_options['inherit_exposed_filters'] = 1;
    /* Display: Count video */
    $handler = $view->new_display('attachment', 'Count video', 'attachment_3');
    $handler->display->display_options['defaults']['group_by'] = FALSE;
    $handler->display->display_options['group_by'] = TRUE;
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['wrapper_class'] = 'media-gallery';
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['hide_empty'] = 0;
    $handler->display->display_options['row_options']['default_field_elements'] = 0;
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: File: File ID */
    $handler->display->display_options['fields']['fid']['id'] = 'fid';
    $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
    $handler->display->display_options['fields']['fid']['field'] = 'fid';
    $handler->display->display_options['fields']['fid']['group_type'] = 'count';
    $handler->display->display_options['fields']['fid']['label'] = '';
    $handler->display->display_options['fields']['fid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['text'] = 'Videos ([fid])';
    $handler->display->display_options['fields']['fid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['path'] = '#type=video';
    $handler->display->display_options['fields']['fid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['link_class'] = 'view-filter';
    $handler->display->display_options['fields']['fid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['fid']['element_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['fid']['element_wrapper_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_default_classes'] = 0;
    $handler->display->display_options['fields']['fid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['fid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['fid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['fid']['separator'] = '';
    $handler->display->display_options['fields']['fid']['format_plural'] = 0;
    $handler->display->display_options['fields']['fid']['format_plural_singular'] = '1 image';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: File: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'file_managed';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      'video' => 'video'
    );
    $handler->display->display_options['filters']['type']['group'] = 0;
    /* Filter criterion: File: Status */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'file_managed';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = array(
      1 => '1'
    );
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block'
    );
    $handler->display->display_options['inherit_exposed_filters'] = 1;
    /* Display: Count audio */
    $handler = $view->new_display('attachment', 'Count audio', 'attachment_2');
    $handler->display->display_options['defaults']['group_by'] = FALSE;
    $handler->display->display_options['group_by'] = TRUE;
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['wrapper_class'] = 'media-gallery';
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['hide_empty'] = 0;
    $handler->display->display_options['row_options']['default_field_elements'] = 0;
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: File: File ID */
    $handler->display->display_options['fields']['fid']['id'] = 'fid';
    $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
    $handler->display->display_options['fields']['fid']['field'] = 'fid';
    $handler->display->display_options['fields']['fid']['group_type'] = 'count';
    $handler->display->display_options['fields']['fid']['label'] = '';
    $handler->display->display_options['fields']['fid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['text'] = 'Audio ([fid])';
    $handler->display->display_options['fields']['fid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['path'] = '#type=audio';
    $handler->display->display_options['fields']['fid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['link_class'] = 'view-filter';
    $handler->display->display_options['fields']['fid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['fid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['fid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['fid']['element_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['fid']['element_wrapper_type'] = '0';
    $handler->display->display_options['fields']['fid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['fid']['hide_empty'] = 1;
    $handler->display->display_options['fields']['fid']['empty_zero'] = 1;
    $handler->display->display_options['fields']['fid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['fid']['separator'] = '';
    $handler->display->display_options['fields']['fid']['format_plural'] = 0;
    $handler->display->display_options['fields']['fid']['format_plural_singular'] = '1 image';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: File: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'file_managed';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      'audio' => 'audio'
    );
    $handler->display->display_options['filters']['type']['group'] = 0;
    /* Filter criterion: File: Status */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'file_managed';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = array(
      1 => '1'
    );
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['displays'] = array(
      'default' => 'default',
      'block' => 'block'
    );
    $handler->display->display_options['inherit_exposed_filters'] = 1;
    $views[$view->name] = $view;
  }
  return $views;
}
