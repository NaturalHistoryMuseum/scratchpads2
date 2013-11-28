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
 * project -> group -> character
 *
 * So typically this would be used to get a project's groups,
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
 * project -> group -> character
 *
 * So typically this would be used to get a character's groups,
 * or a group's projects.
 * 
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
 * project -> group -> character
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
 * project <-> group <-> character
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
      if($endpoint['entity_type'] == $wrap_2->type() && $endpoint['entity_id'] == $wrap_2->getIdentifier()){return TRUE;}
      if($max_depth > 1){
        $endpoint_entity = entity_load_single($endpoint['entity_type'], $endpoint['entity_id']);
        $endpoint_entity_w = entity_metadata_wrapper($endpoint['entity_type'], $endpoint_entity);
        $exclude_rec = $exclude;
        $exclude_rec[] = $wrap_1;
        if(character_editor_association_exists($endpoint_entity_w, $wrap_2, $max_depth - 1, $exclude_rec)){return TRUE;}
      }
    }
  }
  return FALSE;
}
