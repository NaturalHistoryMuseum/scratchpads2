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
/**
 * The RegistryCache class allows us to take advantage of different caching
 * mechanisms used in different applications.
 * A great example of a class that
 * overrides this one, is that provided by the DrupalRegistryCache class present
 * in the Scratchpads code.
 *
 * Simple CRUD interface for cache items.
 *
 * @author simor
 */
class DrupalRegistryCache extends GBIF\RegistryCache{

  public function update($cid, $data){
    cache_set($cid, $data, 'cache_gbif_registry', time() + 3600);
  }

  public function delete($cid){
    cache_clear_all($cid, 'cache_gbif_registry');
  }

  public function retrieve($cid){
    $cache = cache_get($cid, 'cache_gbif_registry');
    if($cache && $cache->expire > time()){return $cache->data;}
    return FALSE;
  }
}