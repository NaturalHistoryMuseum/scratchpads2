<?php
/**
 * This file contains the various classes and interfaces used to
 * maintain and update the vocabulary server integration.
 * 
 * Vocabularies are defined by a version number, and list of terms,
 * each having an id, a label and (potentially) a parent term.
 * 
 * The expected workflow is to have two objects: 
 * 
 * - an implementation of ScratchpadsVocabularies\VocabularyInterface
 *   representing an external vocabulary (for instance a GBIF 
 *   vocabulary) ;
 *   
 * - and an implementation of ScratchpadsVocabularies\ReceiverInterface 
 *   representing a Drupal receiver for the vocabulary (for 
 *   instance a Drupal vocabulary, or a the list of allowed values
 *   for a select field).
 * 
 * When the vocabulary version changes, receivers can be upgraded to
 * the new version. The process of upgrading a receiver is to compare
 * the structures, and from there generate a number of 
 * ScratchpadsVocabularyUpdate objects detailing the needed updates.
 * (See the implementation of ScratchpadsVocabularies\VocabularyReceiver for an
 * example).
 * 
 * The updates can then be applied in two stages: 
 * - structuraly (updating the exisiting definition) 
 * - on the content (updating the content that needs updating,
 *  for instance when a value has been renamed or
 *  deleted). 
 *
 * Applying those updates in stages might require having the actual
 * structure in use on the site to be in a sort of in-between
 * state - the details of how this is done is left to implementers.
 * (for instance the implementer could choose for the structural update
 * to leave deleted items but rename them to "<item> [DELETED]") until
 * the data update has been applied.)
 *   
 */
namespace ScratchpadsVocabularies;

/**
 * VocabularyInterface
 * 
 * Interface used to define an external vocabulary.
 * 
 * This would be implemented by a class that provides a specific
 * type of vocabulary - for instance ScratchpadsVocabularies\GbifXmlVocabulary
 * would extend this class for use with Gbif XML vocabularies.
 * 
 * Implementers may cache data, as long as they ensure fresh data is fetched when
 * refresh is called.
 */
interface VocabularyInterface{

  /**
   * id()
   *
   * Return the id of this vocabulary
   */
  public function id();

  /**
   * name()
   *
   * Return the human readable name of this vocabulary
   */
  public function name();

  /**
   * description()
   *
   * Return a description of this vocabulary
   */
  public function description();

  /**
   * version()
   * 
   *  Returns the version of the vocabulary
   *  represented by this object.
   */
  public function version();

  /**
   * structure()
   * 
   * Return a ScratchpadsVocabularies\VocabularyStructure object
   */
  public function structure();

  /**
   * refresh()
   *
   * Ensure we fetch the latest available data
   */
  public function refresh();
}

/**
 * ReceiverInterface
 * 
 * Interface used to define a vocabulary receiver
 * 
 * This would be implemented by a class that provides a specific
 * receiver - for instance ScratchpadsVocabularies\SelectFieldVocabularyReceiver
 * would implement this to provide a receirver that works by filling
 * in the allowed values of a list text field.
 * 
 * Note that there is a generic implementation of this interface -
 * ScratchpadsVocabularies\VocabularyReceiver - which provides most common
 * features. When applicable, extending that class is prefered over
 * implementing the interface.
 * 
 */
interface ReceiverInterface{

  /**
   * name()
   *
   * Return the human readable name of this vocabulary
   */
  public function name();

  /**
   * id()
   *
   * Return the id of this vocabulary
   */
  public function id();

  /**
   * version()
   *
   *  Returns the version of the vocabulary
   *  represented by this object. A NULL
   *  value would indicate a receiver that
   *  has never been synced.
   */
  public function version();

  /**
   * description()
   *
   * Returns an HTML description of this
   * receiver
   */
  public function description();

  /**
   * structure()
   *
   * Return a ScratchpadsVocabularies\VocabularyStructure object
   */
  public function structure();

  /**
   * canUpgradeStructure
   *
   * Check whether this receiver can be upgraded to the given vocabulary.
   * This should only return FALSE in cases where the site administrator
   * cannot resolve the conflict - eg. applying the upgrade would
   * require changing module code.
   */
  public function canUpgradeStructure(VocabularyInterface $vocabulary);

  /**
   * upgradeStructure()
   *
   * Upgrade this receiver to the structure of the given vocabulary,
   * and enqueue all ScratchpadsVocabularies\VocabularyUpdate updates that are
   * needed.
   *
   * This may be called when there are still some pending updates.
   * It is up to the implementer to decide how to handle this
   * (stack, merge, etc.).
   *
   * Note that the values returned by 'structure()' and 'version()' 
   * should reflect all the updates that have been accepted/enqueued 
   * (regardless of whether they have been applied or not).
   * 
   */
  public function upgradeStructure(VocabularyInterface $vocabulary);

