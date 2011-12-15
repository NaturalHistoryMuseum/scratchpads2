<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 * Handler to filter based on locked nodes
 */

class views_handler_filter_is_locked extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    if (empty($this->value)) {
      $this->query->add_where($this->options['group'], "$this->table_alias.timestamp IS NULL");
    }
    else {
      $this->query->add_where($this->options['group'], "$this->table_alias.timestamp IS NOT NULL");
    }
  }
}
