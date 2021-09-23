<?php
// $Id$

/**
 * Argument handler for entity labels.
 *
 * EntityFieldQuery doesn't support querying labels the same way as other
 * entity metadata, so this handler needs to handle the label as if it were
 * a property.
 */
class efq_views_handler_argument_entity_label extends views_handler_argument_string {
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

    $this->query->query->propertyCondition($this->definition['label column'], $argument, '=');
  }
}
