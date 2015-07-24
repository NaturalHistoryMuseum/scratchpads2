<?php
/**
 * Standard PHP class for interacting with the GBIF registry.
 *
 * Copyright (C) 2012 Simon David Rycroft
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @author s.rycroft@nhm.ac.uk
 */
namespace GBIF;

class Organization extends RegistryObject{

  protected $installations = array();

  final public function getURLPrefix(){
    return 'organization';
  }

  /**
   * Get the datasets
   */
  function getDatasets($type = 'hosted'){
    if($type == 'hosted' || $type == 'owned'){
      return new Datasets($this->getURLPrefix() . "/{$this->key}/dataset/$type");
    }else{
      // Throw an error.
    }
  }

  /**
   * Get the installations.
   *
   * @return \GBIF\Installations
   */
  public function getInstallations(){
    return new Installations($this->getURLPrefix() . "/{$this->key}/installation");
  }

  /**
   * Add a single installation.
   *
   * @return \GBIF\Installation
   */
  public function addInstallation($ti){
    // Ensure we have been sent an array.
    if(!is_array($ti)){return FALSE;}
    // Set the key to this organization's key.
    $ti['organizationKey'] = $this->getKey();
    $ti = new Installation($ti);
    $ti->save();
    return $ti;
  }

  /**
   * Get the graph of the object
   */
  function getGraph($get_png = FALSE){
    $parameters = array();
    if($get_png){
      $parameters['format'] = 'png';
    }
    return Registry::singleton()->get($this->getURLPrefix() . "/{$this->key}/graph", $parameters);
  }
}
