<?php

class PHPExcel_CachedObjectStorageFactoryExtension extends PHPExcel_CachedObjectStorageFactory{

  const cache_to_sqlite3file = 'SQLite3File';

  private static $_storageMethods = array(
    self::cache_in_memory,
    self::cache_in_memory_gzip,
    self::cache_in_memory_serialized,
    self::cache_igbinary,
    self::cache_to_phpTemp,
    self::cache_to_discISAM,
    self::cache_to_apc,
    self::cache_to_memcache,
    self::cache_to_wincache,
    self::cache_to_sqlite,
    self::cache_to_sqlite3,
    self::cache_to_sqlite3file
  );

  private static $_storageMethodDefaultParameters = array(
    self::cache_in_memory => array(),
    self::cache_in_memory_gzip => array(),
    self::cache_in_memory_serialized => array(),
    self::cache_igbinary => array(),
    self::cache_to_phpTemp => array(
      'memoryCacheSize' => '1MB'
    ),
    self::cache_to_discISAM => array(
      'dir' => NULL
    ),
    self::cache_to_apc => array(
      'cacheTime' => 600
    ),
    self::cache_to_memcache => array(
      'memcacheServer' => 'localhost',
      'memcachePort' => 11211,
      'cacheTime' => 600
    ),
    self::cache_to_wincache => array(
      'cacheTime' => 600
    ),
    self::cache_to_sqlite => array(),
    self::cache_to_sqlite3 => array(),
    self::cache_to_sqlite3file => array()
  );

  /**
   * Return the list of all possible cache storage methods
   *
   * @return string[]
   */
  public static function getAllCacheStorageMethods(){
    return self::$_storageMethods;
  }

  /**
   * Return the list of all available cache storage methods
   *
   * @return string[]
   */
  public static function getCacheStorageMethods(){
    $activeMethods = array();
    foreach(self::$_storageMethods as $storageMethod){
      $cacheStorageClass = 'PHPExcel_CachedObjectStorage_' . $storageMethod;
      if(call_user_func(array(
        $cacheStorageClass,
        'cacheMethodIsAvailable'
      ))){
        $activeMethods[] = $storageMethod;
      }
    }
    return $activeMethods;
  }

  /**
   * Identify the cache storage method to use
   *
   * @param	string			$method		Name of the method to use for cell cacheing
   * @param	array of mixed	$arguments	Additional arguments to pass to the cell caching class
   *										when instantiating
   * @return boolean
   */
  public static function initialize($method = self::cache_in_memory, $arguments = array()){
    if(!in_array($method, self::$_storageMethods)){return FALSE;}
    $cacheStorageClass = 'PHPExcel_CachedObjectStorage_' . $method;
    if(!call_user_func(array(
      $cacheStorageClass,
      'cacheMethodIsAvailable'
    ))){return FALSE;}
    self::$_storageMethodParameters[$method] = self::$_storageMethodDefaultParameters[$method];
    foreach($arguments as $k => $v){
      if(isset(self::$_storageMethodParameters[$method][$k])){
        self::$_storageMethodParameters[$method][$k] = $v;
      }
    }
    if(self::$_cacheStorageMethod === NULL){
      self::$_cacheStorageClass = 'PHPExcel_CachedObjectStorage_' . $method;
      self::$_cacheStorageMethod = $method;
    }
    return TRUE;
  }

  /**
   * Initialise the cache storage
   *
   * @param	PHPExcel_Worksheet 	$parent		Enable cell caching for this worksheet
   * @return	PHPExcel_CachedObjectStorage_ICache
   */
  public static function getInstance(PHPExcel_Worksheet $parent){
    $cacheMethodIsAvailable = TRUE;
    if(self::$_cacheStorageMethod === NULL){
      $cacheMethodIsAvailable = self::initialize();
    }
    if($cacheMethodIsAvailable){
      $instance = new self::$_cacheStorageClass($parent, self::$_storageMethodParameters[self::$_cacheStorageMethod]);
      if($instance !== NULL){return $instance;}
    }
    return FALSE;
  }
}