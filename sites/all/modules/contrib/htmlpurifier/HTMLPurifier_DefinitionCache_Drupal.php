<?php
/**
 * @file
 * Cache handler that stores all data in drupals builtin cache
 */
require_once 'HTMLPurifier/DefinitionCache.php';

class HTMLPurifier_DefinitionCache_Drupal extends HTMLPurifier_DefinitionCache {
  /**
   * Add an object to the cache without overwriting
   */
  function add($def, $config) {
    if (!$this->checkDefType($def)) return;
    $key = $this->generateKey($config);

    if ($this->fetchFromDrupalCache($key)) {
      // already cached
      return FALSE;
    }
    $this->storeInDrupalCache($def, $key);
    return TRUE;
  }

  /**
   * Unconditionally add an object to the cache, overwrites any existing object.
   */
  function set($def, $config) {
    if (!$this->checkDefType($def)) return;
    $key = $this->generateKey($config);

    $this->storeInDrupalCache($def, $key);
    return TRUE;
  }

  /**
   * Replace an object that already exists in the cache.
   */
  function replace($def, $config) {
    if (!$this->checkDefType($def)) return;
    $key = $this->generateKey($config);

    if (!$this->fetchFromDrupalCache($key)) {
      // object does not exist in cache
      return FALSE;
    }

    $this->storeInDrupalCache($def, $key);
    return TRUE;
  }

  /**
   * Retrieve an object from the cache
   */
  function get($config) {
    $key = $this->generateKey($config);
    return $this->fetchFromDrupalCache($key);
  }

  /**
   * Delete an object from the cache
   */
  function remove($config) {
    $key = $this->generateKey($config);
    cache_clear_all("htmlpurifier:$key", 'cache');
    return TRUE;
  }

  function flush($config) {
    cache_clear_all("htmlpurifier:*", 'cache', TRUE);
    return TRUE;
  }

  function cleanup($config) {
    // TODO: This does not work with the pluggable cache system in Drupal 7,
    // since it assumes a database cache is being used.
    $res = db_query("SELECT cid FROM {cache} WHERE cid LIKE :cid", array(':cid' => 'htmlpurifier:%'));
    foreach ($res as $row) {
      $key = substr($row->cid, 13); // 13 == strlen('htmlpurifier:')
      if ($this->isOld($key, $config)) {
        cache_clear_all($row->cid, 'cache');
      }
    }
  }

  function fetchFromDrupalCache($key) {
    $cached = cache_get("htmlpurifier:$key");
    if ($cached) return unserialize($cached->data);
    return FALSE;
  }

  function storeInDrupalCache($def, $key) {
    cache_set("htmlpurifier:$key", serialize($def), 'cache', CACHE_PERMANENT);
  }

}

