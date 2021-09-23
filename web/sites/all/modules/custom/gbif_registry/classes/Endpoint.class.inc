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

class Endpoint extends RegistryChild{

  protected $id;

  protected $url;

  protected $type;
  
  protected $parent;

  public function __construct($data = FALSE, $parent){
    $this->parent = $parent;
    if($data){
      foreach($data as $key => $value){
        switch($key){
          case 'id':
          case 'type':
          case 'url':
          case 'machineTags':
            $this->{$key} = $value;
            break;
        }
      }
    }
  }
  
  public function getURLPrefix(){
    return 'endpoint';
  }

  public function getID(){
    return $this->id;
  }

  public function getURL(){
    return $this->url;
  }

  public function getType(){
    return $this->type;
  }

  public function setID($id){
    $this->id = $id;
  }

  public function setURL($url){
    $this->url = $url;
  }

  public function setType($type){
    $this->type = $type;
  }

  public function __toString(){
    return "{$this->id}: {$this->type} - {$this->url}";
  }
}