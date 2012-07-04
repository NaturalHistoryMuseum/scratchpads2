<?php
// $Id$

/**
 * @file
 * VotingAPI module integration
 */

/**
 * Destination class for the votingapi_vote table.
 */
class MigrateDestinationVotingapi extends MigrateDestination {
  public function __toString() {
    return t('votingapi');
  }
  
  /**
   * An array with content ids of imported votes. Used for recalculating results.
   */
  var $importedIds = array();

  static public function getKeySchema() {
    return array(
      'vote_id' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete the provided votes and recalculate the results.
   *
   * @param $id
   *  IDs to be deleted.
   */
  public function bulkRollback(array $ids) {
    migrate_instrument_start(__METHOD__);
    
    foreach ($ids as $id) {
      $votes[]['vote_id'] = $id;
    }
    votingapi_delete_votes($votes);
    
    // foreach($votes as $vote) {
    //       votingapi_recalculate_results($vote['content_type'], $vote['content_id'], TRUE);
    //     }

    migrate_instrument_stop(__METHOD__);
  }

  /**
   * Import a single vote.
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
    $this->prepare($entity, $row);

    if (isset($entity->timestamp)) {
      $entity->timestamp = Migration::timestamp($entity->timestamp);
    }

    // Just converting $entity to an array doesn't work...
    $vote = array();
    $vote['entity_type'] = $entity->entity_type;
    $vote['entity_id'] = $entity->entity_id;
    $vote['value_type'] = $entity->value_type;
    $vote['value'] = $entity->value;
    $vote['uid'] = $entity->uid;
    $vote['tag'] = $entity->tag;
    $vote['timestamp'] = $entity->timestamp;
    $vote['vote_source'] = $entity->vote_source;
    votingapi_add_votes($vote);
    if (isset($vote[0]['vote_id'])) {
      $entity->vote_id = $vote[0]['vote_id'];
      $this->complete($entity, $row);
      $this->importedIds[$entity->entity_type][] = $entity->entity_id;
      return array($entity->vote_id);
    }
    else {
      return FALSE;
    }
  }

  /**
   * We're done with importing votes, recalculate the results.
   */
  function postImport() {
    $this->importedIds = array_unique($this->importedIds);

    foreach ($this->importedIds as $entity_type => $entity_ids) {
      foreach ($entity_ids as $entity_id) {
        votingapi_recalculate_results($entity_type, $entity_id, TRUE);
      }
    }
  }

  /**
   * Returns a list of fields available to be mapped/
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    return array(
        'vote_id' => 'Vote ID',
        'entity_type' => "Entity Type (defaults to 'node')",
        'entity_id' => 'Entity ID',
        'value' => 'Numeric vote value',
        'value_type' => "Value type (percent/points, defaults to 'percent')",
        'tag' => "Tag (defaults to 'vote')",
        'uid' => 'User ID',
        'timestamp' => 'Timestamp',
        'vote_source' => 'Vote Source IP Address',
    );
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *  Entity object to build. Prefilled with any fields mapped in the Migration.
   * @param $source_row
   *  Raw source data object - passed through to prepare handlers.
   */
  public function prepare(stdClass $entity, stdClass $source_row) {
    $migration = Migration::currentMigration();
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $source_row);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $object
   *  Entity object to build. This is the complete object after saving.
   * @param $source_row
   *  Raw source data object - passed through to complete handlers.
   */
  public function complete(stdClass $entity, stdClass $source_row) {
    $migration = Migration::currentMigration();

    // Call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $source_row);
    }
  }
}
