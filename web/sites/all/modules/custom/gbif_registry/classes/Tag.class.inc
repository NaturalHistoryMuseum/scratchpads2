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

class Tag extends RegistryChild{

  protected $predicate;

  protected $networkEntityKey;

  public function __construct($data = FALSE){
    if($data){
      foreach($data as $key => $value){
        switch($key){
          case 'predicate':
          case 'networkEntityKey':
            $this->{$key} = $value;
            break;
        }
      }
    }
  }

  public function __toString(){
    return "predicate={$this->predicate} networkEntityKey={$this->networkEntityKey}";
  }
}