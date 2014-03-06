<?php
namespace ScratchpadsVocabularies;

/**
 * Persistent
 * 
 * This class provides functionality for an object to save some
 * of its properties to database, and restore them when needed.
 * 
 * Typical use would be for a class to extend this class. The extending
 * class should then call:
 * 
 *  - 'persistentKey' to set the key that will be used in the database.
 *     This must be unique ;
 *  - 'persistentProperties' to define which properties should be saved to the
 *     database ;
 *  - 'saveProperties' to save the properties to the database ;
 *  - 'loadProperties' to restore the properties from the database.
 *  
 *  This will avoid loops when loading/saving properties that include
 *  an object with the same persistent key.
 *  
 *  This implements __sleep and __wakup, such that:
 *  - savePersistentProperties is called when the object is serialized, and only
 *    the persistentKey and the list of persistent properties are serialized ;
 *  - loadPersistentProperties is called on unserialize ;
 *    
 */
class Persistent{

  private static $saving = array();

  private static $loading = array();

  protected $persistentProperties = array();

  protected $persistentKey = NULL;

  /**
   * Set/get the database unique key for this object.
   *
   * Note that this must be unique amongst all classes.
   * To facilitate this, if $prefix is TRUE, then the key
   * will be prefixed with the class name of $this.
   * 
   * The total size of the key must be less than 255 chars
   * (see the schema).
   *
   * This returns the value of the persistent key.
   */
  protected function persistentKey($k = NULL, $prefix = TRUE){
    if($k !== NULL){
      if($prefix){
        $this->persistentKey = get_class($this) . '::' . $k;
      }else{
        $this->persistentKey = $k;
      }
      $this->persistentKey = trim($this->persistentKey);
    }
    return $this->persistentKey;
  }

  /**
   * Adds properties that should be considered
   * persistent, and saved/loaded from the
   * database. Note that these must be protected
   * or public, private properties will not be saved.
   */
  protected function persistentProperties($props = array()){
    $this->persistentProperties = array_merge($this->persistentProperties, $props);
  }

  /**
   * Save the persistent properties to the database
   */
  protected function savePersistentProperties(){
    if(!$this->persistentKey){return;}
    if(!empty(Persistent::$saving[$this->persistentKey])){return;}
    Persistent::$saving[$this->persistentKey] = TRUE;
    $data = array();
    foreach($this->persistentProperties as $name){
      if(property_exists($this, $name)){
        $data[$name] = $this->{$name};
      }
    }
    $record = array(
      'pid' => $this->persistentKey,
      'data' => serialize($data)
    );
    $exists = db_query("SELECT 1 FROM {scratchpads_vocabularies} WHERE pid = :pid", array(
      ':pid' => $this->persistentKey
    ))->fetchField();
    if($exists){
      drupal_write_record('scratchpads_vocabularies', $record, 'pid');
    }else{
      drupal_write_record('scratchpads_vocabularies', $record);
    }
    unset(Persistent::$saving[$this->persistentKey]);
  }

  /**
   * Load persistent properties from the database. Note that the loaded properties
   * are those currently defined by persistentProperties, not those that were
   * defined when the save occured.
   */
  protected function loadPersistentProperties(){
    if(!$this->persistentKey){return;}
    if(isset(Persistent::$loading[$this->persistentKey])){
      Persistent::$loading[$this->persistentKey][] = $this;
      return;
    }
    Persistent::$loading[$this->persistentKey] = array();
    $row = db_query("
      SELECT data
        FROM {scratchpads_vocabularies}
       WHERE pid = :pid
    ", array(
      ':pid' => $this->persistentKey
    ))->fetchField();
    if($row){
      $data = unserialize($row);
      foreach($this->persistentProperties as $name){
        if(isset($data[$name])){
          $this->{$name} = $data[$name];
        }
      }
    }
    foreach(Persistent::$loading[$this->persistentKey] as $obj){
      $obj->copyPersistentProperties($this);
      if(method_exists($obj, '__wakeup')){
        $obj->__wakeup();
      }
    }
    unset(Persistent::$loading[$this->persistentKey]);
  }

  /**
   * copyPersistentProperties()
   * 
   * Copy the persistent properties from the given object
   */
  protected function copyPersistentProperties($source){
    foreach($this->persistentProperties as $name){
      if(isset($source->{$name})){
        $this->{$name} = $source->{$name};
      }
    }
  }

  /**
   * persistePropertiesExist
   * 
   * Return TRUE if there is already an entry for this field in the persistent database
   */
  public function persistentPropertiesExist(){
    $exists = db_query("SELECT 1 FROM {scratchpads_vocabularies} WHERE pid = :pid", array(
      ':pid' => $this->persistentKey
    ))->fetchField();
    return $exists ? TRUE : FALSE;
  }

  /**
   * deletePersistentProperties
   * 
   * Delete existing persistent properties for this object
   */
  public function deletePersistentProperties(){
    db_query("DELETE FROM {scratchpads_vocabularies} WHERE pid = :pid", array(
      ':pid' => $this->persistentKey
    ));
  }

  /**
   * __sleep
   * 
   * Ensure only the persistentKey is serialized
   */
  public function __sleep(){
    $this->savePersistentProperties();
    return array(
      'persistentKey',
      'persistentProperties'
    );
  }

  /**
   * __wakeup
   */
  public function __wakeup(){
    $this->loadPersistentProperties();
  }
}