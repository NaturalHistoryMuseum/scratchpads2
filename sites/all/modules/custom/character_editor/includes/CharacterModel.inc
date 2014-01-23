<?php

/**
 * class CharacterModel
 * 
 * This class is a high level wrapper around characters
 */
class CharacterModel{

  /**
   * __construct
   * 
   * Create a CharacterModel from an entity metadata wrapper, an
   * entity type and an entity object or idk, or a string of the
   * form 'entity type:entity id[:ignored].
   * 
   * See character_editor_wrapper for more information.
   */
  function __construct($info, $id = NULL){
    $this->w = character_editor_wrapper($info, $id);
  }

  /**
   * id
   *
   * Return a string id that represents this character.
   * 
   */
  public function id(){
    return $this->w->getBundle() . ':' . $this->w->getIdentifier();
  }

  /**
   * label
   * 
   * Return the label of this model
   */
  public function label(){
    return $this->w->label();
  }

  /**
   * isGroup
   * 
   * Return true if this model is a group, false otherwise
   */
  public function isGroup(){
    return $this->w->getBundle() == 'group';
  }

  /**
   * isComputed
   *
   * Return TRUE if this model is a computed character, false otherwise
   */
  public function isComputed(){
    return $this->w->getBundle() == 'computed';
  }

  /**
   * isControlled
   * 
   * Return TRUE if this model is a controlled character, false otherwise
   */
  public function isControlled(){
    return $this->w->getBundle() == 'controlled';
  }

  /**
   * isNumeric
   */
  public function isNumeric(){
    return $this->w->getBundle() == 'numeric';
  }

  /**
   * wrapper
   * 
   * Return the entity metadata wrapper for this character
   */
  public function wrapper(){
    return $this->w;
  }

  /**
   * getTooltip
   * 
   * Return the information needed by slickgrid to generate the tooltip for this column.
   * This is formated for view's style options.
   */
  function getTooltip(){
    $entity_info = $this->w->entityInfo();
    // The label
    $bt = t('<h2>!label</h2><span class="character-type">!type</span>', array(
      '!type' => $entity_info['bundles'][$this->w->getBundle()]['label'],
      '!label' => $this->w->label()
    ));
    // The rendered character
    $controller = entity_get_controller($this->w->type());
    $content = $controller->view(array(
      $this->w->getIdentifier() => clone $this->w->value()
    ));
    $tooltip = array(
      'bt' => $bt,
      'char' => drupal_render($content),
      'charType' => $this->w->getBundle()
    );
    // Add extra information depending on character type
    if($this->w->getBundle() == 'numeric' && $this->w->field_char_unit->value()){
      $tooltip['bt'] .= t('<span>!unit</span>', array(
        '!unit' => $this->w->field_char_unit->value()
      ));
    }else if($this->w->getBundle() == 'controlled'){
      $value_to_delta = array();
      if($this->w->field_char_states->count() > 0){
        $conj = '';
        foreach($this->w->field_char_states as $state_delta => $state_w){
          $tooltip['bt'] .= $conj . t('<span class="state-delta">@delta</span>@state', array(
            '@delta' => $state_delta,
            '@state' => $state_w->field_char_state_label->value()
          ));
          $conj = '<br/>';
          $value_to_delta[$state_w->getIdentifier()] = $state_delta;
        }
      }else{
        $tooltip['bt'] .= t('There are no states for this character');
      }
    }
    return $tooltip;
  }

  /**
   * getControlledMap
   * 
   * If this is a controlled character, return the value associated with the given
   * state id
   */
  public function getControlledMap(){
    if($this->w->getBundle() !== 'controlled'){return array();}
    if(empty($this->state_values)){
      $this->state_values = array();
      if($this->w->field_char_states->count() > 0){
        foreach($this->w->field_char_states as $state_w){
          $this->state_values[$state_w->getIdentifier()] = $state_w->field_char_state_label->value();
        }
      }
    }
    return $this->state_values;
  }

  /**
   * getEditorName
   * 
   * Return the slickgrid editor name needed to edit this type of character
   */
  public function getEditorName(){
    switch($this->w->getBundle()){
      case 'text':
      case 'dna':
        return 'ModalCharacter';
        break;
      case 'numeric':
        return 'InlineCharacter';
        break;
      case 'controlled':
        return 'ControlledCharacter';
        break;
      default:
        return FALSE;
    }
  }

  /**
   * getValues
   *
   * Return the values associated with this character
   *
   * $row_ids may define an array of rows for which to fetch the values (otherwise
   * all values are defined), as an array parsable by character_editor_parse_entities.
   * 
   * Note that that this does not evaluate computed or inherited values ; it only returns
   * defined values.
   */
  public function getValues($row_ids = array()){
    return character_editor_get_character_values($this->w, $row_ids);
  }

  /**
   * getComputedValue
   * 
   * Given a list of rows of the form:
   * $row_id => array(
   *   $character_id => value
   * )
   * 
   * Return the computed value for this character within that row.
   */
  public function getComputedValues($rows, $order){
    if(!$this->isComputed()){return array();}
    module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
    $result = array();
    try{
      $parser = new CharacterExpressionParser($this->w->field_char_expr->value());
    }
    catch(Exception $e){
      $error = t("Could not parse the expression:") . $e->getMessage();
    }
    foreach($rows as $row_id => $row){
      if($error){
        $result[$row_id] = $error;
      }else{
        try{
          $result[$row_id] = $parser->evaluate($row, $order, CharacterExpressionParser::FORMAT_EDITOR);
        }
        catch(Exception $e){
          if($e->getCode() == CharacterExpressionParser::MISSING_VARIABLE){
            $result[$row_id] = FALSE;
          }else{
            $result[$row_id] = $e->getMessage();
          }
        }
      }
    }
    return $result;
  }

  /**
   * hasOptions
   *
   * Return TRUE if this character has options to pass on to the editor
   */
  public function hasOptions(){
    return $this->w->getBundle() == 'controlled';
  }

  /**
   * getOptions
   *
   * Return the options to pass on to the editor
   */
  public function getOptions(){
    if($this->w->getBundle() == 'controlled'){
      $states = array();
      foreach($this->w->field_char_states->raw() as $state_id){
        $state = character_editor_wrapper('field_collection_item', $state_id);
        $states[$state_id] = $state->field_char_state_label->value();
      }
      return array(
        'options' => $states,
        'type' => $this->w->field_char_and_or->value()
      );
    }else{
      return array();
    }
  }
}