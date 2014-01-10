<?php

/**
 * class CharacterTree
 * 
 * This class represents a node in a character tree. Note that for processing
 * character trees are passed as flat arrays of tree nodes, presented in the
 * correct order (rather than an acutal tree structure)
 */
class CharacterTreeNode{

  private $id; // Id of this node

  private $column_id;

  private $parent_id; // Parent node id

  private $model; // Character model

  private $relation; // Relation object that links the node to it's parent

  private $depth; // Depth of the node

  private $weight; // Weight value on the relation that links to parent

  /**
   * __construct
   * 
   * Create a new node from an item returned by
   * character_editor_get_tree
   */
  public function __construct($id, $node){
    $this->id = $id;
    $this->column_id = preg_replace('/^.+:(\d+):(\d+)$/', 'character_$1_$2', $id);
    $this->model = new CharacterModel($node['wrapper']);
    $this->relation = $node['relation'];
    $this->parent_id = $node['parent_id'];
    $this->depth = $node['depth'];
    $this->weight = $node['weight'];
  }

  /**
   * id
   * 
   * Return the id of the node
   */
  public function id(){
    return $this->id;
  }

  /**
   * columnId
   * 
   * Return the id of the node formatted to be used as a view's
   * column id
   */
  public function columnId(){
    return $this->column_id;
  }

  /**
   * model
   * 
   * Return the model associated with this node
   */
  public function model(){
    return $this->model;
  }

  /**
   * getEditorTreeDefinition
   * 
   * Return an array definition this node to be used by the character tree widget
   */
  public function getEditorTreeDefinition(){
    return array(
      'id' => $this->columnId(),
      'label' => $this->model()->label(),
      'depth' => $this->depth,
      'group' => $this->model()->isGroup(),
      'visible' => true,
      'parent' => preg_replace('/^.+:(\d+):(\d+)$/', 'character_$1_$2', $this->parent_id)
    );
  }

  /**
   * getViewFieldDefinition
   * 
   * Return a view field definition for the field that will represent this node
   */
  public function getViewFieldDefinition(){
    return array(
      'id' => $this->column_id,
      'relationship' => 'none',
      'group_type' => 'group',
      'label' => $this->model->label(),
      'hide_empty' => 0
    );
  }

  /**
   * getViewStyleOptions
   * 
   * Returns the view style options for the field that will represent this node.
   * Note that this includes the rendered character for tooltips.
   */
  public function getViewStyleOptions(){
    return array(
      'width' => 35,
      'filter' => '',
      'headerCssClass' => 'character character-type-' . $this->model->wrapper()->getBundle() . ' ' . $this->column_id,
      'data' => $this->model->getTooltip()
    );
  }

  /**
   * getValues
   * 
   * Return the values associated with this node's characters, in a format
   * used to provide the data to slickgrid (to views)
   */
  public function getValuesForViews(){
    $result = array();
    $values = $this->model->getValues();
    foreach($values as $value){
      if(!isset($value['data'][0]['safe_value'])){
        continue;
      }
      $data = $value['data'][0]['safe_value'];
      // If this is a controlled character, we want to use the delta, not the value
      if($this->model->wrapper()->getBundle() == 'controlled'){
        $data = $this->model->getControlledStateValue($data);
      }
      $char_id = $value['entity_w']->type() . ':' . $value['entity_w']->getIdentifier();
      $result[$char_id][$this->column_id] = $data;
    }
    return $result;
  }
}