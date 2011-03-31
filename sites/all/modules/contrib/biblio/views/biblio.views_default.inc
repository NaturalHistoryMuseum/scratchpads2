<?php
/**
 * @file
 * Contains default views on behalf of the biblio module.
 */

/**
 * Implementation of hook_default_view_views().
 */
function biblio_views_default_views() {
  $view = new view;
  $view->name = 'biblio_views';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to TRUE to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
  'citation' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'style_name' => 'vancouver',
    'exclude' => 0,
    'id' => 'citation',
    'table' => 'biblio',
    'field' => 'citation',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('sorts', array(
  'biblio_year' => array(
    'order' => 'DESC',
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'relationship' => 'none',
  ),
  'name' => array(
    'id' => 'name',
    'table' => 'biblio_types',
    'field' => 'name',
  ),
  ));
  $handler->override_option('arguments', array(
  'biblio_year' => array(
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
  ),
  ));
  $handler->override_option('filters', array(
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
  ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'biblio' => 'biblio',
  ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
  ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('access', array(
  'type' => 'none',
  ));
  $handler->override_option('cache', array(
  'type' => 'none',
  ));
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('empty', 'Nothing found');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_options', array(
  'grouping' => 'name',
  ));
  $handler->override_option('row_options', array(
  'inline' => array(),
  'separator' => '',
  ));
  $handler = $view->new_display('page', 'Group by Year', 'page_1');
  $handler->override_option('fields', array(
  'citation' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'style_name' => 'vancouver',
    'exclude' => 0,
    'id' => 'citation',
    'table' => 'biblio',
    'field' => 'citation',
    'relationship' => 'none',
  ),
  'biblio_year' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'biblio_label' => 0,
    'exclude' => 1,
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'override' => array(
      'button' => 'Override',
  ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('arguments', array(
  'biblio_year' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'glossary' => 0,
    'limit' => '0',
    'case' => 'none',
    'path_case' => 'none',
    'transform_dash' => 0,
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
  ),
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'image' => 0,
      'biblio' => 0,
      'book' => 0,
      'feed' => 0,
      'page' => 0,
      'story' => 0,
      'test' => 0,
  ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '2' => 0,
      '3' => 0,
      '4' => 0,
  ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'image_size' => array(
      '_original' => '_original',
      'thumbnail' => 'thumbnail',
      'preview' => 'preview',
  ),
    'validate_argument_php' => '',
  ),
  ));
  $handler->override_option('style_options', array(
  'grouping' => 'biblio_year',
  ));
  $handler->override_option('path', 'view/biblio/year');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Group by Type', 'page_2');
  $handler->override_option('fields', array(
  'citation' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'style_name' => 'vancouver',
    'exclude' => 0,
    'id' => 'citation',
    'table' => 'biblio',
    'field' => 'citation',
    'relationship' => 'none',
  ),
  'name' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'exclude' => 1,
    'id' => 'name',
    'table' => 'biblio_types',
    'field' => 'name',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('sorts', array(
  'name' => array(
    'order' => 'ASC',
    'id' => 'name',
    'table' => 'biblio_types',
    'field' => 'name',
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
  ),
  'biblio_year' => array(
    'order' => 'DESC',
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('path', 'view/biblio/type');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Group by Title', 'page_3');
  $handler->override_option('fields', array(
  'citation' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'style_name' => 'vancouver',
    'exclude' => 0,
    'id' => 'citation',
    'table' => 'biblio',
    'field' => 'citation',
    'relationship' => 'none',
  ),
  'biblio_year' => array(
    'label' => 'Year of Publication',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'biblio_label' => 1,
    'exclude' => 1,
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('sorts', array(
  'biblio_year' => array(
    'order' => 'DESC',
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'relationship' => 'none',
  ),
  'title' => array(
    'order' => 'ASC',
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('style_options', array(
  'grouping' => 'biblio_year',
  ));
  $handler->override_option('path', 'view/biblio/title');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Filter by Keyword ID', 'page_4');
  $handler->override_option('arguments', array(
  'kid' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'Contains the keyword %1',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'break_phrase' => 1,
    'add_table' => 0,
    'require_value' => 0,
    'reduce_duplicates' => 0,
    'id' => 'kid',
    'table' => 'biblio_keyword',
    'field' => 'kid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
  ),
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'image' => 0,
      'biblio' => 0,
      'book' => 0,
      'feed' => 0,
      'page' => 0,
      'story' => 0,
      'test' => 0,
  ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '2' => 0,
      '3' => 0,
      '4' => 0,
  ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'image_size' => array(
      '_original' => '_original',
      'thumbnail' => 'thumbnail',
      'preview' => 'preview',
  ),
    'validate_argument_php' => '',
  ),
  ));
  $handler->override_option('path', 'view/biblio/keyword');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Filter by Author ID', 'page_5');
  $handler->override_option('arguments', array(
  'cid' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'break_phrase' => 1,
    'add_table' => 0,
    'require_value' => 0,
    'reduce_duplicates' => 0,
    'id' => 'cid',
    'table' => 'biblio_contributor',
    'field' => 'cid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
  ),
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'image' => 0,
      'biblio' => 0,
      'book' => 0,
      'feed' => 0,
      'page' => 0,
      'story' => 0,
      'test' => 0,
  ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '2' => 0,
      '3' => 0,
      '4' => 0,
  ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'image_size' => array(
      '_original' => '_original',
      'thumbnail' => 'thumbnail',
      'preview' => 'preview',
  ),
    'validate_argument_php' => '',
  ),
  ));
  $handler->override_option('path', 'view/biblio/author');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Browse by Year', 'page_6');
  $handler->override_option('fields', array(
  'biblio_year' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
  ),
    'biblio_label' => 0,
    'exclude' => 0,
    'id' => 'biblio_year',
    'table' => 'biblio',
    'field' => 'biblio_year',
    'override' => array(
      'button' => 'Use default',
  ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('sorts', array());
  $handler->override_option('arguments', array());
  $handler->override_option('distinct', 0);
  $handler->override_option('style_plugin', 'grid');
  $handler->override_option('style_options', array(
  'grouping' => '',
  'columns' => '4',
  'alignment' => 'horizontal',
  ));
  $handler->override_option('path', 'view/biblio/years');
  $handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  ));

  $views[$view->name] = $view;
  return $views;
}