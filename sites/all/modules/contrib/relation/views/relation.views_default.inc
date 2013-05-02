<?php

/**
 * @file
 * Relation default views.
 */

/**
 * Relation's default views.
 */
function relation_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'relations';
  $view->description = 'Lists relations of a certain type';
  $view->tag = 'default';
  $view->base_table = 'relation';
  $view->human_name = 'Relations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Relations';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints']['id'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['label'] = '';
  $handler->display->display_options['fields']['endpoints']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['external'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['endpoints']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['endpoints']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['html'] = 0;
  $handler->display->display_options['fields']['endpoints']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['endpoints']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['endpoints']['hide_empty'] = 0;
  $handler->display->display_options['fields']['endpoints']['empty_zero'] = 0;
  $handler->display->display_options['fields']['endpoints']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['endpoints']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints']['group_rows'] = 1;
  $handler->display->display_options['fields']['endpoints']['delta_offset'] = '0';
  $handler->display->display_options['fields']['endpoints']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['endpoints']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['endpoints']['field_api_classes'] = 0;
  /* Contextual filter: Relation: Relation type */
  $handler->display->display_options['arguments']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['table'] = 'relation';
  $handler->display->display_options['arguments']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['relation_type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['relation_type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['relation_type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['relation_type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['relation_type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['relation_type']['glossary'] = 0;
  $handler->display->display_options['arguments']['relation_type']['limit'] = '0';
  $handler->display->display_options['arguments']['relation_type']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['relation_type']['break_phrase'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'relations';
  $views['relations'] = $view;

  return $views;
}
