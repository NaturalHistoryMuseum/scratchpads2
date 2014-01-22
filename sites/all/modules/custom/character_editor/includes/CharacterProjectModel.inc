<?php

/**
 * class CharacterProjectModel
 * 
 * This class is used to fetch data from a character project
 */
class CharacterProjectModel{

  private $w; // The project entity metadata wrapper

  private $tree; // Cached tree of characters

  private $flags; // Cached list of flags

  /**
   * __construct
   * 
   * Create a CharacterProjectModel from an entity metadata wrapper, an
   * entity type and an entity object or idk, or a string of the
   * form 'entity type:entity id[:ignored].
   * 
   * See character_editor_wrapper for more information.
   */
  function __construct($info, $id = NULL, $reset = FALSE){
    $this->w = character_editor_wrapper($info, $id, $reset);
  }

  /**
   * static getProjectOfEntity
   * 
   * Given an entity type and entity, return a new character project model
   * for the project that the given entity belongs to.
   * 
   * Returns false if no such project was found.
   */
  static function getProjectOfEntity($entity_type, $entity){
    $entity_w = character_editor_wrapper($entity_type, $entity);
    $project_w = character_editor_get_character_project($entity_w);
    if($project_w){
      return new CharacterProjectModel($project_w);
    }else{
      return FALSE;
    }
  }

  /**
   * id
   * 
   * Return a string id that represents this character project
   */
  public function id(){
    return $this->w->getBundle() . ':' . $this->w->getIdentifier();
  }

  /**
   * entityId
   * 
   * Return the entity id of this project model's entity
   */
  public function entityId(){
    return $this->w->getIdentifier();
  }

  /**
   * uid
   * 
   * Return the user id of the project's author
   */
  public function uid(){
    return $this->w->uid->uid->value();
  }

  /**
   * getFlags
   * 
   * Return the list of available flags for this project.
   * Returned is an array of arrays defining 'flag', 'abbr' and 'id'
   */
  public function getFlags(){
    if(empty($this->flags)){
      $this->flags = array();
      foreach($this->w->field_char_proj_flags as $f){
        $flagid = $f->field_char_proj_flags_id->value();
        $this->flags[$flagid] = array(
          'flag' => $f->field_char_proj_flags_flag->value(),
          'abbr' => $f->field_char_proj_flags_abbr->value(),
          'id' => $flagid
        );
      }
    }
    return $this->flags;
  }

  /**
   * getAvailableVariables
   * 
   * Returns the list of variables available for calculating
   * expressions
   */
  public function getAvailableVariables(){
    $tree = $this->getTree();
    $result = array();
    foreach ($tree as $item){
      if (!$item->model()->isGroup() && !$item->model()->isComputed()){
        $r = '{' . $item->model()->label() . ':' . preg_replace('/^.+:/', '', $item->model()->id()) . '}';
        if ($item->model()->isControlled()){
          $map = $item->model()->getControlledMap();
          if (count($map)){
            $r .= '(states: ';
            $states = array();
            foreach ($map as $id => $label){
              $states[] = "'$label:$id'";
            }
            $r .= implode(', ', $states);
            $r .= ')';
          }
        }
        $result[] = $r;
      }
    }
    return $result;
  }
  
  /**
   * getTree
   * 
   * Return the tree of characters of this project as a flat
   * array of CharacterTreeNode objects. The array is indexed
   * by each node's id and, while flat, is ordered as if
   * displayed in a hierarchy.
   * 
   */
  public function getTree(){
    if(empty($this->tree)){
      $this->tree = array();
      $tree = character_editor_get_tree($this->w);
      foreach($tree as $id => $tree_node){
        $this->tree[$id] = new CharacterTreeNode($id, $tree_node);
      }
    }
    return $this->tree;
  }

  /**
   * getGroupSelect
   * 
   * Return the list of groups in this project formatted to be used
   * in a select item
   */
  public function getGroupSelect(){
    $groups = array();
    $tree = character_editor_get_tree($this->w);
    foreach($tree as $tree_item){
      if($tree_item['wrapper']->getBundle() !== 'group'){
        continue;
      }
      $group_id = $tree_item['wrapper']->getIdentifier();
      $groups[$group_id] = str_repeat('-', $tree_item['depth']) . ' ' . $tree_item['wrapper']->title->value();
    }
    return $groups;
  }

  /**
   * getRowValues
   *
   * Given a set of rows (as a Views result), returns the row extended to include all
   * the character values for this project.
   */
  public function getRowValues($rows){
    // Read the ids
    $row_ids = array();
    foreach($rows as $r){
      $row_ids[] = $r->id;
    }
    // Check for collapsed rows
    $collapsed = array();
    if(isset($_GET['character_entity_field'])){
      $collapsed = explode(',', $_GET['character_entity_field']);
    }
    // Fetch the values
    $characters = array();
    foreach($this->getTree() as $item_key => $tree_item){
      $characters = array_merge_recursive($characters, $tree_item->getValues($row_ids));
    }
    // Compute values
    foreach($this->getTree() as $item_key => $tree_item){
      if ($tree_item->model()->isComputed()){
        $characters = $tree_item->updateComputedValues($characters);
      }
    }
    // Assign them to the rows
    $ids = array();
    foreach($rows as $row){
      $id = $row->id;
      // Assign character values
      if(isset($characters[$id]) && is_array($characters[$id])){
        foreach($characters[$id] as $field => $value){
          $row->{$field} = drupal_json_encode($value);
        }
      }
      $ids[$id] = $row;
    }
    // Build the tree
    $tree = array();
    foreach($ids as $id => $row){
      $list = array();
      $pid = $id;
      while($pid && !preg_match('/:0$/', $pid)){
        if(isset($ids[$pid])){
          $list[] = $pid;
          $pid = $ids[$pid]->parent;
        }else{
          $pid = 0;
        }
      }
      $list = array_reverse($list);
      $ptr = & $tree;
      foreach($list as $pid){
        if(!isset($ptr[$pid])){
          $ptr[$pid] = array(
            'value' => $ids[$pid],
            'children' => array(),
            'collapsed' => in_array($pid, $collapsed)
          );
        }
        $ptr = & $ptr[$pid]['children'];
      }
      unset($ptr);
    }
    // And order the items
    return $this->assign_tree_values($tree, 0);
  }

  /**
   * assign_tree_values
   *
   * Helper method used to create the tree of rows for the Views
   * display
   */
  private function assign_tree_values($tree, $depth){
    $result = array();
    foreach($tree as $item){
      $item['value']->depth = $depth;
      $item['value']->children = count($item['children']);
      $item['value']->collapsed = $item['collapsed'];
      $result[] = $item['value'];
      if(!$item['collapsed']){
        $result = array_merge($result, $this->assign_tree_values($item['children'], $depth + 1));
      }
    }
    return $result;
  }
}