<?php
// $Id$

/**
 * Generic field handler for properties.
 */
class efq_views_handler_field_property extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['column'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['column'] = array(
      '#type' => 'textfield',
      '#title' => t('Property column'),
      '#default_value' => isset($this->options['column']) ? $this->options['column'] : '',
      '#description' => t('Name of the database column.'),
      '#weight' => -1,
    );

    // We know which properties are available, so we can show a select box.
    if (isset($this->definition['available_properties'])) {
      $form['column']['#type'] = 'select';
      $form['column']['#options'] = $this->definition['available_properties'];
    }
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $this->query->query->propertyOrderBy($this->options['column'], $order);
  }

  /**
   * Override the parent's query method, since it doesn't need to do anything.
   */
  function query() {}

  function render($values) {
    $value = $values->{$this->options['column']};
    return check_plain($value);
  }
}
