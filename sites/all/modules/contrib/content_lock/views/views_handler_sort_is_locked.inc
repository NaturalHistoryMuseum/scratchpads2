<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 * Handler to identify if node is locked or not
 */

class views_handler_sort_is_locked extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    $this->query->add_orderby($this->table_alias, 'timestamp', $this->options['order']);
  }
}
