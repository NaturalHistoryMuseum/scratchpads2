<?php

/**
 * @file
 * Import User Relationships.
 */

/**
 * Destination class implementing migration into user_relationships table.
 */
class MigrateDestinationUserRelationships extends MigrateDestination {
  protected $typeID;
  protected $typeName;

  /**
   * Construct a destination for the specified user relationship type. Interprets
   * the type as a type name - if that fails, assumes it's a type ID (rtid).
   *
   * @param mixed $type_name
   *  Name of a user relationship type, or its rtid.
   */
  public function __construct($type_name) {
    parent::__construct();
    $type = user_relationships_type_load(array('name' => $type_name));
    if ($type) {
      $this->typeName = $type_name;
      $this->typeID = $type->rtid;
    }
    else {
      $type = user_relationships_type_load($type_name);
      $this->typeName = $type->name;
      $this->typeID = $type_name;
    }
  }

  static public function getKeySchema() {
    return array(
      'rid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Relationship ID',
      ),
    );
  }

  /**
   * Delete a membership.
   *
 * @param $id
   *  ID to be deleted.
   */
  public function rollback(array $id) {
    migrate_instrument_start(__METHOD__);
    if ($relationship = user_relationships_load(array('rid' => $id['destid1']))) {
      $account = new stdClass;
      user_relationships_delete_relationship(current($relationship), $account);
    }
    migrate_instrument_stop(__METHOD__);
  }

  /**
   * Import a single membership.
   *
   * @param $entity
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    // Normalize to unix timestamps.
    foreach (array('created_at', 'updated_at') as $property) {
      if (isset($entity->$property)) {
        $entity->$property = Migration::timestamp($entity->$property);
      }
    }

    $entity->rtid = $this->typeID;

    $op = isset($entity->op) ? $entity->op : 'approve';
    if ($saved = user_relationships_save_relationship($entity, $op)) {
      return array($saved->rid);
    }
  }

  public function fields() {
    return array(
        'rid' => 'Relationship ID',
        'requester_id' => 'User ID of relationship requestor',
        'requestee_id' => 'User ID of relationship requestee',
        'approved' => 'Whether the requestee approved the relationship',
        'created_at' => 'Timestamp when the relationship was created',
        'updated_at' => 'Timestamp when the relationship was last updated',
        'flags' => 'UR_OK (0) or UR_BANNED (1)',
        'op' => 'Default value is \'approve\'. Sent as second param to user_relationships_save_relationship().'
    );
  }

  public function __toString() {
    return t('User relationship type: !type', array('!type' => $this->typeName));
  }
}
