<?php

/**
 * This files contains helpers for managing relations
 * across character projects, groups and characters.
 *
 * The functions here should be considered as low-level
 * functions, and are often wrapped in higher level models
 * such as the CharacterProjectModel class
 *
 * The structure of relations defines a tree of
 * characters with a project at it's root.
 *
 * - The project is always at the root of the tree ;
 * - There is only one project per tree ;
 * - Each character belongs to only one tree ;
 * - A character can be present several times in the
 *   tree. Each presence is considered a separate instance
 *   so that the structure remains a tree ;
 * - Groups are just special kinds of characters.
 *
 * The user interface does not allow users to set non-group
 * characters as parents ; the rest of the code however
 * would (and should) allow that, so this decision can
 * be reverted easily.
 *
 * Unless otherwise specified, functions that take a
 * parent and child entity wrapper will have the first
 * as the parent and the second as the child.
 */
/**
 * character_editor_get_characters
 *
 * Given an entity metadata wrapper return an array 
 * containing all the entities (as wrappers) 
 * that are children of that entity via a 'character' 
 * relation, sorted using the field_character_weight
 * field.
 *
 * Typically this would be used to get a project's groups,
 * and a group's characters.
 * 
 * It is possible to get the parents, rather than the children, of the
 * entity by setting $source to FALSE - however it is clearer to
 * use character_editor_get_parent() for that.
 * 
 */
function character_editor_get_characters($wrapper, $source = TRUE){
  $direction = $source ? 0 : 1;
  $result = relation_query($wrapper->type(), $wrapper->getIdentifier(), $direction)->entityCondition('bundle', 'character')->execute();
  if(empty($result)){return array();}
  $relations = relation_load_multiple(array_keys($result));
  $characters = array();
  $order = array();
  foreach($relations as $relation){
    $endpoints = field_get_items('relation', $relation, 'endpoints');
    foreach($endpoints as $endpoint){
      if($endpoint['entity_type'] == $wrapper->type() && $endpoint['entity_id'] == $wrapper->getIdentifier()){
        continue;
      }
      if(isset($relation->field_character_weight)){
        $weight = field_get_items('relation', $relation, 'field_character_weight');
        $weight = empty($weight[0]['value']) ? 0 : $weight[0]['value'];
      }else{
        $weight = 0;
      }
      try{
        $characters[] = character_editor_wrapper($endpoint['entity_type'], $endpoint['entity_id']);
        $order[] = $weight;
      }
      catch(Exception $e){
      }
    }
  }
  array_multisort($order, $characters);
  return $characters;
}

/**
 * character_editor_get_parents
 * 
 * Given an entity metadata wrapper, returns the entities
 * (as wrappers) that are parent of that entity via a 
 * 'character' relation.
 * 
 * Typically this would be used to get a character's groups,
 * or a group's projects.
 * 
 */
function character_editor_get_parents($wrapper){
  return character_editor_get_characters($wrapper, FALSE);
}

/**
 * character_editor_get_character_project
 *
 * Given an entity metadata wrapper, returns the
 * project the given character belongs to.
 */
function character_editor_get_character_project($entity_w, $exclude = array()){
  // Note: the character may be at several places, but it's always
  // in the same tree. We have to be carefull of loops however!
  if($entity_w->type() == 'character_editor_project'){return $entity_w;}
  $id = $entity_w->type() . ':' . $entity_w->getIdentifier();
  if(in_array($id, $exclude)){return FALSE;}
  $exclude[] = $id;
  $parents = character_editor_get_parents($entity_w);
  foreach($parents as $parent_w){
    $result = character_editor_get_character_project($parent_w);
    if($result){return $result;}
  }
  return FALSE;
}

