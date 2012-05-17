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

class Organization extends RegistryObject{

  public function __construct(Registry $reg, $data = FALSE){
    parent::__construct($reg);
    // If we passed a string, we assume it's the ID of a .
    if(is_string($data)){
      $data = $reg->get_json("node/$data");
    }
    // If we passed an array, we assume we're building this node from a
    // "Organizations"
    // query.
    if(is_array($data)){
      foreach($data as $key => $value){
        switch($key){
          case 'key':
          case 'title':
          case 'country':
          case 'homepage':
          case 'description':
          case 'address':
          case 'city':
          case 'phone':
          case 'email':
          case 'zip':
            $this->{$key} = $value;
            break;
          case 'contacts':
          case 'endpoints':
          case 'identifiers':
          case 'tags':
            // We ignore the contacts/endpoints/identifiers/tags, and rely on
            // the /organization/<k1>/x service
            break;
          case 'language':
            break;
          default:
            // Save the extra data that we don't know anything about. This will
            // allow this class to be easily extended.
            $this->extra_data[$key] = $value;
        }
      }
    }
  }

  final public function getURLPrefix(){
    return 'organization';
  }
  
  /**
   * Get the datasets
   */
  function getDatasets($type = 'hosted'){
    
  }

  /**
   * Save
   *
   * We should save the organization at this point. We also need to add any
   * contacts
   * that have been added.
   *
   * FIXME - This does not yet function
   */
  public function save(){
    ;
  }
}