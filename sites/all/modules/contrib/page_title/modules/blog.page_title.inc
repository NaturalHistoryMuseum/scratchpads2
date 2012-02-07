<?php

/**
 * @file
 * Blog implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function blog_page_title_alter(&$title) {
  $menu_item = menu_get_item();

  // If we're looking at a users blog page, get the user title
  if (!strncmp($menu_item['path'], 'blog/%', 6) && ($user = menu_get_object('user_uid_optional'))) {
    if (variable_get('page_title_blog_showfield', 0) && ($blog_title = page_title_load_title(arg(1), 'user'))) {
      $title = $blog_title;
    }
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function blog_page_title_pattern_alter(&$pattern, &$types) {
  $menu_item = menu_get_item();

  if (!strncmp($menu_item['path'], 'blog/%', 6) && ($user = menu_get_object('user_uid_optional'))) {
    $types['user'] = $user;
    $pattern = variable_get('page_title_blog', '');
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function blog_page_title_settings() {
  return array(
    'page_title_blog' => array(
      'label' => 'Blog Homepage',
      'scopes' => array('global', 'user'),
      'show field' => FALSE,
      'description' => 'This pattern will be used for a users blog page (ie <code>/blog/1</code>)',
    ),
  );
}

