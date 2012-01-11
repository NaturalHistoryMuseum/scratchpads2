<?php

/**
 * @file
 * Adds default views.
 */

/**
 * Implements hook_views_default_views().
 */
function fontyourface_views_default_views() {
  /*
   * View 'fontyourface_disabled'
   */
  $view = new view;
  $view->name = 'fontyourface_disabled';
  $view->description = 'All disabled fonts.';
  $view->tag = '@font-your-face';
  $view->view_php = '';
  $view->base_table = 'fontyourface_font';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'tid' => array(
      'label' => 'Font Tag',
      'required' => 0,
      'id' => 'tid',
      'table' => 'fontyourface_tag_font',
      'field' => 'tid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_font' => 'edit',
      'display_as' => 'preview',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'fontyourface_font',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'foundry' => array(
      'label' => 'Foundry',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_foundry' => 'foundry_url',
      'exclude' => 0,
      'id' => 'foundry',
      'table' => 'fontyourface_font',
      'field' => 'foundry',
      'relationship' => 'none',
    ),
    'license' => array(
      'label' => 'License',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_license' => 'license_url',
      'exclude' => 0,
      'id' => 'license',
      'table' => 'fontyourface_font',
      'field' => 'license',
      'relationship' => 'none',
    ),
    'tid' => array(
      'label' => 'Tags',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'fontyourface_tag_font',
      'field' => 'tid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'name' => array(
      'order' => 'ASC',
      'id' => 'name',
      'table' => 'fontyourface_font',
      'field' => 'name',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'enabled' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'enabled',
      'table' => 'fontyourface_font',
      'field' => 'enabled',
      'relationship' => 'none',
    ),
    'name' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'name_op',
        'identifier' => 'name',
        'label' => 'Name',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 0,
      'id' => 'name',
      'table' => 'fontyourface_font',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'tid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'tid_op',
        'identifier' => 'tid',
        'label' => 'Tags',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
      ),
      'type' => 'select',
      'id' => 'tid',
      'table' => 'fontyourface_tag',
      'field' => 'tid',
      'relationship' => 'tid',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer @font-your-face',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Add fonts');
  $handler->override_option('header', 'Click a font name to view details and add the font.');
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'foundry' => 'foundry',
      'license' => 'license',
      'tid' => 'tid',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'foundry' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'license' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'tid' => array(
        'separator' => '',
      ),
    ),
    'default' => 'name',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/appearance/fontyourface/add');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $views[$view->name] = $view;

  /*
   * View 'fontyourface_enabled'
   */
  $view = new view;
  $view->name = 'fontyourface_enabled';
  $view->description = 'All enabled fonts.';
  $view->tag = '@font-your-face';
  $view->view_php = '';
  $view->base_table = 'fontyourface_font';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_font' => 'edit',
      'display_as' => 'preview',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'fontyourface_font',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'provider' => array(
      'label' => 'Provider',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'provider',
      'table' => 'fontyourface_font',
      'field' => 'provider',
      'relationship' => 'none',
    ),
    'css_family' => array(
      'label' => 'font-family',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'css_family',
      'table' => 'fontyourface_font',
      'field' => 'css_family',
      'relationship' => 'none',
    ),
    'tid' => array(
      'label' => 'Tags',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'fontyourface_tag_font',
      'field' => 'tid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'name' => array(
      'order' => 'ASC',
      'id' => 'name',
      'table' => 'fontyourface_font',
      'field' => 'name',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'enabled' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'enabled',
      'table' => 'fontyourface_font',
      'field' => 'enabled',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'No fonts currently enabled.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'provider' => 'provider',
      'css_family' => 'css_family',
      'tid' => 'tid',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'provider' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'css_family' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'tid' => array(
        'separator' => '',
      ),
    ),
    'default' => 'name',
  ));
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  return $views;
}
