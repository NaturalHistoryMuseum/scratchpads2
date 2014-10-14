<?php

/*
 * major part of this code was adapted from agrovoc_api module
 * @author guillaumev
 */
class agrovoc_remote extends agrovoc {

  var $source = '';
  var $agrovoc_ws_url = '';
  var $agrovoc_ws_uri = '';
  var $_agrovoc_api_client = NULL;
  // lang is language, source is remote or local

  function __construct($agrovoc_ws_url, $agrovoc_ws_uri) {
    $this->source = 'remote';
    $this->agrovoc_ws_url = $agrovoc_ws_url;
    $this->agrovoc_ws_uri = $agrovoc_ws_uri;
  }

  /**
   * Returns the SOAP client.
   */
  function _agrovoc_api_get_client() {
    global $_agrovoc_api_client;
    if ($_agrovoc_api_client == NULL) {
        $_agrovoc_api_client = new SoapClient(NULL, array('location' => $this->agrovoc_ws_url, 'uri' => $this->agrovoc_ws_uri, 'exceptions' => 0));
    }
    return $_agrovoc_api_client;
  }

  /**
   * Implements the call to the simpleSearchByMode2 method of the Agrovoc web services.
   *
   * @param string $term
   *  Term searched
   * @param string $mode
   *  Search mode (starting, containing, exact, end with, exact word)
   * @param string $separator
   *  Separator (set to , by default)
   * @param bool $as_array
   *  Whether to return the results under the form of an array of the form
   *  array(1 => array(id => 'id', lang => 'lang', term => 'term')). Set to TRUE by default
   * @param int $limit
   *  Limits the number of results to be returned. Only used if $as_array is set to TRUE
   * @param string $filter_language
   *  If set, will return only terms of a specific language. Only used if as_array is set to true
   *
   * @return
   *  Response of the Agrovoc API
   */
  function simple_search_by_mode2($term, $mode = 'exact', $separator = ',', $as_array = TRUE, $limit = 0, $filter_language = '') {
    $params = array('searchString' => $term, 'searchmode' => $mode, 'separator' => $separator);
    // Create the unique ID of this method call
    $id_string = 'simpleSearchByMode2' . implode(',', $params);
    $md5_string = md5($id_string);
    // See if the response of the request is in the cache
    $cache_response = cache_get('agrovoc_api_' . $md5_string);
    $cache_hit = TRUE;
    if (!$cache_response) {
      // Do the SOAP call
      $cache_hit = FALSE;
      $client = $this->_agrovoc_api_get_client();
      $response = $client->__soapCall('simpleSearchByMode2', $params);
    }
    else {
      $response = $cache_response->data;
    }
    if (is_soap_fault($response)) {
      drupal_set_message(t('An error occured while calling the Agrovoc API'), 'error');
      if ($as_array == TRUE) {
        return array();
      }
      else {
        return NULL;
      }
    }
    else {
      // If it was not found in the cache, store the response in the cache
      if (!$cache_hit) {
        cache_set('agrovoc_api_' . $md5_string, $response);
      }
      if ($as_array == TRUE) {
        return $this->simple_search_by_mode2_response_to_array($response, $separator, $limit, $filter_language);
      }
      else {
        return $response;
      }
    }
  }

  /**
   * Formats the elements returned in an array.
   *
   * @param string $response
   *  Response of Agrovoc web service
   * @param string $separator
   *  Separator
   * @param int $limit
   *  Limit the number of results to return
   * @param string $filter_language
   *  Filter results by language
   *
   * @return array
   *  Array of results
   */
  //anto add other function to get non-descriptor
  function getConceptInfoByTermcode_response_to_array($response, $separator){
  	
  	//$response = trim($response, '[]');
    $responsevalue = $response['getConceptInfoByTermcodeReturn'][3];
    $response_array=explode(',',$responsevalue);
    if (count($response_array)>1){
    	$final_array=array('0' => array('id'=>trim(str_ireplace(']', '', $response_array[1]))),'1' =>'non-descriptor');
    	
       }
       else{
       	
         $final_array=array('0' => array('id'=>trim(str_ireplace(']', '', $response_array[0]))),'1' =>'descriptor');
       }
    
    return $final_array;
  }
  //
  function simple_search_by_mode2_response_to_array($response, $separator, $limit, $filter_language) {
    $response = trim($response, '[]');
    $response_array = explode($separator, $response);
    // Pop out last two elements
    array_pop($response_array);
    array_pop($response_array);
    $final_array = array();
    $i = 0;
    $j = -1;
    $term = array();
    foreach ($response_array as $element) {
      if ($i % 3 == 0) {
        $term['id'] = $element;
      }
      elseif ($i % 3 == 1) {
        $term['term'] = $element;
      }
      elseif ($i % 3 == 2) {
        $term['lang'] = $element;
        // If the language is the right one, add the term
        if ($filter_language != '' && strtolower($term['lang']) == $filter_language) {
          $j++;
          if ($limit > 0 && $j == $limit - 1) { // Limit has been reached
            break;
          }
          $final_array[$j] = $term;
        }
        elseif ($filter_language == '') {
          $j++;
          if ($limit > 0 && $j == $limit - 1) { // Limit has been reached
            break;
          }
          $final_array[$j] = $term;
        }
      }
      $i++;
    }
    return $final_array;
  }

