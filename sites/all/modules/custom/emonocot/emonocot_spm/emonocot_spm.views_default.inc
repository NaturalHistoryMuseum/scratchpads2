<?php
/**
 * @file
 * emonocot_spm.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function emonocot_spm_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'biblio_citation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Biblio Citation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['label'] = '';
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
  $handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_authors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '0';
  $handler->display->display_options['fields']['biblio_authors']['initialize'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['short_form'] = 1;
  $handler->display->display_options['fields']['biblio_authors']['name_order'] = 'last-first';
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['label'] = '';
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
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_year']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_year']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_year']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_year']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_year']['biblio_label'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[biblio_authors] ([biblio_year]) [title]';
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
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Sort criterion: Biblio: Author last name */
  $handler->display->display_options['sorts']['lastname']['id'] = 'lastname';
  $handler->display->display_options['sorts']['lastname']['table'] = 'biblio_contributor_data';
  $handler->display->display_options['sorts']['lastname']['field'] = 'lastname';
  /* Sort criterion: Biblio: Year of Publication */
  $handler->display->display_options['sorts']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['sorts']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['sorts']['biblio_year']['field'] = 'biblio_year';
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
    'biblio' => 'biblio',
  );

  /* Display: References */
  $handler = $view->new_display('references', 'References', 'references_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'references_fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'biblio_authors' => 'biblio_authors',
    'biblio_year' => 'biblio_year',
    'title' => 'title',
  );
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['biblio_citation'] = $view;

  return $export;
}
