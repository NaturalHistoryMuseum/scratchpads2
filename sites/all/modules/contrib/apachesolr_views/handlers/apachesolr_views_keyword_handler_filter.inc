<?php

/**
 * @file
 * Filter handler to do fulltext keyword search.
 */

class apachesolr_views_keyword_handler_filter extends apachesolr_views_handler_filter {

  /**
   * Add this filter to the query.
   */
  public function query() {
    if (!empty($this->value)) {
      $this->query->add_parameter('q', $this->value);
    }
  }
}
