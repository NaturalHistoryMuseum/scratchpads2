<?php

/**
 * Handling specific to a Drupal 6 source for comments.
 */
class DrupalComment6Migration extends DrupalCommentMigration {

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Version-specific field mappings
    $this->addFieldMapping('comment_body', 'comment');
    $this->addFieldMapping('comment_body:format', 'format');

    $this->addFieldMapping('created', 'timestamp');
    $this->addFieldMapping('changed', 'timestamp');
  }

  /**
   * Implementation of DrupalCommentMigration::commentQuery().
   *
   * We join to {node} so that we can use separate comment migration classes
   * for each associated node type.
   *
   * @return SelectQueryInterface
   */
  protected function commentQuery($sourceType) {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('comments', 'c')
             ->fields('c', array('cid', 'pid', 'nid', 'uid', 'subject',
               'comment', 'hostname', 'timestamp', 'status', 'thread', 'name',
               'mail', 'homepage', 'format'));
    $query->join('node', 'n', 'c.nid = n.nid');
    $query->condition('n.type', $sourceType);

    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}
