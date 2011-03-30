<?php

/**
 * @file
 * Filter by redirect type.
 */

class redirect_handler_filter_redirect_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Redirect type');
      $types = db_query("SELECT DISTINCT(type) FROM {redirect}")->fetchCol();
      foreach ($types as $type) {
        $options[$type] = t($type);
      }
      $this->value_options = $options;
    }
  }
}
