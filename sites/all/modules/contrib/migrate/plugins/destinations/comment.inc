<?php

/**
 * @file
 * Support for comment destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys

/**
 * Destination class implementing migration into comments.
 */
class MigrateDestinationComment extends MigrateDestinationEntity {

  static public function getKeySchema() {
    return array(
      'cid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination entity',
      ),
    );
  }

  /**
   * Save the original setting of comment_maintain_node_statistics
   *
   * @var boolean
   */
  protected $maintainNodeStatistics;

  /**
   * Return an options array for comment destinations.
   *
   * @param string $language
   *  Default language for comments created via this destination class.
   * @param string $text_format
   *  Default text format for comments created via this destination class.
   */
  static public function options($language, $text_format) {
    return compact('language', 'text_format');
  }

  /**
   * Basic initialization
   *
   * @param string $bundle
   *  A.k.a. the content type (page, article, etc.) of the ... comment?.
   * @param array $options
   *  Options applied to comments.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct('comment', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for comments attached to
   * a particular bundle (node type)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (comment table) properties
    $fields['cid'] = t('<a href="@doc">Existing comment ID</a>',
      array('@doc' => 'http://drupal.org/node/1349714#cid'));
    $fields['nid'] = t('<a href="@doc">Node (by Drupal ID)</a>',
      array('@doc' => 'http://drupal.org/node/1349714#nid'));
    $fields['uid'] = t('<a href="@doc">User (by Drupal ID)</a>',
      array('@doc' => 'http://drupal.org/node/1349714#uid'));
    $fields['pid'] = t('<a href="@doc">Parent (by Drupal ID)</a>',
      array('@doc' => 'http://drupal.org/node/1349714#pid'));
    $fields['subject'] = t('<a href="@doc">Subject</a>',
      array('@doc' => 'http://drupal.org/node/1349714#subject'));
    $fields['created'] = t('<a href="@doc">Created timestamp</a>',
      array('@doc' => 'http://drupal.org/node/1349714#created'));
    $fields['changed'] = t('<a href="@doc">Modified timestamp</a>',
      array('@doc' => 'http://drupal.org/node/1349714#changed'));
    $fields['status'] = t('<a href="@doc">Status</a>',
      array('@doc' => 'http://drupal.org/node/1349714#status'));
    $fields['hostname'] = t('<a href="@doc">Hostname/IP address</a>',
      array('@doc' => 'http://drupal.org/node/1349714#hostname'));
    $fields['name'] = t('<a href="@doc">User name (not username)</a>',
      array('@doc' => 'http://drupal.org/node/1349714#name'));
    $fields['mail'] = t('<a href="@doc">Email address</a>',
      array('@doc' => 'http://drupal.org/node/1349714#mail'));
    $fields['homepage'] = t('<a href="@doc">Homepage</a>',
      array('@doc' => 'http://drupal.org/node/1349714#homepage'));
    $fields['language'] = t('<a href="@doc">Language</a>',
      array('@doc' => 'http://drupal.org/node/1349714#language'));
    $fields['thread'] = t('<a href="@doc">Thread</a>',
      array('@doc' => 'http://drupal.org/node/1349714#thread'));

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('Comment', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
  }

  /**
   * Delete a batch of comments at once.
   *
   * @param $cids
   *  Array of comment IDs to be deleted.
   */
  public function bulkRollback(array $cids) {
    migrate_instrument_start('comment_delete_multiple');
    $this->prepareRollback($cids);
    $result = comment_delete_multiple($cids);
    $this->completeRollback($cids);
    migrate_instrument_stop('comment_delete_multiple');
    return $result;
  }

  /**
   * Import a single comment.
   *
   * @param $comment
   *  Comment object to build. Prefilled with any fields mapped in the
   *   Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields (cid only in this case) of the comment that was saved
   *   if successful. FALSE on failure.
   */
  public function import(stdClass $comment, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      if (isset($comment->cid)) {
        if ($comment->cid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming cid !cid and map destination nid !destid1 don't match",
            array(
              '!cid' => $comment->cid,
              '!destid1' => $row->migrate_map_destid1,
            )));
        }
      }
      else {
        $comment->cid = $row->migrate_map_destid1;
      }
    }
    // Fix up timestamps
    if (isset($comment->created)) {
      $comment->created = MigrationBase::timestamp($comment->created);
    }
    if (isset($comment->changed)) {
      $comment->changed = MigrationBase::timestamp($comment->changed);
    }

    if (!isset($comment->node_type)) {
      $comment->node_type = $this->bundle;
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($comment->cid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination cid provided'));
      }
      $rawcomment = $comment;
      $old_comment = comment_load($comment->cid);
      if (empty($old_comment)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but commend !cid does not exist',
          array('!cid' => $comment->cid)));
      }
      if (!isset($comment->nid)) {
        $comment->nid = $old_comment->nid;
      }
      if (!isset($comment->created)) {
        $comment->created = $old_comment->created;
      }
      if (!isset($comment->changed)) {
        $comment->changed = $old_comment->changed;
      }
      $this->prepare($comment, $row);
      foreach ($rawcomment as $field => $value) {
        $old_comment->{$field} = $comment->{$field};
      }
      $comment = $old_comment;
    }
    else {
      // Set some default properties.
      $defaults = array(
        'language' => $this->language,
        'node_type' => $this->bundle,
        'subject' => '',
        'status' => COMMENT_PUBLISHED,
        'uid' => 0,
        'cid' => 0,
        'pid' => 0,
      );
      foreach ($defaults as $field => $value) {
        if (!isset($comment->{$field})) {
          $comment->{$field} = $value;
        }
      }

      $this->prepare($comment, $row);
      // Make sure we have a nid
      if (!isset($comment->nid) || !$comment->nid) {
        throw new MigrateException(t('No node ID provided for comment'));
      }

      // comment_save() hardcodes hostname, so if we're trying to set it we
      // need to save it and apply it after
      if (isset($comment->hostname)) {
        $hostname = $comment->hostname;
      }
    }

    if (isset($comment->cid) && $comment->cid) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    // Validate field data prior to saving.
    MigrateDestinationEntity::fieldAttachValidate('comment', $comment);

    migrate_instrument_start('comment_save');
    comment_save($comment);
    migrate_instrument_stop('comment_save');
    if (isset($hostname) && isset($comment->cid) && $comment->cid > 0) {
      db_update('comment')
        ->fields(array('hostname' => $hostname))
        ->condition('cid', $comment->cid)
        ->execute();
    }
    $this->complete($comment, $row);
    if (isset($comment->cid) && $comment->cid > 0) {
      $return = array($comment->cid);
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
    }
    else {
      $return = FALSE;
    }
    return $return;
  }

  /**
   * Implements MigrateDestination::preImport().
   */
  public function preImport() {
    $this->disableStatistics();
  }

  /**
   * Implements MigrateDestination::postImport().
   */
  public function postImport() {
    $this->enableStatistics();
  }

  /**
   * Implements MigrateDestination::preRollback().
   */
  public function preRollback() {
    $this->disableStatistics();
  }

  /**
   * Implements MigrateDestination::postRollback().
   */
  public function postRollback() {
    $this->enableStatistics();
  }

  /**
   * Updating node statistics on every comment imported or rolled back is
   * expensive. We disable node statistics while performing imports and
   * rollbacks, then re-enable and compute them in bulk when done.
   */
  protected function disableStatistics() {
    // If maintaining node statistics is enabled, temporarily disable it
    $this->maintainNodeStatistics =
      variable_get('comment_maintain_node_statistics', TRUE);
    if ($this->maintainNodeStatistics) {
      $GLOBALS['conf']['comment_maintain_node_statistics'] = FALSE;
    }
  }

  /**
   * Re-enable and recompute node statistics after an import or rollback
   * operation.
   */
  protected function enableStatistics() {
    // If originally enabled, re-enable and rebuild the stats
    if ($this->maintainNodeStatistics) {
      $GLOBALS['conf']['comment_maintain_node_statistics'] = TRUE;

      // Copied from devel_rebuild_node_comment_statistics

      // Empty table
      db_truncate('node_comment_statistics')->execute();

      // DBTNG. IGNORE keyword is not compatible with Postgres. SQLite?
      switch (db_driver()) {
        case 'pgsql':
          // We still want to run this under Postgres. On the very rare occasion
          // when we have 2 comments on the same node with the same timestamp
          // we will lose data.
          $sql = "
            INSERT INTO {node_comment_statistics} (nid, cid, last_comment_timestamp, last_comment_name, last_comment_uid, comment_count) (
              SELECT c.nid, c.cid, c.created, c.name, c.uid, c2.comment_count
              FROM {comment} c
              JOIN (
                SELECT c.nid, MAX(c.created) AS created, COUNT(*) AS comment_count
                FROM {comment} c
                WHERE status=:published
                GROUP BY c.nid
              ) AS c2 ON c.nid = c2.nid AND c.created=c2.created
            )";
          break;

        default:
          $sql = "
            INSERT IGNORE INTO {node_comment_statistics} (nid, cid, last_comment_timestamp, last_comment_name, last_comment_uid, comment_count) (
              SELECT c.nid, c.cid, c.created, c.name, c.uid, c2.comment_count
              FROM {comment} c
              JOIN (
                SELECT c.nid, MAX(c.created) AS created, COUNT(*) AS comment_count
                FROM {comment} c
                WHERE status=:published
                GROUP BY c.nid
              ) AS c2 ON c.nid = c2.nid AND c.created=c2.created
            )";
      }
      try {
        db_query($sql, array(':published' => COMMENT_PUBLISHED));
      } catch (Exception $e) {
        // Our edge case has been hit. A Postgres migration has likely just
        // lost data. Let the user know.
        Migration::displayMessage(t('Failed to update node comment statistics: !message',
          array('!message' => $e->getMessage())
        ));
      }

      // Insert records into the node_comment_statistics for nodes that are missing.
      $query = db_select('node', 'n');
      $query->leftJoin('node_comment_statistics', 'ncs', 'ncs.nid = n.nid');
      $query->addField('n', 'created', 'last_comment_timestamp');
      $query->addField('n', 'uid', 'last_comment_uid');
      $query->addField('n', 'nid');
      $query->addExpression('0', 'comment_count');
      $query->addExpression('NULL', 'last_comment_name');
      $query->isNull('ncs.comment_count');

      db_insert('node_comment_statistics')
        ->from($query)
        ->execute();
    }
  }
}

class MigrateCommentNodeHandler extends MigrateDestinationHandler {

  public function __construct() {
    $this->registerTypes(array('node'));
  }

  /**
   * Implementation of MigrateDestinationHandler::fields().
   */
  public function fields($entity_type, $bundle, $migration = NULL) {
    $fields = array();
    $fields['comment'] = t('Whether comments may be posted to the node');
    return $fields;
  }
}
