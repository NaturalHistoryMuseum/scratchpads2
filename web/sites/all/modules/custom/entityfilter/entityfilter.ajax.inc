<?php

/**
 * @file
 * Callback which request the view 'ckeditor_mentions' and inject the typed value as a realname filter.
 */
function entityfilter_get_suggesions(){
  if(strpos($_GET['typed'], '[entity') === 0){
    // Remove the "[".
    $_GET['typed'] = substr($_GET['typed'], 1);
    $_GET['typed'] = array_filter(explode(':', $_GET['typed']));
    $entity_types = entity_get_info();
    $items = array();
    switch(count($_GET['typed'])){
      case 1:
      case 2:
        // We've got entity:entity-type, try to match on the entity type, if we
        // get only one result, return it.
        foreach($entity_types as $type => $entity_info){
          if(count($_GET['typed']) == 1 || strpos($type, $_GET['typed'][1]) === 0){
            $items[] = array(
              'data' => 'Entity:' . $entity_info['label'],
              'data-textcontent' => 'entity:' . $type . ':'
            );
          }
        }
        break;
      case 3:
        $aliases = entityfilter_get_entity_aliases();
        if(isset($aliases[$_GET['typed'][1]])){
          $_GET['typed'][1] = $aliases[$_GET['typed'][1]];
        }
        // We search only on the title of the entity, and return the 10 most
        // recent entities.
        // User entity info doesn't contain the label - Grrr
        if(!isset($entity_types[$_GET['typed'][1]]['entity keys']['label'])){
          switch($_GET['typed'][1]){
            case 'user':
              $entity_types[$_GET['typed']['1']]['entity keys']['label'] = 'name';
              break;
          }
        }
        $query = new EntityFieldQuery();
        $results = $query->entityCondition('entity_type', $_GET['typed'][1])->propertyOrderBy($entity_types[$_GET['typed'][1]]['entity keys']['id'], 'DESC')->propertyCondition($entity_types[$_GET['typed'][1]]['entity keys']['label'], db_like($_GET['typed'][2]) . "%", "LIKE")->range(0, 10)->execute();
        $ids = array();
        foreach($results[$_GET['typed'][1]] as $row){
          $ids[] = $row->{$entity_types[$_GET['typed'][1]]['entity keys']['id']};
        }
        $entities = entity_load($_GET['typed'][1], $ids);
        foreach($entities as $id => $entity){
          $items[] = array(
            'data' => 'Entity:' . $entity_types[$_GET['typed'][1]]['label'] . ':' . $entity->{$entity_types[$_GET['typed'][1]]['entity keys']['label']},
            'data-textcontent' => 'entity:' . $_GET['typed'][1] . ':' . $id . ']'
          );
        }
        break;
    }
    drupal_json_output(array(
      'html' => theme('item_list', array(
        'items' => $items
      ))
    ));
    drupal_exit();
  }
}