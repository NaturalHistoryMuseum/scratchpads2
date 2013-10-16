<?php
// Load the Imu library.
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
class FeedsIMUFetcher extends FeedsFetcher{

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source){
    $source_config = $source->getConfigFor($this);
    $kemuHost = trim($source_config['kemuHost']);
    $kemuPort = trim($source_config['kemuPort']);
    $IMUrecord_status = trim($source_config['IMUrecord_status']);
    $importTo = trim($source_config['importTo']);
    $asXML = trim($source_config['asXML']);
    $debugImu = trim($source_config['debugImu']);
    $fetchDelay = trim($source_config['fetchDelay']);
    $fetchDelay = $fetchDelay ? $fetchDelay : 4; //default to 4 sec
    $returnNumRows = trim($source_config['returnNumRows']);
    $returnNumRows = $returnNumRows ? $returnNumRows : 300; // default to 300 rows in a batch
    if($importTo == 'specimen'){
      $termArray = array(
        'and',
        array(
          array(
            'ColRecordType',
            'Specimen'
          )
        )
      );
    }
    if($importTo == 'parasite_card'){
      $termArray = array(
        'and',
        array(
          array(
            'ColRecordType',
            'Parasite Card'
          )
        )
      );
    }
    /* if($importTo == 'location'){
      $termArray = array(
          'and',
          array(
              array(
                  'irn',
                  '2765235'
              )
          )
      );
    }*/   
    /* if($importTo == 'parasite_card_host_taxonomy'){
        $termArray = array(
          'and',
          array(
            array(
              'irn',
              '2765235'
            )
          )
        );
      }*/    
   /* if($importTo == 'parasite_card_parasite_taxonomy'){
     $termArray = array(
         'and',
         array(
             array(
                 'irn',
                 '2765235'
             )
         )
     );
    }*/    
    /* if($importTo == 'specimen_taxonomy'){
        $termArray = array(
          'and',
          array(
            array(
              'irn',
              '2821192'
            )
          )
        );
      }*/
    /* Always only search for those items with flag on for internet access */
    $imusearchArray = array(
      'and',
      array(
        array(
          'AdmPublishWebNoPassword',
          'Yes'
        ),
        array(
          'EntRecRecordStatus_tab',
          $IMUrecord_status
        )
      )
    );
    if(isset($termArray) && is_array($termArray)){
      $imusearchArray[1][] = $termArray;
    }
    $moduleName = 'ecatalogue';
    $kemuColumns = _returnRequiredColumns($importTo);
    try{
      /* create an IMuSession and connect to the IMu server with the hostname and port */
      $imusession = new IMuSession($kemuHost, $kemuPort);
      /* access a KEmu module/table */
      $imumodule = new IMuModule($moduleName, $imusession);
      /* map columns to a fetchset for greater efficiency */
      $imumodule->addFetchSet('kemu_columns', $kemuColumns);
      if($debugImu){
        watchdog('imu debug', '<pre> importing as ' . print_r($importTo, TRUE) . '</pre>');
        watchdog('imu debug', '<pre> searched on ' . print_r($imusearchArray, TRUE) . '</pre>');
      }
      /* findTerms is the most flexible way to search for records within a module/table */
      $kemuResultsHits = $imumodule->findTerms($imusearchArray);
      $totalarray = array();
      $totalRows = array();
      $totalCalls = array();
      $rownum = 1;
      $i = 0;
      /* Get through querying in batches to not overburdern KE  */
      if($kemuResultsHits != 0){
        while($i < $kemuResultsHits){
          $imuBatch = ($i % $returnNumRows);
          if($imuBatch == 0){
            /* Get information from matching records flag, offset, count, columns  */
            $results = $imumodule->fetch('start', $rownum - 1, $returnNumRows, 'kemu_columns');
            $last = $results->rows[count($results->rows) - 1];
            if($last['rownum'] < $results->hits){
              $rownum = $last['rownum'] + 1;
            }
            $totalarray[] = $results;
            $totalRows[] = $results->rows;
            if($debugImu){
              $totalCalls[] = " row " . $i . "-" . $last['rownum'] . " " . date('h:i:s');
            }
            sleep($fetchDelay);
          }
          $i++;
        }
        $mergedRows = call_user_func_array('array_merge', $totalRows);
        $resultsArray = $totalarray[0];
        $resultsArray->rows = _uniqueGUIDS($importTo, $mergedRows);
      }else{
        // Return an empty FeedsFetcherResult if we didn't get anything from Kemu
        throw new Exception(t('No records were returned for %IMUrecord_status from the Kemu server %kemuHost:%kemuPort.', array(
          '%IMUrecord_status' => $IMUrecord_status,
          '%kemuHost' => $kemuHost,
          '%kemuPort' => $kemuPort
        )));
      }
      /* if there are any rows returned */
      if($results->hits){
        if($debugImu){
          $times = implode(',', $totalCalls);
          watchdog('imu debug', '<pre>' . print_r($times, TRUE) . '</pre>');
          watchdog('imu debug', '<pre>number of rows returned ' . print_r($kemuResultsHits, TRUE) . '</pre>');
          watchdog('imu debug', '<pre>merged total rows returned ' . print_r($mergedRows, TRUE) . '</pre>');
          watchdog('imu debug', '<pre>unique rows returned ' . print_r($resultsArray, TRUE) . '</pre>');
        }
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
          $doc->writeElement('results', $resultsArray);
          $results = $doc->saveXML();
        }
        return new FeedsFetcherResult($results);
      }else{
        // Return an empty FeedsFetcherResult if we didn't get anything from Kemu
        throw new Exception(t('No records were returned for %IMUrecord_status from the Kemu server %kemuHost:%kemuPort.', array(
          '%IMUrecord_status' => $IMUrecord_status,
          '%kemuHost' => $kemuHost,
          '%kemuPort' => $kemuPort
        )));
      }
    }
    catch(Exception $error){
      watchdog('imu debug', '<pre> Sorry, an error occurred: ' . print_r($error, TRUE) . '</pre>');
    }
  }

  /**
   * Override parent::configDefaults().
   * These form are viewed from the structure feeds importer page
   */
  public function configDefaults(){
    return array(
      'IMUrecord_status' => 'SCAN_record',
      'importTo' => 'specimen',
      'kemuHost' => 'host.name.ac.uk',
      'kemuPort' => '00000',
      'returnNumRows' => '300',
      'asXML' => TRUE,
      'debugImu' => FALSE
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state){
    $importOptions = feeds_imu_fetcher_import_options();
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Set the host and port of the KEMU server holding the data you wish to import below.')
    );
    $form['IMUrecord_status'] = array(
      '#type' => 'textfield',
      '#title' => t('Search KEMU record status'),
      '#description' => t('Enter a record status to search on.'),
      '#default_value' => isset($this->config['IMUrecord_status']) ? $this->config['IMUrecord_status'] : 'SCAN_record',
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
    $form['fetchDelay'] = array(
      '#type' => 'textfield',
      '#title' => t('Delay in secs between Imu fetches. Defaults to 4.'),
      '#description' => t('Number of seconds to delay in between fetches.'),
      '#default_value' => isset($this->config['fetchDelay']) ? $this->config['fetchDelay'] : 4,
      '#required' => TRUE
    );
    $form['asXML'] = array(
      '#type' => 'checkbox',
      '#title' => t('Return results as XML'),
      '#description' => t('Return results as XML.'),
      '#default_value' => isset($this->config['asXML']) ? $this->config['asXML'] : TRUE,
      '#required' => FALSE
    );
    $form['debugImu'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug Imu results'),
      '#description' => t('Send some debug Imu results to watchdog.'),
      '#default_value' => isset($this->config['debugImu']) ? $this->config['debugImu'] : FALSE,
      '#required' => FALSE
    );
    $form['importTo'] = array(
      '#type' => 'radios',
      '#title' => t('What would you like to import'),
      '#options' => $importOptions,
      '#description' => t('What would you like to import.'),
      '#default_value' => isset($this->config['importTo']) ? $this->config['importTo'] : $importOptions['specimen'],
      '#required' => TRUE
    );
    $form['returnNumRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Return number of rows at a time'),
      '#description' => t('Enter the number of rows to return. If too big this may break the fetch. Default is 300.'),
      '#default_value' => isset($this->config['returnNumRows']) ? $this->config['returnNumRows'] : '300',
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
      'IMUrecord_status' => 'SCAN_record',
      'importTo' => 'specimen',
      'kemuHost' => 'host.name.ac.uk',
      'kemuPort' => '00000',
      'fetchDelay' => 4,
      'returnNumRows' => '300',
      'asXML' => TRUE,
      'debugImu' => FALSE
    );
  }

  /**
   * Override parent::sourceForm().
   * this form appears on the import page
   */
  public function sourceForm($source_config){
    $importOptions = feeds_imu_fetcher_import_options();
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Set the host and port of the KEMU server holding the data you wish to import below.')
    );
    $form['IMUrecord_status'] = array(
      '#type' => 'textfield',
      '#title' => t('Search KEMU record status'),
      '#description' => t('Enter a record status to search on.'),
      '#default_value' => isset($source_config['IMUrecord_status']) ? $source_config['IMUrecord_status'] : 'SCAN_record',
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
    $form['fetchDelay'] = array(
      '#type' => 'textfield',
      '#title' => t('Delay in secs between Imu fetches. Defaults to 4.'),
      '#description' => t('Number of seconds to delay in between fetches.'),
      '#default_value' => isset($source_config['fetchDelay']) ? $source_config['fetchDelay'] : 4,
      '#required' => TRUE
    );
    $form['asXML'] = array(
      '#type' => 'checkbox',
      '#title' => t('Return results as XML'),
      '#description' => t('Return results as XML.'),
      '#default_value' => isset($source_config['asXML']) ? $source_config['asXML'] : TRUE,
      '#required' => FALSE
    );
    $form['debugImu'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug Imu results'),
      '#description' => t('Send some debug Imu results to watchdog.'),
      '#default_value' => isset($source_config['debugImu']) ? $source_config['debugImu'] : FALSE,
      '#required' => FALSE
    );
    $form['importTo'] = array(
      '#type' => 'radios',
      '#title' => t('What would you like to import'),
      '#options' => $importOptions,
      '#description' => t('What would you like to import.'),
      '#default_value' => isset($source_config['importTo']) ? $source_config['importTo'] : $importOptions['specimen'],
      '#required' => TRUE
    );
    $form['returnNumRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Return number of rows at a time'),
      '#description' => t('Enter the number of rows to return. If too big this may break the fetch. Default is 300.'),
      '#default_value' => isset($source_config['returnNumRows']) ? $source_config['returnNumRows'] : '300',
      '#required' => TRUE
    );
    $form['imuVersion'] = array(
      '#type' => 'item',
      '#title' => t('IMU version'),
      '#markup' => t('Using IMu API version ' . IMu::VERSION)
    );
    /* xml parser needs a source field */
    $form['source'] = array(
      '#type' => 'hidden',
      '#default_value' => "IMU v. " . IMu::VERSION,
      '#required' => TRUE
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values){
    $IMUrecord_status = $values['IMUrecord_status'];
    $kemuHost = $values['kemuHost'];
    $kemuPort = $values['kemuPort'];
    $importTo = $values['importTo'];
    $fetchDelay = $values['fetchDelay'];
    $returnNumRows = $values['returnNumRows'];
    if(!empty($returnNumRows) && !is_numeric($returnNumRows)){
      form_set_error('feeds][returnNumRows', t('You must enter a number.'));
    }
    if(!empty($fetchDelay) && !is_numeric($fetchDelay)){
      form_set_error('feeds][fetchDelay', t('You must enter a number.'));
    }
    if(empty($importTo)){
      form_set_error('feeds][importTo', t('You must what you want to import.'));
    }
    if(empty($IMUrecord_status)){
      form_set_error('feeds][IMUrecord_status', t('You must enter a record status to search on.'));
    }
    if(empty($kemuHost) || $kemuHost == 'host.name.ac.uk'){
      form_set_error('feeds][kemuHost', t('You must enter a valid KEMU server address.'));
    }
    if(empty($kemuPort) || $kemuPort == '00000'){
      form_set_error('feeds][kemuPort', t('You must enter a valid KEMU server port number'));
    }
  }
}

