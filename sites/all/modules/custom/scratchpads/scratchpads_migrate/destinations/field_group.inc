<?php

class MigrateDestinationFieldGroup extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'group_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  static public function getFieldGroupModes(){
    return array(
      'form',
      'default'
    );
  }

  public function __toString(){
    return t('Create field groups');
  }

  public function import(stdClass $object, stdClass $row){
    // Ensure the node type exists
    $node_info = node_entity_info();
    if(!isset($node_info['node']['bundles'][$object->bundle])){// If it doesn't throw an exception
throw new MigrateException(t("Node type %type does not exist", array(
        '%type' => $object->bundle
      )));}
    $settings = unserialize($row->settings);
    $status = 0;
    // Create both front end and form field groups
    foreach($this->getFieldGroupModes() as $mode){
      $object->mode = $mode;
      $object->identifier = $object->group_name . '|' . $object->entity_type . '|' . $object->bundle . '|' . $mode;
      $object->disabled = false;
      switch($mode){
        case 'form':
          $object->format_type = $settings['form']['style'];
          break;
        // For front end 
        case 'default':
          $object->format_type = 'div';
          break;
      }
      // Save the group
      // Save and enable it in ctools
      $status += ctools_export_crud_save('field_group', $object);
      ctools_export_crud_enable('field_group', $object->identifier);
    }
    if($status == 2 * SAVED_NEW){
      $this->numCreated++;
      watchdog('scratchpads_migrate', t('Added field group %name to %entity_type:%bundle'), array(
        '%name' => $object->group_name,
        '%entity_type' => $object->entity_type,
        '%bundle' => $object->bundle
      ), WATCHDOG_INFO);
      return array(
        $object->entity_type . ':' . $object->bundle . ':' . $object->group_name
      );
    }else{
      return FALSE;
    }
  }

  public function fields(){
    return array(
      'bundle' => t('Bundle'),
      'label' => t('Label'),
      'group_name' => t('Group name'),
      'weight' => t("Weight"),
      'entity_type' => t('Entity type'),
      'children' => t('Children')
    );
  }

  public function rollback(array $ids){
    list($entity_type, $bundle, $group_name) = explode(':', $ids['destid1']);
    foreach($this->getFieldGroupModes() as $mode){
      $group = field_group_load_field_group($group_name, $entity_type, $bundle, $mode);
      if(!empty($group)){
        field_group_group_export_delete($group, false);
      }
    }
  }
}