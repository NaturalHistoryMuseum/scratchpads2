<?php
// $Id$

/**
 * Filter handler for textual properties.
 */
class efq_views_handler_filter_property_string extends views_handler_filter_string {
  /**
   * We don't support every operator from the parent class ("not between", for example),
   * hence the need to define only the operators we do support.
   */
  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'short' => t('='),
        'method' => 'op_equal',
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'short' => t('!='),
        'method' => 'op_equal',
        'values' => 1,
      ),
      'contains' => array(
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'op_contains',
        'values' => 1,
      ),
      'starts' => array(
        'title' => t('Starts with'),
        'short' => t('begins'),
        'method' => 'op_starts',
        'values' => 1,
      ),
    );

    return $operators;
  }

  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($this->real_field);
    }
  }

  function op_equal($column) {
    $this->query->query->propertyCondition($column, $this->value, $this->operator);
  }

  function op_contains($column) {
    $this->query->query->propertyCondition($column, $this->value, "CONTAINS");
  }

  function op_starts($column) {
    $this->query->query->propertyCondition($column, $this->value, "STARTS_WITH");
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty() {}
}
