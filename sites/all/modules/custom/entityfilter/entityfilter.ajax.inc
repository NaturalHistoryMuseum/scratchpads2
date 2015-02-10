<?php

/**
 * @file
 * Callback which request the view 'ckeditor_mentions' and inject the typed value as a realname filter.
 */
function entityfilter_get_suggesions(){
  $typed = $_REQUEST['typed'];
  if(strpos($typed, '[entity') === 0){
    // Remove the "[".
    $typed = substr($typed, 1);
    $typed = array_filter(explode(':', $typed));
    $entity_types = entity_get_info();
    $items = array();
    switch(count($typed)){
      case 1:
        // We've only got 'entity', give a list of entities.
        foreach($entity_types as $type => $entity_info){
          $items[] = array(
            'data' => 'Entity:' . $entity_info['label'],
            'data-textcontent' => 'entity:' . $type
          );
        }
        break;
      case 2:
        // We've got entity:entity-type, try to match on the entity type, if we
        // get only one result, return it.
        foreach($entity_types as $type => $entity_info){
          if(strpos($type, $typed[1]) === 0){
            $items[] = array(
              'data' => 'Entity:' . $entity_info['label'],
              'data-textcontent' => 'entity:' . $type
            );
          }
        }
        break;
      case 3:
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