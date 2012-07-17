<?php

class DrupalFieldContentProfileMigration extends DrupalFieldMigration{

  /**
   * Field name mappings 
   */
  public static function getFieldNameMappings(){
    return array(
      'field_title' => 'field_user_title',
      'field_givennames' => 'field_user_given_names',
      'field_familyname' => 'field_user_family_name',
      'field_institution' => 'field_user_institution',
      'field_taxonomicinterest' => 'field_user_taxonomic_interest',
//      'field_homepage' => 'field_user_homepage',
      'field_email' => 'mail'
    );
  }

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Map to user:user entity
    $this->removeFieldMapping('entity_type');
    $this->addFieldMapping('entity_type')->defaultValue('user');
    $this->removeFieldMapping('bundle');
    $this->addFieldMapping('bundle')->defaultValue('user');
  }

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
    $query->condition('i.type_name', 'profile');
    $query->condition('f.field_name', array_keys($this->getFieldNameMappings()), 'NOT IN');
    return $query;
  }
}