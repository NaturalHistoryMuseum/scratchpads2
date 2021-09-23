<?php
// $Id$

/**
 * Handle matching of multiple options selectable via checkboxes
 */
class efq_views_handler_filter_field_in_operator extends views_handler_filter_in_operator {
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

  function op_simple() {
    if (empty($this->value)) {
      return;
    }

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $delta = is_numeric($this->options['delta']) ? $this->options['delta'] : NULL;
    $this->query->query->fieldCondition($this->definition['field_name'], $this->real_field, array_values($this->value), $this->operator, $delta);
  }

  /**
   * NULL comparisons are not supported
   */
  function op_empty() {}
}