  /**
   * pendingUpdates()
   * 
   * Return the list of pending ScratchpadsVocabularyUpdate updates
   * 
   */
  public function pendingUpdates();

  /**
   * updateModifiesContent
   *
   * Return TRUE if the given update needs to modify content to
   * be fully applied, and FALSE otherwise
   *
   */
  public function updateModifiesContent($update);

  /**
   * contentModifiedBy
   *
   * Return the content modified by applying the given update. If
   * $id is not NULL, then only return the content that is modified
   * when applying the given update for that particular item.
   *
   * The returned array should be entity ids indexed by entity type.
  */
  public function contentModifiedBy($update, $id = NULL);

  /**
   * applyPendingUpdates
   * 
   * Apply the structural part of the pending updates. Updates that
   * do not need to modify any content should be removed from the list.
   * Others should be provided with an object that implements ModificationInterface
   */
  public function applyPendingUpdates();

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
   */
  public function applyModifications($entities = array(), $updates = array());
}

/**
 * ModificationInterface
 * 
 * Interface used to define a set of moficiations that are required for
 * an update to be applied.
 * 
 * This would be implemented by a class that provides a way for users
 * to go through an update that requires user intervention.
 * 
 * For instance a REMOVE update might require users to re-map content
 * before the item can be removed.
 * 
 */
interface ModificationInterface{

  /**
   * explanation
   * 
   * Returns a human readable string that explains what
   * modification is required.
   */
  public function explanation();

  /**
   * getForm
   * 
   * Returns a form array that is used by the user
   * to perform any required actions before the update
   * can be applied.
   * 
   */
  public function getForm($form, &$form_state);

  /**
   * validateForm
   *
   * Validate the form
   *
   */
  public function validateForm($form, &$form_state);

  /**
   * submitForm
   *
   * Submit the form
   */
  public function submitForm($form, &$form_state);
}

/**
 * VocabularyUpdate
 *
 * This class represents an update that can be applied to a receiver.
 *
 * There are four types of updates:
 *  - "add" (when items need to be added),
 *  - "delete" (when items need to be deleted),
 *  - "renamed" (when items need to be renamed, but have not been reparented)
 *  - "reparent" (when items need to be reparented, and potentially renamed)
 *  - "data" (when the item data has changed. Note that data update can be 
 *    added in addition to a rename or reparent update.)
 *
 * In addition, an update can be in three different status:
 * - "none", whereby the update has not been applied at all ;
 * - "structure", whereby the update has been applied structuraly
 *   but the actual content that uses the vocabulary has not been
 *   updated ;
 * - "complete", whereby the update has been fully applied.
 * 
 * Finally an update might have a progress. This is usefull for updates
 * that must be run in batches - how this is used is left to each
 * receiver's implementer. However an update should not be moved
 * to the next stage until it is complete.
 * 
 */
class VocabularyUpdate{
  /* Define the various update types */
  const T_ADD = 'add';

  const T_REMOVE = 'remove';

  const T_RENAME = 'renamed';

  const T_REPARENT = 'reparent';

  const T_DATA = 'data';
  /* Define the various update statuses */
  const S_INITIAL = 'initial';

  const S_STRUCTURE_APPLIED = 'structure';

  const S_UPDATE_APPLIED = 'all';
  /* Private properties */
  private $type;

  private $data;

  private $status;

  private $progress;

  private $modification;

  /**
   * __construct()
   * 
   * Create a new object from a type and associated
   * data.
   */
  public function __construct($type, $data){
    $this->type = $type;
    $this->data = $data;
    $this->status = VocabularyUpdate::S_INITIAL;
    $this->progress = 0;
    $this->modification = NULL;
  }

  /**
   * function __toString
   *
   * Generate a string that can be used to compare
   * two update objects
   *
   */
  public function __toString(){
    return $this->type . ':' . $this->status . ':' . serialize($this->data);
  }

  /**
   * type()
   * 
   * Return the update type
   */
  public function type(){
    return $this->type;
  }

  /**
   * items()
   * 
   * Return the update items, as a list
   * of ids => label.
   */
  public function items(){
    return $this->data;
  }

  /**
   * status()
   * 
   * Return the status of this update
   */
  public function status(){
    return $this->status;
  }

  /**
   * setStatus()
   * 
   * Set the status of this update
   */
  public function setStatus($status){
    $this->status = $status;
  }

  /**
   * progress
   * 
   * Return the progress of the update. This is used
   * for large updates that must be run in batches
   */
  public function progress(){
    return $this->progress;
  }

  /**
   * setProgress
   * 
   * Set the progress of the update. This is used
   * for large updates that must be run in batches.
   */
  public function setProgress($p){
    $this->progress = $p;
  }

