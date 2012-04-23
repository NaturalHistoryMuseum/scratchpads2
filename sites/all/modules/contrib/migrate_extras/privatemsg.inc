<?php

/**
 * @file
 * Privatemag module integration
 *
 * Limitations:
 *   - No updating of messages.
 *   - No threading
 *   - Messages are marked as deleted and not actually deleted. Thats the
 *       privatemsg API.
 *   - All these limitations can be helped by http://drupal.org/node/1184984.
 */

class MigrateDestinationPrivateMsg extends MigrateDestinationEntity {

  /**
   * An array with content ids of imported messages. Not yet used.
   */
  var $importedIds = array();

  static public function getKeySchema() {
    return array(
      'mid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to private messages.
   */
  public function __construct(array $options = array()) {
    parent::__construct('privatemsg_message', 'privatemsg_message', $options);
  }

  /**
   * Returns a list of fields available to be mapped/
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array(
        // 'mid' => 'Message ID', // Updating not supported. See http://drupal.org/node/1184984.
        'subject' => 'Subject',
        'body' => 'Body',
        'format' => 'Text format name for the Body',
        'recipients' => 'User IDs of recipients',
        'timestamp' => 'Timestamp',
        'author' => 'User ID of author',
        'is_new' => 'TRUE if unread by recipient, FALSE if read by recipient',
    );

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle);
    $fields += migrate_handler_invoke_all('PrivateMsg', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Mark provided message as deleted.
   *
   * @param $id
   *  IDs to be deleted.
   */
  public function rollback(array $id) {
    migrate_instrument_start(__METHOD__);
    // Delete recipients of the message.
    db_delete('pm_index')
      ->condition('mid', reset($id))
      ->execute();
    // Delete message itself.
    db_delete('pm_message')
      ->condition('mid', reset($id))
      ->execute();
    migrate_instrument_stop(__METHOD__);
  }

  /**
   * Import a single message.
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

    // The privatemsg API does not support updating. See http://drupal.org/node/1184984
    // $message['mid'] = $entity->mid;

    // The two user_load() calls here could by slow. If so, one could experiment
    // with entity cache module - http://drupal.org/project/entitycache.
    $options = array();
    if (isset($entity->timestamp)) $options['timestamp'] = Migration::timestamp($entity->timestamp);
    if (isset($entity->author)) $options['author'] = user_load($entity->author);
    if (isset($entity->format)) $options['format'] = $entity->format;

    if (!is_array($entity->recipients)) {
      $entity->recipients = array($entity->recipients);
    }

    foreach ($entity->recipients as $uid) {
      $entity->to[] = user_load($uid);
    }

    // FYI, API is at http://api.worldempire.ch/api/privatemsg/privatemsg.module/function/privatemsg_new_thread/7-2
    $return = privatemsg_new_thread($entity->to, $entity->subject, $entity->body, $options);

    if (!empty($return['success'])) {
      $this->complete((object)$return, $row);
      // Set the read status for the recipient (defaults to unread, so only need
      // to set if read)
      $mid = $return['message']->mid;
      if (isset($entity->is_new) && $entity->is_new == PRIVATEMSG_READ) {
        foreach ($entity->to as $account) {
          privatemsg_message_change_status($mid, $entity->is_new, $account);
        }
      }
      return array($mid);
    }
    else {
      $migration = Migration::currentMigration();
      $migration->saveMessage(reset($return['messages']['error']));
      return FALSE;
    }
  }

}
