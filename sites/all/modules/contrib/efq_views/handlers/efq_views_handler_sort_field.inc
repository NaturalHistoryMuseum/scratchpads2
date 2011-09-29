<?php
// $Id$

/**
 * Sort handler for fields.
 */
class efq_views_handler_sort_field extends views_handler_sort {
  function query() {
    $this->query->query->fieldOrderBy($this->definition['field_name'], $this->real_field, $this->options['order']);
  }
}