function feeds_imu_fetcher_import_options(){
  return array(
    'location' => t('Location'),
    'specimen_taxonomy' => t('Specimen taxonomy'),
    'parasite_card_host_taxonomy' => t('Parasite card host taxonomy'),
    'parasite_card_parasite_taxonomy' => t('Parasite card parasite taxonomy'),
    'specimen' => t('Specimens'),
    'parasite_card' => t('Parasite card')
  );
}

function _uniqueGUIDS($importTo, $mergedRows){
  $unique_GUIDS = array();
  $unique_rows = array();
  switch($importTo){
    case "parasite_card_host_taxonomy":
      foreach($mergedRows as $row){
        if(isset($row['host_taxa_GUID']) && !in_array($row['host_taxa_GUID'], $unique_GUIDS)){
          $unique_GUIDS[] = $row['host_taxa_GUID'];
          $unique_rows[] = $row;
        }
      }
      break;
    case "parasite_card_parasite_taxonomy":
      foreach($mergedRows as $row){
        if(isset($row['parasite_taxa_GUID']) && !in_array($row['parasite_taxa_GUID'], $unique_GUIDS)){
          $unique_GUIDS[] = $row['parasite_taxa_GUID'];
          $unique_rows[] = $row;
        }
      }
      break;
    case "specimen_taxonomy":
      foreach($mergedRows as $row){
        if(isset($row['specimen_taxa_GUID']) && !in_array($row['specimen_taxa_GUID'], $unique_GUIDS)){
          $unique_GUIDS[] = $row['specimen_taxa_GUID'];
          $unique_rows[] = $row;
        }
      }
      break;
    case "location":
      foreach($mergedRows as $row){
        if(isset($row['location_GUID']) && !in_array($row['location_GUID'], $unique_GUIDS)){
          $unique_GUIDS[] = $row['location_GUID'];
          $unique_rows[] = $row;
        }
      }
      break;
    case "specimen":
    case "parasite_card":
      $unique_rows = $mergedRows;
      break;
  }
  return $unique_rows;
}

