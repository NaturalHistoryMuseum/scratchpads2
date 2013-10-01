<?php

class agrovoc_local extends agrovoc {

	var $source = '';
	var $agrovoc_ws_url = '';
	var $agrovoc_ws_uri = '';
  var $_agrovoc_api_client = NULL;
	// lang is language, source is remote or local

  function __construct($agrovoc_ws_url, $agrovoc_ws_uri) {
	  $this->source = 'local';
	  $this->agrovoc_ws_url = $agrovoc_ws_url;
	  $this->agrovoc_ws_uri = $agrovoc_ws_uri;
	}
	

	// only declaration to implement in subclasses according source
  function simple_search_by_mode2($searchstring = NULL, $searchmode = 'exact', $separator = ',', $as_array = TRUE, $limit = 1, $lang = '') {
  $array_terms = array();
   switch ($searchmode) {
     case 'starting'   : $string = $searchstring . '%'; break;
     case 'containing' : $string = '%' . $searchstring . '%'; break;
     case 'exact word' : $string = $searchstring ; break;
     case 'exact'      : $string = $searchstring; break;
     case 'end with'   : $string = '%' . $searchstring; break;
    }

  $query = db_select('agrovocfield_data', 'a')->fields('a');
  if (!empty($lang)) {
    $query->condition('languagecode', $lang, '=');
  }
  $query->condition('termspell', $string, 'LIKE');
  $query->range(0, $limit);

  $rows = $query->execute();
  $num_rows = $query->countQuery()->execute()->fetchField();

  if(!empty($num_rows)) {
   	 $i = 0;
     foreach ($rows as $row) {
     	 $i++;
       $array_terms[$i]['id'] = $row->termcode;
       $array_terms[$i]['term'] = $row->termspell;   
       $array_terms[$i]['lang'] = $row->languagecode;
       $string_terms[$i] = implode(',', $array_terms[$i]);
     }
  }
  $output = $as_array == TRUE ? $array_terms : "[" . implode(',', $string_terms) .  "NumberOfResults," . $num . "]";
  return $output;
}

	// only declaration to implement in subclasses according source	
	function get_all_labels_by_termcode2($termcode = NULL, $separator = ',', $as_array = TRUE) {
    $query = db_select('agrovocfield_data', 'a')
    ->fields('a')
    ->condition('termcode', $termcode, '=');

    $rows = $query->execute();    

    foreach ($rows as $row) {
      $array_terms[$row->languagecode] = $row->termspell;
      $string_terms[] = implode(',', array($row->termspell, $row->languagecode));
    }
    $output = $as_array == TRUE ? $array_terms : "[" . implode(',', $string_terms) . "]";    
    return $output;
	}

  function agrovocfield_load_object($label) {
    $languages_supported =  language_list();
    $term_labels = array();
  	try {
      $term = $this->simple_search_by_mode2($label, 'exact', ',', TRUE, 1);
      if(count($term) == 0) {
        throw new Exception(t('@label is not a AGROVOC term.', array('@label' => $label)));
      }
      $term = array_values($term);
      $term_labels = $this->get_all_labels_by_termcode2($term[0]['id']);
      $agrovoc = new stdClass();
      $agrovoc->id = $term[0]['id'];
      foreach ($languages_supported  as $k => $v) {
       // Use substr because FAO Chinese language is ZH and Drupal Chinese language is zh-hans, 
       // therefore, you  need only 2 first characters.
      if(isset($term_labels[strtoupper(substr($k, 0, 2))])){ 
        $labels[$k] = $term_labels[strtoupper(substr($k, 0, 2))];
      }
      }
      $agrovoc->labels = $labels;
      return $agrovoc;      
    }
    catch (Exception $e) { 
       drupal_set_message($e->getMessage(), 'error');
       $agrovoc_null = new stdClass();
       $agrovoc_null->id = '';
       foreach ($languages_supported  as $k => $v) {
         $labels[$k] = $term_labels[strtoupper(substr($k, 0, 2))];
       }    
       $agrovoc_null->labels = $labels;
       return $agrovoc_null;  
    }
  }
  
}  
