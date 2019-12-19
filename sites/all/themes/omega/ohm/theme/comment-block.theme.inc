<?php

/**
 * @file
 * Contains the theme function override for 'comment_block'.
 */

/**
 * Returns HTML for a list of recent comments for the comment block.
 *
 * @ingroup themeable
 */
function ohm_comment_block() {
  $items = array();
  $number = variable_get('comment_block_count', 10);
  foreach (comment_get_recent($number) as $comment) {
    $items[] = l($comment->subject, 'comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid)) . '&nbsp;<span>' . t('@time ago', array('@time' => format_interval(REQUEST_TIME - $comment->changed))) . '</span>';
  }

  if ($items) {
    return theme('item_list', array('items' => $items, 'attributes' => array('class' => array('border-list'))));
  }
  else {
    return t('No comments available.');
  }
}
