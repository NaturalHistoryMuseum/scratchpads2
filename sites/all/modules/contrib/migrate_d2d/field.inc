<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->fieldQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function fieldQuery();
}

abstract class MigrateDestinationField extends MigrateDestination{

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
    // Create the import
    if(!module_exists($row->module)){
      throw new MigrateException(t('Module %module does not exist', array(
        '%module' => $row->module
      )));
      return false;
    }
    // Only create if the field does not already exist
    $field = field_read_field($row->field_name);
    if(empty($field)){
      // Create the field if it doesn't exist
      $field_config = array(
        'active' => $row->active,
        'cardinality' => ($row->multiple === 0 ? 1 : -1),
        'deleted' => '0',
        'entity_types' => array(
          'node'
        ),
        'field_name' => $row->field_name,
        'module' => $row->module,
        'settings' => unserialize($row->global_settings),
        'translatable' => '1',
        'type' => $row->type
      );
      $field_instance['widget'] = array(
        'active' => $row->active,
        'weight' => $row->weight,
        'type' => $row->widget_type,
        'module' => $row->widget_module
      );
      // Let content migrate module change all the field settings for D7 
      content_migrate_content_migrate_field_alter($field_config, $field_instance);
      try{
        field_create_field($field_config);
      }
      catch(FieldException $e){
        throw new MigrateException($e->getMessage());
        return false;
      }
    }
    $field_instance = (array)$object;
    $widget_settings = unserialize($row->widget_settings);
    if(isset($widget_settings['default_value'])){
      $field_instance['default_value'] = $widget_settings['default_value'][0]['value'];
    }
    $field_instance['required'] = $row->required;
    $field_instance = field_create_instance($field_instance);
    // Has the field been created?
    if(isset($field_instance['field_id'])){
      $this->numCreated++;
      return array(
        $object->entity_type . ':' . $object->bundle . ':' . $object->field_name
      );
    }else{
      return FALSE;
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