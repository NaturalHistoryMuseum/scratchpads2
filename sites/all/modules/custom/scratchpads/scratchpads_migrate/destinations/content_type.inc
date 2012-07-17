<?php

class MigrateDestinationContentType extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create content types');
  }

  public function import(stdClass $object, stdClass $row){
    $type = node_type_set_defaults();
    $type->type = $object->type;
    $type->name = $object->name;
    $type->description = $object->description;
    $type->help = $object->help;
    $type->title_label = $object->title_label;
    $type->has_title = TRUE;
    $type->base = 'node_content';
    $type->custom = 1;
    $type->modified = TRUE;
    $type->locked = 0;
    $type->module = 'node';
    $status = node_type_save($type);
    if($status){
      node_types_rebuild();
      menu_rebuild();
      $this->numCreated++;
      watchdog('scratchpads_migrate', t('Created node %type.'), array(
        '%type' => $object->type
      ), WATCHDOG_INFO);
      return array(
        $type->type
      );
    }else{
      throw new MigrateException(t('There was an error creating node %type.'), array(
        '%type' => $object->type
      ));
      return FALSE;
    }
  }

  public function fields(){
    return array(
      'type' => t('Machine name'),
      'name' => t('Human readable name'),
      'description' => t('Node type description'),
      'title_label' => t('Title label'),
    	'help' => t("Help")
    );
  }

  public function rollback(array $ids){
    node_type_delete($ids['destid1']);
  }
}