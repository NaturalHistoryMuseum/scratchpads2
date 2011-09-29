<?php
// $Id$

/**
 * Numeric filter for fields.
 */
class efq_views_handler_filter_field_numeric extends views_handler_filter_numeric {
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

  function op_simple($column) {
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $column, $this->value['value'], $this->operator, $delta);
  }

  function op_between($column) {
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $column, array($this->value['min'], $this->value['max']), "BETWEEN", $delta);
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty($field) {}
}
