<?php
// $Id$

/**
 * Handle matching of multiple options selectable via checkboxes
 */
class efq_views_handler_filter_property_in_operator extends views_handler_filter_in_operator {
  function op_simple() {
    if (empty($this->value)) {
      return;
    }

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->query->propertyCondition($this->real_field, array_values($this->value), $this->operator);
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty() {}
}