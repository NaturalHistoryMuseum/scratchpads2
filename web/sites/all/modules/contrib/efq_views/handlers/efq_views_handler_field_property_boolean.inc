<?php
// $Id$

/**
 * Field handler for boolean properties.
 */
class efq_views_handler_field_property_boolean extends views_handler_field_boolean {
  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $this->query->query->propertyOrderBy($this->real_field, $order);
  }

  /**
   * Override the parent's query method, since it doesn't need to do anything.
   */
  function query() {}

  /**
   * Get the value that's supposed to be rendered.
   *
   * @param $values
   *   An entity object containing all retrieved values.
   * @param $field
   *   Optional name of the field where the value is stored.
   */
  function get_value($values, $field = NULL) {
    $field = isset($field) ? $field : $this->real_field;
    $wrapper = entity_metadata_wrapper($this->query->entity_type, $values);
    return $wrapper->{$field}->value();
  }
}
