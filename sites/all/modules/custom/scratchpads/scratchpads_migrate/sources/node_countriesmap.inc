<?php

class ScratchpadsNodeCountriesmapMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    $this->manualFieldMapping = array(
      'country' => 'field_spm_map'
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
      'field_associations',
      'field_associations:format',
      'field_associations:language',
      'field_behavious',
      'field_behavious:format',
      'field_behavious:language',
      'field_biology',
      'field_biology:format',
      'field_biology:language',
      'field_conservation_status',
      'field_conservation_status:format',
      'field_conservation_status:language',
      'field_cyclicity',
      'field_cyclicity:format',
      'field_cyclicity:language',
      'field_cytology',
      'field_cytology:format',
      'field_cytology:language',
      'field_diagnostic_description',
      'field_diagnostic_description:format',
      'field_diagnostic_description:language',
      'field_diseases',
      'field_diseases:format',
      'field_diseases:language',
      'field_dispersal',
      'field_dispersal:format',
      'field_dispersal:language',
      'field_distribution',
      'field_distribution:format',
      'field_distribution:language',
      'field_ecology',
      'field_ecology:format',
      'field_ecology:language',
      'field_evolution',
      'field_evolution:format',
      'field_evolution:language',
      'field_general_description',
      'field_general_description:format',
      'field_general_description:language',
      'field_genetics',
      'field_genetics:format',
      'field_genetics:language',
      'field_growth',
      'field_growth:format',
      'field_growth:language',
      'field_habitat',
      'field_habitat:format',
      'field_habitat:language',
      'field_legislation',
      'field_legislation:format',
      'field_legislation:language',
      'field_life_cycle',
      'field_life_cycle:format',
      'field_life_cycle:language',
      'field_life_expectancy',
      'field_life_expectancy:format',
      'field_life_expectancy:language',
      'field_look_alikes',
      'field_look_alikes:format',
      'field_look_alikes:language',
      'field_management',
      'field_management:format',
      'field_management:language',
      'field_migration',
      'field_migration:format',
      'field_migration:language',
      'field_molecular_biology',
      'field_molecular_biology:format',
      'field_molecular_biology:language',
      'field_morphology',
      'field_morphology:format',
      'field_morphology:language',
      'field_phylogeny',
      'field_phylogeny:format',
      'field_phylogeny:language',
      'field_physiology',
      'field_physiology:format',
      'field_physiology:language',
      'field_population_biology',
      'field_population_biology:format',
      'field_population_biology:language',
      'field_procedures',
      'field_procedures:format',
      'field_procedures:language',
      'field_reproduction',
      'field_reproduction:format',
      'field_reproduction:language',
      'field_risk_statement',
      'field_risk_statement:format',
      'field_risk_statement:language',
      'field_size',
      'field_size:format',
      'field_size:language',
      'field_taxon_biology',
      'field_taxon_biology:format',
      'field_taxon_biology:language',
      'field_threats',
      'field_threats:format',
      'field_threats:language',
      'field_trends',
      'field_trends:format',
      'field_trends:language',
      'field_trophic_strategy',
      'field_trophic_strategy:format',
      'field_trophic_strategy:language',
      'field_uses',
      'field_uses:format',
      'field_uses:language'
    ));
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['country'] = 'array';
    return $fields;
  }

  protected function nodeQuery(){
    // TODO - Must have TID
    $query = parent::nodeQuery();
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    // Get the TDGW region data
    // Add the location data
    $query = Database::getConnection('default', $this->sourceConnection)->select('countriesmap_tdwg', 't');
    $query->fields('t', array(
      'level4code'
    ));
    $query->innerJoin('countriesmap_nodes', 'cmn', 'cmn.cmid = t.cmid');
    $query->condition('cmn.vid', $row->vid);
    $result = $query->execute();
    foreach($result as $location){
      $level_3 = substr($location->level4code, 0, 3);
      $level_4 = substr($location->level4code, 3, 2);
      // Get the level 1 & level 2 codes
      $top_levels = db_select('gm3_region_data', 'rd')->fields('rd', array(
        'level_1_code',
        'level_2_code'
      ))->condition('level_3_code', $level_3)->condition('level_4_code', $level_4)->execute()->fetchObject();
      $row->country[] = $top_levels->level_1_code . ':' . $top_levels->level_2_code . ':' . $level_3;
    }
  }
}
