<?php
/**
 * @file
 * agidrupal_feeds_and_importers.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function agidrupal_feeds_and_importers_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ag_harvester';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'ag-harvester';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Available document importers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'These importers are already configured for importing bibliographic records from an AGRIS AP XML file. You only need to change the URL of the source XML file in each importer: the URL should be the same for all importers. (You can leave the original URL to test the functionality with sample documents from the IICA archive)

To import records correctly, make sure you execute the Agris AP importers in the correct order. The important thing is to execute the AGRIS AP Document importer last.
';
  $handler->display->display_options['header']['area']['format'] = '1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Feeds source: Source */
  $handler->display->display_options['fields']['config']['id'] = 'config';
  $handler->display->display_options['fields']['config']['table'] = 'feeds_source';
  $handler->display->display_options['fields']['config']['field'] = 'config';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'books_feed' => 'books_feed',
    'xmlimport_dlios' => 'xmlimport_dlios',
    'agris_authors_feed' => 'agris_authors_feed',
    'agris_conferences_feed' => 'agris_conferences_feed',
    'agris_corporate_feed' => 'agris_corporate_feed',
    'agris_journals_feed' => 'agris_journals_feed',
    'agris_publishers_feed' => 'agris_publishers_feed',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'ag_harvesters';
  $export['ag_harvester'] = $view;

  return $export;
}