  /**
   * modification
   * 
   * When an update requires content modification,
   * this may return an object that implements
   * ModificationInterface
   * 
   * This should be set using setModification by
   * the receiver.
   */
  function modification(){
    return $this->modification;
  }

  /**
   * setModification
   * 
   * Set the updates' modification.
   */
  function setModification($modification){
    $this->modification = $modification;
  }

  /**
   * removeItem
   * 
   * Remove a given item from the list of items to update
   */
  public function removeItem($id){
    unset($this->data[$id]);
  }

  /**
   * updateItem
   *
   * Update a given item in the list of items to update
   */
  public function updateItem($id, $label){
    $this->data[$id] = $label;
  }
}

/**
 * VocabularyStructure
 * 
 * This class represents a vocabulary structure. 
 * 
 * When represented as an array (either as input or output), the
 * vocabulary structure is implemented as:
 * 
 * array(
 *   'id1' => 'label1',
 *   'id2' => 'label2',
 *   ...
 *   'idn' => 'labeln',
 *   'idm' => array(
 *      'label' => 'labelm',
 *      'children' => array(
 *        ...
 *      )
 *    )
 *  )
 * 
 * ids must be unique for the whole structure. Other data that relates to each
 * item maybe provided in $item_data as an array mapping each item id to
 * some extra data. When checking for data updates, the === comparaison operator
 * is used.
 * 
 */
class VocabularyStructure{

  private $structure;

  private $itemData;

  private $cacheParents = NULL;

  private $cacheFlat = NULL;

  /**
   * __construct
   * 
   * Create a new object from a (potentiall nested) array of
   * id => label, or from an existing ScratchpadsVocabularies\VocabularyStructure object
   * 
   */
  public function __construct($structure = array(), $item_data = array()){
    if(is_object($structure) && get_class($structure) == 'ScratchpadsVocabularies\VocabularyStructure'){
      $this->structure = $structure->structure();
    }else if(is_array($structure)){
      $this->structure = $structure;
    }else{
      throw new Exception("ScratchpadsVocabularies\VocabularyStructure::__construct() - expecting array as parameter");
    }
    $this->itemData = $item_data;
  }

  /**
   * structure
   *
   * Return the array representing the structure as a (potentially
   * nested) array of id => label
   */
  public function structure(){
    return $this->structure;
  }

  /**
   * flatStructure
   * 
   * Returns the flattened structure
   * 
   */
  public function flatStructure($root = NULL){
    $initial = FALSE;
    if($root === NULL){
      if($this->cacheFlat){return $this->cacheFlat;}
      $root = $this->structure;
      $initial = TRUE;
    }
    $result = array();
    foreach($root as $k => $v){
      if(!is_array($v)){
        $result[$k] = $v;
      }else{
        $result[$k] = $v['label'];
        $result = array_merge($result, $this->flatStructure($v['children']));
      }
    }
    if($initial){
      $this->cacheFlat = $result;
    }
    return $result;
  }

  /**
   * selectStructure
   * 
   * Returns the structure flattened as an array of $id => $label
   * such that labels include the parent names (separated with the given
   * separator). This is usefull for constructing select form items.
   */
  public function selectStructure($sep = ' > '){
    $flat = $this->flatStructure();
    $out = array();
    foreach($flat as $id => $label){
      $p = $this->itemParents($id);
      if(count($p)){
        $p[] = $label;
        $label = explode($sep, $p);
      }
      $out[$id] = $label;
    }
    return $out;
  }

  /**
   * allParents
   * 
   * Returns an array of item id to parent id
   */
  public function allParents($root = NULL, $parent = NULL){
    $initial = FALSE;
    if($root === NULL){
      if($this->cacheParents){return $this->cacheParents;}
      $root = $this->structure;
      $initial = TRUE;
      $parent = NULL;
    }
    $parents = array();
    foreach($root as $k => $v){
      $parents[$k] = $parent;
      if(is_array($v)){
        $parents = array_merge($parents, $this->allParents($v['children'], $k));
      }
    }
    if($initial){
      $this->cacheParents = $parents;
    }
    return $parents;
  }

  /**
   * itemParents
   * 
   * Return an ordered array containing the parents of the given
   * item (by id), as id => label
   */
  public function itemParents($id){
    $parents = $this->allParents();
    $flat = $this->flatStructure();
    $result = array();
    while(!empty($parents[$id])){
      $id = $parents[$id];
      $result[$id] = $flat[$id];
    }
    return array_reverse($result);
  }

  /**
   * itemData
   * 
   * Return the additional data associated with a given item
   */
  public function itemData($id){
    if(isset($this->itemData[$id])){
      return $this->itemData[$id];
    }else{
      return NULL;
    }
  }

