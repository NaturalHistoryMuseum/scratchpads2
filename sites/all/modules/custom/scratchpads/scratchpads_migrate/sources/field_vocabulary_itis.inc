<?php

class DrupalFieldVocabularyItisTermMigration extends DrupalFieldVocabularyMigration{

  /**
   * Field name mappings 
   */
  public static function getFieldNameMappings(){
    $fieldMappings = array(
      'field_taxon_author' => 'field_authors',
      'field_page_number' => 'field_page_number',
      'field_rank_name' => 'field_rank',
      'field_reference' => 'field_reference',
      'field_unacceptability_reason' => 'field_unacceptability_reason',
      'field_usage' => 'field_usage',
      'field_unit_ind1' => 'field_unit_indicator1',
      'field_unit_ind2' => 'field_unit_indicator2',
      'field_unit_ind3' => 'field_unit_indicator3',
      'field_unit_ind4' => 'field_unit_indicator4',
      'field_unit_name1' => 'field_unit_name1',
      'field_unit_name2' => 'field_unit_name2',
      'field_unit_name3' => 'field_unit_name3',
      'field_unit_name4' => 'field_unit_name4',
      'field_unit_name4' => 'field_unit_name4',
      'field_vernacular_name' => 'field_vernacular_names',
      'field_accepted_name' => NULL, // Special use case: maps to field_aan_{vid}
      // TODO - Emonocot only - remove
      'field_type_references' => 'field_type_ref',
      'field_type_taxon' => NULL, // Special use case: maps to field_type_taxon_{vid}. THERE IS NO CONTENT IN THIS FIELD
      'field_id' => 'field_id',
      'field_itis_em_etymology' => 'field_itis_em_etymology',
      'field_itis_em_other_ref' => 'field_itis_em_other_ref',
      'field_itis_em_url' => 'field_itis_em_url'
    );
    return $fieldMappings;
  }

  /**
   * Query for the basic source query.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = parent::sourceQuery();
    $query->condition('i.field_name', array_keys($this->getFieldNameMappings()), 'NOT IN');
    return $query;
  }

  protected function postImport(){
    // Update the misc group with any created fields
    foreach(MigrateDestinationFieldGroup::getFieldGroupModes() as $mode){
      $group = field_group_load_field_group('group_misc', 'taxonomy_term', $this->vocabulary->machine_name, $mode);
      if($group){
        $group->children = array_merge($group->children, $this->destination->getFieldsCreated());
        field_group_group_save($group);
      }
    }
  }
}