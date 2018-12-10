<?php

/**
 * @file
 * Support for poll nodes.
 *
 * Each poll node will have multiple choices, and multiple votes. It's
 * not practical to bring this information in through your migration's
 * source query, you need to pull it separately in prepareRow():
 *
 * @code
 * ...
 * $this->addFieldMapping('active')
 *      ->defaultValue(1);
 * $this->addFieldMapping('runtime', 'seconds_to_run')
 * $this->addFieldMapping('choice', 'src_choices')
 *      ->description('src_choices populated in prepareRow()');
 * $this->addFieldMapping('votes', 'src_votes')
 *      ->description('src_votes populated in prepareRow()');
 * ...
 * public function prepareRow($row);
 *   $choices =  Database::getConnection('default', 'legacy')
 *               ->select('src_poll_choice', 'c')
 *               ->fields('c', array('choice_label', 'choice_order',
 *   'choice_total'))
 *               ->condition('c.choiceid', $row->src_contentid);
 *               ->execute();
 *   $row->src_choices = array();
 *   foreach ($choices as $choice) {
 *     $row->src_choices[] = array(
 *       'chtext' => $choice->choice_label,
 *       'chvotes' => $choice->choice_total,
 *       'weight' => $choice->choice_order,
 *     );
 *   }
 *   // Note that we won't know until much later what the chid is for each
 *   // choice, so it's best to tie the votes to choices by text.
 *   $query =  Database::getConnection('default', 'legacy')
 *             ->select('src_poll_vote', 'v')
 *             ->fields('v', array('choice_uid', 'hostname', 'timestamp'))
 *             ->condition('v.choiceid', $row->src_contentid);
 *   $votes = $query->innerJoin('src_poll_choice', 'c',
 *   'v.choice_id=c.choice_id')
 *            ->fields('c', array('choice_label'))
 *            ->execute();
 *   $row->src_votes = array();
 *   foreach ($votes as $vote) {
 *     $row->src_votes[] = array(
 *       'chtext' => $choice->choice_label,
 *       'uid' => $choice->choice_uid,
 *       'hostname' => $choice->hostname,
 *       'timestamp' => $choice->timestamp,
 *     );
 *   }
 *   return TRUE;
 * }
 * @endcode
 */

class MigratePollEntityHandler extends MigrateDestinationHandler {

  public function __construct() {
    $this->registerTypes(array('node'));
  }

  /**
   * Implementation of MigrateDestinationHandler::fields().
   */
  public function fields($entity_type, $bundle, $migration = NULL) {
    if ($bundle == 'poll') {
      $fields = array(
        'active' => t('Active status'),
        'runtime' => t('How long the poll runs for in seconds'),
        'choice' => t('Choices. Each choice is an array with chtext, chvotes, and weight keys.'),
        'votes' => t('Votes. Each vote is an array with chid (or chtext), uid, hostname, and timestamp keys'),
      );
    }
    else {
      $fields = array();
    }
    return $fields;
  }

  public function complete($entity, stdClass $row) {
    if ($entity->type == 'poll') {
      // Update settings overridden by !user_access('administer nodes') check in
      // poll_insert().
      db_update('poll')
        ->fields(array('active' => $entity->active))
        ->condition('nid', $entity->nid)
        ->execute();

      // Update vote summary count, again overridden by
      // !user_access('administer nodes') check in poll_insert().
      foreach ($row->choice as $choice) {
        // Have no mapping tracking for chid, so assume choice text is unique.
        db_update('poll_choice')
          ->fields(array(
            'chvotes' => $choice['chvotes'],
            'weight' => $choice['weight'],
          ))
          ->condition('nid', $entity->nid)
          ->condition('chtext', $choice['chtext'])
          ->execute();
      }

      // Insert actual votes.
      foreach ($row->votes as $vote) {
        if (!isset($vote['chid'])) {
          $result = db_select('poll_choice', 'pc')
            ->fields('pc', array('chid'))
            ->condition('pc.nid', $entity->nid)
            ->condition('pc.chtext', $vote['chtext'])
            ->execute();
          $chid = $result->fetchField();
        }
        else {
          $chid = $vote['chid'];
        }
        db_insert('poll_vote')
          ->fields(array(
            'chid' => $chid,
            'nid' => $entity->nid,
            'uid' => $vote['uid'],
            'hostname' => $vote['hostname'],
            'timestamp' => $vote['timestamp'],
          ))
          ->execute();
      }
    }
  }
}
