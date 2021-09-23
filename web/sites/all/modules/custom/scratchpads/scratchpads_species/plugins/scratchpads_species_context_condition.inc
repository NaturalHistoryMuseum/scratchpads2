<?php

/**
 * Expose book properties as a context condition.
 */
class scratchpads_species_context_condition extends context_condition{

  function condition_values(){
    $values = array(
      'overview' => t("Overview")
    );
    foreach(scratchpads_species_get_tabs() as $tab){
      $values[strtolower($tab)] = check_plain($tab);
    }
    return $values;
  }

  function execute($tab){
    foreach($this->get_contexts($tab) as $context){
      $this->condition_met($context);
    }
  }
}