/**
 * character_editor_get_tree
 *
 * Given an entity metadata wrapper, return the tree of
 * entities of which this entity is the root, and such
 * that the tree is build using 'character' relations.
 *
 * If $mode == 'tree', then the returned structure is:
 *
 * array(
 *   'wrapper' => <entity wrapper>,
 *   'relation' => <relation object that links to parent>
 *   'children' => array(...)
 * )
 *
 * And it includes the root item provided.
 *
 * If $mode == 'flat', then the returned structure is:
 *
 * array(
 *   <entitytype:entityid:relationid> => array(
 *     'wrapper' => <entity wrapper>,
 *     'relation' => <relation object that links to parent>
 *     'parent_id' => <parent entitytype:entityid:relationid>,
 *     'depth' => <depth in tree>,
 *     'weight' => <weight value on the relation that links to parent>
 *   ),
 *   ...
 * )
 *
 * And it excludes the root item provided, such that the parent_id of the initial children
 * is set to 0. Also note that relationid is the id of the relation from the parent to the
 * item.
 *
 * Items may be present several times in the tree, however
 * loops are prevented. Siblings in the tree are sorter according to the
 * field_character_weight field.
 */
function character_editor_get_tree($entity_w, $mode = 'flat', $parent_id = 0, $exclude = array()){
  // Build the list of children
  $children = character_editor_get_characters($entity_w);
  $accepted = array();
  $exclude[] = $entity_w->type() . ':' . $entity_w->getIdentifier();
  foreach($children as $child_entity_w){
    if(!in_array($child_entity_w->type() . ':' . $child_entity_w->getIdentifier(), $exclude)){
      $accepted[] = $child_entity_w;
    }
  }
  if($mode == 'tree'){
    $tree = array(
      'wrapper' => $entity_w,
      'children' => array()
    );
    foreach($accepted as $child_w){
      $tree['children'] = array_merge($tree['children'], character_editor_get_tree($child_w, $exclude));
    }
    return $tree;
  }else{
    $flat = array();
    $relation_id = 0;
    $weight = 0;
    if($parent_id){
      $parent_w = character_editor_wrapper($parent_id);
      $relation = character_editor_association_exists($parent_w, $entity_w);
      if($relation){
        $relation_id = $relation->rid;
        $weight = field_get_items('relation', $relation, 'field_character_weight');
        $weight = empty($weight[0]['value']) ? 0 : $weight[0]['value'];
      }
    }
    $id = $entity_w->type() . ':' . $entity_w->getIdentifier() . ':' . $relation_id;
    if($parent_id){
      $flat[$id] = array(
        'wrapper' => $entity_w,
        'parent_id' => $parent_id,
        'depth' => -1,
        'weight' => $weight
      );
    }
    foreach($accepted as $child_w){
      $flat = array_merge($flat, character_editor_get_tree($child_w, 'flat', $id, $exclude));
    }
    foreach($flat as $key => $item){
      if(isset($parent_w) && $parent_w){
        $flat[$key]['depth']++;
      }else if($item['parent_id'] == $id){
        $flat[$key]['parent_id'] = 0;
      }
    }
    return $flat;
  }
}

/**
 * character_editor_associate_character
 * 
 * Associate two characters (or a project and a character) via a relation. 
 * 
 * For instance use character_editor_set_character($project, $group) and
 * character_editor_set_character($group, $character)
 * 
 * Note that the entities must have an ID before a relation can be
 * created - so the entities must be saved.
 * 
 * Returns the created/loaded relation.
 */
function character_editor_associate_character(EntityDrupalWrapper $parent_w, EntityDrupalWrapper $child_w){
  $relation = character_editor_association_exists($parent_w, $child_w);
  if(!$relation){
    $relation = relation_create('character', array(
      0 => array(
        'entity_type' => $parent_w->type(),
        'entity_id' => $parent_w->getIdentifier()
      ),
      1 => array(
        'entity_type' => $child_w->type(),
        'entity_id' => $child_w->getIdentifier()
      )
    ));
    relation_save($relation);
  }
  return $relation;
}

/**
 * character_editor_detatch_character
 *
 * Remove the relations of a given character to it's
 * direct parents.
 *
 * If $parent_w is specified, then only remove the relation
 * to that particular parent (if it exists).
 *
 * If $keep is TRUE, then ensure the character remains in the
 * project by re-attaching it to the project root (even if that
 * was the parent it was just removed from) if it were to have
 * no parent left.
 */
