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
    $this->relation = isset($node['relation']) ? $node['relation'] : NULL;
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
  public function getViewStyleOptions($project_model){
    $options = array(
      'width' => 35,
      'filter' => '',
      'headerCssClass' => 'character character-type-' . $this->model->wrapper()->getBundle() . ' ' . $this->column_id,
      'data' => $this->model->getTooltip($project_model)
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
   *       'value' => <if different from data, the internal value for the cell>'
   *       'variable' => <if different from data, the value of the cell to be used
   *                      as variable in the expression parser>
   *       'metadata' => <cell metadata, as array defining 'flag' and 'pass'>
   *     )
   *   )
   * )
   *
   * $row_ids may define an array of rows for which to fetch the values (otherwise
   * all values are defined), as an array parsable by character_editor_parse_entities.
   * 
   * This return placeholder values for computed characters
   */
  public function getValues($row_ids = array()){
    $result = array();
    if($this->model()->isComputed()){
      foreach($row_ids as $id){
        $result[$id][$this->column_id] = array(
          'data' => '',
          'metadata' => array(
            'flag' => 'computed',
            'pass' => ''
          )
        );
      }
    }else{
      // Pre-fill empty values
      foreach($row_ids as $id){
        $result[$id][$this->column_id] = array(
          'data' => '',
          'metadata' => array(
            'flag' => '',
            'pass' => ''
          )
        );
      }
      // Get the values from the character, and insert them.
      $values = $this->model->getValues($row_ids);
      if($this->model->wrapper()->getBundle() == 'controlled'){
        $value_map = $this->model->getControlledMap();
        module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
      }
      foreach($values as $value){
        $data = '';
        $metadata = array();
        if(isset($value['data'][0]['value'])){
          $data = $value['data'][0]['value'];
        }
        if(isset($value['flag'][0]['value'])){
          $metadata['flag'] = $value['flag'][0]['value'];
        }
        if(isset($value['pass'][0]['value'])){
          $metadata['pass'] = $value['pass'][0]['value'];
        }
        $char_id = $value['entity_w']->type() . ':' . $value['entity_w']->getIdentifier();
        if(!empty($value_map) && $data){
          try{
            $expression = new CharacterExpressionParser($data);
            $expr_result = $expression->evaluate();
            if($expr_result->type() == 'list'){
              // A manually entered value will always be either 'or' or 'and', so
              // the flattened value is safe to use.
              $expr_flat = $expression->evaluate()->getFlattenedValue();
              $expr_str = $expression->rename(array(), $value_map)->evaluate()->asString();
            }else{
              $expr_flat = array(
                (string)$expr_result->value()
              );
              $expr_str = (string)$expression->rename(array(), $value_map)->evaluate()->value();
            }
            $result[$char_id][$this->column_id] = array(
              'data' => $expr_str,
              'value' => implode(',', $expr_flat),
              'variable' => $data,
              'metadata' => $metadata
            );
          }
          catch(Exception $e){
            $result[$char_id][$this->column_id] = array(
              'data' => "Invalid expression: " . $e->getMessage(),
              'value' => '',
              'metadata' => $metadata
            );
          }
        }else{
          $result[$char_id][$this->column_id] = array(
            'data' => $data,
            'metadata' => $metadata
          );
        }
      }
    }
    return $result;
  }

  /**
   * updateComputedValues
   * 
   * Inserts the computed values of this character into the row of value
   * The rows are formatted as per getValues.
   */
  public function updateComputedValues($rows, $variables, $state_order){
    if(!$this->model()->isComputed()){return $rows;}
    $values = $this->model->getComputedValues($variables, $state_order);
    foreach($values as $row_id => $v){
      if($v !== FALSE){
        $rows[$row_id][$this->column_id]['data'] = $v;
      }else{
        unset($rows[$row_id][$this->column_id]);
      }
    }
    return $rows;
  }

  /**
   * updateConditions
   * 
   * Inserts the condition value of this character into the row of value.
   * The rows are formatted as per getValues
   */
  public function updateConditions($rows, $variables, $state_order){
    if(!$this->model->hasCondition()){return $rows;}
    $conditions = $this->model->getConditions($variables, $state_order);
    foreach($conditions as $row_id => $value){
      if(!$value){
        $rows[$row_id][$this->column_id]['disabled'] = TRUE;
      }
    }
    return $rows;
  }
}