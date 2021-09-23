<?php
// $Id$

/**
 * Numeric argument handler for entity keys.
 */
class efq_views_handler_argument_entity_numeric extends views_handler_argument_numeric {
  function query() {
    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    if (count($this->value) > 1) {
      $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
      $this->query->query->entityCondition($this->real_field, $this->value, $operator);
    }
    else {
      $operator = empty($this->options['not']) ? '=' : '!=';
      $this->query->query->entityCondition($this->real_field, $this->argument, $operator);
    }
  }
}