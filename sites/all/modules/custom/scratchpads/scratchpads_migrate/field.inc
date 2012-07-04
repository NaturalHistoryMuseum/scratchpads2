<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){
      watchdog('scratchpads_migrate', 'nope');
      return FALSE;
    }
    // Check if this field has a different destination entity type / bundle
    $mapping = scratchpads_migrate_type_mappings($row->type_name);
    if($mapping && is_array($mapping)){
      $row->entity_type = $mapping['entity_type'];
      $row->type_name = $mapping['bundle'];
      if(isset($mapping['fields']) && isset($mapping['fields'][$row->field_name])){
        $row->field_name = $mapping['fields'][$row->field_name];
      }
    }else{
      $row->entity_type = 'node';
    }
    // We want all image fields to use the media field
    if($row->type == 'image' || $row->type == 'filefield'){
      $row->field_name = 'field_media';
    }
    // Does an instance of this field already exists?
    if(field_read_instance($row->entity_type, $row->field_name, $row->type_name)){
      watchdog('scratchpads_migrate', t('Skipped field %name - already exists %entity_type:%bundle'), array(
        '%name' => $row->field_name,
        '%entity_type' => $row->entity_type,
        '%bundle' => $row->type_name
      ), WATCHDOG_NOTICE);
      return false; // Instance already exists, so skip it
    }
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function sourceQuery();
}

class MigrateDestinationField extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'field_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create fields');
  }

  public function import(stdClass $object, stdClass $row){
    // Entity type has been set in prepareRow()
    $object->entity_type = $row->entity_type;
    $field_instance = (array)$object;
    $field_instance['widget'] = array(
      'active' => $row->active,
      'weight' => $row->weight,
      'type' => $row->widget_type,
      'module' => $row->widget_module,
      'settings' => unserialize($row->widget_settings),
      'description' => $row->description
    );
    $field_instance['display'] = unserialize($row->display_settings);
    if(is_array($field_instance['display'])){
      foreach(array_keys($field_instance['display']) as $display){
        // Ensure it's an array
        // Fields groups are added as a string here, so it's safe to remove
        // FieldGroup import will handle the groups 
        if(is_array($display)){
          $field_instance['display'][$display]['type'] = $field_instance['display'][$display]['format'];
        }else{
          unset($field_instance['display'][$display]);
        }
      }
    }
    // Only create if the field does not already exist
    $field = field_info_field($object->field_name);
    if(empty($field)){
      // Create the field if it doesn't exist
      $field_config = array(
        'active' => $row->active,
        'cardinality' => ($row->multiple == 0 ? 1 : -1),
        'deleted' => '0',
        'entity_types' => array(
          $object->entity_type
        ),
        'field_name' => $object->field_name,
        'module' => $row->module,
        'settings' => unserialize($row->global_settings),
        'translatable' => '1',
        'type' => $row->type
      );
      // Instance alterations
      drupal_alter('content_migrate_instance', $field_instance, $field_config);
      // Let content migrate module change all the field settings for D7 
      drupal_alter('content_migrate_field', $field_config, $field_instance);
      // Merge settings
      $field_instance['settings'] = array_merge_recursive($field_config['settings'], $field_instance['widget']['settings']);
      // Make my own alterations
      switch($field_config['type']){
        case 'filefield':
          $field_config['module'] = 'image';
          $field_config['type'] = 'image';
          break;
      }
      switch($field_instance['widget']['type']){
        case 'image':
          $field_instance['widget']['type'] = 'media_generic';
          $field_instance['widget']['module'] = 'media';
          $settings = array(
            'file_extensions' => 'file_path',
            'max_filesize' => 'max_filesize_per_file',
            'file_directory' => 'file_path',
            'description_field' => 'file_path',
            'max_resolution' => 'max_resolution',
            'min_resolution' => 'min_resolution',
            'alt_field' => 'custom_alt',
            'title_field' => 'custom_title'
          );
          foreach($settings as $new_setting => $old_setting){
            if(isset($field_instance['widget']['settings'][$old_setting])){
              $field_instance['widget']['settings'][$new_setting] = $field_instance['widget']['settings'][$old_setting];
            }
          }
          break;
      }
      // Check the widget exists
      if(!field_info_widget_types($field_instance['widget']['type'])){
        throw new MigrateException(t('Field widget %widget does not exist', array(
          '%widget' => $field_instance['widget']['type']
        )));
        return false;
      }
      // Check the module exists - needs to happen here after the field has been altered
      if(!module_exists($field_config['module'])){
        throw new MigrateException(t('Module %module does not exist', array(
          '%module' => $field_config['module']
        )));
        return false;
      }
      try{
        field_create_field($field_config);
      }
      catch(FieldException $e){
        throw new MigrateException($e->getMessage());
        return false;
      }
    }
    if(isset($field_instance['widget']['settings']['default_value'])){
      $field_instance['default_value'] = array_shift($field_instance['widget']['settings']['default_value'][0]);
    }
    try{
      $field_instance = field_create_instance($field_instance);
      // Has the field been created?
      if(isset($field_instance['field_id'])){
        watchdog('scratchpads_migrate', t('Added field %name to %entity_type:%bundle'), array(
          '%name' => $object->field_name,
          '%entity_type' => $object->entity_type,
          '%bundle' => $object->bundle
        ), WATCHDOG_INFO);
        $this->numCreated++;
        return array(
          $object->entity_type . ':' . $object->bundle . ':' . $object->field_name
        );
      }else{
        watchdog('scratchpads_migrate', t('Field %name on %entity_type:%bundle not created'), array(
          '%name' => $object->field_name,
          '%entity_type' => $object->entity_type,
          '%bundle' => $object->bundle
        ), WATCHDOG_ERROR);
        return false;
      }
    }
    catch(FieldException $e){
      throw new MigrateException($e->getMessage());
      return false;
    }
  }

  public function fields(){
    return array(
      // field_config
      'field_name' => t('Field name'),
      'bundle' => t('bundle'),
      'entity_type' => t('Entity type'),
      'label' => t('Label'),
      'description' => t('Description'),
      'required' => t('Required')
    );
  }

  public function rollback(array $ids){
    list($entity_type, $bundle, $field_name) = explode(':', $ids['destid1']);
    // Delete the instances
    $instance = field_read_instance($entity_type, $field_name, $bundle);
    field_delete_instance($instance);
  }
}