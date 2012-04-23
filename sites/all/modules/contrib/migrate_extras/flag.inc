<?php

/**
 * @file
 * Flag module integration
 */

/**
 * Destination class implementing when you want just an insert into flag_content table.
 */
class MigrateDestinationFlagSimple extends MigrateDestination {
  public function __construct($fid) {
    parent::__construct();
    $this->fid = $fid;
  }

  public function __toString() {
    $flag = flag_get_flag(NULL, $this->fid);
    return t('flag (!flag)', array('!flag' => $flag->name));
  }

  static public function getKeySchema() {
    return array(
      'fcid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete a membership.
   *
   * @param $id
   *  IDs to be deleted.
   */
  public function bulkRollback(array $ids) {
    migrate_instrument_start(__METHOD__);
    db_delete('flag_content')
      ->condition('fcid', $ids, 'IN')
      ->execute();
    migrate_instrument_stop(__METHOD__);
  }

  /**
   * Import a single flag_content.
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
    if (isset($entity->timestamp)) {
      $entity->timestamp = Migration::timestamp($entity->timestamp);
    }

    $entity->fid = $this->fid;

    if (!empty($entity->fcid)) {
      $return = drupal_write_record('flag_content', $entity, 'fcid');
    }
    else {
      $return = drupal_write_record('flag_content', $entity);
    }
    if ($return) {
      // Update the flag_counts table.
      $count = db_select('flag_content')
        ->fields('flag_content')
        ->condition('fid', $this->fid)
        ->condition('content_type', $entity->content_type)
        ->condition('content_id', $entity->content_id)
        ->countQuery()
        ->execute()
        ->fetchField();
      db_merge('flag_counts')
        ->key(array(
          'fid' => $this->fid,
          'content_id' => $entity->content_id,
        ))
        ->fields(array(
          'content_type' => $entity->content_type,
          'count' => $count,
          'last_updated' => REQUEST_TIME,
        ))
        ->execute();
      return array($entity->fcid);
    }
  }

  public function fields() {
    return array(
        'fcid' => 'Flag content ID',
        'fid' => 'Flag ID',
        'content_type' => '',
        'content_id' => '',
        'uid' => 'User ID',
        'timestamp' => '',
    );
  }
}

/**
 * Field handler - this will expose flags as fields on the object they're
 * attached to, and set the flag to the value mapped in addFieldMapping().
 */
abstract class MigrateExtrasFlagHandler extends MigrateDestinationHandler {
  /**
   * Make the flags assigned to this object visible.
   */
  public function fields($entity_type, $bundle) {
    $fields = array();
    if (module_exists('flag')) {
      $flags = flag_get_flags($entity_type, $bundle);
      foreach ($flags as $flag_name => $flag) {
        $fields[$flag_name] = $flag->title;
      }
    }
    return $fields;
  }
}

/**
 * Because we can't identify what kind of entity is passed to complete, we
 * implement a separate handler for each type.
 */
class MigrateExtrasNodeFlagHandler extends MigrateExtrasFlagHandler {
  public function __construct() {
    $this->registerTypes(array('node'));
  }

  public function complete($node, stdClass $row) {
    if (!module_exists('flag')) return;

    $flags = flag_get_flags('node', $node->type);
    $fields = array();
    foreach ($flags as $flag_name => $flag) {
      if (!empty($node->$flag_name)) {
        flag('flag', $flag_name, $node->nid);
      }
    }
  }
}

class MigrateExtrasUserFlagHandler extends MigrateExtrasFlagHandler {
  public function __construct() {
    $this->registerTypes(array('user'));
  }

  public function complete($user, stdClass $row) {
    if (!module_exists('flag')) return;

    $flags = flag_get_flags('user', 'user');
    $fields = array();
    foreach ($flags as $flag_name => $flag) {
      if (!empty($user->$flag_name)) {
        flag('flag', $flag_name, $user->uid);
      }
    }
  }
}
