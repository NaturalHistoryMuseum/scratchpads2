<?php
/**
 * Standard PHP class for interacting with the GBIF registry.
 *
 * Copyright (C) 2012 Simon David Rycroft
 *
 * @author s.rycroft@nhm.ac.uk
 *        
 *         This program is free software: you can redistribute it and/or modify
 *         it under
 *         the terms of the GNU General Public License as published by the Free
 *         Software
 *         Foundation, either version 3 of the License, or (at your option) any
 *         later
 *         version.
 *        
 *         This program is distributed in the hope that it will be useful, but
 *         WITHOUT
 *         ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *         FITNESS
 *         FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *         details.
 *        
 *         You should have received a copy of the GNU General Public License
 *         along with
 *         this program. If not, see <http://www.gnu.org/licenses/>.
 */
namespace GBIF;

class Organization extends RegistryObject{

  final public function getURLPrefix(){
    return 'organization';
  }

  /**
   * Get the datasets
   */
  function getDatasets($type = 'hosted'){
    if($type == 'hosted' || $type == 'owned'){
      return new Datasets($this->registry, $this->getURLPrefix() . "/{$this->key}/dataset/$type");
    }else{
      // Throw an error.
    }
  }

  /**
   * Get the graph of the object
   */
  function getGraph($get_png = FALSE){
    return $this->registry->get($this->getURLPrefix() . "/{$this->key}/graph" . ($get_png ? '?format=png' : ''));
  }
}