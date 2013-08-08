<?php
// Load the PHPExcel library.
if(function_exists('libraries_get_path')){
  $path = libraries_get_path('imu-api-1.0.03');
  $imuPath = "$path/IMu.php";
  $sessionPath = "$path/Session.php";
  $modulePath = "$path/Module.php";
  $documentPath = "$path/Document.php";
}else{
  $path = drupal_get_path('module', 'feeds_imu') . '/imu-api-1.0.03';
  $imuPath = "$path/IMu.php";
  $sessionPath = "$path/Session.php";
  $modulePath = "$path/Module.php";
  $documentPath = "$path/Document.php";
}
require_once $imuPath;
require_once $sessionPath;
require_once $modulePath;
require_once $documentPath;

/**
 * @file
 * Fetches data from an IMU database.
 */
class FeedsIMUspecimensFetcher extends FeedsFetcher{

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source){
    $source_config = $source->getConfigFor($this);
    $IMUsearch = trim($source_config['IMUsearch']);
    $importTo = trim($source_config['importTo']);
    $kemuHost = trim($source_config['kemuHost']);
    $kemuPort = trim($source_config['kemuPort']);
    $asXML = trim($source_config['asXML']);
    $returnNumRows = trim($source_config['returnNumRows']);
    if($importTo == 'specimen_observation'){
      $moduleName = 'ecatalogue';
      $termArray = array(
        'DarGenus',
        $IMUsearch
      );
      /*
       * Columns we want to fetch for specimens
       */
      $kemuColumns = array(
        'DarGlobalUniqueIdentifier',
        'DarBasisOfRecord',
        'DarCatalogNumber',
        'DarCollectionCode',
        'DarCollector',
        'DarCollectorNumber',
        'DarIndividualCount',
        'DarStartYearCollected',
        'DarStartMonthCollected',
        'DarStartDayCollected',
        'DarEndYearCollected',
        'DarEndMonthCollected',
        'DarEndDayCollected',
        'DarYearIdentified',
        'DarMonthIdentified',
        'DarDayIdentified',
        'DarGenBankNum',
        'DarIdentificationQualifier',
        'DarIdentifiedBy',
        'DarInstitutionCode',
        'DarLifeStage',
        'DarFieldNotes',
        'DarFieldNumber',
        'DarOtherCatalogNumbers',
        'DarRemarks',
        'DarSex',
        'DarGenus',
        'DarSpecies',
        'DarTypeStatus'
      );
    }
    if($importTo == 'location'){
      $moduleName = 'ecatalogue';
      $termArray = array(
        'DarGenus',
        $IMUsearch
      );
      /*
       * Columns we want to fetch for specimens
       */
      $kemuColumns = array(
        'DarGlobalUniqueIdentifier',
        'DarContinentOcean',
        'DarOriginalCoordinateSystem',
        'DarCoordinateUncertaintyInMeter',
        'DarCountry',
        'DarCounty',
        'DarGeodeticDatum',
        'DarGeorefMethod',
        'DarIslandGroup',
        'DarIsland',
        'DarLocality',
        'DarDecimalLatitude',
        'DarDecimalLongitude',
        'DarMaximumDepthInMeters',
        'DarMaximumElevationInMeters',
        'DarMinimumDepthInMeters',
        'DarMinimumElevationInMeters',
        'DarStateProvince'
      );
    }
    if($importTo == 'taxonomy'){
      $moduleName = 'etaxonomy';
      $termArray = array(
        'DarGenus',
        $IMUsearch
      );
      $kemuColumns = array();
    }
    if($importTo == 'scan'){
      $moduleName = 'ecatalogue';
      $termArray = array(
        'EntRecRecordStatus_tab',
        'SCAN_record'
      );
      $kemuColumns = array(
        'host=CardHostRef_tab.[
			irn,
			rank=TaxClassRank_tab,
			name=TaxClassValue_tab,
			author=TaxClassAuthors_tab
		]',
        'parasite_stage=CardParasiteStage_nesttab',
        'parasite=CardParasiteRef_tab.[
			irn,
			rank=TaxClassRank_tab,
			name=TaxClassValue_tab,
			author=TaxClassAuthors_tab
		]',
        'collectedStart=PrePreparedDateFrom',
        'collectionEvent=EntLocEventSiteRef.[
	        ecollectioneventsIRN=irn,
	        colleventSumm=SummaryData,
	        colleventNotes=NteText0,
    	    site=ColSiteRef.[
        		site_guid=irn,
        		SecRecordStatus,	  
        		continent=LocContinent_tab,
        		country=LocCountry_tab,
        		province=LocProvinceStateTerritory_tab, 
        		district=LocDistrictCountyShire_tab, 
        		town=LocTownship_tab,
        		decLat=LatLatitudeDecimal_nesttab,
        		decLong=LatLongitudeDecimal_nesttab,
        		lat=LatLatitude_nesttab,
        		lng=LatLongitude_nesttab,
        		waterBodyType=AquWaterBodyType
    	    ]
	     ]',
        'collector=PrePreparedByRef.[
			collector_irn=irn,
			collector_full_name=NamFullName,
            collector_last_name=NamLast,
			collector_brief_name=NamBriefName,
			collector_salutation=NamSalutation,
			collector_organization=NamOrganisation,
			collector_dept=NamDepartment			
		]',
        'barcode=CardBarcode',
        'scan_record_status=EntRecRecordStatus_tab',
        'catalogue_guid=irn',
        'count=CardNotes',
        'PreType',
        'CatKindOfCollection'
      );
    }
    /* Always only search for those items with flag on for internet access */
    $imusearchArray = array(
      'and',
      array(
        array(
          'AdmPublishWebNoPassword',
          'Yes'
        ),
        $termArray
      )
    );
    try{
      /*
       * create an IMuSession and connect to the IMu server with the hostname
       * and port
       */
      $imusession = new IMuSession($kemuHost, $kemuPort);
	  $imusession->timeout= 300;
      /* access a KEmu module/table */
      $imumodule = new IMuModule($moduleName, $imusession);
      /*
       * search the module/table findTerms is the most flexible way to search
       * for records within a module/table
       */
      $kemuResults = $imumodule->findTerms($imusearchArray);
      /*
       * Get information from matching records flag, offset, count, columns
       */
      //    $results = $imumodule->fetch('start', 0, -1, $kemuColumns);
      $results = $imumodule->fetch('start', 0, $returnNumRows, $kemuColumns);
      /* if there are any rows returned */
      if($results->hits){
        /* if export as xml checkbox ticked */
        if($asXML){
          /*
           * use the IMu PHP library IMuDocument class, a subclass of the
           * standard PHP DOMDocument class
           */
          $doc = new IMuDocument();
          $dom->preserveWhiteSpace = false;
          $dom->formatOutput = true;
          /*
           * one advantage of the writeElement is that it can be passed the
           * object returned from IMuModule's fetch method
           */
          $doc->writeElement('results', $results);
          $results = $doc->saveXML();
        }
        return new FeedsFetcherResult($results);
      }
      // Return an empty FeedsFetcherResult if we didn't get anything from Kemu
      throw new Exception(t('No records were returned for %IMUsearch from the Kemu server %kemuHost:%kemuPort.', array(
        '%IMUsearch' => $IMUsearch,
        '%kemuHost' => $kemuHost,
        '%kemuPort' => $kemuPort
      )));
    }
    catch(Exception $error){
      print("Sorry, an error occurred: $error\n");
    }
  }

  /**
   * Override parent::configDefaults().
   * These form are viewed from the structure feeds importer page
   */
  public function configDefaults(){
    return array(
      'IMUsearch' => 'Schistosoma',
      'importTo' => 'specimen_observation',
      /* vetted, non-sensitive data from outward facing server for delivery to the website */
      'kemuHost' => 'host.name.ac.uk',
      'kemuPort' => '00000',
      'returnNumRows' => '500',
      'asXML' => TRUE
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state){
    $importOptions = feeds_imu_specimens_import_options();
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Set the host and port of the KEMU server holding the data you wish to import below.')
    );
    $form['IMUsearch'] = array(
      '#type' => 'textfield',
      '#title' => t('Search KEMU'),
      '#description' => t('Enter an search term.'),
      '#default_value' => isset($this->config['IMUsearch']) ? $this->config['IMUsearch'] : 'Schistosoma',
      '#required' => TRUE
    );
    $form['kemuHost'] = array(
      '#type' => 'textfield',
      '#title' => t('KEMU host'),
      '#description' => t('Enter the KEMU host from which to allow fetching the data in the form host.name.ac.uk.'),
      '#default_value' => $this->config['kemuHost'],
      '#required' => TRUE
    );
    $form['kemuPort'] = array(
      '#type' => 'textfield',
      '#title' => t('KEMU port'),
      '#description' => t('Enter the KEMU port from which to allow fetching the data.'),
      '#default_value' => $this->config['kemuPort'],
      '#required' => TRUE
    );
    $form['asXML'] = array(
      '#type' => 'checkbox',
      '#title' => t('Return results as XML'),
      '#description' => t('Return results as XML.'),
      '#default_value' => isset($this->config['asXML']) ? $this->config['asXML'] : TRUE,
      '#required' => FALSE
    );
    $form['importTo'] = array(
      '#type' => 'radios',
      '#title' => t('What would you like to import'),
      '#options' => $importOptions,
      '#description' => t('What would you like to import.'),
      '#default_value' => isset($this->config['importTo']) ? $this->config['importTo'] : $importOptions['specimen_observation'],
      '#required' => TRUE
    );
    $form['returnNumRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Return number of rows'),
      '#description' => t('Enter the number of rows to return.'),
      '#default_value' => isset($this->config['returnNumRows']) ? $this->config['returnNumRows'] : '500',
      '#required' => TRUE
    );
    $form['imuVersion'] = array(
      '#type' => 'item',
      '#title' => t('IMU version'),
      '#markup' => t('Using IMu API version ' . IMu::VERSION)
    );
    return $form;
  }

  /**
   * Override parent::sourceDefaults().
   */
  function sourceDefaults(){
    return array(
      'IMUsearch' => 'Schistosoma',
      'importTo' => 'specimen_observation',
      'kemuHost' => 'host.name.ac.uk',
      'kemuPort' => '00000',
      'returnNumRows' => '500',
      'asXML' => TRUE
    );
  }

  /**
   * Override parent::sourceForm().
   * this form appears on the import page
   */
  public function sourceForm($source_config){
    $importOptions = feeds_imu_specimens_import_options();
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Set the host and port of the KEMU server holding the data you wish to import below.')
    );
    $form['IMUsearch'] = array(
      '#type' => 'textfield',
      '#title' => t('Search KEMU'),
      '#description' => t('Enter an search term.'),
      '#default_value' => isset($source_config['IMUsearch']) ? $source_config['IMUsearch'] : 'Schistosoma',
      '#required' => TRUE
    );
    $form['kemuHost'] = array(
      '#type' => 'textfield',
      '#title' => t('KEMU host'),
      '#description' => t('Enter the KEMU host from which to allow fetching the data in the form host.name.ac.uk.'),
      '#default_value' => isset($source_config['kemuHost']) ? $source_config['kemuHost'] : 'host.name.ac.uk',
      '#required' => TRUE
    );
    $form['kemuPort'] = array(
      '#type' => 'textfield',
      '#title' => t('KEMU port'),
      '#description' => t('Enter the KEMU port from which to allow fetching the data.'),
      '#default_value' => isset($source_config['kemuPort']) ? $source_config['kemuPort'] : '00000',
      '#required' => TRUE
    );
    $form['asXML'] = array(
      '#type' => 'checkbox',
      '#title' => t('Return results as XML'),
      '#description' => t('Return results as XML.'),
      '#default_value' => isset($source_config['asXML']) ? $source_config['asXML'] : TRUE,
      '#required' => FALSE
    );
    $form['importTo'] = array(
      '#type' => 'radios',
      '#title' => t('What would you like to import'),
      '#options' => $importOptions,
      '#description' => t('What would you like to import.'),
      '#default_value' => isset($source_config['importTo']) ? $source_config['importTo'] : $importOptions['specimen_observation'],
      '#required' => TRUE
    );
    $form['returnNumRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Return number of rows'),
      '#description' => t('Enter the number of rows to return.'),
      '#default_value' => isset($source_config['returnNumRows']) ? $source_config['returnNumRows'] : '500',
      '#required' => TRUE
    );
    $form['imuVersion'] = array(
      '#type' => 'item',
      '#title' => t('IMU version'),
      '#markup' => t('Using IMu API version ' . IMu::VERSION)
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values){
    $IMUsearch = $values['IMUsearch'];
    $kemuHost = $values['kemuHost'];
    $kemuPort = $values['kemuPort'];
    $importTo = $values['importTo'];
    $returnNumRows = $values['returnNumRows'];
    if(empty($importTo)){
      form_set_error('feeds][importTo', t('You must enter a place to import to.'));
    }
    if(empty($IMUsearch)){
      form_set_error('feeds][IMUsearch', t('You must enter a Genus to search on.'));
    }
    if(empty($kemuHost) || $kemuHost == 'host.name.ac.uk'){
      form_set_error('feeds][kemuHost', t('You must enter a valid KEMU server address.'));
    }
    if(empty($kemuPort) || $kemuPort == '00000'){
      form_set_error('feeds][kemuPort', t('You must enter a valid KEMU server port number'));
    }
    if(empty($returnNumRows)){
      form_set_error('feeds][returnNumRows', t('You must enter the number of rows to return'));
    }
  }
}

function feeds_imu_specimens_import_options(){
  return array(
    'specimen_observation' => t('Specimen Observations'),
    'location' => t('Location'),
    'taxonomy' => t('Taxonomy'),
    'scan' => t('SCAN')
  );
}
