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
  function __construct($entity_type = NULL, $entity_id = NULL, $r_index = NULL) {
    if (isset($entity_type)) {
      $this->related($entity_type, $entity_id, $r_index);
    }
    $this->entityCondition('entity_type', 'relation');
  }

  /**
   * Add a related entity to the query.
   *
   * @param $entity_type
   *   Entity type of the related entity.
   * @param $entity_id
   *   Entity id of the related entity. Can be an array of entity IDs.
   * @param $r_index
   *   The index of the related entity within the requested relation(s).
   *
   * @return RelationQuery
   */
  function related($entity_type, $entity_id, $r_index = NULL) {
    $this->fieldCondition('endpoints', 'entity_type', $entity_type, '=', $this->delta_group);
    $this->fieldCondition('endpoints', 'entity_id', $entity_id, NULL, $this->delta_group);
    if (isset($r_index)) {
      $this->fieldCondition('endpoints', 'r_index', $r_index, '=', $this->delta_group);
    }
    $this->delta_group++;
    return $this;
  }

  function execute() {
    $results = parent::execute();
    if ($this->count) {
      return $results;
    }
    return isset($results['relation']) ? $results['relation'] : array();
  }
}
