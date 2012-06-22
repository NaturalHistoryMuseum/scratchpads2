<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalViewsMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * @abstract
   * Version-specific classes must implement sourceQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function sourceQuery();
}

class MigrateDestinationViews extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create views');
  }

  public function import(stdClass $object, stdClass $row){
    $view = views_new_view();
    $view->name = $object->name;
    $view->description = $object->name;
    $view->tag = $object->tag;
    $view->core = $object->core;
    // Only migrate node-based views
    // There probably be won't be any others anyway
    if($view->base_table !== 'node'){throw new MigrateException(t('Unknown Base table %base_table on view %name. Manual migration required.', array(
        '%base_table' => $view->base_table,
        '%name' => $view->name
      )));}
    // Get the views displays
    $views_displays = Database::getConnection('default', 'scratchpad_1')->query('SELECT * FROM {views_display} WHERE vid = :vid', array(
      ':vid' => $row->vid
    ))->fetchAllAssoc('id');
    foreach(array_keys($views_displays) as $name){
      if($name != 'default'){
        $view->add_display($views_displays[$name]->display_plugin, $views_displays[$name]->display_title, $name);
      }
      $view->display[$name]->display_options = unserialize($views_displays[$name]->display_options);
      $display_options = &$view->display[$name]->display_options;
      // Validate & update display settings
      // Check node type filters and remap node types
      if(isset($display_options['filters']['type'])){
        foreach(array_keys($display_options['filters']['type']['value']) as $node_type){
          // Has this node type name been changed?
          $mapping = scratchpads_migrate_type_mappings($node_type);
          if($mapping && isset($mapping['bundle'])){
            unset($display_options['filters']['type']['value'][$node_type]);
            $display_options['filters']['type']['value'][$mapping['bundle']] = $mapping['bundle'];
            $node_type = $mapping['bundle'];
          }
          // Check the node type exists
          if(!node_type_load($node_type)){throw new MigrateException(t('Node type %type does not exist for filter in view %name.', array(
              '%type' => $node_type,
              '%name' => $view->name
            )));}
        }
      }
      // Check fields exists
      if(isset($display_options['fields'])){
        foreach(array_keys($display_options['fields']) as $field_id){
          $field = &$display_options['fields'][$field_id];
          // Remove none relationship - seems to cause a missing relationship error
          switch($field['table']){
            case 'darwincore':
              switch($field_id){
                case 'taxonomicname':
                  $field = array(
                    'id' => 'field_taxonomic_name',
                    'table' => 'field_data_field_taxonomic_name',
                    'field' => 'field_taxonomic_name',
                    'label' => 'Taxonomic name',
                    'type' => 'taxonomy_term_reference_link'
                  );
                  break;
              }
              break;
          }
        }
      }
    }
    $view->human_name = $view->display['default']->display_options['title'];
    try{
      $errors = $view->validate();
      if(is_array($errors)){
        foreach($errors as $error){
          throw new MigrateException($error);
        }
        return false;
      }
      views_save_view($view);
      if($view->vid){
        $this->numCreated++;
        return array(
          $view->name
        );
      }else{
        throw new MigrateException(t('View %name not created.', array(
          '%name' => $view->name
        )));
      }
    }
    catch(Exception $e){
      throw new MigrateException($e->getMessage());
      return false;
    }
  }

  public function fields(){
    return array(
      'name' => t('Machine name'),
      'human_name' => t('Human name'),
      'description' => t('View description'),
      'description' => t("Description"),
      'tag' => t("Tag"),
      'base_table' => t("Base table"),
      'core' => t("Core")
    );
  }

  public function rollback(array $ids){
    $view = views_get_view($ids['destid1']);
    if($view){
      views_delete_view($view);
    }
  }
}