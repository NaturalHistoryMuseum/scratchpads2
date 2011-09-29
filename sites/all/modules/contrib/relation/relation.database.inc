<?php

/**
 * @file
 * Database query functions.
 */

/**
 * Handler class for entity relations.
 */
class RelationQuery extends EntityFieldQuery {

  /**
   * Delta group placeholder.
   */
  protected $delta_group = 0;

  /**
   * Constructor for RelationQuery.
   */
  function __construct($entity_type = NULL, $entity_id = NULL, $index = NULL) {
    if (isset($entity_type)) {
      $this->related($entity_type, $entity_id, $index);
    }
    $this->entityCondition('entity_type', 'relation');
    $this->range(0, 50);
  }

  /**
   * Add a related entity to the query.
   *
   * @param $entity_type
   *   Entity type of the related entity.
   * @param $entity_id
   *   Entity id of the related entity.
   * @param $index
   *   The index of the related entity within the requested relation(s).
   *
   * @return RelationQuery
   */
  function related($entity_type, $entity_id, $index = NULL) {
    $this->fieldCondition('endpoints', 'entity_type', $entity_type, '=', $this->delta_group);
    $this->fieldCondition('endpoints', 'entity_id', $entity_id, '=', $this->delta_group);
    if (isset($index)) {
      $this->fieldCondition('endpoints', 'r_index', $index, '=', $this->delta_group);
    }
    $this->delta_group++;
    return $this;
  }

  function execute() {
    $results = parent::execute();
    return isset($results['relation']) ? $results['relation'] : array();
  }
}