  /**
   * render
   *
   * Returns an HTML representation of the structure.
   *
   * If a given item's data is an array and has a 'render' key,
   * then that is used instead of the item's label when generating
   * the HTML.
   *
   * $root is used internally for recursion.
   *
   */
  public function render($root = NULL){
    $out = array();
    if($root === NULL){
      $root = $this->structure;
    }
    foreach($root as $key => $data){
      $out[] = array(
        '#type' => 'markup',
        '#markup' => $this->renderItem($key, TRUE)
      );
    }
    return drupal_render($out);
  }

  /**
   * renderItem
   *
   * Returns the HTML representation of a single item
   * defined by it's key $key. If $children is not FALSE,
   * then the rendering will include the item's children,
   * if any.
   *
   * If a given item's data is an array and has a 'render' key,
   * then that is used instead of the item's label when generating
   * the HTML.
   */
  public function renderItem($key, $children = FALSE){
    $out = array();
    $data = $this->structure[$key];
    $item_data = $this->itemData($key);
    if(is_array($item_data) && isset($item_data['render'])){
      $out[] = array(
        '#prefix' => '<div class="scratchpads-vocabularies-item">',
        '#suffix' => '</div>',
        '#markup' => $item_data['render']
      );
    }else if(is_array($data)){
      $out[] = array(
        '#prefix' => '<div class="scratchpads-vocabularies-item">',
        '#suffix' => '</div>',
        '#markup' => check_plain($data['label'])
      );
    }else{
      $out[] = array(
        '#prefix' => '<div class="scratchpads-vocabularies-item">',
        '#suffix' => '</div>',
        '#markup' => check_plain($data)
      );
    }
    if($children && is_array($data)){
      $out[] = array(
        '#prefix' => '<div class="scratchpads-vocabularies-indent">',
        '#suffix' => '</div>',
        'children' => $this->render($data['children'])
      );
    }
    return drupal_render($out);
  }

  /**
   * updatesToUpgrade
   * 
   * Return the list of updates (as ScratchpadsVocabularies\VocabulariesUpdate object)
   * required to change for this structure to the provided one.
   * 
   */
  public function updatesToUpgrade($struct){
    $updates = array();
    $a = $this->flatStructure();
    $b = $struct->flatStructure();
    $added_ids = array_diff(array_keys($b), array_keys($a));
    $added_items = array_intersect_key($b, array_flip($added_ids));
    if($added_items){
      $updates[] = new VocabularyUpdate(VocabularyUpdate::T_ADD, $added_items);
    }
    $removed_ids = array_diff(array_keys($a), array_keys($b));
    $removed_items = array_intersect_key($a, array_flip($removed_ids));
    if($removed_items){
      $updates[] = new VocabularyUpdate(VocabularyUpdate::T_REMOVE, $removed_items);
    }
    $items_with_matching_key_a = array_intersect_key($a, $b);
    $items_with_matching_key_b = array_intersect_key($b, $a);
    $a_parents = $this->allParents();
    $b_parents = $struct->allParents();
    $common_a_parents = array_diff_key($a_parents, array_flip($removed_ids));
    $common_b_parents = array_diff_key($b_parents, array_flip($added_ids));
    $reparented_ids = array_keys(array_diff_assoc($common_b_parents, $common_a_parents));
    $reparented_items = array_intersect_key($b, array_flip($reparented_ids));
    if($reparented_items){
      $updates[] = new VocabularyUpdate(VocabularyUpdate::T_REPARENT, $reparented_items);
    }
    $renamed_items = array_diff_assoc($items_with_matching_key_b, $items_with_matching_key_a);
    // If something is both renamed and reparented, only keep it in reparent.
    $renamed_items = array_diff_key($renamed_items, $reparented_items);
    if($renamed_items){
      $updates[] = new VocabularyUpdate(VocabularyUpdate::T_RENAME, $renamed_items);
    }
    // Check if any of the non-deleted items have a data update too.
    $non_deleted_ids = array_diff(array_keys($a), $removed_ids);
    $data_update_ids = array();
    foreach($non_deleted_ids as $ndid){
      if($this->itemData($ndid) !== $struct->itemData($ndid)){
        $data_update_ids[] = $ndid;
      }
    }
    if($data_update_ids){
      $data_updates = array_intersect_key($b, array_flip($data_update_ids));
      $updates[] = new VocabularyUpdate(VocabularyUpdate::T_DATA, $data_updates);
    }
    return $updates;
  }

  /**
   * __sleep()
   * 
   * We don't need to serialize caching variables
   */
  function __sleep(){
    $vars = array();
    foreach(array_keys(get_object_vars($this)) as $var){
      if(!preg_match('/^cache/', $var)){
        $vars[] = $var;
      }
    }
    return $vars;
  }
}

