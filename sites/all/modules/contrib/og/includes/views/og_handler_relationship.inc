<?php


/**
 * @file
 * Contains various relationship handlers.
 */

/**
 * Specialized relationship handler associating groups and their entity.
 *
 * @ingroup views
 */
class og_handler_relationship extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $entity = $this->definition['entity'];

    $this->definition['extra'][] = array(
      'field' => 'entity_type',
      'value' => $this->definition['entity'],
    );
    parent::query();
  }
}