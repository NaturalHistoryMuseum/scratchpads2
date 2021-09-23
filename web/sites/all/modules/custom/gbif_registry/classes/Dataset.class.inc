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

class Dataset extends RegistryObject {
  
  /**
   * @return the $urlprefix
   */
  public function getURLPrefix(){
    return 'dataset';
  }

  /**
   *
   * @return the $description
   */
  public function getDescription(){
    return $this->description;
  }

  /**
   *
   * @return the $homepage
   */
  public function getHomepage(){
    return $this->homepage;
  }

  /**
   *
   * @return the $key
   */
  public function getKey(){
    return $this->key;
  }

  /**
   *
   * @return the $title
   */
  public function getTitle(){
    return $this->title;
  }

  /**
   *
   * @return the $type
   */
  public function getType(){
    return $this->type;
  }

  /**
   *
   * @param field_type $description          
   */
  public function setDescription($description){
    $this->description = $description;
  }

  /**
   *
   * @param field_type $homepage          
   */
  public function setHomepage($homepage){
    $this->homepage = $homepage;
  }

  /**
   *
   * @param field_type $key          
   */
  public function setKey($key){
    $this->key = $key;
  }

  /**
   *
   * @param field_type $title          
   */
  public function setTitle($title){
    $this->title = $title;
  }

  /**
   *
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
  public function getContacts(){
    return new Contacts("dataset/{$this->key}/contact");
  }

  /**
   * Request GBIF to re-ingest the dataset
   *
   * @return bool True on success, false on error
   */
  public function requestCrawl(){
    $crawlEndpoint = "dataset/{$this->key}/crawl";
    return Registry::singleton()->post($crawlEndpoint);
  }

  public function __toString(){
    return "{$this->key} - {$this->title}";
  }
}
