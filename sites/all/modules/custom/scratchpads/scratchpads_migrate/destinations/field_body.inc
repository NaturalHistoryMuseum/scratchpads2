<?php

class MigrateDestinationFieldBody extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create body fields');
  }

  public function import(stdClass $object, stdClass $row){
    if(node_type_load($object->type)){
      $instance = node_add_body_field($object, $object->body_label);
      // Has the field been created?
      if(isset($instance['field_id'])){
        // Do not show the summary
        $instance['settings']['display_summary'] = 0;
        field_update_instance($instance);
        $this->numCreated++;
        watchdog('scratchpads_migrate', t('Created body field for %type'), array(
          '%type' => $object->type
        ), WATCHDOG_INFO);
        return array(
          $object->type
        );
      }
    }else{
      throw new MigrateException(t('Node %type does not exist', array(
        '%type' => $object->type
      )));
    }
    return false;
  }

  public function fields(){
    return array(
      'type' => t('Machine name'),
      'body_label' => t('Title label')
    );
  }

  public function rollback(array $ids){
    $instance = field_read_instance('node', 'body_field', $ids['destid1']);
    field_delete_instance($instance);
  }
}