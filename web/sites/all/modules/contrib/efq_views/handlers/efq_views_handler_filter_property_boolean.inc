<?php
// $Id$

/**
 * Filter handler for boolean properties.
 */
class efq_views_handler_filter_property_boolean extends views_handler_filter_boolean_operator {
  function query() {
    if (empty($this->value)) {
      $this->query->query->propertyCondition($this->real_field, 0, "=");
    }
    else {
      $this->query->query->propertyCondition($this->real_field, 0, "!=");
    }
  }
}
