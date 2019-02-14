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

class RegistryObject extends RegistryChild
{
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

    protected $hostingOrganization;

    protected $contacts = array();
  // Endpoints can be associated with a node/organization/installation
    protected $endpoints = array();

    protected $identifiers = array();

    public function __construct($data = false)
    {
        parent::__construct();
    // If we passed a string, we assume it's the ID of a node.
        if (is_string($data)) {
            $data = Registry::singleton()->get_json($this->getURLPrefix() . "/$data");
        }
    // If we passed an array, we assume we're building this object from an
    // "Objects" query.
        if (is_array($data)) {
            foreach ($data as $key => $value) {
                switch ($key) {
                    default:
                        $this->{$key} = $value;
                        break;
                    case 'hostingOrganizationKey':
                        $this->hostingOrganization = $value;
                        $this->hostingOrganizationKey = $value;
                        break;
                    case 'hostingOrganization':
                    case 'endorsingNode':
            // We get a complex object here, but we only actually need to save
            // the key, so that's all we're saving.
                        if (isset($value['key'])) {
                            $this->{$key} = $value['key'];
                            $key = $key . 'Key';
                            $this->{$key} = $value['key'];
                        }
                        break;
                    case 'installations':
                    case 'contacts':
                    case 'endpoints':
                    case 'identifiers':
            // We ignore the contacts/endpoints/identifiers, and rely on
            // the /[node|organization|...]/<k1>/[contact|identifier|...]
            // service
                        break;
                }
            }
        }
    }

    /**
     *
     * @return the $endorsingNode
     */
    public function getEndorsingNode()
    {
        return $this->endorsingNode;
    }

    /**
     *
     * @return the $latitude
     */
    public function getLatitude()
    {
        return $this->latitude;
    }

    /**
     *
     * @return the $longitude
     */
    public function getLongitude()
    {
        return $this->longitude;
    }

    /**
     *
     * @return the $logoURL
     */
    public function getLogoURL()
    {
        return $this->logoURL;
    }

    /**
     *
     * @return the $language
     */
    public function getLanguage()
    {
        return $this->language;
    }

    /**
     *
     * @param field_type $endorsingNode
     */
    public function setEndorsingNode($endorsingNode)
    {
        $this->endorsingNode = $endorsingNode;
    }

    /**
     *
     * @param field_type $latitude
     */
    public function setLatitude($latitude)
    {
        $this->latitude = $latitude;
    }

    /**
     *
     * @param field_type $longitude
     */
    public function setLongitude($longitude)
    {
        $this->longitude = $longitude;
    }

    /**
     *
     * @param field_type $logoURL
     */
    public function setLogoURL($logoURL)
    {
        $this->logoURL = $logoURL;
    }

    /**
     *
     * @param field_type $language
     */
    public function setLanguage($language)
    {
        $this->language = $language;
    }

    /**
     * Save.
     *
     * This saves a basic Registry object, and adds the children to it.
     */
    public function save()
    {
        if ($this->key) {
      // A key has already been set. This object already exists on the server,
      // and therefore just needs updating.
            $current_data = Registry::singleton()->get_json($this->getURLPrefix() . "/{$this->key}");
            foreach ($this->getAsArray() as $key => $value) {
                $current_data[$key] = $value;
            }
            $f = tmpfile();
            fwrite($f, json_encode($current_data));
            fseek($f, 0);
            Registry::singleton()->put($this->getURLPrefix() . "/{$this->key}", $f);
            unlink($filename);
        } else {
      // No key, we simply need to create a new object, and save the key.

            $this->key = json_decode(Registry::singleton()->post($this->getURLPrefix(), json_encode($this->getAsArray())), true);
            if (!$this->key) {
                // An error occurred. PANIC!
                throw new Exception('Unable to save object');
            }
        }
    }

    /**
     * DELETE
     *
     * The RegistryObject is deleted. We should destroy this object at this point
     * too.
     */
    function delete()
    {
        Registry::singleton()->delete($this->getURLPrefix() . "/{$this->key}");
    }

    /**
     */
    public function getIdentifiers()
    {
        return new Identifiers($this->getURLPrefix() . "/{$this->key}/identifier");
    }

    /**
     */
    public function getTags()
    {
        return new Tags($this->getURLPrefix() . "/{$this->key}/tag");
    }

    /**
     */
    public function getEndpoints()
    {
        return new Endpoints($this->getURLPrefix() . "/{$this->key}/endpoint");
    }

    /**
     */
    public function getContacts()
    {
        return new Contacts($this->getURLPrefix() . "/{$this->key}/contact");
    }

    /**
     * Return the default URL prefix, which we actually set to be blank.
     */
    public function getURLPrefix()
    {
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
     */
    public function addContact($data)
    {
        $contact = new Contact($data, $this);
        $contact->save();
        $this->contacts[] = $contact;
    }

    /**
     * Add a single endpoint to this object.
     */
    public function addEndpoint($data)
    {
        $endpoint = new Endpoint($data, $this);
        $endpoint->save();
        $this->endpoints[] = $endpoint;
    }

    /**
     * Gets the organizations endorsed by this node.
     */
    public function getHostingOrganization()
    {
        return new Organization($this->hostingOrganization);
    }

    /**
     *
     * @return the $key
     */
    public function getKey()
    {
        return $this->key;
    }

    /**
     *
     * @return the $title
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     *
     * @return the $country
     */
    public function getCountry()
    {
        return $this->country;
    }

    /**
     *
     * @return the $homepage
     */
    public function getHomepage()
    {
        return $this->homepage;
    }

    /**
     *
     * @return the $description
     */
    public function getDescription()
    {
        return $this->description;
    }

    /**
     *
     * @return the $address
     */
    public function getAddress()
    {
        return $this->address;
    }

    /**
     *
     * @return the $city
     */
    public function getCity()
    {
        return $this->city;
    }

    /**
     *
     * @return the $phone
     */
    public function getPhone()
    {
        return $this->phone;
    }

    /**
     *
     * @return the $email
     */
    public function getEmail()
    {
        return $this->email;
    }

    /**
     *
     * @return the $zip
     */
    public function getZip()
    {
        return $this->zip;
    }

    /**
     *
     * @param field_type $key
     */
    public function setKey($key)
    {
        $this->key = $key;
    }

    /**
     *
     * @param field_type $title
     */
    public function setTitle($title)
    {
        $this->title = $title;
    }

    /**
     *
     * @param field_type $country
     */
    public function setCountry($country)
    {
        $this->country = $country;
    }

    /**
     *
     * @param field_type $homepage
     */
    public function setHomepage($homepage)
    {
        $this->homepage = $homepage;
    }

    /**
     *
     * @param field_type $description
     */
    public function setDescription($description)
    {
        $this->description = $description;
    }

    /**
     *
     * @param field_type $address
     */
    public function setAddress($address)
    {
        $this->address = $address;
    }

    /**
     *
     * @param field_type $city
     */
    public function setCity($city)
    {
        $this->city = $city;
    }

    /**
     *
     * @param field_type $phone
     */
    public function setPhone($phone)
    {
        $this->phone = $phone;
    }

    /**
     *
     * @param field_type $email
     */
    public function setEmail($email)
    {
        $this->email = $email;
    }

    /**
     *
     * @param field_type $zip
     */
    public function setZip($zip)
    {
        $this->zip = $zip;
    }
}