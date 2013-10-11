<?php
namespace ScratchpadsVocabularies;
require_once str_replace('receiver.inc', 'classes.inc', __FILE__);
require_once str_replace('receiver.inc', 'persistent.inc', __FILE__);

/**
 * VocabularyReceiver
 * 
 * This class implements ScratchpadsVocabularies\ReceiverInterface and
 * provides a default implementation for most features that will be
 * common to many receivers.
 * 
 * This class cannot be used directly, and must be extended by a
 * receiver class which must at least define:
 * - function canUpgradeStructure which returns TRUE if it is possible
 *   to upgrade to the given structure ;
 * 
 * - function structureInUse() which returns a ScratchpadsVocabularies\VocabularyStructure
 *   object representing the structure as it is currently in use on the site
 *   (which may include partial updates);
 *   
 * - function applyUpdate($update) which applies the structural part of
 *   a single update. This must update $update to reflect the update
 *   status
 *
 * - function applyModifications($update, $entities) which applies the 
 *   modification part of a single update. This must update $update to reflect 
 *   the update status
 *   
 * - function updateModifiesContent($update) as defined in the ReceiverInterface.
 * 
 * - function contentModifiedBy($update, $id) as defined in the ReceicerInterface.
 *   
 * In addition the extending class must ensure that the constructor is called
 * for the provided functionality to work.
 * 
 */
abstract class VocabularyReceiver extends Persistent implements ReceiverInterface{

  protected $name;

  protected $id;

  protected $pendingUpdates = array();

  protected $version = NULL;

  protected $cleanStructure = NULL;

  /**
   * __construct
   * 
   * Creates a new ScratchpadsVocabularies\VocabularyReceiver from a unique
   * $id and a human readable $name (if not provided, defaults
   * to $id).
   * 
   * Note that the constructor calls loadPersistentField (so any
   * additional fields must be added prior to the constructor
   * being called), and expects structureInUse() to return a valid
   * value.
   * 
   */
  public function __construct($id, $name = NULL){
    $this->id = $id;
    if(!$name){
      $this->name = $id;
    }else{
      $this->name = $name;
    }
    $this->persistentKey($this->id);
    $this->persistentProperties(array(
      'id',
      'name',
      'pendingUpdates',
      'version',
      'cleanStructure'
    ));
    $this->loadPersistentProperties();
    if(!$this->cleanStructure){
      $this->cleanStructure = $this->structureInUse();
    }
  }

  /**
   * structureInUse
   *
   * Return the VocabularyStructure object
   * representing the structure as it is currently
   * in use on the site. This may contain structures
   * will partial or missing updates.
   */
  abstract protected function structureInUse();

  /**
   * applyUpdate
   *
   * Apply the given update, as represented by a
   * ScratchpadsVocabularies\VocabularyUpdate object.
   * 
   * This function must update the status of the update object.
   *
  */
  abstract protected function applyUpdate($update);

  /**
   * applyModification
   *
   */
  abstract protected function applyModification($entities, $update, $map);

  /**
   * mergeUpdate
   * 
   * Merge the given update into this receiver's update list. 
   * This may require reverting structural updates.
   * 
   */
  abstract protected function mergeUpdate($update, $new_structure);

  /**
   * name()
   *
   * Return the human readable name of this vocabulary
   */
  public function name(){
    return $this->name;
  }

  /**
   * id()
   *
   * Return the id of this vocabulary
  */
  public function id(){
    return $this->id;
  }

  /**
   * version()
   *
   *  Returns the version of the vocabulary
   *  represented by this object.
   *  
   *  In this implementation, this will always be
   *  the version of the vocabulary of the last
   *  structureUpdate.
   */
  public function version(){
    return $this->version;
  }

  /**
   * setVersion()
   * 
   * Sets the current version of this receiver
   */
  protected function setVersion($version){
    $this->version = $version;
  }

  /**
   * structure()
   *
   * Return a ScratchpadsVocabularies\VocabularyStructure object
   * 
   * In this implementation, this will always be
   * the structure of the vocabulary of the last
   * structureUpdate.
   */
  public function structure(){
    return $this->cleanStructure;
  }

  /**
   * setStructure
   * 
   * Set the current clean vocabulary structure
   */
  protected function setStructure($v){
    $this->cleanStructure = $v;
  }

  /**
   * upgradeStructure()
   *
   * Update this receiver to the structure of the given vocabulary,
   * and enqueue all ScratchpadsVocabularies\VocabularyUpdate updates that are
   * needed.
   *
   * In this implementation, this will always set 'structure' and
   * 'version' to be that of the given vocabulary.
   * 
   */
  public function upgradeStructure(VocabularyInterface $vocabulary){
    $current = $this->structure();
    $updated = $vocabulary->structure();
    $updates = $current->updatesToUpgrade($updated);
    if(count($this->pendingUpdates)){
      foreach($updates as $update){
        $this->mergeUpdate($update, $updated);
      }
    }else{
      $this->pendingUpdates = $updates;
    }
    $this->setVersion($vocabulary->version());
    $this->setStructure($updated);
    $this->savePersistentProperties();
  }

  /**
   * pendingUpdates()
   *
   * Return the list of pending ScratchpadsVocabularies\VocabularyUpdate updates
   * as an array of version number to list of updates.
   */
  public function pendingUpdates(){
    return $this->pendingUpdates;
  }

  /**
   * applyPendingUpdates
   *
   * Apply the structural part of the pending updates. Updates that
   * do not need to modify any content should be removed from the list.
   *
   * If $types is defined, then only apply the updates of the given type
   * (from ScratchpadsVocabularies\VocabularyUpdate::T_*)
   */
  public function applyPendingUpdates($types = array()){
    foreach($this->pendingUpdates as $index => $update){
      if($types && !in_array($update->type(), $types)){
        continue;
      }
      if($update->status() !== VocabularyUpdate::S_INITIAL){
        continue;
      }
      $this->applyUpdate($update);
      if($update->status() == VocabularyUpdate::S_UPDATE_APPLIED){
        unset($this->pendingUpdates[$index]);
      }
    }
    $this->savePersistentProperties();
  }

  /**
   * applyModifications
   *
   * Apply the content part of the pending updates whose structural
   * part has already been applied. Updates that have been successfully
   * applied should be removed from the list of pending updates.
   * 
   * If $entities is defined (as an list of entities ids indexed by
   * entity type), then only the given entities should be modified.
   * If there are remaining entities, then the update should not be
   * marked as complete or removed.
   *
   * If $updates is defined, then the modification should only be applied
   * for the given updates.
   * 
   * $map may contain data that will be passed on to the receiver.
   */
  public function applyModifications($entities = array(), $updates = array(), $map = array()){
    foreach($this->pendingUpdates as $index => $update){
      if($update->status() !== VocabularyUpdate::S_STRUCTURE_APPLIED){
        continue;
      }
      if(!empty($updates) && !in_array((string)$update, $updates)){
        continue;
      }
      $this->applyModification($entities, $update, $map);
      if($update->status() == VocabularyUpdate::S_UPDATE_APPLIED){
        unset($this->pendingUpdates[$index]);
      }
    }
    $this->savePersistentProperties();
  }
}

