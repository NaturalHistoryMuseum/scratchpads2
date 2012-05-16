<?php
/**
 * Standard PHP class for interacting with the GBIF registry.
 *
 * Copyright (C) 2012 Simon David Rycroft
 * @author s.rycroft@nhm.ac.uk
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
namespace GBIF;

/**
 * The Registry class is the central access point to all other GBIF Registy
 * functions.
 * This class provides the functions for accessing the registry, and
 * also provides the authentication methods.
 *
 * This class should be created like:
 *
 * $gbif_registry = new GBIF\Registry();
 *
 * @author simor
 */
class Registry{
  // Note, the registryURL MUST HAVE A TRAILING SLASH.
  // protected $registryURL = 'http://gbrds.gbif.org/registry/';
  protected $registryURL = 'http://localhost:8080/registry-ws/';

  protected $alternativeFunctions = array();

  protected $organisationKey, $organisationPassword;

  public function __construct($alternatives = FALSE){
    // Set alternatives if we've been sent any.
    if($alternatives){
      $this->setAlternativeFunctions($alternatives);
    }
    // Check we have a valid "put" function, if not, we throw an error.
    if(!isset($this->alternatives['put']) || !function_exists($this->alternatives['put'])){
      // We check for the presence of the PECL http library, if not present, we
      // throw an error.
      if(function_exists('http_put_file')){
        // We were unable to set the alternatives.
        $this->alternativeFunctions = array();
      }else{
        throw new Exception($this->translate('PECL HTTP not installed, please pass alternatives to the constructor.'));
      }
    }
  }

  /**
   * Return the current set Registry URL.
   *
   * @return the $registryURL
   */
  public function getRegistryURL(){
    return $this->registryURL;
  }

  /**
   *
   * @param field_type $registryURL          
   */
  public function setRegistryURL($registryURL){
    // Ensure that the URL we're setting has a trailing slash, else we throw an
    // error.
    if(substr($registryURL, -1) != '/'){throw new Exception($this->translate('Registry URL must have a trailing slash'));}
    $this->registryURL = $registryURL;
  }

  /**
   * Simple function that returns a GBIF\Nodes object that can then be itterated
   * over to potentially return a list of all nodes.
   */
  public function getNodes(){
    return new Nodes($this);
  }

  /**
   * Similar to the getNodes function, but this returns organizations.
   */
  public function getOrganizations(){
    return new Organizations($this);
  }

  /**
   * Allows for defining of alternative functions to be called for HTTP
   * operations.
   *
   * @param Array $alternatives          
   */
  public function setAlternativeFunctions($alternatives, $clear = FALSE){
    $this->alternativeFunctions = array_merge($this->alternativeFunctions, $alternatives);
  }

  /**
   * Wrapper for the http_put_file function.
   * This allows us to simplify the
   * call, and also allows us to use an alternative function (CURL, etc) if
   * required.
   *
   * @param String $path          
   * @param String $file_path          
   */
  public function put($path, $file){
    if(isset($this->alternativeFunctions['put']) && function_exists($this->alternativeFunctions['put'])){
      return call_user_func($this->alternativeFunctions['put'], $this->registryURL . $path, $file);
    }else{
      return http_put_file($this->registryURL . $path, $file);
    }
  }

  /**
   * Wrapper function for file_get_contents, allowing us to use an alternative
   * function if required.
   */
  public function get($path){
    try{
      if(isset($this->alternativeFunctions['put']) && function_exists($this->alternativeFunctions['put'])){
        return call_user_func($this->alternativeFunctions['put'], $this->registryURL . $path, $file);
      }else{
        if($html = @file_get_contents($this->registryURL . $path)){
          return $html;
        }else{
          return '[]';
        }
      }
    }
    catch(Exception $e){
      // Possibly 404 error. We should probably check!
      // We return "[]" because when decoded it is an empty array.
      return '[]';
    }
  }

  /**
   * Simple wrapper to the get function that returns a json_decoded'd array.
   */
  public function get_json($path){
    return json_decode($this->get($path), TRUE);
  }

  /**
   * Simple function to allow us to translate text.
   * Note, this is included to
   * work nicely with Drupal.
   */
  public function translate($untranslated_text){
    if(isset($this->alternativeFunctions['translate']) && function_exists($this->alternativeFunctions['translate'])){
      call_user_func($this->alternativeFunctions['translate'], $untranslated_text);
    }else{
      return $untranslated_text;
    }
  }
}