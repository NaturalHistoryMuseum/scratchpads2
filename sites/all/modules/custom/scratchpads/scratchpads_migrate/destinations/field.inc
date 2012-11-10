<?php

class MigrateDestinationField extends MigrateDestination{

  protected $fieldsCreated = array();

  public function getFieldsCreated(){
    return $this->fieldsCreated;
  }

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
    $field_instance = (array)$object;
    $field_instance['widget'] = array(
      'active' => $object->active,
      'weight' => $object->weight,
      'type' => $object->widget_type,
      'module' => $object->widget_module,
      'settings' => $object->widget_settings,
      'description' => $object->description
    );
    $field_instance['display'] = $object->display_settings;
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
    // Alter field and widget settings
    $field_config = array(
      'active' => $object->active,
      'cardinality' => (isset($row->multiple) && $row->multiple == 0 ? 1 : -1),
      'deleted' => '0',
      'entity_types' => array(
        $object->entity_type
      ),
      'field_name' => $object->field_name,
      'module' => $object->module,
      'settings' => $object->global_settings,
      'translatable' => '1',
      'type' => $object->type,
      'required' => $object->required
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
      case 'content_taxonomy':
        $vid = $object->widget_settings['vid'];
        if($vocabulary = taxonomy_vocabulary_load($vid)){
          $field_config['settings']['allowed_values'] = array(
            array(
              'vocabulary' => $vocabulary->machine_name,
              'parent' => 0
            )
          );
        }else{
          drupal_set_message(t('Unknown vocabulary !vid for !bundle:!field_name (field.inc:88).', array(
            '!vid' => $vid,
            '!bundle' => $object->bundle,
            '!field_name' => $object->field_name
          )), 'error');
        }
        break;
      case 'node_reference':
        $field_instance['widget']['type'] = 'node_reference_autocomplete';
        $field_instance['widget']['module'] = 'node_reference';
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
        // Set default display settings
        $field_instance['display'] = array(
          'default' => array(
            'label' => 'above',
            'module' => 'file',
            'settings' => array(),
            'type' => 'styles_file_medium',
            'weight' => 2
          ),
          'teaser' => array(
            'label' => 'above',
            'module' => 'file',
            'settings' => array(),
            'type' => 'styles_file_thumbnail',
            'weight' => 2
          )
        );
        break;
      case 'content_taxonomy_autocomplete':
        $field_config['module'] = 'taxonomy';
        $field_config['type'] = 'taxonomy_term_reference';
        $field_instance['widget']['type'] = 'taxonomy_autocomplete';
        $field_instance['widget']['module'] = 'taxonomy';
        $settings = array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60
        );
        break;
      case 'options_select':
        $field_config['module'] = 'list';
        $field_config['type'] = 'list_text';
        $field_instance['widget']['type'] = 'options_select';
        $field_instance['widget']['module'] = 'options';
        // Parse allowed values into
        if(isset($field_config['settings']['allowed_values'])){
          if(!is_array($field_config['settings']['allowed_values'])){
            $allowed_values = explode(PHP_EOL, $field_config['settings']['allowed_values']);
          }else{
            $allowed_values = $field_config['settings']['allowed_values'];
          }
          $allowed_values = str_replace(array(
            "<p>", // FFS!!!! Some muppets have wrapped <p> tags around allowed values!! Strip them out
            "</p>",
            "\r",
            "\r\n",
            "\n"
          ), '', $allowed_values);
          foreach($allowed_values as $delta => $allowed_value){
            if(empty($allowed_value)){
              unset($allowed_values[$delta]);
            }
          }
          $field_config['settings']['allowed_values'] = array_combine($allowed_values, $allowed_values);
        }
        break;
    }
    // Fix missing max length bug
    // ERROR: Invalid field/widget combination: The field 'field_page' in the bundle 'citation' is an unlimited length field using a textfield widget, not allowed in D7. The field length will be set to 255.
    if($field_config['type'] == 'text' && $field_instance['widget']['type'] != 'text_textarea' && isset($field_config['settings']['max_length']) && empty($field_config['settings']['max_length'])){
      $field_config['settings']['max_length'] = 256;
    }
    // Only create if the field does not already exist
    $field = field_info_field($object->field_name);
    if(empty($field)){
      // Create the field if it doesn't exist
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
        $this->fieldsCreated[] = $field_config['field_name'];
      }
      catch(FieldException $e){
        throw new MigrateException($e->getMessage());
        return false;
      }
    }else{
      // Need to ensure this entity type is allowed in the field settings
      if(!in_array($object->entity_type, $field['entity_types'])){
        $field['entity_types'][] = $object->entity_type;
        scratchpads_migrate_update_field_entity_type($field);
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
      'required' => t('Required'),
      // New
      'active' => t('Active'),
      'widget_settings' => t('Widget settings'),
      'display_settings' => t('Display settings'),
      'global_settings' => t('Global settings'),
      'widget_type' => t('Widget type'),
      'module' => t('Module'),
      'widget_module' => t('Widget module'),
      'weight' => t('Weight'),
      'type' => t('Type')
    );
  }

  public function rollback(array $ids){
    list($entity_type, $bundle, $field_name) = explode(':', $ids['destid1']);
    // Delete the instances
    $instance = field_read_instance($entity_type, $field_name, $bundle);
    field_delete_instance($instance);
  }
}