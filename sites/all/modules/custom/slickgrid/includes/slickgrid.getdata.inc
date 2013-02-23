<?php

/**
 * Return the data for a dynamically loaded SlickGrid.
 * 
 * Note, this function may need to be tweaked if we wish to accept arguments
 * to a view.
 */
function slickgrid_get_data($view_name, $offset){
  $view = views_get_view($view_name);
  $view->render();
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  echo drupal_json_encode($view->data);
  ajax_footer();
}