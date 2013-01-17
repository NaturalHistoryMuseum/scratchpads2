<?php

class DrupalFieldImageMigration extends DrupalFieldMigration{

  /**
   * Query for the basic source query.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $query->fields('i');
    $query->innerJoin('content_node_field', 'f', 'f.field_name = i.field_name');
    $query->fields('f');
    $query->condition('i.widget_active', 1);
    $query->condition('i.type_name', 'image');
    $query->condition('f.type', array(
      'filefield'
    ), 'NOT IN');
    $query->condition('f.field_name', array(
      'image_publication',
      'image_specimen'
    ), 'NOT IN');
    return $query;
  }

  public function prepareRow($row){
    $row->entity_type = 'file';
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    $row->type_name = 'image';
  }
}
