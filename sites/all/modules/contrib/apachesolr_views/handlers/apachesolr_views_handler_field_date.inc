<?php

/**
 * Field handler for Solr date fields.
 */
class apachesolr_views_handler_field_date extends views_handler_field_date {

  // Transform a Solr date field into a Unix timestamp
  function get_value($values, $field = NULL) {
    $value = parent::get_value($values, $field);
    if (isset($value)) {
      return strtotime($value);
    }
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $sort_field = (isset($this->definition['click sort field']) ? $this->definition['click sort field'] : $this->real_field);
    $this->query->add_sort($sort_field, $order);
  }
}
