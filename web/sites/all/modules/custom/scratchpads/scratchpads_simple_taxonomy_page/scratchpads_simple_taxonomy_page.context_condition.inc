<?php

/**
 *
 */
class scratchpads_simple_taxonomy_page_context_condition extends context_condition{

  function condition_values(){
    $values = array();
    foreach(taxonomy_get_vocabularies() as $vocab){
      $values[$vocab->machine_name] = check_plain($vocab->name);
    }
    return $values;
  }

  function execute($term){
    foreach($this->get_contexts($term->vocabulary_machine_name) as $context){
      $this->condition_met($context, $term->vocabulary_machine_name);
    }
  }
}