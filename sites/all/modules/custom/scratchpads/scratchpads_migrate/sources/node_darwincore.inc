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
class ScratchpadsNodeDarwincoreMigration extends ScratchpadsNodeMigration{

  /**
   * Mapping fixed term IDs to new field values
   *
   * @var array
   */
  protected $termMappings = array();

  public function __construct(array $arguments){
    // Add manual field mappingss
    $this->manualFieldMapping = array(
      'sex' => 'field_sex',
      'basisofrecord' => 'field_basis_of_record',
      'typestatus' => 'field_type_status',
      'institutioncode' => 'field_institution_code',
      'collectioncode' => 'field_collection_code',
      'cataloguenumbertext' => 'field_catalogue_number',
      'identificationqualifier' => 'field_identification_qualifier',
      'identifiedby' => 'field_identified_by',
      'collectornumber' => 'field_collector_number',
      'fieldnumber' => 'field_number',
      'collector' => 'field_collector',
      'fieldnotes' => 'field_notes',
      'othercataloguenum' => 'field_other_catalogue_numbers',
      'lifestage' => 'field_lifestage',
      'count' => 'field_count',
      'genbanknum' => 'field_genbank_number',
      'remarks' => 'field_remarks',
      'taxonomicname' => 'field_taxonomic_name',
      'dateidentifiedyear' => 'field_date_identified',
      'earliestdatecollected' => 'field_date_collected'
    );
    parent::__construct($arguments);
    // Add location
    $this->addFieldMapping('field_location', 'locationnode')->sourceMigration('Darwincorelocation');
    // There isn't a media field
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
      'field_media:display'
    ));
    // Handled in prepare row
    $this->addUnmigratedSources(array(
      'dateidentifiedmonth',
      'dateidentifiedday',
      'latestdatecollected'
    ));
    // Load term mappings
    $this->termMappings = array();
    foreach(array(
      'sex' => 'Sex',
      'typestatus' => 'Type Status',
      'basisofrecord' => 'Basis of record'
    ) as $delta => $vocabulary_name){
      $this->termMappings[$delta] = scratchpads_migrate_get_terms($vocabulary_name);
    }
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->innerJoin('darwincore', 'd', 'd.vid = n.vid');
    $query->fields('d', array(
      'sex',
      'basisofrecord',
      'typestatus',
      'institutioncode',
      'collectioncode',
      'cataloguenumbertext',
      'identificationqualifier',
      'identifiedby',
      'collectornumber',
      'fieldnumber',
      'collector',
      'fieldnotes',
      'othercataloguenum',
      'lifestage',
      'count',
      'genbanknum',
      'remarks',
      'taxonomicname',
      'dateidentifiedyear',
      'earliestdatecollected',
      'dateidentifiedmonth',
      'dateidentifiedday',
      'latestdatecollected',
      'locationnode'
    ));
    return $query;
  }

  protected function countQuery(){
    $query = parent::countQuery();
    $query->innerJoin('darwincore', 'd', 'd.vid = n.vid');
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    foreach($this->termMappings as $delta => $term_mapping){
      if(isset($row->{$delta})){
        if($row->{$delta} == 0){
          unset($row->{$delta});
        }else{
          if(array_key_exists($row->{$delta}, $term_mapping)){
            $row->{$delta} = $term_mapping[$row->{$delta}];
          }else{
            watchdog('scratchpads_migrate', 'Error, %delta term %term doesn\'t exist', array(
              '%delta' => $delta,
              '%term' => $row->{$delta}
            ));
          }
        }
      }
    }
    if(isset($row->dateidentifiedyear) && $row->dateidentifiedyear && isset($row->dateidentifiedmonth) && $row->dateidentifiedmonth && isset($row->dateidentifiedday) && $row->dateidentifiedday){
      $row->dateidentifiedyear = drupal_json_encode(array(
        'from' => $row->dateidentifiedyear . '/' . $row->dateidentifiedmonth . '/' . $row->dateidentifiedday
      ));
    }
    if(isset($row->earliestdatecollected) && $row->earliestdatecollected){
      $collected_date = array(
        'from' => $row->earliestdatecollected
      );
      if(isset($row->latestdatecollected)){
        $collected_date['to'] = $row->latestdatecollected;
      }
      $row->earliestdatecollected = drupal_json_encode($collected_date);
    }
    // Sometimes taxonomic name field is missing
    // Check it's not in the classification  
    if(!isset($row->taxonomicname)){
      $vids = array_keys(variable_get('biological_vids', array()));
      if(count($vids)){
        foreach($vids as $vid){
          if(isset($row->{$vid})){
            $row->taxonomicname = reset($row->{$vid});
          }
        }
      }
    }
  }
}
