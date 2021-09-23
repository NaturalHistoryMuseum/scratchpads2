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

class Iterator extends RegistryChild implements \Iterator{

  protected $position = 0;
  // Set the default limit. By us setting it, we won't receive any nasty
  // surprises if the default on the service is changed.
  protected $resultsPerPage = 20;
  // Store the most recent result of the call to the webservice. Common sense!
  protected $results = array();
  // Store whether or not the previous data call should be the last.
  protected $endReached = FALSE;
  // The path used to access the results.
  protected $path;
  // Some of the classes support paging, some don't. The child class should
  // override this variable if they support paging.
  protected $supportsPaging = FALSE;
  // Allow the provision of parameters
  protected $parameters = array();
  // Allow us to start from a set point rather than from the start.
  protected $offset;

  public function __construct($path, $parameters = array()){
    parent::__construct();
    $this->parameters = $parameters;
    $this->path = $path;
  }

  public function rewind(){
    $this->position = 0 + $this->offset;
  }

  public function setOffset($offset){
    $this->offset = $offset;
  }

  public function current(){
    return $this->results[$this->position];
  }

  public function key(){
    return $this->position;
  }

  public function next(){
    $this->position++;
  }

  public function valid(){
    if(isset($this->results[$this->position])){return TRUE;}
    $this->getResults();
    return isset($this->results[$this->position]);
  }

  protected function getResults(){
    // We get the results from the "paged" service, and rekey them depending on
    // the offset.
    if(!$this->endReached){
      $parameters['offset'] = $this->position;
      $parameters['limit'] = $this->resultsPerPage;
      $data = Registry::singleton()->get_json($this->path, $parameters);
      $i = $this->position;
      if($this->supportsPaging && isset($data['endOfRecords'])){
        $this->endReached = $data['endOfRecords'];
        if(is_array($data['results'])){
          foreach($data['results'] as $result){
            $this->results[$i] = $result;
            $i++;
          }
        }
      }else{
        // We don't use paging for this object type.
        $this->endReached = TRUE;
        foreach($data as $result){
          $this->results[$i] = $result;
          $i++;
        }
      }
    }
  }
}