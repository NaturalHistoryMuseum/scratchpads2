<?php
// $Id$

/**
 * Argument handler for entity bundles.
 */
class efq_views_handler_argument_entity_bundle extends views_handler_argument_string {
  function query() {
    $this->query->query->entityCondition($this->real_field, $this->argument, '=');
  }
}