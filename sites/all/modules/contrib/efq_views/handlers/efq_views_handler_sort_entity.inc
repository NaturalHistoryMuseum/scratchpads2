<?php
// $Id$

/**
 * Sort handler for entity keys.
 */
class efq_views_handler_sort_entity extends views_handler_sort {
  function query() {
    $this->query->query->entityOrderBy($this->real_field, $this->options['order']);
  }
}
