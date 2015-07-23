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

class Dataset extends RegistryChild{

  private $bibliographicCitations;

  private $citation;

  private $contacts;

  private $curatorialUnits;

  private $dataAvailable;

  private $dataIndexable;

  private $description;

  private $endpoints;

  private $geographicCoverages;

  private $homepage;

  private $installation;

  private $installationKey;

  private $identifiers;

  private $isoCountryCodes;

  private $key;

  private $keywordCollections;

  private $keywords;

  private $language;

  private $logoURL;

  private $networkOfOrigin;

  private $owningOrganization;

  private $owningOrganizationKey;

  private $registered;

  private $tags;

  private $taxonomicCoverages;

  private $temporalCoverages;

  private $title;

  private $type;

  public function __construct($data = FALSE){
    parent::__construct();
    // If we passed a string, we assume it's the ID of a dataset.
    if(is_string($data)){
      $data = Registry::singleton()->get_json("dataset/$data");
    }
    foreach($data as $key => $value){
      switch($key){
        default:
          $this->{$key} = $value;
        case 'contacts':
        case 'endpoints':
        case 'identifiers':
        case 'tags':
          break;
      }
    }
  }

  /**
 * @return the $description
 */
public function getDescription(){
    return $this->description;
}

/**
 * @return the $homepage
 */
public function getHomepage(){
    return $this->homepage;
}

/**
 * @return the $key
 */
public function getKey(){
    return $this->key;
}

/**
 * @return the $title
 */
public function getTitle(){
    return $this->title;
}

/**
 * @return the $type
 */
public function getType(){
    return $this->type;
}

/**
 * @param field_type $description
 */
public function setDescription($description){
    $this->description = $description;
}

/**
 * @param field_type $homepage
 */
public function setHomepage($homepage){
    $this->homepage = $homepage;
}

/**
 * @param field_type $key
 */
public function setKey($key){
    $this->key = $key;
}

/**
 * @param field_type $title
 */
public function setTitle($title){
    $this->title = $title;
}

/**
 * @param field_type $type
 */
public function setType($type){
    $this->type = $type;
}

/**
   */
  public function getIdentifiers(){
    return new Identifiers("dataset/{$this->key}/identifier");
  }

  /**
   */
  public function getTags(){
    return new Tags("dataset/{$this->key}/tag");
  }

  /**
   */
  public function getEndpoints(){
    return new Endpoints("dataset/{$this->key}/endpoint");
  }

  /**
   */
  public function getContacts(){
    return new Contacts("dataset/{$this->key}/contact");
  }

  public function __toString(){
    return "{$this->key} - {$this->title}";
  }
}