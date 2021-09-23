<?php

function publication_views_default_views(){
  $views = array();
  $view = new view();
  $view->name = 'publication_user';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'efq_multiple';
  $view->human_name = 'Publication user';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Publication user';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'relation_select';
  /* Field: Fields: Family name */
  $handler->display->display_options['fields']['field_user_family_name']['id'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['table'] = 'efq_multiple';
  $handler->display->display_options['fields']['field_user_family_name']['field'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['label'] = '';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_family_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['field_api_classes'] = 0;
  /* Field: Fields: Other/given name(s)  */
  $handler->display->display_options['fields']['field_user_given_names']['id'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['table'] = 'efq_multiple';
  $handler->display->display_options['fields']['field_user_given_names']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['label'] = '';
  $handler->display->display_options['fields']['field_user_given_names']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_given_names']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['field_api_classes'] = 0;
  /* Field: Fields: Institution */
  $handler->display->display_options['fields']['field_user_institution']['id'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['table'] = 'efq_multiple';
  $handler->display->display_options['fields']['field_user_institution']['field'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['label'] = '';
  $handler->display->display_options['fields']['field_user_institution']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_institution']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_institution']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_user_institution']['field_api_classes'] = 0;
  /* Filter criterion: Entity: Entity Type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'efq_multiple';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'user';
  /* Filter criterion: Fields: Family name (field_user_family_name) - value */
  $handler->display->display_options['filters']['field_user_family_name-value']['id'] = 'field_user_family_name-value';
  $handler->display->display_options['filters']['field_user_family_name-value']['table'] = 'efq_multiple';
  $handler->display->display_options['filters']['field_user_family_name-value']['field'] = 'field_user_family_name-value';
  $handler->display->display_options['filters']['field_user_family_name-value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_user_family_name-value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['operator_id'] = 'field_user_family_name-value_op';
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['label'] = 'Family name';
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['operator'] = 'field_user_family_name-value_op';
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['identifier'] = 'field_user_family_name-value';
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['field_user_family_name-value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_user_family_name-value']['delta'] = '';
  /* Filter criterion: Fields: Other/given name(s)  (field_user_given_names) - value */
  $handler->display->display_options['filters']['field_user_given_names-value']['id'] = 'field_user_given_names-value';
  $handler->display->display_options['filters']['field_user_given_names-value']['table'] = 'efq_multiple';
  $handler->display->display_options['filters']['field_user_given_names-value']['field'] = 'field_user_given_names-value';
  $handler->display->display_options['filters']['field_user_given_names-value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_user_given_names-value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['operator_id'] = 'field_user_given_names-value_op';
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['label'] = 'Other/given name(s)';
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['operator'] = 'field_user_given_names-value_op';
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['identifier'] = 'field_user_given_names-value';
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['field_user_given_names-value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_user_given_names-value']['delta'] = '';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'publication-user';
  $views['publication_user'] = $view;
  return $views;
}