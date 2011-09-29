<?php
// $Id$

/**
 * String argument handler for fields.
 */
class efq_views_handler_argument_field_string extends views_handler_argument_string {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // We don't support glossary currently.
    unset($form['glossary']);
  }
  
  function query() {
    $argument = $this->argument;
    if (!empty($this->options['transform_dash'])) {
      $argument = strtr($argument, '-', ' ');
    }

    $this->query->query->fieldCondition($this->definition['field_name'], $this->real_field, $argument, '=');
  }
}