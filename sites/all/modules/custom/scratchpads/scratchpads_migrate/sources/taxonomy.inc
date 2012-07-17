<?php

/**
 * Handling specific to a Drupal 6 source for taxonomy terms.
 */
class ScratchpadTermMigration extends DrupalTerm6Migration{

  public function __construct(array $arguments){
    // Remap destination vocabulary to machine name
    $vocabulary = taxonomy_vocabulary_load($arguments['destination_vocabulary']);
    $arguments['destination_vocabulary'] = $vocabulary->machine_name;
    $term_nodes = scratchpads_migrate_get_term_vocabularies();
    // Does this taxonomy have a term node to migrate at the same time?
    if(array_key_exists($vocabulary->vid, $term_nodes)){
      // See if we have a migration class with field mappings?
      $machine_name = scratchpads_migrate_get_machine_name('FieldVocabulary', $term_nodes[$vocabulary->vid]);
      $class_name = scratchpads_migrate_get_class_name($machine_name);
      if($term_nodes[$vocabulary->vid] == 'itis_term'){
        // Add the field_aan_x field
        $aan_field = 'field_aan_' . $vocabulary->vid;
        $this->addFieldMapping($aan_field, 'field_accepted_name');
        $this->addFieldMapping($aan_field . ':source_type')->defaultValue('tid');
        $this->addFieldMapping($aan_field . ':create_term')->defaultValue(false);
      }
      if(class_exists($class_name) && method_exists($class_name, 'getFieldNameMappings')){
        $field_mappings = $class_name::getFieldNameMappings();
        foreach($field_mappings as $d6_field => $d7_field){
          //          $this->addFieldMapping($d7_field, $d6_field);
        }
      }
      // Get the fields for ITIS term
      $query = Database::getConnection('default', $arguments['source_connection'])->select('content_node_field_instance', 'i');
      $query->addField('i', 'field_name');
      $query->condition('i.widget_active', 1);
      $query->condition('i.type_name', $term_nodes[$vocabulary->vid]);
      $result = $query->execute();
      // Add them as source queries
      foreach($result as $row){
        $this->sourceFields[$row->field_name] = $row->field_name;
        if(isset($field_mappings) && array_key_exists($row->field_name, $field_mappings)){
          $this->addFieldMapping($field_mappings[$row->field_name], $row->field_name);
        }else{
          $this->addFieldMapping($row->field_name, $row->field_name);
        }
      }
    }
    parent::__construct($arguments);
  }
}
