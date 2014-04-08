<?php

/**
 * class CharacterTreeNode
 *
 * This class represents a character at a specific position within a character
 * tree (eg. under a specific parent).
 *
 * It covers the functionality that is specific to a character when at a particular
 * position - such as the definition for Views fields.
 *
 * It also stands in between the CharacterProjectModel and the CharacterModel to
 * ensure that the data is formatted as expected by the slickgrid/character editor.
 * 
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
   * Return the model associated with this node, as an instance
   * of CharacterModel object
   */
  public function model(){
    return $this->model;
  }

  /**
   * getEditorTreeDefinition
   * 
   * Return an array definition of this node to be used by the character tree widget
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
      'width' => 55,
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
        $metadata = array(
          'flag' => $value->flag,
          'pass' => $value->pass
        );
        if(!empty($value_map) && $value->data){
          try{
            $expression = new CharacterExpressionParser($value->data);
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
            $expr_delimited = preg_replace('| <em>[^<]*</em> |', ',', $expr_str);
            $result[$value->entity_char_id][$this->column_id] = array(
              'data' => $expr_str,
              'data_delimited' => $expr_delimited,
              'value' => implode(',', $expr_flat),
              'variable' => $value->data,
              'metadata' => $metadata,
              'list' => ''
            );
          }
          catch(Exception $e){
            $result[$value->entity_char_id][$this->column_id] = array(
              'data' => "Invalid expression: " . $e->getMessage(),
              'value' => '',
              'metadata' => $metadata
            );
          }
        }else{
          $result[$value->entity_char_id][$this->column_id] = array(
            'data' => $value->data,
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