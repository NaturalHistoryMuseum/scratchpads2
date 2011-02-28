<?php
// $Id: date_views.views_default.inc,v 1.3 2011/01/18 00:37:35 karens Exp $

/**
 * @file
 * Default views for Date API.
 */

/**
 * Implementation of hook_views_default_views().
 */
function date_views_views_default_views() {

$view = new view;
$view->name = 'date_browser';
$view->description = 'Browse through nodes by year, month, day, or week. Date browser attachment adds back/next navigation to the top of the page.';
$view->tag = 'Date';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 3;
$view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('arguments', array(
  'date_argument' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'default_argument_type' => 'date',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'date_fields' => array(
      'node.changed' => 'node.changed',
    ),
    'year_range' => '-3:+3',
    'date_method' => 'OR',
    'granularity' => 'month',
    'id' => 'date_argument',
    'table' => 'node',
    'field' => 'date_argument',
    'relationship' => 'none',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
    'default_options_div_prefix' => '',
    'validate_argument_signup_status' => 'any',
    'validate_argument_signup_node_access' => 0,
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
  'role' => array(),
  'perm' => '',
));
$handler->override_option('items_per_page', 0);
$handler->override_option('use_pager', '1');
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'teaser' => 1,
  'links' => 1,
  'comments' => 0,
));
$handler = $view->new_display('page', 'Page', 'page');
$handler->override_option('path', 'date-browser');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler = $view->new_display('date_nav', 'Date browser', 'date_nav_1');
$handler->override_option('style_plugin', 'date_nav');
$handler->override_option('row_plugin', 'fields');
$handler->override_option('row_options', array());
$handler->override_option('attachment_position', 'before');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'page' => 'page',
  'default' => 0,
));

  $views[$view->name] = $view;
  return $views;
}