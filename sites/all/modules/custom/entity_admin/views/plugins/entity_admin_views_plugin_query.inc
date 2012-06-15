<?php

class entity_admin_views_plugin_query extends views_plugin_query_default{

  function get_result_entities($results, $relationship = NULL){
    $base_table = $this->base_table;
    $base_table_alias = $base_table;
    if(!empty($relationship)){
      foreach($this->view->relationship as $current){
        if($current->alias == $relationship){
          $base_table = $current->definition['base'];
          $base_table_alias = $relationship;
          break;
        }
      }
    }
    $table_data = views_fetch_data($base_table);
    // Bail out if the table has not specified the according entity-type.
    if(!isset($table_data['table']['entity type'])){return FALSE;}
    $entity_type = $table_data['table']['entity type'];
    $info = entity_get_info($entity_type);
    $id_alias = $this->get_field_alias($base_table_alias, $info['entity keys']['id']);
    // Assemble the ids of the entities to load.
    $ids = array();
    foreach($results as $key => $result){
      if(isset($result->$id_alias)){
        $ids[$key] = $result->$id_alias;
      }
    }
    // The only change to views_plugin_query_default is to pass entity load to our own function
    $entities = entity_admin_entity_load_simple($entity_type, $ids);
    // Re-key the array by row-index.
    $result = array();
    foreach($ids as $key => $id){
      $result[$key] = isset($entities[$id]) ? $entities[$id] : FALSE;
    }
    return array(
      $entity_type, 
      $result
    );
  }
}
