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
    // Get the views with taxonomy/term/%/[xxx] paths.
    $views = views_get_applicable_views('uses hook menu');
    foreach($views as $data){
      list($view, $display_id) = $data;
      $result = $view->execute_hook_menu($display_id);
      if(is_array($result)){
        foreach($result as $path => $menu){
          if(strpos($path, 'taxonomy/term/%/') === 0 && (MENU_LOCAL_TASK & $menu['type'])){
            $values[str_replace('taxonomy/term/%/', '', $path)] = $menu['title'];
          }
        }
      }
      $view->destroy();
    }
    return $values;
  }

  function execute($tab){
    foreach($this->get_contexts($tab) as $context){
      $this->condition_met($context);
    }
  }
}
