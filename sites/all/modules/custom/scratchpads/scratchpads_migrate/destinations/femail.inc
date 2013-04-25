<?php

class MigrateDestinationFemail extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'msgid' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Migrate Femail mappings');
  }

  public function import(stdClass $object, stdClass $row){
    db_insert('femail_msgs')->fields(array(
      'nid',
      'cid',
      'msgid'
    ), array(
      $row->nid,
      $row->cid,
      $row->msgid
    ))->execute();
    return array(
      $row->msgid
    );
  }

  public function fields(){
    return array(
      'nid' => t('Node id'),
      'cid' => t('Comment id'),
      'msgid' => t('Message id')
    );
  }

  public function rollback(array $ids){
    db_query("DELETE FROM femail_msgs");
    db_query("DELETE FROM femail_user_emails");
    db_query("DELETE FROM femail_user_subscriptions");
  }
}