function character_editor_detach_character($character_w, $parent_w = NULL, $keep = FALSE){
  if($parent_w === NULL){
    $parents = character_editor_get_parents($character_w);
  }else{
    $parents = array(
      $parent_w
    );
  }
  $project_w = FALSE;
  if($keep){
    $project_w = character_editor_get_character_project($character_w);
  }
  foreach($parents as $parent_w){
    $relation = character_editor_association_exists($character_w, $parent_w);
    if($relation){
      relation_delete($relation->rid);
    }
  }
  if($project_w){
    $parents = character_editor_get_parents($character_w);
    if(empty($parents)){
      character_editor_associate_character($project_w, $character_w);
    }
  }
}

/**
 * character_editor_association_exists
 * 
 * Checks if two characters already have a relation. For this
 * function the order of the entities is not important (so
 * calling $project, $group and $group, $project would return
 * the same thing).
 *
 * Returns the first level of the relation on success
 * (this is mostly usefull when max_depth = 1) and FALSE otherwise.
 */
function character_editor_association_exists(EntityDrupalWrapper $wrap_1, EntityDrupalWrapper $wrap_2, $max_depth = 1, $exclude = array()){
  $result = relation_query($wrap_1->type(), $wrap_1->getIdentifier())->entityCondition('bundle', 'character')->execute();
  if(empty($result)){return FALSE;}
  $relations = relation_load_multiple(array_keys($result));
  foreach($relations as $relation){
    $endpoints = field_get_items('relation', $relation, 'endpoints');
    foreach($endpoints as $endpoint){
      if($endpoint['entity_type'] == $wrap_1->type() && $endpoint['entity_id'] == $wrap_1->getIdentifier()){
        continue;
      }
      $excluded = FALSE;
      foreach($exclude as $exclude_entity_w){
        if($endpoint['entity_type'] == $exclude_entity_w->type() && $endpoint['entity_id'] == $exclude_entity_w->getIdentifier()){
          $excluded = TRUE;
          break;
        }
      }
      if($excluded){
        continue;
      }
      if($endpoint['entity_type'] == $wrap_2->type() && $endpoint['entity_id'] == $wrap_2->getIdentifier()){return $relation;}
      if($max_depth > 1){
        $endpoint_entity_w = character_editor_wrapper($endpoint['entity_type'], $endpoint['entity_id']);
        $exclude_rec = $exclude;
        $exclude_rec[] = $wrap_1;
        if(character_editor_association_exists($endpoint_entity_w, $wrap_2, $max_depth - 1, $exclude_rec)){return $relation;}
      }
    }
  }
  return FALSE;
}

/**
 * character_editor_set_character_value
 *
 * Given a character and an entity, set the data and metadata
 * for that tuple.
 *
 * This will update the existing relation, or create a new one
 * if none exist
 */
function character_editor_set_character_value(EntityDrupalWrapper $character_w, EntityDrupalWrapper $entity_w, $data = NULL, $metadata = NULL){
  global $user;
  // Get the relation or create a new one
  $values = character_editor_get_character_values($character_w, $entity_w);
  $values = reset($values);
  // Note that languages are per-project only, all values are language independent.
  $lang = LANGUAGE_NONE;
  if(empty($values) || empty($values['relation_w'])){
    $relation = (object)(array(
      'relation_type' => 'character_state',
      'uid' => $user->uid,
      'created' => time(),
      'changed' => time(),
      'endpoints' => array(
        $lang => array(
          array(
            'entity_type' => $entity_w->type(),
            'entity_id' => $entity_w->getIdentifier(),
            'r_index' => 0
          ),
          array(
            'entity_type' => $character_w->type(),
            'entity_id' => $character_w->getIdentifier(),
            'r_index' => 0
          )
        )
      )
    ));
  }else{
    $relation = $values['relation_w']->raw();
  }
  // Assign the field values
  if($data !== NULL){
    $relation->field_character_state_data = array(
      $lang => array(
        array(
          'value' => $data
        )
      )
    );
  }
  if($metadata !== NULL && isset($metadata['flag'])){
    $relation->field_character_state_flag = array(
      $lang => array(
        array(
          'value' => $metadata['flag']
        )
      )
    );
  }
  if($metadata !== NULL && isset($metadata['pass'])){
    $relation->field_character_state_pass = array(
      $lang => array(
        array(
          'value' => $metadata['pass']
        )
      )
    );
  }
  return entity_save('relation', $relation) !== false;
}

