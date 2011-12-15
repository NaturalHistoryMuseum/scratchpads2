<?php

/**
 * @file
 * The default view for the media browser library tab.
 */

$view = new view;
$view->name = 'media_default';
$view->description = 'Default view for the media browser library tab.';
$view->tag = 'media, default';
$view->base_table = 'file_managed';
$view->human_name = 'Media browser';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['autosubmit'] = 1;
$handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'media_browser';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'No files available.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: File: Name */
$handler->display->display_options['fields']['filename']['id'] = 'filename';
$handler->display->display_options['fields']['filename']['table'] = 'file_managed';
$handler->display->display_options['fields']['filename']['field'] = 'filename';
$handler->display->display_options['fields']['filename']['label'] = '';
$handler->display->display_options['fields']['filename']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['filename']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['filename']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
$handler->display->display_options['fields']['filename']['alter']['html'] = 0;
$handler->display->display_options['fields']['filename']['hide_empty'] = 0;
$handler->display->display_options['fields']['filename']['empty_zero'] = 0;
$handler->display->display_options['fields']['filename']['link_to_file'] = 1;
/* Sort criterion: File: Upload date */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'file_managed';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
$handler->display->display_options['sorts']['timestamp']['exposed'] = TRUE;
$handler->display->display_options['sorts']['timestamp']['expose']['label'] = 'Upload date';
/* Sort criterion: SUM(File Usage: Use count) */
$handler->display->display_options['sorts']['count']['id'] = 'count';
$handler->display->display_options['sorts']['count']['table'] = 'file_usage';
$handler->display->display_options['sorts']['count']['field'] = 'count';
$handler->display->display_options['sorts']['count']['group_type'] = 'sum';
$handler->display->display_options['sorts']['count']['exposed'] = TRUE;
$handler->display->display_options['sorts']['count']['expose']['label'] = 'Use count';
/* Filter criterion: File: Status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'file_managed';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = array(
  1 => '1',
);
/* Filter criterion: File: Name */
$handler->display->display_options['filters']['filename']['id'] = 'filename';
$handler->display->display_options['filters']['filename']['table'] = 'file_managed';
$handler->display->display_options['filters']['filename']['field'] = 'filename';
$handler->display->display_options['filters']['filename']['operator'] = 'contains';
$handler->display->display_options['filters']['filename']['exposed'] = TRUE;
$handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
$handler->display->display_options['filters']['filename']['expose']['label'] = 'File name';
$handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
$handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
$handler->display->display_options['filters']['filename']['expose']['required'] = 0;
$handler->display->display_options['filters']['filename']['expose']['multiple'] = FALSE;
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

/* Display: Media browser */
$handler = $view->new_display('media_browser', 'Media browser', 'media_browser_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'View Library';
$translatables['media_default'] = array(
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
  t('No files available.'),
  t('Upload date'),
  t('Use count'),
  t('File name'),
  t('Type'),
  t('Media browser'),
  t('View Library'),
);
