<?php

/**
 * @file
 * webform_webforms view definition.
 */

// --- Paste exported view below ---

$view = new view();
$view->name = 'webform_webforms';
$view->description = 'An administrative list of webform nodes';
$view->tag = 'webform';
$view->base_table = 'webform';
$view->human_name = 'Webforms';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '25';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'created' => 'created',
  'status' => 'status',
  'webform_results' => 'webform_results',
  'webform_results_2' => 'webform_results',
  'webform_results_3' => 'webform_results',
  'webform_results_4' => 'webform_results',
  'edit_node' => 'edit_node',
  'webform_edit' => 'edit_node',
  'webform_results_1' => 'edit_node',
);
$handler->display->display_options['style_options']['default'] = 'created';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'status' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'webform_results' => array(
    'align' => '',
    'separator' => ' &nbsp; ',
    'empty_column' => 0,
  ),
  'webform_results_2' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'webform_results_3' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'webform_results_4' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'edit_node' => array(
    'align' => '',
    'separator' => ' &nbsp; ',
    'empty_column' => 0,
  ),
  'webform_edit' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'webform_results_1' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Webform: Node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'webform';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = TRUE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
/* Field: Content: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['relationship'] = 'nid';
$handler->display->display_options['fields']['created']['label'] = 'Created';
$handler->display->display_options['fields']['created']['date_format'] = 'short';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Webform: Status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'webform';
$handler->display->display_options['fields']['status']['field'] = 'status';
/* Field: Webform: Webform results link */
$handler->display->display_options['fields']['webform_results']['id'] = 'webform_results';
$handler->display->display_options['fields']['webform_results']['table'] = 'node';
$handler->display->display_options['fields']['webform_results']['field'] = 'webform_results';
$handler->display->display_options['fields']['webform_results']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_results']['label'] = 'View';
$handler->display->display_options['fields']['webform_results']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['webform_results']['text'] = 'Submissions';
/* Field: Webform: Webform results link */
$handler->display->display_options['fields']['webform_results_2']['id'] = 'webform_results_2';
$handler->display->display_options['fields']['webform_results_2']['table'] = 'node';
$handler->display->display_options['fields']['webform_results_2']['field'] = 'webform_results';
$handler->display->display_options['fields']['webform_results_2']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_results_2']['label'] = 'Analysis';
$handler->display->display_options['fields']['webform_results_2']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['webform_results_2']['text'] = 'Analysis';
$handler->display->display_options['fields']['webform_results_2']['subpath'] = 'analysis';
/* Field: Webform: Webform results link */
$handler->display->display_options['fields']['webform_results_3']['id'] = 'webform_results_3';
$handler->display->display_options['fields']['webform_results_3']['table'] = 'node';
$handler->display->display_options['fields']['webform_results_3']['field'] = 'webform_results';
$handler->display->display_options['fields']['webform_results_3']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_results_3']['label'] = 'Table';
$handler->display->display_options['fields']['webform_results_3']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['webform_results_3']['text'] = 'Table';
$handler->display->display_options['fields']['webform_results_3']['subpath'] = 'table';
/* Field: Webform: Webform results link */
$handler->display->display_options['fields']['webform_results_4']['id'] = 'webform_results_4';
$handler->display->display_options['fields']['webform_results_4']['table'] = 'node';
$handler->display->display_options['fields']['webform_results_4']['field'] = 'webform_results';
$handler->display->display_options['fields']['webform_results_4']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_results_4']['label'] = 'Download';
$handler->display->display_options['fields']['webform_results_4']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['webform_results_4']['text'] = 'Download';
$handler->display->display_options['fields']['webform_results_4']['subpath'] = 'download';
/* Field: Content: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['relationship'] = 'nid';
$handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['edit_node']['alter']['text'] = 'Edit';
$handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
/* Field: Webform: Webform edit link */
$handler->display->display_options['fields']['webform_edit']['id'] = 'webform_edit';
$handler->display->display_options['fields']['webform_edit']['table'] = 'node';
$handler->display->display_options['fields']['webform_edit']['field'] = 'webform_edit';
$handler->display->display_options['fields']['webform_edit']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_edit']['label'] = 'Components';
$handler->display->display_options['fields']['webform_edit']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['webform_edit']['text'] = 'Components';
$handler->display->display_options['fields']['webform_edit']['subpath'] = 'components';
/* Field: Webform: Webform results link */
$handler->display->display_options['fields']['webform_results_1']['id'] = 'webform_results_1';
$handler->display->display_options['fields']['webform_results_1']['table'] = 'node';
$handler->display->display_options['fields']['webform_results_1']['field'] = 'webform_results';
$handler->display->display_options['fields']['webform_results_1']['relationship'] = 'nid';
$handler->display->display_options['fields']['webform_results_1']['label'] = 'Clear';
$handler->display->display_options['fields']['webform_results_1']['text'] = 'Clear';
$handler->display->display_options['fields']['webform_results_1']['subpath'] = 'clear';
$translatables['webform_webforms'] = array(
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
  t('Node for webform'),
  t('Title'),
  t('Created'),
  t('Status'),
  t('View'),
  t('Submissions'),
  t('Analysis'),
  t('Table'),
  t('Download'),
  t('Operations'),
  t('Edit'),
  t('Components'),
  t('Clear'),
);

// --- Paste exported view above ---

$views[$view->name] = $view;
