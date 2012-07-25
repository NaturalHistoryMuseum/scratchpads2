<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 * dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 * the default.
 */
class ScratchpadsNodeSpmMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    // Add manual field mappingss
    $this->manualFieldMapping = array(
      'GeneralDescription' => 'field_general_description',
      'Biology' => 'field_biology',
      'ConservationStatus' => 'field_conservation_status',
      'Legislation' => 'field_legislation',
      'Management' => 'field_management',
      'Procedures' => 'field_procedures',
      'Threats' => 'field_threats',
      'Trends' => 'field_trends',
      'Behaviour' => 'field_behavious',
      'Cytology' => 'field_cytology',
      'DiagnosticDescription' => 'field_diagnostic_description',
      'Genetics' => 'field_genetics',
      'Growth' => 'field_growth',
      'LookAlikes' => 'field_look_alikes',
      'MolecularBiology' => 'field_molecular_biology',
      'Morphology' => 'field_morphology',
      'Physiology' => 'field_physiology',
      'Size' => 'field_size',
      'TaxonBiology' => 'field_taxon_biology',
      'Associations' => 'field_associations',
      'Cyclicity' => 'field_cyclicity',
      'Dispersal' => 'field_dispersal',
      'Distribution' => 'field_distribution',
      'Ecology' => 'field_ecology',
      'Habitat' => 'field_habitat',
      'LifeCycle' => 'field_life_cycle',
      'LifeExpectancy' => 'field_life_expectancy',
      'Migration' => 'field_migration',
      'TrophicStrategy' => 'field_trophic_strategy',
      'PopulationBiology' => 'field_population_biology',
      'Reproduction' => 'field_reproduction',
      'Evolution' => 'field_evolution',
      'Phylogeny' => 'field_phylogeny',
      'Diseases' => 'field_diseases',
      'RiskStatement' => 'field_risk_statement',
      'Uses' => 'field_uses'
    );
    parent::__construct($arguments);
    $this->addUnmigratedDestinations(array(
      'field_media',
      'field_media:file_class',
      'field_media:language',
      'field_media:destination_dir',
      'field_media:destination_file',
      'field_media:file_replace',
      'field_media:preserve_files',
      'field_media:source_dir',
      'field_media:description',
      'field_media:display',
      'field_spm_map'
    ));
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->innerJoin('spm', 'spm', 'spm.nid = n.nid');
    foreach(array_keys($this->manualFieldMapping) as $field){
      $query->addField('spm', $field, strtolower($field));
    }
    return $query;
  }
}
