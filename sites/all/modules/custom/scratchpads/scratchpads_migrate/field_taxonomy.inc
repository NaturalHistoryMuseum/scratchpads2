<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldTaxonomyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map    
    $this->source = new MigrateSourceSQL($this->fieldQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Is this a taxa field?
    if(!empty($row->biological_classification_vid)){
      $row->field_name = 'field_taxonomic_name';
      $row->label = 'Classification';
    }else{
      $row->field_name = $this->getFieldName($row->name);
      $row->label = $row->name;
    }
    if(field_read_instance('node', $row->field_name, $row->type)){// Instance already exists, so skip it
return false;}
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function fieldQuery();

  /**
   * Generate a field name from the vocabulary name
   */
  public function getFieldName($name){
    return 'field_' . ereg_replace('[^a-z0-9]', '_', strtolower($name));
  }
}

class MigrateDestinationFieldTaxonomy extends MigrateDestination{

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
    return t('Create taxonomy fields');
  }

  public function import(stdClass $object, stdClass $row){
    $field = field_read_field($row->field_name);
    if(empty($field)){
      // Create the field if it doesn't exist
      $field_config = array(
        'active' => 1,
        'cardinality' => ($object->multiple == 0 ? 1 : -1),
        'deleted' => '0',
        'entity_types' => array(
          'node'
        ),
        'field_name' => $row->field_name,
        'module' => 'taxonomy',
        'translatable' => '1',
        'type' => 'taxonomy_term_reference'
      );
      try{
        field_create_field($field_config);
      }
      catch(FieldException $e){
        throw new MigrateException($e->getMessage());
        return false;
      }
    }
    $field_instance = array(
      'required' => $object->required,
      'entity_type' => 'node',
      'bundle' => $object->bundle,
      'field_name' => $row->field_name,
      'module' => 'taxonomy',
      'type' => 'taxonomy_term_reference',
      'label' => $row->label
    );
    if($row->field_name == 'field_taxonomic_name'){
      $field_instance['widget'] = array(
        'type' => 'taxonomy_autocomplete',
        'module' => 'taxonomy'
      );
    }else{
      $field_instance['widget'] = array(
        'type' => 'options_select',
        'module' => 'options'
      );
    }
    try{
      $field_instance = field_create_instance($field_instance);
    }
    catch(FieldException $e){
      throw new MigrateException($e->getMessage());
      return false;
    }
    // Has the field been created?
    if(isset($field_instance['field_id'])){
      $this->numCreated++;
      return array(
        $object->bundle . ':' . $row->field_name
      );
    }
    return false;
  }

  public function fields(){
    return array(
      'vid' => t('Vocabulary ID'),
      'bundle' => t('Bundle'),
      'name' => t('Vocabulary name'),
      'multiple' => t('Multiple'),
      'required' => t('Required'),
      'tags' => t('Tags')
    );
  }

  public function rollback(array $ids){
    list($bundle, $field_name) = explode(':', $ids['destid1']);
    // Delete the instances
    $instance = field_read_instance('node', $field_name, $bundle);
    field_delete_instance($instance);
  }
}