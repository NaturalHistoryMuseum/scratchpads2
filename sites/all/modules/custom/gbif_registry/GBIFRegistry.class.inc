<?php

/**
 * Standard class for interacting with the GBIF registry.  Note, this code
 * should NOT rely on any Drupal functions to aid in its portability.  If 
 * Drupal specific functions need to be used, an alternative should be provided.
 * 
 * @author simor
 */
class GBIFRegistry{

  //private $registry_url = 'http://gbrds.gbif.org/registry/';
  private $registry_url = 'http://gbrdsdev.gbif.org/registry/';

  private $alternative_functions = array();

  private $organisation_key, $organisation_password;

  public function __construct($alternatives = FALSE){
    // Set alternatives if we've been sent any.
    if($alternatives){
      $this->set_alternative_functions($alternatives);
    }
    // Check we have a valid "put" function, if not, we throw an error.
    if(!isset($this->alternatives['put']) || !function_exists($this->alternatives['put'])){
      // We check for the presence of the PECL http library, if not present, we
      // throw an error.
      if(!function_exists('http_put_file')){throw new GBIFRegistryError($this->translate('PECL HTTP not installed, please pass alternatives to the constructor.'));}
    }
  }

  /**
   * Allows for defining of alternative functions to be called for HTTP 
   * operations.
   * 
   * @param Array $alternatives
   */
  public function set_alternative_functions($alternatives){
    $this->alternative_functions = array_merge($this->alternative_functions, $alternatives);
  }

  /**
   * @return the $alternative_functions
   */
  public function get_alternative_functions(){
    return $this->alternative_functions;
  }

  /**
   * @return the $registry_url
   */
  public function get_registry_url(){
    return $this->registry_url;
  }

  /**
   * @param field_type $registry_url
   */
  public function set_registry_url($registry_url){
    $this->registry_url = $registry_url;
  }

  // -- NODES ------------------------------------------------------------------
  // FIXME - As the Scratchpads are unlikely to create new nodes, we probably
  // don't need this code yet.  This can be worked on in the future.
  /**
   * Simple function to get a list of all organisations
   */
  function get_nodes(){
    try{
      return json_decode($this->get('node.json'), TRUE);
    }
    catch(Exception $e){
      // Throw our own error here.
      throw new GBIFRegistryError($this->translate('Unable to decode the "node" JSON'));
    }
  }

  // -- ORGANISATIONS ----------------------------------------------------------
  /**
   * Simple function to get a list of all organisations
   */
  function get_organisations(){
    try{
      return json_decode($this->get('organisation.json'), TRUE);
    }
    catch(Exception $e){
      // Throw our own error here.
      throw new GBIFRegistryError($this->translate('Unable to decode the "organisations" JSON'));
    }
  }

  /**
   * Simple function to return a list of organisations for a particular node.
   */
  function get_organisations_by_node($node_key){
    if(!$node_key){throw new GBIFRegistryError($this->translate('You must provide a node key.'));}
    try{
      return json_decode($this->get('organisation.json?nodeKey=' . $node_key), TRUE);
    }
    catch(Exception $e){
      throw new GBIFRegistryError($this->translate('Unable to decode the "organisation" JSON'));
    }
  }

  /**
   * Simple function to return a single organisation
   */
  function get_organisation($uuid = FALSE){
    if(!$uuid){throw new GBIFRegistryError($this->translate('You must provide a UUID.'));}
    try{
      return json_decode($this->get('organisation/' . $uuid . '.json'), TRUE);
    }
    catch(Exception $e){
      throw new GBIFRegistryError($this->translate('Unable to decode the "organisation" JSON'));
    }
  }

  /**
   * Simple function to return an organisation's contacts.
   */
  function get_organisation_contacts($uuid = FALSE){
    if(!$uuid){throw new GBIFRegistryError($this->translate('You must provide a UUID.'));}
    try{
      return json_decode($this->get('organisation/' . $uuid . '.json?op=contacts'), TRUE);
    }
    catch(Exception $e){
      throw new GBIFRegistryError($this->translate('Unable to decode the "organisation" JSON'));
    }
  }

  /**
   * Set the organisation key/password to be used for requests that require
   * authentication.
   */
  function set_organisation($key, $pass){}

  // -- RESOURCES --------------------------------------------------------------
  function create_resource($resource){
    // We check the integrity of the data before posting it.
  }

  /**
   * Wrapper for the http_put_file function.  This allows us to simplify the
   * call, and also allows us to use an alternative function (CURL, etc) if 
   * required.
   * 
   * @param String $path
   * @param String $file_path
   */
  private function put($path, $file){
    if(isset($this->alternative_functions['put']) && function_exists($this->alternative_functions['put'])){
      return call_user_func($this->alternative_functions['put'], $this->registry_url . $path, $file);
    }else{
      return http_put_file($this->registry_url . $path, $file);
    }
  }

  /**
   * Wrapper function for file_get_contents, allowing us to use an alternative
   * function if required.
   */
  private function get($path){
    if(isset($this->alternative_functions['put']) && function_exists($this->alternative_functions['put'])){
      return call_user_func($this->alternative_functions['put'], $this->registry_url . $path, $file);
    }else{
      return file_get_contents($this->registry_url . $path);
    }
  }

  /**
   * Simple function to allow us to translate text.  Note, this is included to
   * work nicely with Drupal.
   */
  function translate($untranslated_text){
    if(isset($this->alternative_functions['translate']) && function_exists($this->alternative_functions['translate'])){
      call_user_func($this->alternative_functions['translate'], $untranslated_text);
    }else{
      return $untranslated_text;
    }
  }
}

class GBIFRegistryError extends Exception{

  public function __construct($message, $code, $previous){
    return parent::__construct($message, $code, $previous);
  }
}