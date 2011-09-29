<?php
// $Id$

/**
 * Filter handler for numeric properties.
 */
class efq_views_handler_filter_property_numeric extends views_handler_filter_numeric {
  /**
   * We don't support every operator from the parent class ("not between", for example),
   * hence the need to define only the operators we do support.
   */
  function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'method' => 'op_simple',
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'method' => 'op_simple',
        'short' => t('<='),
        'values' => 1,
      ),
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      ),
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'method' => 'op_simple',
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'method' => 'op_simple',
        'short' => t('>'),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
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

  function op_simple($column) {
    $this->query->query->propertyCondition($column, $this->value['value'], $this->operator);
  }

  function op_between($column) {
    $this->query->query->propertyCondition($column, array($this->value['min'], $this->value['max']), "BETWEEN");
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty() {}
}