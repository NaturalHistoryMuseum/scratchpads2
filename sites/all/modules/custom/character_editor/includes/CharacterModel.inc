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
   * isTextual
   *
   * Return TRUE if this model is a textual (text or dna) character
   */
  public function isTextual(){
    return $this->w->getBundle() == 'text' || $this->w->getBundle() == 'dna';
  }

  /**
   * isOrCharacter
   * 
   * For controlled characters, returns TRUE if this is an OR character
   */
  public function isOrCharacter(){
    return $this->w->getBundle() == 'controlled' && $this->w->field_char_and_or->value() == 'OR';
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
  function getTooltip($project_model){
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
    // Translate the expressions and conditions to human readable form
    if(count($content['character_editor_character']) && ($this->isComputed() || $this->hasCondition())){
      $variables = $project_model->getAvailableVariables();
      $children = element_children($content['character_editor_character']);
      $child = reset($children);
      module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
      if($this->isComputed()){
        $expr = $this->w->field_char_expr->value();
        try{
          $e = new CharacterExpressionParser($expr);
          $e->rename(array_flip($variables['character_map']), array_flip($variables['state_map']));
          $expr = $e->getExpression();
        }
        catch(Exception $e){
          $expr = "Could not parse saved expression: " . $e->getMessage();
        }
        $content['character_editor_character'][$child]['field_char_expr'][0]['#markup'] = htmlspecialchars($expr);
      }
      if($this->hasCondition()){
        $expr = $this->w->field_char_condition->value();
        try{
          $e = new CharacterExpressionParser($expr);
          $e->rename(array_flip($variables['character_map']), array_flip($variables['state_map']));
          $expr = $e->getExpression();
        }
        catch(Exception $e){
          $expr = "Could not parse condition: " . $e->getMessage();
        }
        $content['character_editor_character'][$child]['field_char_condition'][0]['#markup'] = htmlspecialchars($expr);
      }
    }
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
    // We could use character_editor_get_character_values but that's too slow for mass processing.
    // Build the query
    $query = db_select('field_data_endpoints', 'e');
    // Select a relation
    $query->condition('e.entity_type', 'relation');
    $query->condition('e.bundle', 'character_state');
    $query->addExpression('CONCAT(e.endpoints_entity_type, \':\', e.endpoints_entity_id)', 'entity_char_id');
    // Ensure it has our character as one of it's endpoints
    $query->innerJoin('field_data_endpoints', 'e1', 'e1.entity_type = e.entity_type AND e1.entity_id = e.entity_id');
    $query->condition('e1.endpoints_entity_type', 'character_editor_character');
    $query->condition('e1.endpoints_entity_id', $this->w->getIdentifier());
    // Match the row ids.
    $entity_types = array();
    $db_or = db_or();
    foreach($row_ids as $row_id){
      list($row_entity_type, $row_entity_id) = explode(':', $row_id);
      $entity_types[$row_entity_type][] = $row_entity_id;
    }
    foreach($entity_types as $entity_type => $entity_ids){
      $db_and = db_and();
      $db_and->condition('e.endpoints_entity_type', $entity_type);
      $db_and->condition('e.endpoints_entity_id', $entity_ids, 'IN');
      $db_or->condition($db_and);
    }
    $query->condition($db_or);
    // Fetch the relation data
    $query->leftJoin('field_data_field_character_state_data', 'data', 'data.entity_id = e.entity_id');
    $query->addField('data', 'field_character_state_data_value', 'data');
    $query->leftJoin('field_data_field_character_state_flag', 'flag', 'flag.entity_id = e.entity_id');
    $query->addField('flag', 'field_character_state_flag_value', 'flag');
    $query->leftJoin('field_data_field_character_state_pass', 'pass', 'pass.entity_id = e.entity_id');
    $query->addField('pass', 'field_character_state_pass_value', 'pass');
    $results = $query->execute()->fetchAll();
    return $results;
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
    $error = FALSE;
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
          $var = $parser->evaluate($row, $order);
          $value = '';
          if($var->type() == 'range'){
            $value = $var->value();
            $value = $value['min']->value() . ' - ' . $value['max']->value();
          }else if($var->type() == 'list'){
            $value = $var->asExpression();
          }else{
            $value = $var->value();
          }
          $result[$row_id] = $value;
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
   * hasCondition
   * 
   * Return true if this character has a condition
   */
  public function hasCondition(){
    if($this->isGroup()){return FALSE;}
    return $this->w->field_char_condition->value() != '';
  }

  /**
   * getConditions
   *
   * Given a list of rows of the form:
   * $row_id => array(
   *   $character_id => value
   * )
   *
   * Return a matching list of boolean equal to the evaluation of the character's condition.
   * Note that if a row is not included in the result, then it should be assumed the condition
   * evaluated to TRUE.
   */
  public function getConditions($rows, $order){
    if(!$this->hasCondition()){return array();}
    $result = array();
    module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
    try{
      $parser = new CharacterExpressionParser($this->w->field_char_condition->value());
    }
    catch(Exception $e){
      return $result;
    }
    foreach($rows as $row_id => $row){
      try{
        $result[$row_id] = ($parser->evaluate($row, $order)->value() !== 0);
      }
      catch(Exception $e){
      }
    }
    return $result;
  }

  /**
   * hasOptions
   *
   * Return TRUE if this character has options to pass on to the
   * slickgrid editor
   */
  public function hasOptions(){
    return $this->w->getBundle() == 'controlled';
  }

  /**
   * getOptions
   *
   * Return the options to pass on to the slickgrid editor
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

  /**
   * getTaxonAggregatedValue
   *
   * Get the aggregated (or 'pass-up') value for a given taxon
   * in the given tree (as returned by CharacterProjectModel::getInheritanceTree)
   *
   * If the taxon has a value just return that value, unless it's tid is in the
   * $force array.
   */
  public function getTaxonAggregatedValue($tid, $tree, $force = array()){
    // Get the value for this tid
    $entity_w = character_editor_wrapper('taxonomy_term', $tid);
    if(!in_array($tid, $force)){
      $values = character_editor_get_character_values($this->w, $entity_w);
      if(strlen($values[0]['data'][0]['value']) > 0){return $values[0]['data'][0]['value'];}
    }
    $value = '';
    // Recurse to children tids
    foreach($tree['tree'][$tid]['children'] as $child_tid){
      $child_value = $this->getTaxonAggregatedValue($child_tid, $tree, $force);
      $value = $this->mergeValues($value, $child_value);
    }
    // Get the specimen values. Using proper entity API would be too expensive here, so
    // we query directly.
    $values = db_query("
          SELECT DISTINCT s.field_character_state_data_value
            FROM {field_data_field_character_state_data} s
      INNER JOIN {field_data_endpoints} e_spec
              ON e_spec.entity_id = s.entity_id
      INNER JOIN {field_data_endpoints} e_char
              ON e_char.entity_id = s.entity_id
      INNER JOIN {field_data_field_taxonomic_name} t
              ON t.entity_id = e_spec.endpoints_entity_id
           WHERE e_spec.endpoints_entity_type = 'node'
             AND e_char.endpoints_entity_type = 'character_editor_character'
             AND e_char.endpoints_entity_id = :character_id
             AND t.field_taxonomic_name_tid = :tid
             AND t.entity_type = 'node'
             AND t.bundle = 'specimen_observation'
      ", array(
      ':character_id' => $this->w->getIdentifier(),
      ':tid' => $tid
    ))->fetchCol();
    foreach($values as $spec_value){
      $value = $this->mergeValues($value, $spec_value);
    }
    return $value;
  }

  /**
   * getPassDownEntities
   *
   * Given a taxon, return all the entity ids whose value should be set to that
   * taxon's value through pass-down inheritance
   */
  public function getPassDownEntities($tid, $project_id, $tree, $ignore = array()){
    $result = array();
    // Include this taxon?
    if(!in_array($tid, $ignore)){
      $entity_w = character_editor_wrapper('taxonomy_term', $tid);
      $values = character_editor_get_character_values($this->w, $entity_w);
      if(!isset($values[0]['data'][0]['value']) || strlen($values[0]['data'][0]['value']) == 0 || (isset($values[0]['flag'][0]['value']) && $values[0]['flag'][0]['value'] == 'inherited')){
        $result[] = 'taxonomy_term:' . $tid;
      }
    }
    // Recurse to children tids
    foreach($tree['tree'][$tid]['children'] as $child_tid){
      $child_result = $this->getPassDownEntities($child_tid, $project_id, $tree, $ignore);
      $result = array_merge($result, $child_result);
    }
    // Check specimen
    $specimens = db_query("
          SELECT DISTINCT ps.field_char_proj_specimen_nid
            FROM {field_data_field_char_proj_specimen} ps
      INNER JOIN {field_data_field_taxonomic_name} tn
              ON tn.entity_id = ps.field_char_proj_specimen_nid
       LEFT JOIN ({field_data_endpoints} e_spec
                  INNER JOIN {field_data_endpoints} e_char
                          ON e_char.entity_id = e_spec.entity_id
                         AND e_char.endpoints_entity_type = 'character_editor_character'
                         AND e_char.endpoints_entity_id = :character_id
                  INNER JOIN {field_data_field_character_state_data} d
                          ON d.entity_id = e_char.entity_id
                   LEFT JOIN {field_data_field_character_state_flag} f
                          ON f.entity_id = e_char.entity_id
                 )
              ON e_spec.endpoints_entity_id = ps.field_char_proj_specimen_nid
             AND e_spec.endpoints_entity_type = 'node'
           WHERE ps.entity_id = :project_id
             AND tn.entity_type = 'node'
             AND tn.field_taxonomic_name_tid = :tid
             AND (d.field_character_state_data_value IS NULL
                  OR d.field_character_state_data_value = ''
                  OR f.field_character_state_flag_value = 'inherited')
   ", array(
      ':character_id' => $this->w->getIdentifier(),
      ':tid' => $tid,
      ':project_id' => $project_id
    ))->fetchCol();
    foreach($specimens as $nid){
      $result[] = 'node:' . $nid;
    }
    return $result;
  }

  /**
   * mergeValues
   *
   * Helper function used when evaluating inheritance:
   * Merge too values into one encompassing both values
   * for this character.
   *
   */
  private function mergeValues($v1, $v2){
    if(strlen($v1) == 0){
      return $v2;
    }else if(strlen($v2) == 0){return $v1;}
    // Merge numeric values
    if($this->isNumeric()){
      if(preg_match('/^\s*(-?\d+(?:\.\d+)?)\s*-\s*(-?\d+(?:\.\d+)?)\s*$/', $v1, $matches)){
        $min = $matches[1] > $matches[2] ? $matches[2] : $matches[1];
        $max = $matches[1] > $matches[2] ? $matches[1] : $matches[2];
      }else{
        $min = floatval($v1);
        $max = floatval($v1);
      }
      if(preg_match('/^\s*(-?\d+(?:\.\d+)?)\s*-\s*(-?\d+(?:\.\d+)?)\s*$/', $v2, $matches)){
        $min = $matches[1] < $min ? $matches[1] : $min;
        $min = $matches[2] < $min ? $matches[2] : $min;
        $max = $matches[1] > $max ? $matches[1] : $max;
        $max = $matches[2] > $max ? $matches[2] : $max;
      }else{
        $min = $v2 < $min ? $v2 : $min;
        $max = $v2 > $max ? $v2 : $max;
      }
      if($min == $max){
        return $min;
      }else{
        return "$min - $max";
      }
    }
    // Merge controlled characters
    if($this->isControlled()){
      module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
      try{
        $e = new CharacterExpressionParser("[$v1|$v2]");
        return $e->evaluate()->asExpression();
      }
      catch(Exception $e){
        return '';
      }
    }
    // Merge text characters?
    if($this->isTextual()){return $v1 . ' ' . $v2;}
    return '';
  }
}