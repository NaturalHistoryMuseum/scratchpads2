<?php

/**
 * Return the data for a dynamically loaded SlickGrid.
 * 
 * Note, this function may need to be tweaked if we wish to accept arguments
 * to a view.
 */
function slickgrid_get_data($view_name, $offset, $rows, $sortcol = FALSE, $sortdir = 'asc'){
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
  $potential_filters = $_GET;
  unset($potential_filters['q']);
  foreach($potential_filters as $field => $value){
    // Start with a basic text filter - this will work with most fields.
    $filter = array(
      'id' => $field,
      'table' => 'node',
      'field' => $field,
      'relationship' => 'none',
      'group_type' => 'group',
      'ui_name' => '',
      'operator' => 'contains',
      'value' => $value,
      'group' => 1,
      'exposed' => FALSE,
      'expose' => array(),
      'is_grouped' => FALSE,
      'group_info' => array()
    );
    $real_field = field_info_field($field);
    if($real_field){
      foreach($real_field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
        foreach($more_info as $key => $name){
          if($key != 'format'){
            $field = $name;
            $filter['id'] = $name;
            $filter['field'] = $name;
            $filter['table'] = $table;
            break;
          }
        }
      }
      switch($real_field['type']){
        case 'taxonomy_term_reference':
          foreach($real_field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
            foreach($more_info as $key => $name){
              if($key != 'format'){
                $filter['operator'] = 'or';
                $filter['value'] = array_keys(taxonomy_get_term_by_name($value));
                $filter['reduce_duplicates'] = 0;
                $filter['type'] = 'textfield';
                $filter['limit'] = 100;
                $filter['vocabulary'] = 'cod'; // FIXME
                $filter['hierarchy'] = 0;
                break;
              }
            }
          }
          break;
      }
    }
    $view->display['default']->display_options['filters'][$field] = $filter;
  }
  $view->render();
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  // Prevent null from being returned when there are no results.
  if(!$view->data){
    $view->data = array();
  }
  echo drupal_json_encode($view->data);
  ajax_footer();
}