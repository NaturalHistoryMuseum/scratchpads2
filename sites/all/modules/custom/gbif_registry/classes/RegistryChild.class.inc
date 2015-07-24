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

class RegistryChild {

  public function __construct(){}

  public function getAsArray(){
    return get_object_vars($this);
  }

  public function __toString(){
    return (isset($this->key) ? $this->key . ': ' : '') . (isset($this->title) ? $this->title : '');
  }

  public function save(){
    if($this->key){
      // A key has already been set. This object already exists on the server,
      // and therefore just needs updating.
    }else{
      // No key, we simply need to create a new object, and save the key.
      $this->key = json_decode(Registry::singleton()->post($this->parent->getURLPrefix() . '/' . $this->parent->getKey() . '/' . $this->getURLPrefix(), json_encode($this->getAsArray())), TRUE);
      if(!$this->key){
        // An error occurred. PANIC!
        throw new Exception('Unable to save object');
      }
    }
  }
}
