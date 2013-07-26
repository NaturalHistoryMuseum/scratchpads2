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
      'nid' => t('Node ID'),
      'cid' => t('Comment ID'),
      'msgid' => t('Message ID')
    );
  }

  public function rollback(array $ids){
    db_query("DELETE FROM femail_msgs");
  }
}

class MigrateDestinationFemailEmail extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'uid' => array(
        'type' => 'int'
      )
    );
  }

  public function __toString(){
    return t('Migrate Femail user emails');
  }

  public function import(stdClass $object, stdClass $row){
    db_insert('femail_user_emails')->fields(array(
      'uid',
      'email',
      'status'
    ), array(
      $row->uid,
      $row->email,
      $row->status
    ))->execute();
    return array(
      $row->uid
    );
  }

  public function fields(){
    return array(
      'uid' => t('User ID'),
      'email' => t('Email'),
      'status' => t('Status')
    );
  }

  public function rollback(array $ids){
    db_query("DELETE FROM femail_user_emails");
  }
}

class MigrateDestinationFemailSubscription extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'uid' => array(
        'type' => 'int'
      ),
      'tid' => array(
        'type' => 'int'
      )
    );
  }

  public function __toString(){
    return t('Migrate Femail user subscriptions');
  }

  public function import(stdClass $object, stdClass $row){
    db_insert('femail_user_subscriptions')->fields(array(
      'uid',
      'tid'
    ), array(
      $row->uid,
      $row->tid
    ))->execute();
    return array(
      $row->uid,
      $row->tid
    );
  }

  public function fields(){
    return array(
      'uid' => t('User ID'),
      'tid' => t('Term ID')
    );
  }

  public function rollback(array $ids){
    db_query("DELETE FROM femail_user_subscriptions");
  }
}
