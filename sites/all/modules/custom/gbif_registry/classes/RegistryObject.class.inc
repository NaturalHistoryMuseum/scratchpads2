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

class RegistryObject extends RegistryChild{
  // String properties
  // FIXME - A lot of these properties will need moving to the Node class. We
  // need to find what overlap there is with other objects.
  protected $key;

  protected $title;

  protected $country;

  protected $homepage;

  protected $description;

  protected $address;

  protected $city;

  protected $phone;

  protected $email;

  protected $zip;

  protected $contacts = array();

  protected $extra_data = array();

  protected $endpoints = array();

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
        echo $key."\n";
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
            // the /[node|organization|...]/<k1>/[contact|identifier|...]
            // service
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