<?php

/**
 * Handling specific to a Drupal 6 source for taxonomy terms.
 */
class ScratchpadTermMigration extends DrupalTerm6Migration{

  protected $termNode = false;

  public function __construct(array $arguments){
    // Remap destination vocabulary to machine name
    $vocabulary = taxonomy_vocabulary_load($arguments['destination_vocabulary']);
    $arguments['destination_vocabulary'] = $vocabulary->machine_name;
    $term_nodes = scratchpads_migrate_get_term_vocabularies();
    // Added in DrupalTerm6Migration::prepareRow()
    $this->sourceFields = array(
      'parent' => t('Parent')
    );
    // Added in this::prepareRow()
    $this->sourceFields['path'] = t("Path");
    // Does this taxonomy have a term node to migrate at the same time?
    if(array_key_exists($vocabulary->vid, $term_nodes)){
      $this->termNode = $term_nodes[$vocabulary->vid];
      // See if we have a migration class with field mappings?
      $machine_name = scratchpads_migrate_get_machine_name('FieldVocabulary', $this->termNode);
      $class_name = scratchpads_migrate_get_class_name($machine_name);
      if($this->termNode == 'itis_term'){
        // Add the field_aan_x field
        $aan_field = 'field_aan_' . $vocabulary->vid;
        $this->addFieldMapping($aan_field, 'field_accepted_name');
        $this->addFieldMapping($aan_field . ':source_type')->defaultValue('tid');
        $this->addFieldMapping($aan_field . ':create_term')->defaultValue(false);
      }
      if(class_exists($class_name) && method_exists($class_name, 'getFieldNameMappings')){
        $field_mappings = $class_name::getFieldNameMappings();
      }
      // Get the fields for ITIS term
      $query = Database::getConnection('default', $arguments['source_connection'])->select('content_node_field_instance', 'i');
      $query->addField('i', 'field_name');
      $query->condition('i.widget_active', 1);
      $query->condition('i.type_name', $this->termNode);
      $query->innerJoin('content_node_field', 'f', 'i.field_name = f.field_name');
      $query->fields('f', array(
        'type'
      ));
      $source_fields = $query->execute()->fetchAllKeyed();
      // Add source fields (needs to come before parent::__construct()
      foreach($source_fields as $source_field => $source_field_type){
        $this->sourceFields[$source_field] = $source_field_type;
      }
    }
    parent::__construct($arguments);
    $this->destination = new ScratchpadsMigrateDestinationTerm($this->destinationVocabulary);
    // Add field mappings
    if(isset($source_fields) && count($source_fields)){
      foreach(array_keys($source_fields) as $source_field){
        if(isset($field_mappings) && array_key_exists($source_field, $field_mappings)){
          $d7_field = $field_mappings[$source_field];
        }else{
          $d7_field = $source_field;
        }
        $this->addFieldMapping($d7_field, $source_field);
        foreach(array(
          'format',
          'language'
        ) as $subfield_type){
          $subfield = $d7_field . ':' . $subfield_type;
          if(array_key_exists($subfield, $this->destination->fields())){
            $this->addFieldMapping($subfield, NULL);
          }
        }
      }
    }
    // DrupalTermMigration::__construct() specifies current machine name as being sourceMigration
    // So a parent term is lost if it hasn't been previously imported
    $this->removeFieldMapping('parent');
    $this->addFieldMapping('parent', 'parent');
    // Keep the TID
    $this->addFieldMapping('tid', 'tid');
    // Path had a NULL source - remap it & we'll set it in the prepareRow()
    $this->removeFieldMapping('path');
    $this->addFieldMapping('path', 'path');
    // Basic CCK field suffixes
    $this->fieldSuffixes = array(
      'computed' => 'value',
      'date' => 'value',
      'datestamp' => 'value',
      'datetime' => 'value',
      'number_decimal' => 'value',
      'number_integer' => 'value',
      'text' => 'value',
      'filefield' => 'fid', // @todo: Handle list and data suffixes
      'nodereference' => 'nid',
      'content_taxonomy' => 'value'
    );
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Cannot specify a TID in taxonomy_term_save unless term already exists so create a stub term
    if(!taxonomy_term_load($row->tid)){
      $term = new StdClass();
      $term->tid = $row->tid;
      $term->name = 'Stub term: ' . $row->tid;
      $term->description = '';
      $term->vid = $row->tid;
      $status = drupal_write_record('taxonomy_term_data', $term);
    }
    // Get term node fields
    if($this->termNode){
      // Get associated node vid
      $query = Database::getConnection('default', $this->sourceConnection)->select('term_node', 'tn');
      $query->addField('tn', 'vid');
      $query->innerJoin('node', 'n', 'n.vid = tn.vid');
      $query->condition('tn.tid', $row->tid);
      $query->condition('n.type', $this->termNode);
      $vid = $query->execute()->fetchField();
      if($vid){
        foreach($this->sourceFields as $source_field => $source_field_type){
          $table = "content_$source_field";
          if(isset($this->fieldSuffixes[$source_field_type])){
            $suffixed_field = $source_field . '_' . $this->fieldSuffixes[$source_field_type];
            if(Database::getConnection('default', $this->sourceConnection)->schema()->tableExists($table)){
              $row->{$source_field} = Database::getConnection('default', $this->sourceConnection)->select($table, 'f')->fields('f', array(
                $suffixed_field
              ))->condition('vid', $vid)->execute()->fetchCol();
            }elseif(Database::getConnection('default', $this->sourceConnection)->schema()->fieldExists('content_type_' . $this->termNode, $suffixed_field)){
              $row->{$source_field} = Database::getConnection('default', $this->sourceConnection)->select('content_type_' . $this->termNode, 'f')->fields('f', array(
                $suffixed_field
              ))->condition('vid', $vid)->execute()->fetchField();
            }else{
              watchdog('scratchpads_migrate', 'Unknown source table for field %field', array(
                '%field' => $source_field
              ), WATCHDOG_ERROR);
            }
          }
        }
      // If there isn't a term node associated with a term, set unit name 1 to default to the term name  
      }elseif($this->termNode == 'itis_term'){
      	$row->field_unit_name1 = $row->name;
      }
    }
    // Add the url alias
    $query = Database::getConnection('default', $this->sourceConnection)->select('url_alias', 'a');
    $query->addField('a', 'dst');
    $query->condition('a.src', 'taxonomy/term/' . $row->tid);
    $row->path = $query->execute()->fetchField();
  }

  protected function postImport(){
    if(method_exists($this->destination, 'postImport')){
      $this->destination->postImport();
    }
  }
}
