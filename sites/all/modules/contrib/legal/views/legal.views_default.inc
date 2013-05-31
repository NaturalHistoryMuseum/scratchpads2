<?php

/**
 * @file
 * Default views for the legal module.
 */

/**
 * Implements hook_views_default_views().
 */
function legal_views_default_views() {
  /*
   * View 'legal_acceptances'
   */

  $view               = new view;
  $view->name         = 'legal_terms';
  $view->description  = 'Legal terms & Conditions';
  $view->tag          = 'Legal';
  $view->view_php     = '';
  $view->base_table   = 'legal_conditions';
  $view->is_cacheable = FALSE;
  $view->api_version  = 2;
  $view->disabled     = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler            = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'version'    => array(
      'label'        => 'Version',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'version',
      'table'        => 'legal_conditions',
      'field'        => 'version',
      'relationship' => 'none',
    ),
    'language'   => array(
      'label'        => 'Language',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'language',
      'table'        => 'legal_conditions',
      'field'        => 'language',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'revision'   => array(
      'label'        => 'Revision',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'revision',
      'table'        => 'legal_conditions',
      'field'        => 'revision',
      'relationship' => 'none',
    ),
    'conditions' => array(
      'label'        => 'Terms & Conditions',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'conditions',
      'table'        => 'legal_conditions',
      'field'        => 'conditions',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'extras'     => array(
      'label'        => 'Additional',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'extras',
      'table'        => 'legal_conditions',
      'field'        => 'extras',
      'relationship' => 'none',
    ),
    'changes'    => array(
      'label'        => 'Changes',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'changes',
      'table'        => 'legal_conditions',
      'field'        => 'changes',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'date'       => array(
      'label'              => 'Date Added',
      'alter'              => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'              => '',
      'hide_empty'         => 0,
      'empty_zero'         => 0,
      'date_format'        => 'small',
      'custom_date_format' => '',
      'exclude'            => 0,
      'id'                 => 'date',
      'table'              => 'legal_conditions',
      'field'              => 'date',
      'override'           => array(
        'button' => 'Override',
      ),
      'relationship'       => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'version'  => array(
      'order'        => 'DESC',
      'id'           => 'version',
      'table'        => 'legal_conditions',
      'field'        => 'version',
      'relationship' => 'none',
    ),
    'language' => array(
      'order'        => 'ASC',
      'id'           => 'language',
      'table'        => 'legal_conditions',
      'field'        => 'language',
      'relationship' => 'none',
    ),
    'revision' => array(
      'order'        => 'DESC',
      'id'           => 'revision',
      'table'        => 'legal_conditions',
      'field'        => 'revision',
      'relationship' => 'none',
      'override'     => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer Terms and Conditions',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Terms & Conditions');
  $handler->override_option('empty', 'No Terms & Conditions have been added yet.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky'   => 1,
    'order'    => 'asc',
    'columns'  => array(
      'version'  => 'version',
      'revision' => 'revision',
    ),
    'info'     => array(
      'version'  => array(
        'sortable'  => 1,
        'separator' => '',
      ),
      'revision' => array(
        'sortable'  => 1,
        'separator' => '',
      ),
    ),
    'default'  => '-1',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/config/people/legal/terms_history');
  $handler->override_option('menu', array(
    'type'        => 'tab',
    'title'       => 'T&C History',
    'description' => 'Terms & Conditions',
    'weight'      => '3',
    'name'        => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type'        => 'none',
    'title'       => '',
    'description' => '',
    'weight'      => 0,
    'name'        => 'navigation',
  ));

  $views[$view->name] = $view;

  $view               = new view;
  $view->name         = 'legal_users';
  $view->description  = '';
  $view->tag          = '';
  $view->view_php     = '';
  $view->base_table   = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version  = 2;
  $view->disabled     = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler            = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'name'       => array(
      'label'               => 'User',
      'alter'               => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'               => '',
      'hide_empty'          => 0,
      'empty_zero'          => 0,
      'link_to_user'        => 1,
      'overwrite_anonymous' => 0,
      'anonymous_text'      => '',
      'exclude'             => 0,
      'id'                  => 'name',
      'table'               => 'users',
      'field'               => 'name',
      'relationship'        => 'none',
    ),
    'version'    => array(
      'label'        => 'Version',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'version',
      'table'        => 'legal_accepted',
      'field'        => 'version',
      'relationship' => 'none',
    ),
    'language_1' => array(
      'label'        => 'Language',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'language_1',
      'table'        => 'legal_accepted',
      'field'        => 'language',
      'relationship' => 'none',
    ),
    'revision'   => array(
      'label'        => 'Revision',
      'alter'        => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'        => '',
      'hide_empty'   => 0,
      'empty_zero'   => 0,
      'exclude'      => 0,
      'id'           => 'revision',
      'table'        => 'legal_accepted',
      'field'        => 'revision',
      'relationship' => 'none',
    ),
    'accepted'   => array(
      'label'              => 'Time Accepted',
      'alter'              => array(
        'alter_text'    => 0,
        'text'          => '',
        'make_link'     => 0,
        'path'          => '',
        'link_class'    => '',
        'alt'           => '',
        'prefix'        => '',
        'suffix'        => '',
        'target'        => '',
        'help'          => '',
        'trim'          => 0,
        'max_length'    => '',
        'word_boundary' => 1,
        'ellipsis'      => 1,
        'html'          => 0,
        'strip_tags'    => 0,
      ),
      'empty'              => '',
      'hide_empty'         => 0,
      'empty_zero'         => 0,
      'date_format'        => 'small',
      'custom_date_format' => '',
      'exclude'            => 0,
      'id'                 => 'accepted',
      'table'              => 'legal_accepted',
      'field'              => 'accepted',
      'relationship'       => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'uid'      => array(
      'order'        => 'DESC',
      'id'           => 'uid',
      'table'        => 'users',
      'field'        => 'uid',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'version'  => array(
      'order'        => 'DESC',
      'id'           => 'version',
      'table'        => 'legal_accepted',
      'field'        => 'version',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'language' => array(
      'order'        => 'ASC',
      'id'           => 'language',
      'table'        => 'legal_accepted',
      'field'        => 'language',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'revision' => array(
      'order'        => 'ASC',
      'id'           => 'revision',
      'table'        => 'legal_accepted',
      'field'        => 'revision',
      'override'     => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'version' => array(
      'operator'     => 'not empty',
      'value'        => array(
        'value' => '',
        'min'   => '',
        'max'   => '',
      ),
      'group'        => '0',
      'exposed'      => FALSE,
      'expose'       => array(
        'operator'     => 'version_op',
        'label'        => 'Accepted',
        'use_operator' => 0,
        'identifier'   => 'accepted',
        'optional'     => 1,
        'remember'     => 1,
      ),
      'id'           => 'version',
      'table'        => 'legal_accepted',
      'field'        => 'version',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer Terms and Conditions',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Accepted');
  $handler->override_option('empty', 'No users have accepted Terms & Conditions yet.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 50);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky'   => 1,
    'order'    => 'desc',
    'columns'  => array(
      'name'       => 'name',
      'version'    => 'version',
      'language_1' => 'language_1',
      'revision'   => 'revision',
      'accepted'   => 'accepted',
    ),
    'info'     => array(
      'name'       => array(
        'sortable'  => 1,
        'separator' => '',
      ),
      'version'    => array(
        'sortable'  => 1,
        'separator' => '',
      ),
      'language_1' => array(
        'sortable'  => 1,
        'separator' => '',
      ),
      'revision'   => array(
        'sortable'  => 1,
        'separator' => '',
      ),
      'accepted'   => array(
        'sortable'  => 1,
        'separator' => '',
      ),
    ),
    'default'  => '-1',
  ));
  $handler = $view->new_display('page', 'Users', 'page_1');
  $handler->override_option('path', 'admin/config/people/legal/users');
  $handler->override_option('menu', array(
    'type'        => 'tab',
    'title'       => 'Accepted',
    'description' => 'Accepted',
    'weight'      => '5',
    'name'        => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type'        => 'none',
    'title'       => '',
    'description' => '',
    'weight'      => 0,
    'name'        => 'navigation',
  ));

  $views[$view->name] = $view;

  return $views;
}
