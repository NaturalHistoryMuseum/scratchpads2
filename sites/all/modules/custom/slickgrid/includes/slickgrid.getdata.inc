<?php

/**
 * Return the data for a dynamically loaded SlickGrid.
 * 
 * Note, this function may need to be tweaked if we wish to accept arguments
 * to a view.
 */
function slickgrid_get_data($view_name, $offset, $sortcol = FALSE, $sortdir = 'asc'){
  $view = views_get_view($view_name);
  if($sortcol){
    $sort = array(
      'id' => $sortcol,
      'table' => 'node',
      'field' => $sortcol,
      'relationship' => 'none',
      'group_type' => 'group',
      'ui_name' => '',
      'order' => strtoupper($sortdir),
      'exposed' => FALSE
    );
    $key = $sortcol;
    if($sortcol != 'title'){
      // Load the field so that we can see what type of field it is
      $field = field_info_field($sortcol);
      if($field){
        foreach($sort['id'] = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
          foreach($more_info as $key => $name){
            if($key != 'format'){
              $sort['id'] = $name;
              $sort['field'] = $name;
              $sort['table'] = $table;
              break;
            }
          }
        }
      }
    }
    $view->display['default']->display_options['sorts'][$key] = $sort;
  }
  $view->render();
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  echo drupal_json_encode($view->data);
  ajax_footer();
}