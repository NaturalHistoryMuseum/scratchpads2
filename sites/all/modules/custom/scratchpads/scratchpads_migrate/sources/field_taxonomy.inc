<?php

class DrupalFieldTaxonomyMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->sourceFields['entity_type'] = t('Entity type');
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      ),
      'vid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE
      )
    ), MigrateDestinationFieldTaxonomy::getKeySchema());
    $this->destination = new MigrateDestinationFieldTaxonomy();
    $this->addSimpleMappings(array(
      'name',
      'multiple',
      'required',
      'tags'
    ));
    $this->addFieldMapping('entity_type', 'entity_type')->defaultValue('node');
    $this->addFieldMapping('bundle', 'type');
    $this->addFieldMapping('vid', 'vid')->sourceMigration('Vocabulary');
    $this->addFieldMapping(NULL, 'classification_type');
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = $this->baseQuery();
    $query->condition('vnt.type', array(
      DrupalFieldMigration::excludedNodeTypes()
    ), 'NOT IN');
    return $query;
  }

  protected function baseQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('vocabulary_node_types', 'vnt');
    $query->fields('vnt', array(
      'type'
    ));
    $query->innerJoin('vocabulary', 'v', 'v.vid = vnt.vid');
    $query->fields('v', array(
      'vid',
      'name',
      'multiple',
      'required',
      'tags'
    ));
    $query->condition('v.module', 'taxonomy');
    $query->leftJoin('scratchpads_migrate_biological_classification', 'mbc', 'v.vid = mbc.vid');
    $query->addField('mbc', 'type', 'classification_type');
    // We're not migrating image galleries vocabulary
    $query->condition('v.vid', scratchpads_migrate_get_image_gallery_vid(), '!=');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Is this a taxa field?
    if(!empty($row->classification_type)){
      $row->field_name = 'field_taxonomic_name';
      $row->label = 'Classification';
    }else{
      $row->field_name = $this->getFieldName($row->name);
      $row->label = $row->name;
    }
    if(!isset($row->entity_type)){
      $row->entity_type = 'node';
    }
    if(field_read_instance($row->entity_type, $row->field_name, $row->type)){      // Instance already exists, so skip it
return false;}
  }

  /**
   * Generate a field name from the vocabulary name
   */
  static public function getFieldName($name){
    $field_name = 'field_' . preg_replace('/[^a-z0-9]/', '_', strtolower($name));
    // Field name can't be longer than 32 chars
    if(strlen($field_name) > 32){
      $field_name = substr($field_name, 0, 32);
    }
    // See if we have already mapped this field name
    $map = variable_get('scratchpads_migrate_taxonomy_name_map', array());
    if(isset($map[$field_name])){return $map[$field_name];}
    // Ensure we don't clash with an existing field
    $final_name = $field_name;
    $field = field_read_field($final_name);
    $id = 0;
    while(!empty($field) && $field['type'] != 'taxonomy_term_reference'){
      if((strlen($field_name) + strlen($id)) > 32){
        $final_name = substr($field_name, 0, 32 - strlen($id)) . $id;
      }else{
        $final_name = $field_name . $id;
      }
      $id = $id + 1;
      $field = field_read_field($final_name);
    }
    $map[$field_name] = $final_name;
    variable_set('scratchpads_migrate_taxonomy_name_map', $map);
    return $final_name;
  }
}