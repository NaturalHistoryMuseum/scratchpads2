<?php
namespace ScratchpadsVocabularies;
require_once str_replace('test.vocabulary.inc', '../includes/classes.inc', __FILE__);

/**
 * TestVocabulary
 * 
 * Implement VocabularyInterface for testing. This provides a number of methods which modifiy the
 * vocabulary step by step for testing the various functionalities.
 * 
 */
class TestVocabulary implements VocabularyInterface{

  public $version;

  public $structure;

  public $itemData;

  /**
   * __construct
   * 
   * Set initial version & structure
   */
  function __construct(){
    $this->version = 0;
    $this->reset();
  }

  /**
   * reset
   * 
   * Reset the vocabulary (but increase the version)
   */
  function reset(){
    $this->version++;
    $this->structure = array(
      'initial:1' => 'Initial value 1',
      'initial:2' => array(
        'label' => 'Initial value 2',
        'children' => array(
          'initial:3' => 'Initial value 3'
        )
      ),
      'initial:4' => 'Initial value 4'
    );
    $this->itemData = array(
      'initial:1' => NULL,
      'initial:2' => NULL,
      'initial:3' => array(
        'initial:3:extra' => 'Initial 3 extra'
      ),
      'initial:4' => NULL
    );
  }

  /**
   * testAdd
   * 
   * Add some items to the structure.
   * 
   */
  function testAdd(){
    $this->version++;
    $this->structure['add:1'] = 'Add test 1 (root)';
    $this->structure['initial:2']['children']['add:2'] = 'Add test 2 (under initial value 2)';
  }

  /**
   * Rename some elements and increase the version
   */
  function testRename(){
    $this->version++;
    $this->structure['initial:1'] = 'Initial value 1, renamed';
    $this->structure['initial:2']['children']['initial:3'] = 'Initial value 3, renamed';
  }

  /**
   * Reparent some elements and increase the version
   */
  function testReparent(){
    $this->version++;
    $this->structure['initial:2']['children']['initial:1'] = 'Initial value 1';
    unset($this->structure['initial:1']);
  }

  /**
   * Remove some elements and increase the version
   */
  function testRemove(){
    $this->version++;
    unset($this->structure['initial:1']);
  }

  /**
   * Remove some elements and increase the version
   */
  function testRemove2(){
    $this->version++;
    unset($this->structure['initial:1']);
    $this->structure['initial:2'] = 'Initial value 2'; // In effect remove initial:3
  }

  /**
   * Modify the item data and increase the version
   */
  function testItemData(){
    $this->version++;
    $this->itemData['initial:3']['initial:3:extra'] = 'Initial 3 extra, modified';
  }

  /**
   * Set a different structure we will use for merge tests, and increase the version
   */
  function testMergePrepare(){
    $this->version++;
    $this->structure = array(
      'merge:remove:1' => 'Merge: remove + add, no rename',
      'merge:remove:2' => 'Merge: remove + add, with rename',
      'merge:remove:3' => 'Merge: remove + add, with reparent',
      'merge:rename:1' => 'Merge: rename + remove',
      'merge:rename:2' => 'Merge: rename + rename',
      'merge:rename:3' => 'Merge: rename + reparent',
      'merge:reparent:root' => array(
        'label' => 'Merge: reparent root',
        'children' => array()
      ),
      'merge:reparent:root:1' => array(
        'label' => 'Merge: reparent root (1)',
        'children' => array()
      ),
      'merge:reparent:1' => 'Merge: reparent + remove',
      'merge:reparent:2' => 'Merge: reparent + rename',
      'merge:reparent:3' => 'Merge: reparent + reparent'
    );
  }

  /**
   * Prepare some updates that will cause update merges, and increase the version.
   * 
   */
  function testMergeStart(){
    $this->version++;
    // Add
    $this->structure['merge:add:1'] = 'Merge: add + remove';
    $this->structure['merge:add:2'] = 'Merge: add + rename';
    $this->structure['merge:add:3'] = 'Merge: add + reparent';
    // Remove
    unset($this->structure['merge:remove:1']);
    unset($this->structure['merge:remove:2']);
    unset($this->structure['merge:remove:3']);
    // Rename
    $this->structure['merge:rename:1'] = 'Merge: rename + remove, renamed';
    $this->structure['merge:rename:2'] = 'Merge: rename + rename, renamed';
    $this->structure['merge:rename:3'] = 'Merge: rename + reparent, renamed';
    // Reparent
    $this->structure['merge:reparent:root']['children']['merge:reparent:1'] = $this->structure['merge:reparent:1'];
    unset($this->structure['merge:reparent:1']);
    $this->structure['merge:reparent:root']['children']['merge:reparent:2'] = $this->structure['merge:reparent:2'];
    unset($this->structure['merge:reparent:2']);
    $this->structure['merge:reparent:root']['children']['merge:reparent:3'] = $this->structure['merge:reparent:3'];
    unset($this->structure['merge:reparent:3']);
  }

  /**
   * Add some update that will cause update merges with testMergeStart's updates, and increase the version.
   *
   */
  function testMergeEnd(){
    $this->version++;
    // Add
    unset($this->structure['merge:add:1']);
    $this->structure['merge:add:2'] = 'Merge: add + rename, renamed';
    $this->structure['merge:reparent:root']['children']['merge:add:3'] = $this->structure['merge:add:3'];
    unset($this->structure['merge:add:3']);
    // Remove
    $this->structure['merge:remove:1'] = 'Merge: remove + add, no rename';
    $this->structure['merge:remove:2'] = 'Merge: remove + add, with rename, renamed';
    $this->structure['merge:reparent:root']['children']['merge:remove:3'] = 'Merge: remove + add, with reparent';
    // Rename
    unset($this->structure['merge:rename:1']);
    $this->structure['merge:rename:2'] = 'Merge: rename + rename, renamed twice';
    $this->structure['merge:reparent:root']['children']['merge:rename:3'] = $this->structure['merge:rename:3'];
    unset($this->structure['merge:rename:3']);
    // Reparent
    unset($this->structure['merge:reparent:root']['children']['merge:reparent:1']);
    $this->structure['merge:reparent:root']['children']['merge:reparent:2'] = 'Merge: reparent + rename, renamed';
    $this->structure['merge:reparent:root:1']['children']['merge:reparent:3'] = $this->structure['merge:reparent:root']['children']['merge:reparent:3'];
    unset($this->structure['merge:reparent:root']['children']['merge:reparent:3']);
  }

  /**
   * Simple accessors from interface
   */
  function id(){
    return 'testvoc';
  }

  function name(){
    return 'Test Vocabulary';
  }

  function version(){
    return $this->version;
  }

  function structure(){
    return new VocabularyStructure($this->structure, $this->itemData);
  }

  function description(){
    return 'Test Vocabulary description';
  }

  function refresh(){
    /*NO OP*/
  }
}