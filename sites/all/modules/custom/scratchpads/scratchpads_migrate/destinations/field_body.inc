<?php

class MigrateDestinationFieldBody extends MigrateDestination{

  protected $entityType = array();

  public function __construct($entity_type = 'node'){
    $this->entityType = $entity_type;
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
    if($this->entityType == 'node'){
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
    }else{
      $field = field_info_field('field_body');
      $instance = field_info_instance($this->entityType, 'field_body', $object->type);
      if(empty($field)){
        $field = array(
          'field_name' => 'field_body',
          'type' => 'text_with_summary',
          'entity_types' => array(
            $this->entityType
          )
        );
        $field = field_create_field($field);
      }
      if(empty($instance)){
        $instance = array(
          'field_name' => 'field_body',
          'entity_type' => $this->entityType,
          'bundle' => $object->type,
          'label' => 'Body',
          'widget' => array(
            'type' => 'text_textarea_with_summary'
          ),
          'settings' => array(
            'display_summary' => TRUE
          ),
          'display' => array(
            'default' => array(
              'label' => 'hidden',
              'type' => 'text_default'
            ),
            'teaser' => array(
              'label' => 'hidden',
              'type' => 'text_summary_or_trimmed'
            )
          )
        );
        $instance = field_create_instance($instance);
      }
      $this->numCreated++;
      watchdog('scratchpads_migrate', t('Created body field for %type'), array(
        '%type' => $object->type
      ), WATCHDOG_INFO);
      return array(
        $object->type
      );
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