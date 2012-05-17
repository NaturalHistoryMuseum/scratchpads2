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

class RegistryObject extends RegistryChild{
  // String properties
  protected $address;

  protected $city;

  protected $country;

  protected $description;

  protected $email;

  protected $endorsingNode;

  protected $homepage;

  protected $key;

  protected $language;

  protected $latitude;

  protected $logoURL;

  protected $longitude;

  protected $phone;

  protected $tags;

  protected $title;

  protected $zip;

  protected $technicalInstallations = array();

  protected $contacts = array();

  protected $endpoints = array();

  protected $identifiers = array();

  protected $organizations = FALSE;

  public function __construct(Registry $reg, $data = FALSE){
    parent::__construct($reg);
    // If we passed a string, we assume it's the ID of a node.
    if(is_string($data)){
      $data = $reg->get_json($this->getURLPrefix() . "/$data");
    }
    // If we passed an array, we assume we're building this object from an
    // "Objects" query.
    if(is_array($data)){
      foreach($data as $key => $value){
        switch($key){
          default:
            if(is_string($value)){
              $this->{$key} = $value;
            }else{
              // FIXME - Remove this code once complete.
              echo "$key\n";
            }
            break;
          case 'endorsingNode':
            // We get a complex object here, but we only actually need to save
            // the key, so that's all we're saving.
            if(isset($value['key'])){
              $this->endorsingNode = $value['key'];
            }
            break;
          case 'latitude':
          case 'longitude':
            $this->{$key} = $value;
            break;
          case 'technicalInstallations':
          case 'contacts':
          case 'endpoints':
          case 'identifiers':
          case 'tags':
            // We ignore the contacts/endpoints/identifiers/tags, and rely on
            // the /[node|organization|...]/<k1>/[contact|identifier|...]
            // service
            break;
          case 'language':
            $this->language = $value;
            break;
        }
      }
    }
  }

  /**
   *
   * @return the $endorsingNode
   */
  public function getEndorsingNode(){
    return $this->endorsingNode;
  }

  /**
   *
   * @return the $latitude
   */
  public function getLatitude(){
    return $this->latitude;
  }

  /**
   *
   * @return the $longitude
   */
  public function getLongitude(){
    return $this->longitude;
  }

  /**
   *
   * @return the $logoURL
   */
  public function getLogoURL(){
    return $this->logoURL;
  }

  /**
   *
   * @return the $language
   */
  public function getLanguage(){
    return $this->language;
  }

  /**
   *
   * @param field_type $endorsingNode        
   */
  public function setEndorsingNode($endorsingNode){
    $this->endorsingNode = $endorsingNode;
  }

  /**
   *
   * @param field_type $latitude        
   */
  public function setLatitude($latitude){
    $this->latitude = $latitude;
  }

  /**
   *
   * @param field_type $longitude        
   */
  public function setLongitude($longitude){
    $this->longitude = $longitude;
  }

  /**
   *
   * @param field_type $logoURL        
   */
  public function setLogoURL($logoURL){
    $this->logoURL = $logoURL;
  }

  /**
   *
   * @param field_type $language        
   */
  public function setLanguage($language){
    $this->language = $language;
  }

  /**
   * Save
   *
   * We should save the node at this point. We also need to add any contacts
   * that have been added.
   *
   * FIXME - This does not yet function
   */
  public function save(){
    ;
  }

  /**
   * DELETE
   *
   * The RegistryObject is deleted. We should destroy this object at this point
   * too.
   */
  function delete(){
    $this->registry->delete($this->getURLPrefix() . "/{$this->key}");
  }

  /**
   */
  public function getIdentifiers(){
    return new Identifiers($this->registry, $this->getURLPrefix() . "/{$this->key}/identifier");
  }

  /**
   */
  public function getTags(){
    return new Tags($this->registry, $this->getURLPrefix() . "/{$this->key}/tag");
  }

  /**
   */
  public function getEndpoints(){
    return new Endpoints($this->registry, $this->getURLPrefix() . "/{$this->key}/endpoint");
  }

  /**
   */
  public function getContacts(){
    return new Contacts($this->registry, $this->getURLPrefix() . "/{$this->key}/contact");
  }

  /**
   */
  public function getTechnicalInstallations(){
    return new TechnicalInstallations($this->registry, $this->getURLPrefix() . "/{$this->key}/technical_installation");
  }

  /**
   * Return the default URL prefix, which we actually set to be blank.
   */
  public function getURLPrefix(){
    return '';
  }

  /**
   * Sets the contacts for this Node.
   *
   * FIXME - Need to check that this is possible, it doesn't appear to be given
   * that removing a contact actually involves deleting it (really?).
   * public function setContacts($contacts){
   * $this->contacts = $contacts;
   * }
   */
  /**
   * Add a single contact to this node.
   *
   * FIXME - Nothing is done here.
   */
  public function addContact(Contact $contact){
    $this->contacts[] = $contact;
  }

  /**
   * Gets the organizations endorsed by this node.
   */
  public function getOrganizations(){
    return new Organizations($this->registry, "node/{$this->key}/organization");
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
   * @return the $country
   */
  public function getCountry(){
    return $this->country;
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
   * @return the $description
   */
  public function getDescription(){
    return $this->description;
  }

  /**
   *
   * @return the $address
   */
  public function getAddress(){
    return $this->address;
  }

  /**
   *
   * @return the $city
   */
  public function getCity(){
    return $this->city;
  }

  /**
   *
   * @return the $phone
   */
  public function getPhone(){
    return $this->phone;
  }

  /**
   *
   * @return the $email
   */
  public function getEmail(){
    return $this->email;
  }

  /**
   *
   * @return the $zip
   */
  public function getZip(){
    return $this->zip;
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
   * @param field_type $country        
   */
  public function setCountry($country){
    $this->country = $country;
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
   * @param field_type $description        
   */
  public function setDescription($description){
    $this->description = $description;
  }

  /**
   *
   * @param field_type $address        
   */
  public function setAddress($address){
    $this->address = $address;
  }

  /**
   *
   * @param field_type $city        
   */
  public function setCity($city){
    $this->city = $city;
  }

  /**
   *
   * @param field_type $phone        
   */
  public function setPhone($phone){
    $this->phone = $phone;
  }

  /**
   *
   * @param field_type $email        
   */
  public function setEmail($email){
    $this->email = $email;
  }

  /**
   *
   * @param field_type $zip        
   */
  public function setZip($zip){
    $this->zip = $zip;
  }
}