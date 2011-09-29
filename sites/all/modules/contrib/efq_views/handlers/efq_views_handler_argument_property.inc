<?php
// $Id$

/**
 * Argument handler for entity properties.
 */
class efq_views_handler_argument_property extends views_handler_argument {
  function option_definition() {
    $options = parent::option_definition();
    $options['column'] = array('default' => '');

   return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['column'] = array(
      '#type' => 'textfield',
      '#title' => t('Property column'),
      '#default_value' => $this->options['column'],
      '#description' => t('Name of the database column to use in the comparison.'),
    );

    // We know which properties are available, so we can show a select box.
    if (isset($this->definition['available_properties'])) {
      $form['column']['#type'] = 'select';
      $form['column']['#options'] = $this->definition['available_properties'];
    }

    parent::options_form($form, $form_state);
  }

  function query() {
    if (!$this->options['column']) {
      return;
    }

    $this->query->query->propertyCondition($this->options['column'], $this->argument, '=');
  }
}
