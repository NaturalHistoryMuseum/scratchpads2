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

class Iterator extends RegistryChild implements \Iterator{

  protected $position = 0;
  // Set the default limit. By us setting it, we won't receive any nasty
  // surprises if the default on the service is changed.
  protected $resultsPerPage = 20;
  // Store the most recent result of the call to the webservice. Common sense!
  protected $results = FALSE;
  // Store whether or not the previous data call should be the last.
  protected $endReached = FALSE;

  protected $path;

  public function __construct(Registry $reg, $path){
    parent::__construct($reg);
    $this->path = $path;
  }

  public function rewind(){
    $this->position = 0;
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
      $data = $this->registry->get_json($this->path . '?offset=' . $this->position . '&limit=' . $this->resultsPerPage);
      $this->endReached = $data['endOfRecords'];
      $i = $this->position;
      foreach($data['results'] as $node){
        $this->results[$i] = $node;
        $i++;
      }
    }
  }
}