<?php

/**
 * This files contains helpers for managing relations
 * across character projects, groups and characters
 */
/**
 * character_editor_get_characters
 *
 * Given an entity metadata wrapper return an array 
 * containing all the entities (as wrappers) 
 * that are children of that entity via a 'character' 
 * relation.
 *
 * The entities are organised through relations as:
 * 
 * project -> group -> character | group
 *
 * So typically this would be used to get a project's groups,
 * and a group's characters.
 * 
 * It is possible to get the parents, rather than the children, of the
 * entity by setting $source to FALSE - however it is clearer to
 * use character_editor_get_parent() for that.
 * 
 * XXX now that we have sub-groups, do we want depth here?
 */
function character_editor_get_characters($wrapper, $source = TRUE){
  $direction = $source ? 0 : 1;
  $result = relation_query($wrapper->type(), $wrapper->getIdentifier(), $direction)->entityCondition('bundle', 'character')->execute();
  if(empty($result)){return array();}
  $relations = relation_load_multiple(array_keys($result));
  $characters = array();
  foreach($relations as $relation){
    $endpoints = field_get_items('relation', $relation, 'endpoints');
    foreach($endpoints as $endpoint){
      if($endpoint['entity_type'] == $wrapper->type() && $endpoint['entity_id'] == $wrapper->getIdentifier()){
        continue;
      }
      try{
        $character = entity_load_single($endpoint['entity_type'], $endpoint['entity_id']);
        $characters[] = entity_metadata_wrapper($endpoint['entity_type'], $character);
      }
      catch(Exception $e){
      }
    }
  }
  return $characters;
}

/**
 * character_editor_get_parents
 * 
 * Given an entity metadata wrapper, returns the entities
 * (as wrappers) that are parent of that entity via a 
 * 'character' relation.
 * 
 * The entities are organised through relations as:
 * 
 * project -> group -> character | group
 *
 * So typically this would be used to get a character's groups,
 * or a group's projects.
 * 
 * XXX now that we have subgroups, do we want to return all ancestors?
 */
function character_editor_get_parents($wrapper){
  return character_editor_get_characters($wrapper, FALSE);
}

/**
 * character_editor_associate_character
 * 
 * Associate two characters via a relation. Note that the order of
 * relation is:
 * 
 * project -> group -> character | group
 * 
 * So use character_editor_set_character($project, $group) and
 * character_editor_set_character($group, $character)
 * 
 * Note that the entities must have an ID before a relation can be
 * created - so the entities must be saved.
 * 
 * Returns the created relation.
 */
function character_editor_associate_character(EntityDrupalWrapper $parent_w, EntityDrupalWrapper $child_w){
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
  return $relation;
}

/**
 * character_editor_association_exists
 * 
 * Checks if two characters already have a relation. Note that
 * the order of the characters is not important.
 * 
 * project <-> group <-> character | group
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
        $endpoint_entity = entity_load_single($endpoint['entity_type'], $endpoint['entity_id']);
        $endpoint_entity_w = entity_metadata_wrapper($endpoint['entity_type'], $endpoint_entity);
        $exclude_rec = $exclude;
        $exclude_rec[] = $wrap_1;
        if(character_editor_association_exists($endpoint_entity_w, $wrap_2, $max_depth - 1, $exclude_rec)){return $relation;}
      }
    }
  }
  return FALSE;
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
 * If $entity_w is defined, then only return values linked to that term/node.
 */
function character_editor_get_character_values(EntityDrupalWrapper $character_w, EntityDrupalWrapper $entity_w = NULL){
  $values = array();
  $result = relation_query($character_w->type(), $character_w->getIdentifier())->entityCondition('bundle', 'character_state')->execute();
  if(empty($result)){return array();}
  $relations = relation_load_multiple(array_keys($result));
  foreach($relations as $relation){
    $relation_w = entity_metadata_wrapper('relation', $relation);
    $endpoints = field_get_items('relation', $relation, 'endpoints');
    foreach($endpoints as $endpoint){
      if($endpoint['entity_type'] == $character_w->type() && $endpoint['entity_id'] == $character_w->getIdentifier()){
        continue;
      }
      if(isset($entity_w) && ($entity_w->getIdentifier() != $endpoint['entity_id'] || $entity_w->type() != $endpoint['entity_type'])){
        continue;
      }
      $end_term_w = entity_metadata_wrapper($endpoint['entity_type'], entity_load_single($endpoint['entity_type'], $endpoint['entity_id']));
      $values[] = array(
        'entity_w' => $end_term_w,
        'data' => field_get_items('relation', $relation, 'field_character_state_data'),
        'relation_w' => $relation_w
      );
    }
  }
  return $values;
}
