<?php


/**
 * @file Views sort handler for the menu hierarchy.
 */

class menu_node_views_sort_menu extends views_handler_sort {

  function query() {
    $this->ensure_my_table();
    $start_depth = isset($this->options['start_depth']) ? $this->options['start_depth'] : 1;
    $max_depth = !empty($this->options['max_depth']) ? $this->options['max_depth'] : MENU_MAX_DEPTH;
    for ($i = $start_depth; $i <= $max_depth; $i++) {
      $this->query->add_orderby($this->table_alias, "p$i", $this->options['order']);
    }
  }

}
