<?php

/**
 * @file
 * Support for relation destinations.
 */

/**
 * Destination class implementing migration into relation.
 */
class MigrateDestinationRelation extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'rid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Unique relation id (entity id).',
      ),
    );
  }

  /**
   * Basic initialization
   *
   * @param string $bundle
   *  A.k.a. the content type (page, article, etc.) of the node.
   * @param array $options
   *  Options applied to nodes.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct('relation', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for the relation type (bundle)
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    // First the core (relation table) properties
    $fields['rid'] = t('Relation: Existing relation ID');
    $fields['is_new'] = t('Relation: Indicates a new relation with the specified rid should be created');
    $fields['uid'] = t('Relation: Authored by (uid)');
    $fields['created'] = t('Relation: Created timestamp');
    $fields['changed'] = t('Relation: Modified timestamp');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle);
    $fields += migrate_handler_invoke_all('Relation', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Delete a batch of relations at once.
   *
   * @param $rids
   *  Array of relation IDs to be deleted.
   */
  public function bulkRollback(array $rids) {
    migrate_instrument_start('relation_delete_multiple');
    $this->prepareRollback($rids);
    relation_delete_multiple($rids);
    $this->completeRollback($rids);
    migrate_instrument_stop('relation_delete_multiple');
  }

  /**
   * Import a single relation.
   *
   * @param $relation
   *  Relation object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields (rid only in this case) of the relation that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $relation, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      // Make sure is_new is off
      $relation->is_new = FALSE;
      if (isset($relation->rid)) {
        if ($relation->rid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming rid !rid and map destination rid !destid1 don't match",
              array('!rid' => $relation->rid, '!destid1' => $row->migrate_map_destid1)
            ));
        }
      }
      else {
        $relation->rid = $row->migrate_map_destid1;
      }
      // Get the existing vid, so updates don't generate notices
      $values = db_select('relation', 'r')->fields('r', array('vid'))->condition('rid', $relation->rid)->execute()->fetchAssoc();
      $relation->vid = $values['vid'];
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($relation->rid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination rid provided'));
      }
      $old_relation = relation_load($relation->rid);
      if (!isset($relation->created)) {
        $relation->created = $old_relation->created;
      }
      if (!isset($relation->vid)) {
        $relation->vid = $old_relation->vid;
      }
      if (!isset($relation->uid)) {
        $relation->uid = $old_relation->uid;
      }
    }

    // Set some required properties.
    if (!isset($relation->uid)) {
      $relation->uid = $GLOBALS['user']->uid;
    }
    // Set type before invoking prepare handlers - they may take type-dependent actions.
    $relation->relation_type = $this->bundle;

    if ($migration->getSystemOfRecord() == Migration::SOURCE) {

      // relation_save() will blow these away, so save them here and
      // save them later.
      if (isset($relation->created)) {
        $created = MigrationBase::timestamp($relation->created);
      }
      if (isset($relation->changed)) {
        $changed = MigrationBase::timestamp($relation->changed);
      }
    }

    // Trying to update an existing relation
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      // Incoming data overrides existing data, so only copy non-existent fields
      foreach ($old_node as $field => $value) {
        // An explicit NULL in the source data means to wipe to old value (i.e.,
        // don't copy it over from $old_node)
        if (property_exists($relation, $field) && $relation->$field === NULL) {
          // Ignore this field
        }
        elseif (!isset($relation->$field)) {
          $relation->$field = $old_relation->$field;
        }
      }
    }

    // Invoke migration prepare handlers
    $this->prepare($relation, $row);

    if (isset($relation->rid) && empty($relation->is_new)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    // Save relation object
    migrate_instrument_start('relation_save');
    $rid = relation_save($relation);
    migrate_instrument_stop('relation_save');

    if (isset($relation->rid)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }

      // Update changed and created dates if needed.
      if (isset($changed)) {
        db_update('relation')->fields(array('changed' => $changed))->condition('rid', $relation->rid)->execute();
        $relation->changed = $changed;
      }

      if (isset($created)) {
        db_update('relation')->fields(array('created' => $created))->condition('rid', $relation->rid)->execute();
        $relation->created = $created;
      }

      $return = array($relation->rid);
    }
    else {
      $return = FALSE;
    }

    $this->complete($relation, $row);
    return $return;
  }
}

