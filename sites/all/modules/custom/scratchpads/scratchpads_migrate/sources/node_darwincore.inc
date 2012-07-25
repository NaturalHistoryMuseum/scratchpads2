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

  public function __construct(array $arguments){
  	//$this->dependencies = array('ScratchpadsNodeDarwincorelocationMigration');
  	
  	dd($this->dependencies);
  	
    // Add manual field mappingss
    $this->manualFieldMapping = array(
      'sex' => 'field_sex',
      'basisofrecord' => 'field_basis_of_record',
      'typestatus' => 'field_type_status',
      'institutioncode' => 'field_institution_code',
      'collectioncode' => 'field_collection_code',
      'cataloguenumbertext' => 'field_catalogue_number',
      'identificationqualifier' => 'field_identification_qualifier',
      //      'identifiedby' => 'field_identified_by',
      'collectornumber' => 'field_collector_number',
      'fieldnumber' => 'field_number',
      //      'collector' => 'field_collector',
      'fieldnotes' => 'field_notes',
    	'locationnode' => 'field_location',
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
    $this->addFieldMapping('field_taxonomic_name:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_taxonomic_name:create_term')->defaultValue(false);
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
      'field_media:display',
    ));
    // Handled in prepare row
    $this->addUnmigratedSources(array(
      'dateidentifiedmonth',
      'dateidentifiedday',
      'latestdatecollected'
    ));
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->innerJoin('darwincore', 'd', 'd.vid = n.vid');
    foreach(array_keys($this->manualFieldMapping) as $field){
      $query->addField('d', $field, strtolower($field));
    }
    $query->addField('d', 'dateidentifiedmonth');
    $query->addField('d', 'dateidentifiedday');
    $query->addField('d', 'latestdatecollected');
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    $term_mappings = array(
      'sex' => array(
        1161 => 'Female',
        1162 => 'Hermaphrodite',
        1164 => 'Indeterminate',
        1160 => 'Male',
        1167 => 'Multiple',
        1165 => 'Transitional',
        1166 => 'Undetermined',
        1163 => 'Unknown'
      ),
      'typestatus' => array(
        1168 => 'Holotype',
        1169 => 'Isotype',
        1170 => 'Lectotype',
        1171 => 'Neotype',
        1172 => 'Nontype',
        1173 => 'Not Provided',
        1174 => 'Paralectotype',
        1175 => 'Paratype',
        1176 => 'Syntype'
      ),
      'basisofrecord' => array(
        1152 => 'Fossil Specimen',
        1154 => 'Human Observation',
        1153 => 'Living Specimen',
        1155 => 'Machine Observation',
        1157 => 'Moving Image',
        1159 => 'Other Specimen',
        1151 => 'Preserved Specimen',
        1158 => 'Sound Recording',
        1156 => 'Still Image'
      )
    );
    foreach($term_mappings as $delta => $term_mapping){
      if($row->{$delta} == 0){
        unset($row->{$delta});
      }else if(isset($row->{$delta})){
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
    if(!empty($row->dateidentifiedyear)){
      $row->dateidentifiedyear = drupal_json_encode(array(
        'from' => $row->dateidentifiedyear . '/' . $row->dateidentifiedmonth . '/' . $row->dateidentifiedday
      ));
    }
    $row->earliestdatecollected = drupal_json_encode(array(
      'from' => str_replace('-', '/', $row->earliestdatecollected),
      'to' => str_replace('-', '/', $row->latestdatecollected)
    ));
  }
}
