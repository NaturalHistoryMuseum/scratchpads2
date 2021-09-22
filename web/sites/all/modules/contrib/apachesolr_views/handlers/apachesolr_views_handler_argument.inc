<?php

/**
 * @file
 * Argument handler for Apache Solr Views.
 */

class apachesolr_views_handler_argument extends views_handler_argument {

  /**
   * Set up the query parameters for this argument.
   */
  public function query($group_by = FALSE) {
    $this->query->add_where(0, $this->real_field, $this->argument);
  }
}