function _returnRequiredColumns($importTo){
  switch($importTo){
    case "location":
      $kemuColumns = array(
        'location_GUID=EntLocEventSiteRef.[GUID=ColSiteRef],
            collectionEvent=EntLocEventSiteRef.[
			collection_event_GUID=irn,
			colleventNotes=NteText0,            
			site=ColSiteRef.[
				site_title=SummaryData,
				continent=LocContinent_tab,
				country=LocCountry_tab,
				province=LocProvinceStateTerritory_tab,
				district=LocDistrictCountyShire_tab,
				parish=PolParish,
				town=LocTownship_tab,
                island_group=LocIslandGrouping,
                island=LocIslandName,
                dec_lat=LatLatitudeDecimal_nesttab,
				dec_lon=LatLongitudeDecimal_nesttab,
                geodetic_datum=LatDatum_tab,
                depth_from=PhyDepthFromMtr,
                depth_to=PhyDepthToMtr,
                altitude_from=PhyAltitudeFromMtr,
                altitude_to=PhyAltitudeToMtr,
				lat=LatLatitude_nesttab,
				lng=LatLongitude_nesttab,
                verbatim_lat=LatLatitudeVerbatim_nesttab,
				verbatim_lon=LatLongitudeVerbatim_nesttab,
                water_body_type=AquWaterBodyType
			]
		 ]'
      );
      break;
    case "specimen_taxonomy":
      $kemuColumns = array(
        'specimen_taxa_GUID=EntIdeTaxonRef_tab',
        'taxa=EntIdeTaxonRef_tab.[
				taxa_GUID=irn,
				rank=TaxClassRank_tab,
				term_name=TaxClassValue_tab,
				author=TaxClassAuthors_tab
			]'
      );
      break;
    case "parasite_card_host_taxonomy":
      $kemuColumns = array(
        'host_taxa_GUID=CardHostRef_tab',
        'host_taxa=CardHostRef_tab.[
				taxa_GUID=irn,
				rank=TaxClassRank_tab,
				term_name=TaxClassValue_tab,
				author=TaxClassAuthors_tab
			]'
      );
      break;
    case "parasite_card_parasite_taxonomy":
      $kemuColumns = array(
        'parasite_taxa_GUID=CardParasiteRef_tab',
        'parasite_taxa=CardParasiteRef_tab.[
				taxa_GUID=irn,
				rank=TaxClassRank_tab,
				term_name=TaxClassValue_tab,
				author=TaxClassAuthors_tab
			]'
      );
      break;
    case "specimen":
      $kemuColumns = array(
        'specimen_GUID=irn',
        'taxa_GUID=EntIdeTaxonRef_tab',
        'collectionEvent=EntLocEventSiteRef.[
			location_GUID=ColSiteRef
	    ]',
        'basis_of_record=DarBasisOfRecord',
        'catalogue_number=DarCatalogNumber',
        'collection_code=DarCollectionCode',
        'institution_code=DarInstitutionCode',
        'record_status=EntRecRecordStatus_tab',
        'count=DarIndividualCount',
        'life_stage=DarLifeStage',
        'DarCollector',
        'DarCollectorNumber',
        'date_collected_start_year=DarStartYearCollected',
        'date_collected_start_month=DarStartMonthCollected',
        'date_collected_start_day=DarStartDayCollected',
        'date_collected_end_year=DarEndYearCollected',
        'date_collected_end_month=DarEndMonthCollected',
        'date_collected_end_day=DarEndDayCollected',
        'field_notes=DarFieldNotes',
        'DarGenBankNum',
        'DarFieldNumber',
        'other_catalogue_numbers=DarOtherCatalogNumbers',
        'EntCatOtherNumbersKind_tab',
        'DarRemarks',
        'DarSex',
        'DarTypeStatus',
        'CatKindOfCollection',
        'CatKindOfObject',
        'CatPreservative',
        'field_remarks1=EntCatOtherNumbersKind_tab',
        'field_remarks2=EntCatOtherNumbersValue_tab'
      );
      break;
    case "parasite_card":
      $kemuColumns = array(
        'specimen_GUID=irn',
        'host_taxa_GUID=CardHostRef_tab',
        'parasite_taxa_GUID=CardParasiteRef_tab',
        'person_GUID=PrePreparedByRef',
        'collectionEvent=EntLocEventSiteRef.[
			location_GUID=ColSiteRef,
			field_notes=NteText0
	     ]',
        'catalogue_number=CardBarcode',
        'collection_code=irn',
        'record_status=EntRecRecordStatus_tab',
        'count=CardNotes',
        'life_stage=CardParasiteStage_nesttab',
        'collector=PrePreparedByRef.[
			collector_irn=irn,
			collector_brief_name=NamBriefName
		]',
        'date_collected_start=PrePreparedDateFrom',
        'date_collected_end=PrePreparedDateTo',
        'field_remarks1=PreType',
        'field_remarks2=CatKindOfCollection'
      );
      break;
  }
  return $kemuColumns;
}