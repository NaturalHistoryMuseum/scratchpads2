<?php
/**
 * @file
 * Provide node nid argument handler.
 */

/**
 * Argument handler to accept a node id.
 */
class creativecommons_views_handler_argument_nid extends views_handler_argument {
  /**
   * Override the behavior of title(). Get the title of the node.
   */
 /* function title_query() {
    $titles = array();
    $placeholders = implode(', ', array_fill(0, sizeof($this->value), '%d'));

    $result = db_query("SELECT license_uri FROM {creativecommons} c WHERE c.nid IN ($placeholders)", $this->value);
    while ($term = db_fetch_object($result)) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }*/
}