/**
 * character_editor_get_character_values
 *
 * Given a character (as an entity metadata wrapper)
 * return an array of arrays defining:
 * - 'entity_w': an entity wrapper (taxonomic term or node)
 * - 'data': the value of the 'field_character_state_data' field on the relation
 * - 'relation_w' : The relation that links the two
 *
 * Note that the relations are as:
 *
 * term/node -(data field on the relation)-> character
 *
 * $entities may be an item or array of items that can be fetched using character_editor_wrapper.
 * 
 * In which case the returned values are those for the given character and given entities.
 * 
 * Note that this does not evaluate computed or inherited values ; it only returns defined values.
 */
function character_editor_get_character_values(EntityDrupalWrapper $character_w, $entities = NULL){
  // Build the query
  $query = db_select('field_data_endpoints', 'e');
  // Select a relation
  $query->condition('e.entity_type', 'relation');
  $query->condition('e.bundle', 'character_state');
  $query->addField('e', 'entity_id', 'relation_id');
  $query->addField('e', 'endpoints_entity_type', 'entity_type');
  $query->addField('e', 'endpoints_entity_id', 'entity_id');
  // Ensure it has our character as one of it's endpoints
  $query->innerJoin('field_data_endpoints', 'e1', 'e1.entity_type = e.entity_type AND e1.entity_id = e.entity_id');
  $query->condition('e1.endpoints_entity_type', $character_w->type());
  $query->condition('e1.endpoints_entity_id', $character_w->getIdentifier());
  // Ensure it has one of the requested entities as one of it's endpoints
  $entities = character_editor_parse_entities($entities);
  if(!empty($entities)){
    $extra_conditions = array();
    foreach($entities as $limit_elem){
      $extra_conditions[$limit_elem[0]][] = $limit_elem[1];
    }
    // Add another endpoint per entity type in $entities
    $db_or = db_or();
    foreach($extra_conditions as $extra_condition_type => $extra_condition_ids){
      $db_and = db_and();
      $db_and->condition('e.endpoints_entity_type', $extra_condition_type);
      $db_and->condition('e.endpoints_entity_id', $extra_condition_ids);
      $db_or->condition($db_and);
      continue;
      $alias = 'e' . ((string)$count);
      $count++;
      $query->innerJoin('field_data_endpoints', $alias, "$alias.entity_type = e.entity_type AND $alias.entity_id = e.entity_id");
      $db_and = db_and();
      $db_and->condition("$alias.endpoints_entity_type", $extra_condition_type);
      $db_and->condition("$alias.endpoints_entity_id", $extra_condition_ids);
      $db_or->condition($db_and);
    }
    $query->condition($db_or);
  }else{
    // If there are no conditions on the resulting entities, at least ensure we don't return the
    // character itself.
    $db_or = db_or();
    $db_or->condition('e.endpoints_entity_type', $character_w->type(), '<>');
    $db_or->condition('e.endpoints_entity_id', $character_w->getIdentifier(), '<>');
    $query->condition($db_or);
  }
  $results = $query->execute()->fetchAll();
  if(empty($results)){return array();}
  $values = array();
  foreach($results as $row){
    $relation_w = entity_metadata_wrapper('relation', relation_load($row->relation_id));
    $entity_w = entity_metadata_wrapper($row->entity_type, entity_load_single($row->entity_type, $row->entity_id));
    $data = field_get_items('relation', $relation_w->raw(), 'field_character_state_data');
    $flag = field_get_items('relation', $relation_w->raw(), 'field_character_state_flag');
    $pass = field_get_items('relation', $relation_w->raw(), 'field_character_state_pass');
    $values[] = array(
      'entity_w' => $entity_w,
      'relation_w' => $relation_w,
      'data' => $data,
      'flag' => $flag,
      'pass' => $pass
    );
  }
  return $values;
}

