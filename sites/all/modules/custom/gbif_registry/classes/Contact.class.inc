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

class Contact extends RegistryChild {

  protected $key;

  protected $type;

  protected $primary;

  protected $networkEntityKey;

  protected $position;

  protected $firstName;

  protected $lastName;

  protected $email;

  protected $phone;

  protected $address;

  protected $parent;

  public function __construct($data = FALSE, $parent){
    $this->parent = $parent;
    if($data){
      foreach($data as $key => $value){
        $this->{$key} = $value;
      }
    }
  }

  public function __toString(){
    if($return_string = trim("{$this->firstName} {$this->lastName} {$this->email}")){
      return $return_string;
    }else{
      return "Contact ID: {$this->id}";
    }
  }
  
  public function getURLPrefix(){
    return 'contact';
  }
}