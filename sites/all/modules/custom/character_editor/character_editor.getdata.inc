<?php

/**
 * Return the data for a dynamically loaded SlickGrid.
 * 
 * Note, this function may need to be tweaked if we wish to accept arguments
 * to a view.
 */
function character_editor_get_data($entity_id){
  $name = character_editor_get_default_view();
  $display_id = 'default';
  $args = array(
    $entity_id
  );
  // Load the view and render it.
  if(($view = views_get_view($name)) != FALSE){
    $view->execute_display($display_id, $args);
  }
  echo drupal_json_encode(array(
    'data' => empty($view->data) ? array() : $view->data
  ));
  ajax_footer();
}