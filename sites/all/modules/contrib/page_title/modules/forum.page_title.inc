<?php

/**
 * @file
 * Forum implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function forum_page_title_alter(&$title) {
  $menu_item = menu_get_item();
  // Check we're on a forum page and if there is a number to confirm it's a
  // container or forum (rather than root).
  if ( !strncmp($menu_item['path'], 'forum/%', 7) &&
       ($term = menu_get_object('taxonomy_term')) &&
       variable_get('page_title_vocab_' . $term->vocabulary_machine_name . '_showfield', 0) &&
       ($forum_title = page_title_load_title($term->tid, 'term')) ) {
    $title = $forum_title;
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function forum_page_title_pattern_alter(&$pattern, &$types) {
  $menu_item = menu_get_item();

  // Forums Page title Patterns
  if ( !strncmp($menu_item['path'], 'forum/%', 7) &&
       ($forum = menu_get_object('forum_forum', 1)) ) {
    $types['term'] = $forum;

    $forum_vid = variable_get('forum_nav_vocabulary', '');
    $forum_vocab = taxonomy_vocabulary_load($forum_vid);
    $pattern = variable_get('page_title_vocab_' . $forum_vocab->machine_name, '');

    $types['vocabulary'] = $forum_vocab;
  }
  // Otherwise it's the root - let's grab the root pattern.
  elseif ($menu_item['path'] == 'forum') {
    $pattern = variable_get('page_title_forum_root_title', '');
    $forum_vid = variable_get('forum_nav_vocabulary', 0);
    $types['vocabulary'] = taxonomy_vocabulary_load($forum_vid);
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function forum_page_title_settings() {
  return array(
    'page_title_forum_root_title' => array(
      'label' => 'Forum Root',
      'scopes' => array('global'),
      'show field' => FALSE,
      'description' => 'This pattern will be used on the forum root page (ie <code>/forum</code>)',
    ),
  );
}
