<?php

/**
 * @file
 * Comment implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function comment_page_title_alter(&$title) {
  // Get the current menu item and compare the path to the comment reply path.
  $menu_item = menu_get_item();
  if ( !strncmp($menu_item['path'], 'comment/reply/%', 15) &&
       ($node = menu_get_object('node', 2)) ) {
    // If the node has a custom page title and the node type is configured
    // to have a custom page title (ie, it's not a leftover from a previous
    // setting), then use it.
    if ( !empty($node->page_title) &&
         variable_get('page_title_type_' . $node->type . '_showfield', 0) ) {
      $title = $node->page_title;
    }
    // Otherwise set the page-title token to the parent node title. Makes more
    // sense than "Reply to comment".
    else {
      $title = $node->title;
    }
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function comment_page_title_pattern_alter(&$pattern, &$types) {
  $menu_item = menu_get_item();

  // Comment reply page.
  if ( !strncmp($menu_item['path'], 'comment/reply/%', 15) &&
       ($node = menu_get_object('node', 2)) ) {
    // The node ID position is in arg 2.
    $types['node'] = $node;

    // If the node has any taxonomy, grab the first time and pass it over to be
    // passed as a token.
    // TODO: Handle multiple terms? Only pass specific terms per content type?
    if (!empty($types['node']->taxonomy)) {
      $types['taxonomy'] = current($types['node']->taxonomy);
    }

    if (is_numeric(arg(3)) && ($comment = menu_get_object('comment', 3))) {
      // Comment Reply...
      $types['comment'] = $comment;
      $pattern = variable_get('page_title_comment_child_reply', '');
    }
    else {
      // Reply to node...
      $pattern = variable_get('page_title_comment_reply', '');
    }
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function comment_page_title_settings() {
  return array(
    'page_title_comment_reply' => array(
      'label' => 'Comment Reply',
      'scopes' => array('global', 'node'),
      'description' => 'This pattern will be used for comment reply pages, where the reply is directly to a "node"',
    ),
    'page_title_comment_child_reply' => array(
      'label' => 'Comment Child Reply',
      'scopes' => array('global', 'comment', 'node'),
      'description' => 'This pattern with be used for comment reply pages where the reply is to an existing "comment" (eg a comment thread)',
    ),
  );
}