  /**
   * Implements the call to the getAllLabelsByTermcode2 method of the Agrovoc web services.
   *
   * @param int $termcode
   *  Termcode to look for
   * @param string $separator
   *  Separator (set to , by default)
   * @param bool $as_array
   *  Whether to return the results under the form of an array indexed by language code. Set to TRUE by default
   *
   * @return
   *  Response of the Agrovoc web services
   */
   //anto added function to find descriptor code for non-descriptor terms
   function getConceptInfoByTermcode($termcode, $separator = ',', $as_array = TRUE) {
    $md5_string = '';
    $params = array('termcode' => $termcode);
    // Create the unique ID of this method call
    
    $id_string = 'getConceptInfoByTermcode' . implode(',', $params);
    $md5_string = md5($id_string);
    // See if the response of the request is in the cache
    $cache_response = cache_get('agrovoc_api_' . $md5_string);
    $cache_hit = TRUE;
    if (!$cache_response) {
      $cache_hit = FALSE;
      $client = $this->_agrovoc_api_get_client();
      $response = $client->__soapCall('getConceptInfoByTermcode', $params);
    }
    else {
      $response = $cache_response->data;
    }
    if (is_soap_fault($response)) {
      drupal_set_message(t('An error occured while calling the Agrovoc API'), 'error');
      watchdog('agrovoc_api', t('The Agrovoc API returned the following response: %response', array('%response' => $response)), NULL, WATCHDOG_ERROR);
      if ($as_array == TRUE) {
        return array();
      }
      else {
        return NULL;
      }
    }
    else {
      if (!$cache_hit) {
        // Store in cache
        cache_set('agrovoc_api_' . $md5_string, $response);
      }
      if ($as_array == TRUE) {
        return $this->getConceptInfoByTermcode_response_to_array($response, $separator);
      }
      else {
        return $response;
      }
    }
	   
	   
   }
   
  function get_all_labels_by_termcode2($termcode, $separator = ',', $as_array = TRUE) {
    $md5_string = '';
    $params = array('termcode' => (int) $termcode, 'separator' => $separator);
    // Create the unique ID of this method call
    $id_string = 'getAllLabelsByTermcode2' . implode(',', $params);
    $md5_string = md5($id_string);
    // See if the response of the request is in the cache
    $cache_response = cache_get('agrovoc_api_' . $md5_string);
    $cache_hit = TRUE;
    if (!$cache_response) {
      $cache_hit = FALSE;
      $client = $this->_agrovoc_api_get_client();
      $response = $client->__soapCall('getAllLabelsByTermcode2', $params);
    }
    else {
      $response = $cache_response->data;
    }
    if (is_soap_fault($response)) {
      drupal_set_message(t('An error occured while calling the Agrovoc API'), 'error');
      watchdog('agrovoc_api', t('The Agrovoc API returned the following response: %response', array('%response' => $response)), NULL, WATCHDOG_ERROR);
      if ($as_array == TRUE) {
        return array();
      }
      else {
        return NULL;
      }
    }
    else {
      if (!$cache_hit) {
        // Store in cache
        cache_set('agrovoc_api_' . $md5_string, $response);
      }
      if ($as_array == TRUE) {
        return $this->get_all_labels_by_termcode2_response_to_array($response, $separator);
      }
      else {
        return $response;
      }
    }
  }

  /**
   * Formats the element returned by getAllLabelsByTermcode2 as an array.
   *
   * @param string $response
   *  Response of the Agrovoc web services
   * @param string $separator
   *  Separator
   *
   * @return array
   *  Array indexed by language
   */
  function get_all_labels_by_termcode2_response_to_array($response, $separator) {
    $response = trim($response, '[]');
    $response_array = explode($separator, $response);
    $final_array = array();
    $i = 0;
    foreach ($response_array as $element) {
      if ($i % 2 == 0) {
        $term = $element;
      }
      elseif ($i % 2 == 1) {
        $final_array[$element] = $term;
      }
      $i++;
    }
    return $final_array;
  }
  /**
   * Load a Agrovoc Terms Object
   *
   * @param string $label
   *   Agrovoc term
   * @return
   *   object containing others languages and agrovoc id that belongs to this label
   */
  function agrovocfield_load_object($label) {
    $languages_supported =  language_list();
    try {
      $term = $this->simple_search_by_mode2($label, 'exact', ',', TRUE, 10);
      if(count($term) == 0) {
        throw new Exception(t('@label is not a AGROVOC term.', array('@label' => $label)));
      }
      //i change the variable in $termtemp instead $term
      $term = array_values($term);
	   
	  $term_labels = $this->get_all_labels_by_termcode2($term[0]['id']);
	 	  	 
	  $agrovoc = new stdClass();
      $agrovoc->id = $term[0]['id'];
      foreach ($languages_supported  as $k => $v) {
      // Use substr because FAO Chinese language is ZH and Drupal Chinese language is zh-hans,
      // therefore, you  need only 2 first characters.
        $labels[$k] = isset($term_labels[substr($k, 0, 2)]) ? $term_labels[substr($k, 0, 2)] : '';
      }
      $agrovoc->labels = $labels;
      return $agrovoc;
    }

    
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
      $agrovoc_null = new stdClass();
      $agrovoc_null->id = '';
      foreach ($languages_supported  as $k => $v) {
        $labels[$k] = $term_labels[substr($k, 0, 2)];
      }
      $agrovoc_null->labels = $labels;
      return $agrovoc_null;
    }
  }

  /**
   * Checks if a term is part of the Agrovoc.
   *
   * This function is not part of the Agrovoc web services, however it is provided here for convenience.
   *
   * @param string $term
   *  Term to be checked
   * @return
   *  TRUE if the term is part of the Agrovoc, FALSE if it isn't
   */
  function is_agrovoc_term($term) {
    $response = $this->simple_search_by_mode2($term, 'starting', ',', FALSE);
    if (stripos($response, $term) !== FALSE) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  function agrovoc_api_is_available(){
    $headers = get_headers($this->agrovoc_ws_url);
    if ($headers[0] == 'HTTP/1.1 200 OK') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}
