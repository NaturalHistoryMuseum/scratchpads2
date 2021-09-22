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

class Installations extends Iterator{

  protected $supportsPaging = TRUE;
  // We set a default path of "organization" for simplicity as this class can be
  // called direct from the Registry class.
  protected $path = 'installation';

  public function __construct($path = FALSE, $tag = FALSE, $name = FALSE, $isoCountryCode = FALSE){
    if($path){
      $this->path = $path;
    }
    $parameters = array();
    if($tag){
      $parameters['tag'] = $tag;
    }
    if($name){
      $parameters['name'] = $name;
    }
    if($isoCountryCode){
      $parameters['isoCountryCode'] = $isoCountryCode;
    }
    parent::__construct($this->path, $parameters);
  }

  protected function getResults(){
    parent::getResults();
    // Once the parent function has run, we need to convert the array values in
    // $results to "Node" objects. Note, this starts from the beggining EVERY
    // time, and could be an issue for efficiency.
    foreach($this->results as $key => $value){
      if(gettype($value) !== 'object'){
        $this->results[$key] = new Installation($value);
      }
    }
  }
}
