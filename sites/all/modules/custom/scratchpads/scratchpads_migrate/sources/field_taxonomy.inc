<?php

class DrupalFieldTaxonomyMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
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
    $query->condition('v.module', 'forum', '!=');
    $query->leftJoin('scratchpads_migrate_biological_classification', 'mbc', 'v.vid = mbc.vid');
    $query->addField('mbc', 'type', 'classification_type');
    // We're not migrating image galleries vocabulary
    $query->condition('name', 'Image Galleries', '!=');
    $query->condition('vnt.type', array(
      DrupalFieldMigration::excludedNodeTypes()
    ), 'NOT IN');
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
    if(field_read_instance('node', $row->field_name, $row->type)){ // Instance already exists, so skip it
      watchdog('scratchpads_migrate', t('Field %name already exists for %type'), array(
        '%name' => $row->field_name,
        '%type' => $row->type
      ), WATCHDOG_NOTICE);
      return false;
    }
  }

  /**
   * Generate a field name from the vocabulary name
   */
  static public function getFieldName($name){
    return 'field_' . ereg_replace('[^a-z0-9]', '_', strtolower($name));
  }
}