/**
 * character_editor_wrapper
 *
 * Returns the entity metadata wrapper of the given entity.
 *
 * The given entity can be defined as:
 *
 * - A string of the form '<entity type>:<entity id>[:<unused>]':
 *   $w = character_editor_wrapper('character_editor_character:123');
 *
 * - An entity type and an entity id:
 *   $w = character_editor_wrapper('character_editor_character', 123);
 *
 * - An entity type, and an entity object:
 *   $w = character_editor_Wrapper('character_editor_character', entity_load_single('character_editor_character', 123));
 *
 * - An entity object alone:
 *   $w = character_editor_wrapper(entity_load_single('character_editor_character', 123));
 *
 * In that last case the function will guess what the type of the entity is by looking at it's field, as either
 * a character_editor_project or a character_editor_character. It will do this by assuming that character projects
 * always have a field_taxonomic_name, while characters never do.
 *
 * If $reset is TRUE, then this also resets the internal cache for the given entity type before loading. Note that if
 * an entity metadatawrapper object is given it will be returned unchanged, regardless of $reset.
 *
 * The function returns FALSE on failure.
 */
function character_editor_wrapper($info, $id = NULL, $reset = FALSE){
  $entity_type = FALSE;
  $entity = FALSE;
  $entity_id = FALSE;
  if(is_string($info)){
    if($id === NULL){
      $parts = explode(':', $info);
      if(count($parts) < 2){return FALSE;}
      $entity_type = $parts[0];
      $entity_id = $parts[1];
    }else if(is_object($id)){
      $entity_type = $info;
      $entity = $id;
    }else{
      $entity_type = $info;
      $entity_id = $id;
    }
  }else if(is_object($info)){
    if($info instanceof EntityMetadataWrapper){return $info;}
    $entity = $info;
    if(property_exists($entity, 'field_taxonomic_name')){
      $entity_type = 'character_editor_project';
    }else{
      $entity_type = 'character_editor_character';
    }
  }
  if(!$entity_type || (!$entity_id && !$entity)){return FALSE;}
  if(!$entity){
    if($reset){
      entity_get_controller($entity_type)->resetCache();
    }
    $entity = entity_load_single($entity_type, $entity_id);
    if(!$entity){return FALSE;}
  }
  return entity_metadata_wrapper($entity_type, $entity);
}

/**
 * character_editor_parse_entities
 * 
 * Similar to character_editor_wrapper, excepts:
 * 
 * - It may take an array of items rather than one item ;
 * - It returns an array of tuples defining <entity_type>:<entity_id>
 */
function character_editor_parse_entities($info, $id = NULL){
  if(is_array($info) && empty($info)){
    // Empty array, we assume this was an empty list of entities
    return array();
  }else if(is_array($info) && !is_array(reset($info))){
    // This is a list of entities
    $result = array();
    foreach($info as $key => $info_item){
      if(is_array($id) && isset($id[$key])){
        $child_id = $id[$key];
      }else if(!is_array($id)){
        $child_id = $id;
      }
      $result = array_merge($result, character_editor_parse_entities($info_item));
    }
    return $result;
  }else if(is_array($info)){
    // We have entity_type, entity_id - just return it.
    return array(
      $info
    );
  }else if(is_string($info)){
    if($id === NULL){
      return array(
        explode(':', $info)
      );
    }else if(is_object($id)){
      $w = character_editor_wrapper($info, $id);
      return array(
        array(
          $w->type(),
          $w->getIdentifier()
        )
      );
    }else{
      return array(
        array(
          $info,
          $id
        )
      );
    }
  }else if(is_object($info)){
    $w = character_editor_wrapper($info, $id);
    return array(
      array(
        $w->type(),
        $w->getIdentifier()
      )
    );
  }
  return FALSE;
}