<?php

/**
 * @file
 * Definition of handlers for using numeric submission data.
 */

/**
 * Extended version of the numeric field handler specialized for Webform values.
 *
 * @ingroup views_field_handlers
 */
class webform_handler_field_numeric_data extends views_handler_field_numeric {

  var $formula = NULL;

  /**
   * Constructor
   */
  function construct() {
    parent::construct();
    $this->formula = TRUE;
  }
  
  /**
   * Get the formula for this argument.
   *
   * $this->ensure_my_table() MUST have been called prior to this.
   */
  function get_formula() {
    return ("(0.0 + $this->table_alias.$this->real_field)");
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $this->field_alias = $this->query->add_field(NULL, $this->get_formula(), $this->table_alias . '_' . $this->field, $params);

    $this->add_additional_fields();
  }

    /**
   * Shortcut to get a handler's raw field value.
   *
   * This should be overridden for handlers with formulae or other
   * non-standard fields. Because this takes an argument, fields
   * overriding this can just call return parent::get_field($formula)
   */
  function get_field($field = NULL) {
    return parent::get_field($this->get_formula());
  }
  
}

/**
 * Numeric filter handler that works with Webform numeric submission data.
 *
 * @ingroup views_filter_handlers
 */
class webform_handler_filter_numeric_data extends views_handler_filter_numeric {

  /**
   * Get the formula for this argument.
   *
   * $this->ensure_my_table() MUST have been called prior to this.
   */
  function get_formula() {
    return ("(0.0 + $this->table_alias.$this->real_field)");
  }

  /**
   * Called to add the filter to a query.
   */
  function query() {
    $this->ensure_my_table();

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($this->get_formula());
    }
  }

  /**
   * Adds a simple operator condition to the query.
   */
  function op_simple($field) {
    static $sequence = 1;
    $param =  ":value" . $sequence++;
    $this->query->add_where_expression($this->options['group'],
                                       $field . $this->operator . $param,
                                       array($param =>  $this->value['value']));
  }

  /**
   * Adds a between or not-between condition to the query.
   */
  function op_between($field) {
    static $sequence = 1;
    $min =  ":min" . $sequence;
    $max =  ":max" . $sequence++;
    if ($this->operator == 'between') {
      $this->query->add_where_expression($this->options['group'],
                                         "($min <= $field AND $field <= $max)",
                                         array($min => $this->value['min'], $max => $this->value['max']));
    }
    else {
      $this->query->add_where_expression($this->options['group'],
                                         "($min > $field OR $field > $max)",
                                         array($min => $this->value['min'], $max => $this->value['max']));
    }
  }

  /**
   * Adds an empty or not-empty condition to the query.
   */
  function op_empty($field) {
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
    }
    else {
      $operator = "IS NOT NULL";
    }

    $this->query->add_where_expression($this->options['group'],
                                       "$field $operator");
  }

  /**
   * Adds a regular expression condition to the query.
   */
  function op_regex($field) {
    static $sequence = 1;
    $param =  ":expression" . $sequence++;

    $this->query->add_where_expression($this->options['group'],
                                       "$field RLIKE $param",
                                       array($param => $this->value['value']));
  }

}

/**
 * Sort handler that works with Webform numeric submission data.
 *
 * @ingroup views_sort_handlers
 */
class webform_handler_sort_numeric_data extends views_handler_sort {

  /**
   * Get the formula for this sort.
   *
   * $this->ensure_my_table() MUST have been called prior to this.
   */
  function get_formula() {
    return ("(0.0 + $this->table_alias.$this->real_field)");
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $alias = $this->query->add_field(NULL, $this->get_formula(), $this->table_alias . '_' . $this->field . '_sort');
    // Add the sort for the field using only the alias.
    $this->query->add_orderby(NULL, NULL, $this->options['order'], $alias);
  }

}
