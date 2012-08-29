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
      'nodereference',
      'filefield'
    ), 'NOT IN');
    return $query;
  }

  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    $row->entity_type = 'file';
    $row->type_name = 'image';
  }
}
