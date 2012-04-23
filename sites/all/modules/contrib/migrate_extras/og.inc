<?php

/**
 * @file
 * Import Organic group memberships.
 */

/**
 * Destination class implementing migration into an OG Audience field.
 */
class MigrateDestinationOGMembership extends MigrateDestination {

  function __construct() {
    // Get the administrator role ID.
    $roles = array_flip(og_get_global_roles());
    $this->rid = $roles[OG_ADMINISTRATOR_ROLE];
  }

  public function __toString() {
    return t('group');
  }
  
  static public function getKeySchema() {
    return array(
      'gid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Entity ID of group',
      ),
      'uid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'UID of member',
      ),
    );
  }

  /**
   * Delete a membership.
   * @param $id
   *  ID to be deleted.
   */
  public function rollback(array $id) {
    migrate_instrument_start('OGMembership Rollback');
    og_ungroup($id['gid'], 'user', $id['uid']);
    migrate_instrument_stop('OGMembership Rollback');
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
    if (empty($entity->gid)) {
      throw new MigrateException('Missing $entity->gid');
    }
    if (empty($entity->uid)) {
      throw new MigrateException('Missing $entity->uid');
    }

    // Slow, but is required by OG API - og_group().
    $account = user_load($entity->uid);

    $values = array(
        'entity' => $account,
        'state' => isset($entity->state) ? $entity->state : OG_STATE_ACTIVE, // Other state is OG_STATE_PENDING
        'created' => isset($entity->created) ? $entity->created : REQUEST_TIME,
    );
    og_group($entity->gid, $values);

    if ($entity->is_admin) {
      og_role_grant($entity->gid, $entity->uid, $this->rid);
    }

    // No failure handling in OG
    return array($entity->gid, $entity->uid);
  }

  public function fields() {
    return array(
        'gid' => 'Group entity id',
        'uid' => 'User ID',
        'state' => 'User\'s group membership can be OG_STATE_ACTIVE or OG_STATE_PENDING. Defaults to ACTIVE.',
        'is_admin' => 'Is member an administrator in this group. Defaults to No.',
        'created' => 'Create date for this membership. Defaults to REQUEST_TIME',
    );
  }
}

class MigrateGroupFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('group'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $migration = Migration::currentMigration();
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    $return = array();
    foreach ($values as $value) {
      // Don't save empty references
      if ($value) {
        $return[$language][$delta]['gid'] = $value;
        $delta++;
      }
    }
    return $return;
  }
}
