<?php

/**
 * @file
 * webform_results view definition.
 */

// --- Paste exported view below ---

$view = new view();
$view->name = 'webform_results';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'webform_submissions';
$view->human_name = 'Webform Results';
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
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 50, 100, 200, 500, 1000';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = 'All';

$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'view_submission' => 'view_submission',
  'submitted' => 'submitted',
  'name' => 'name',
  'remote_addr' => 'remote_addr',
);
$handler->display->display_options['style_options']['default'] = 'view_submission';
$handler->display->display_options['style_options']['info'] = array(
  'view_submission' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'submitted' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'remote_addr' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'webform_all_fields' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['sticky'] = TRUE;
$handler->display->display_options['style_options']['empty_table'] = TRUE;
/* Header: Global: Result summary with additional tokens */
$handler->display->display_options['header']['webform_result']['id'] = 'webform_result';
$handler->display->display_options['header']['webform_result']['table'] = 'views';
$handler->display->display_options['header']['webform_result']['field'] = 'webform_result';
$handler->display->display_options['header']['webform_result']['content'] = '<div class="webform-results-per-page">Showing @start - @end of @total. &nbsp; @items_per_page_links</div>';
/* No results behavior: Global: Unfiltered text */
$handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
$handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
$handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
$handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
$handler->display->display_options['empty']['area_text_custom']['content'] = 'There are no submissions for this form. <a href="/node/!1">View this form</a>.';
$handler->display->display_options['empty']['area_text_custom']['tokenize'] = TRUE;
/* Relationship: Webform submissions: User */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'webform_submissions';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Field: Webform submissions: View link */
$handler->display->display_options['fields']['view_submission']['id'] = 'view_submission';
$handler->display->display_options['fields']['view_submission']['table'] = 'webform_submissions';
$handler->display->display_options['fields']['view_submission']['field'] = 'view_submission';
$handler->display->display_options['fields']['view_submission']['label'] = '#';
$handler->display->display_options['fields']['view_submission']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['view_submission']['text'] = '[serial]';
$handler->display->display_options['fields']['view_submission']['access_check'] = 0;
/* Field: Webform submissions: Submitted */
$handler->display->display_options['fields']['submitted']['id'] = 'submitted';
$handler->display->display_options['fields']['submitted']['table'] = 'webform_submissions';
$handler->display->display_options['fields']['submitted']['field'] = 'submitted';
$handler->display->display_options['fields']['submitted']['date_format'] = 'short';
$handler->display->display_options['fields']['submitted']['second_date_format'] = 'long';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'User';
$handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
/* Field: Webform submissions: Remote address */
$handler->display->display_options['fields']['remote_addr']['id'] = 'remote_addr';
$handler->display->display_options['fields']['remote_addr']['table'] = 'webform_submissions';
$handler->display->display_options['fields']['remote_addr']['field'] = 'remote_addr';
$handler->display->display_options['fields']['remote_addr']['label'] = 'IP address';
/* Field: Webform submission data: All values */
$handler->display->display_options['fields']['webform_all_fields']['id'] = 'webform_all_fields';
$handler->display->display_options['fields']['webform_all_fields']['table'] = 'webform_submissions';
$handler->display->display_options['fields']['webform_all_fields']['field'] = 'webform_all_fields';
$handler->display->display_options['fields']['webform_all_fields']['label'] = '';
$handler->display->display_options['fields']['webform_all_fields']['exclude'] = TRUE;
$handler->display->display_options['fields']['webform_all_fields']['alter']['nl2br'] = TRUE;
$handler->display->display_options['fields']['webform_all_fields']['format'] = 'text';
/* Contextual filter: Webform submissions: Node */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'webform_submissions';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
$translatables['webform_results'] = array(
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
  t('Showing @start - @end of @total'),
  t('Webform Submission User'),
  t('Serial Number'),
  t('[serial]'),
  t('Submitted'),
  t('User'),
  t('IP address'),
  t('All'),
);

// --- Paste exported view above ---

$views[$view->name] = $view;
