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
   * label
   *
   * Return this project's label
   */
  public function label(){
    return $this->w->label();
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
   * This method returns:
   * - The list of availables characters for calculating expressions,
   *    formatted as a tuple defining display name, type and available states ;
   * - The map from the character display name to the character id ;
   * - The map from the state display name to the state id.
   * 
   */
  public function getAvailableVariables(){
    $tree = $this->getTree();
    $list = array();
    $character_map = array();
    $state_map = array();
    $found = array();
    foreach($tree as $item){
      if(in_array($item->model()->wrapper()->getIdentifier(), $found)){
        // Characters can be in the tree multiple times, but the variable value is always the same.
        continue;
      }
      $found[] = $item->model()->wrapper()->getIdentifier();
      if(!$item->model()->isGroup() && !$item->model()->isComputed()){
        $name = preg_replace('/[{}]/', ' ', $item->model()->label());
        $count = 1;
        while(isset($character_map[$name])){
          $name = preg_replace('/[{}]/', ' ', $item->model()->label()) . ' (' + $count + ')';
          $count++;
        }
        $character_map[$name] = $item->model()->wrapper()->getIdentifier();
        $name = '{' . $name . '}';
        $type = t(ucfirst($item->model()->wrapper()->getBundle()));
        $states = array();
        if($item->model()->isControlled()){
          $item_states = $item->model()->getControlledMap();
          foreach($item_states as $id => $label){
            $item_label = str_replace('\'', ' ', $label);
            $count = 1;
            while(isset($state_map[$item_label])){
              $item_label = str_replace('\'', ' ', $label) . " (" . $count . ")";
              $count++;
            }
            $states[] = "'$item_label'";
            $state_map[$item_label] = $id;
          }
        }
        $list[] = array(
          $name,
          $type,
          $states
        );
      }
    }
    return array(
      'list' => $list,
      'character_map' => $character_map,
      'state_map' => $state_map
    );
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
    // Index the rows
    $ids = array();
    foreach($rows as $index => $row){
      $ids[$row->id] = $index;
    }
    // Determine collapsed rows & query limits
    $collapsed = array();
    if(isset($_GET['character_entity_field'])){
      $collapsed = explode(',', $_GET['character_entity_field']);
    }
    if(arg(0) == 'character_editor' && arg(1) == 'get' && arg(2) == 'data'){
      $offset = arg(5);
      $limit = arg(6);
    }else{
      $offset = 0;
      $limit = 50;
    }
    // Build the tree
    $tree = array();
    foreach($ids as $id => $row_index){
      $list = array();
      $pid = $id;
      while($pid && !preg_match('/:0$/', $pid)){
        if(isset($ids[$pid])){
          $list[] = $pid;
          $pid = $rows[$ids[$pid]]->parent;
        }else{
          $pid = 0;
        }
      }
      $list = array_reverse($list);
      $ptr = & $tree;
      foreach($list as $pid){
        if(!isset($ptr[$pid])){
          $ptr[$pid] = array(
            'value' => &$rows[$ids[$pid]],
            'children' => array(),
            'collapsed' => in_array($pid, $collapsed)
          );
        }
        $ptr = & $ptr[$pid]['children'];
      }
      unset($ptr);
    }
    // Flatten and prune it
    $rows = $this->flattenPruneTree($tree, 0, $offset, $limit);
    $row_ids = array();
    foreach($rows as $row){
      $row_ids[] = $row->id;
    }
    // Fetch the values
    $characters = array();
    $state_order = array();
    foreach($this->getTree() as $item_key => $tree_item){
      $characters = array_merge_recursive($characters, $tree_item->getValues($row_ids));
      if($tree_item->model()->isControlled()){
        $state_order = array_merge($state_order, array_keys($tree_item->model()->getControlledMap()));
      }
    }
    // Compute values and conditions
    $variables = $this->rowsToVariables($characters);
    foreach($this->getTree() as $item_key => $tree_item){
      if($tree_item->model()->isComputed()){
        $characters = $tree_item->updateComputedValues($characters, $variables, $state_order);
      }
      $characters = $tree_item->updateConditions($characters, $variables, $state_order);
    }
    // Assign them to the rows
    foreach($rows as $row){
      if(isset($characters[$row->id]) && is_array($characters[$row->id])){
        foreach($characters[$row->id] as $field => $value){
          $row->{$field} = drupal_json_encode($value);
        }
      }
    }
    return $rows;
  }

  /**
   * flattenPruneTree
   *
   * Given a tree of rows, prune and flatten the tree.
   */
  function flattenPruneTree($tree, $depth, &$offset, &$limit){
    if($limit == 0){return array();}
    $result = array();
    foreach($tree as $item){
      $item['value']->depth = $depth;
      $item['value']->children = count($item['children']);
      $item['value']->collapsed = $item['collapsed'];
      if($offset == 0){
        $result[] = $item['value'];
        $limit--;
      }else{
        $offset--;
      }
      if(!$item['collapsed']){
        $result = array_merge($result, $this->flattenPruneTree($item['children'], $depth + 1, $offset, $limit));
      }
      if($limit == 0){return $result;}
    }
    return $result;
  }

  /**
   * rowsToVariables
   * 
   * Transform rows of values (as returned by CharacterTreeNode::getValues())
   * into variables usabled by the expression parser
   */
  private function rowsToVariables($rows){
    $tree = $this->getTree();
    $variables = array();
    foreach($rows as $row_id => $row){
      $variables[$row_id] = array();
      foreach($row as $char_id => $char_value){
        $tree_id = preg_replace('/^character_(\d+)_(\d+)$/', 'character_editor_character:$1:$2', $char_id);
        $stripped_id = preg_replace('/^character_(\d+)_\d+$/', '$1', $char_id);
        $data = isset($char_value['variable']) ? $char_value['variable'] : $char_value['data'];
        if(empty($data) && !$tree[$tree_id]->model()->isTextual()){
          continue;
        }else if($tree[$tree_id]->model()->isControlled()){
          $data = array(
            'type' => 'expr',
            'value' => $data
          );
        }else if($tree[$tree_id]->model()->isNumeric() == 'numeric'){
          if(preg_match('/^\s*(-?\s*[0-9.]+)\s*-\s*(-?\s*[0-9.]+)\s*?$/', $data, $matches)){
            $data = array(
              'min' => $matches[1] + 0,
              'max' => $matches[2] + 0
            );
          }else{
            $data = $data + 0;
          }
        }
        $variables[$row_id][$stripped_id] = $data;
      }
    }
    return $variables;
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

  /**
   * getInheritanceTree
   *
   * This methods returns a structure that defines:
   *
   * - tree: A flat array, indexed by term id, representing all the
   *         taxons in the project, each element defining the term id
   *         (tid), the parent term id (parent - iff included in the
   *         project), the list of character ids for which this taxon
   *         has the pass up flag (up) and the list of character ids
   *         for which this taxon has the pass down flag (down) ;
   *
   * - up:   An array of character ids to array of tids which have the pass
   *         up flag for that character id. The array is organised with
   *         the deepest tids first ;
   *
   * - down: An array of character ids to array of tids which have the pass
   *         down flag for that character id. The array is organised with
   *         the shallowest tids first.
   *
   * Note that this tree does *not* include the specimen.
   */
  public function getInheritanceTree(){
    // Build the tree
    $tree = array();
    $up = array();
    $down = array();
    $taxons = field_get_items('character_editor_project', $this->w->raw(), 'field_taxonomic_name');
    foreach($taxons as $t){
      $tree[$t['tid']] = array(
        'tid' => $t['tid'],
        'parent' => 0,
        'children' => array(),
        'up' => array(),
        'down' => array(),
        'depth' => 0
      );
    }
    foreach($tree as $tid => $tree_item){
      $parent_tid = db_query("
        SELECT parent
          FROM {taxonomy_term_hierarchy}
         WHERE tid = :tid
      ", array(
        ':tid' => $tid
      ))->fetchField();
      if($parent_tid && isset($tree[$parent_tid])){
        $tree[$tid]['parent'] = $parent_tid;
        $tree[$parent_tid]['children'][] = $tid;
      }
    }
    foreach($tree as $tid => $tree_item){
      if($tree_item['parent'] == 0){
        continue;
      }
      $p = $tree_item['parent'];
      while($p){
        $tree[$tid]['depth']++;
        $p = $tree[$p]['parent'];
      }
    }
    // Get all cells that have been marked as 'pass up' or 'pass down' ;
    $character_tree = $this->getTree();
    foreach($character_tree as $tree_node){
      $character_id = $tree_node->model()->wrapper()->getIdentifier();
      $term_pass = db_query("
        SELECT e_tax.endpoints_entity_id, f.field_character_state_pass_value
          FROM {field_data_field_character_state_pass} f
        INNER JOIN {field_data_endpoints} e_char
                ON f.entity_id = e_char.entity_id
        INNER JOIN {field_data_endpoints} e_tax
                ON f.entity_id = e_tax.entity_id
             WHERE e_char.endpoints_entity_type = 'character_editor_character'
               AND e_char.endpoints_entity_id = :character_id
               AND e_tax.endpoints_entity_type = 'taxonomy_term'
      ", array(
        ':character_id' => $character_id
      ))->fetchAllKeyed();
      foreach($term_pass as $tid => $pass){
        if($pass == 'up' || $pass == 'both'){
          $tree[$tid]['up'][] = $character_id;
          $up[$character_id][$tree[$tid]['depth']][] = $tid;
        }
        if($pass == 'down' || $pass == 'both'){
          $tree[$tid]['down'][] = $character_id;
          $down[$character_id][$tree[$tid]['depth']][] = $tid;
        }
      }
    }
    foreach($up as $character_id => $tids){
      krsort($tids);
      $up[$character_id] = call_user_func_array('array_merge', $tids);
    }
    foreach($down as $character_id => $tids){
      ksort($tids);
      $down[$character_id] = call_user_func_array('array_merge', $tids);
    }
    return array(
      'tree' => $tree,
      'up' => $up,
      'down' => $down
    );
  }
}