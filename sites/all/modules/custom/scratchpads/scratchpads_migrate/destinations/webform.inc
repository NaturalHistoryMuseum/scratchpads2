<?php

class MigrateDestinationWebform extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'nid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Webform node nid'
      )
    );
  }

  public function __toString(){
    return t('Create webform settings');
  }

  public function import(stdClass $object, stdClass $row){
    $webform_tables = array(
      'webform' => array(
        'nid',
        'confirmation',
        'confirmation_format',
        'redirect_url',
        'status',
        'block',
        'teaser',
        'allow_draft',
        'auto_save',
        'submit_notice',
        'submit_text',
        'submit_limit',
        'submit_interval',
        'total_submit_limit',
        'total_submit_interval'
      ),
      'webform_component' => array(
        'nid',
        'cid',
        'pid',
        'form_key',
        'name',
        'type',
        'value',
        'extra',
        'mandatory',
        'weight'
      ),
      'webform_emails' => array(
        'nid',
        'eid',
        'email',
        'subject',
        'from_name',
        'from_address',
        'template',
        'excluded_components',
        'html',
        'attachments'
      ),
      'webform_last_download' => array(
        'nid',
        'uid',
        'sid',
        'requested'
      ),
      'webform_roles' => array(
        'nid',
        'rid'
      ),
      'webform_submissions' => array(
        'sid',
        'nid',
        'uid',
        'is_draft',
        'submitted',
        'remote_addr'
      ),
      'webform_submitted_data' => array(
        'nid',
        'sid',
        'cid',
        'no',
        'data'
      )
    );
    $nid = $row->nid;
    foreach($webform_tables as $table => $fields){
      if($table == 'webform_roles'){
        db_query("DELETE FROM webform_roles");
      }
      $result = Database::getConnection('default', 'scratchpad_1')->select($table, 't')->fields('t', $fields)->condition('nid', $nid)->execute();
      foreach($result as $table_row){
        if($table == 'webform'){
          db_update($table)->fields((array)$table_row)->condition('nid', $nid)->execute();
        }else{
          db_insert($table)->fields((array)$table_row)->execute();
        }
      }
    }
    return array(
      $row->nid
    );
  }

  public function fields(){
    return array(
      'nid' => t('Nid')
    );
  }
}