<?php

class FieldCollectionMigration extends DrupalMigration{

  /**
   * Constructor
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
  }

  /**
   * Override Migration::import
   * - Import all field collections
   */
  protected function import(){
    $db = Database::getConnection('default', $this->sourceConnection);
    // Fetch all vernacular names
    $result = $db->query("
          SELECT vn.field_vernacular_name_value, t.tid
            FROM {content_field_vernacular_name} vn
      INNER JOIN {term_node} t ON t.nid = vn.nid AND t.vid = vn.vid
           WHERE vn.field_vernacular_name_value IS NOT NULL;
    ");
    foreach($result as $row){
      $term = taxonomy_term_load($row->tid);
      $entity = entity_create('field_collection_item', array(
        'field_name' => 'field_vernacular_name_collection'
      ));
      $wrapper = entity_metadata_wrapper('field_collection_item', $entity);
      $wrapper->field_vernacular_name = $row->field_vernacular_name_value;
      $entity->setHostEntity('taxonomy_term', $term, LANGUAGE_NONE, TRUE);
      $entity->save();
    }
  }

  /**
   * Override Migration::sourceCount
   */
  public function sourceCount($refresh = FALSE){
    $db = Database::getConnection('default', $this->sourceConnection);
    $result = $db->query("
      SELECT COUNT(*)
        FROM {content_field_vernacular_name} vn
        WHERE vn.field_vernacular_name_value IS NOT NULL
    ");
    return $result->fetchField();
  }

  public function processedCount(){
    return $this->importedCount();
  }

  public function importedCount(){
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'field_collection_item')->entityCondition('field_name', 'field_vernacular_name_collection');
    $result = $query->execute();
    return count($result['field_collection_item']);
  }

  public function updateCount(){
    return 0;
  }

  public function messageCount(){
    return 0;
  }
}