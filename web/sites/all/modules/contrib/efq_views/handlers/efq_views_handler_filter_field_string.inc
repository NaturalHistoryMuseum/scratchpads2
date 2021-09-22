<?php
// $Id$

/**
 * String filter for fields.
 */
class efq_views_handler_filter_field_string extends views_handler_filter_string {
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

  function option_definition() {
    $options = parent::option_definition();
    $options['delta'] = array('default' => NULL);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['delta'] = array(
      '#type' => 'textfield',
      '#title' => t('Delta'),
      '#default_value' => $this->options['delta'],
      '#description' => t('Numeric delta group identifier. If provided, only values attached to the same delta are matched. Leave empty to match all values.'),
    );
  }

  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($this->real_field);
    }
  }

  function op_equal($column) {
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $column, $this->value, $this->operator, $delta);
  }

  function op_contains($column) {
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $column, $this->value, "CONTAINS", $delta);
  }

  function op_starts($column) {
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $column, $this->value, "STARTS_WITH", $delta);
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty($field) {}
}
