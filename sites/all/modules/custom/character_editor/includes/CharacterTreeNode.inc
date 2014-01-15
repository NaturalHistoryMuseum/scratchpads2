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
    $options = array(
      'width' => 35,
      'filter' => '',
      'headerCssClass' => 'character character-type-' . $this->model->wrapper()->getBundle() . ' ' . $this->column_id,
      'data' => $this->model->getTooltip()
    );
    if($this->model->hasOptions()){
      $options['data'] = array_merge($options['data'], $this->model->getOptions());
    }
    return $options;
  }

  /**
   * getValues
   *
   * Return the values associated with this node's characters, prepared for
   * displaying in the grid. The returned value is of the form:
   *
   * array(
   *   <character id> => array(
   *     <this column's id> => array(
   *       'data' => <cell value>
   *       'metadata' => <cell metadata>
   *     )
   *   )
   * )
   *
   * $row_ids may define an array of rows for which to fetch the values (otherwise
   * all values are defined), as an array parsable by character_editor_parse_entities.
   */
  public function getValues($row_ids = array()){
    $result = array();
    $values = $this->model->getValues($row_ids);
    if ($this->model->wrapper()->getBundle() == 'controlled'){
      $value_map = $this->model->getControlledMap();
    }
    foreach($values as $value){
      $data = '';
      $metadata = '';
      if(isset($value['data'][0]['safe_value'])){
        $data = $value['data'][0]['safe_value'];
      }
      if(isset($value['metadata'][0]['safe_value'])){
        $metadata = $value['metadata'][0]['safe_value'];
      }
      $char_id = $value['entity_w']->type() . ':' . $value['entity_w']->getIdentifier();
      if (!empty($value_map)){
        $result[$char_id][$this->column_id] = array(
          'data' => $value_map[$data],
          'value' => $data,
          'metadata' => $metadata
        );
      } else {
        $result[$char_id][$this->column_id] = array(
          'data' => $data,
          'metadata' => $metadata,
        );
      }
    }
    return $result;
  }
}