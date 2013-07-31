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
    switch($sortcol){
      case 'title':
        // Do nothing for title.
        break;
      case 'filename':
        $sort = array(
          'id' => $sortcol,
          'table' => 'file_managed',
          'field' => $sortcol,
          'order' => strtoupper($sortdir)
        );
        break;
      default:
        // Load the field so that we can see what type of field it is
        $field = field_info_field($sortcol);
        if($field){
          switch($field['type']){
            case 'taxonomy_term_reference':
              // We add a relationship to this view.
              foreach($sort['id'] = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
                foreach($more_info as $key => $name){
                  if($key != 'format'){
                    $view->display['default']->display_options['relationships'][$name]['id'] = $name;
                    $view->display['default']->display_options['relationships'][$name]['table'] = $table;
                    $view->display['default']->display_options['relationships'][$name]['field'] = $name;
                    $sort['id'] = 'name';
                    $sort['field'] = 'name';
                    $sort['table'] = 'taxonomy_term_data';
                    $sort['relationship'] = $name;
                    break;
                  }
                }
              }
              break;
            default:
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
    }
    $view->display['default']->display_options['sorts'][$key] = $sort;
  }
  $potential_filters = $_GET;
  unset($potential_filters['q']);
  foreach($potential_filters as $field => $value){
    if(($value = trim($value)) != FALSE){
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
      switch($field){
        case 'filename':
          $filter['table'] = 'file_managed';
          break;
        default:
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
              case 'node_reference':
                $filter['value'] = drupal_map_assoc(db_select('node', 'n')->condition('title', db_like($value) . '%', 'LIKE')->fields('n', array(
                  'nid'
                ))->execute()->fetchCol());
                unset($filter['operator']);
                break;
              case 'taxonomy_term_reference':
                foreach($real_field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
                  foreach($more_info as $key => $name){
                    if($key != 'format'){
                      $filter['operator'] = 'or';
                      $filter['value'] = array_keys(taxonomy_get_terms_by_name_match($value));
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
      }
      $view->display['default']->display_options['filters'][$field] = $filter;
    }
  }
  // We get the referer so that we can add arguments to the view.
  $referer = drupal_parse_url($_SERVER['HTTP_REFERER']);
  if(strpos($referer['path'], '://')){
    $path = parse_url($referer['path']);
    $path = substr($path['path'], 1);
  }else{
    $path = $referer['path'];
  }
  $path = explode('/', $path);
  $argumnets = array();
  foreach($view->display['default']->display_options['arguments'] as $argument){
    // FIXME - This only works with taxonomy pages.
    switch($argument['default_argument_type']){
      case 'taxonomy_tid':
        if($argument['default_argument_options']['term_page']){
          $arguments[] = $path[2];
        }
    }
  }
  // This should use the standard views hooks, but there isn't a particularly
  // nice way of doing so.  For now, this function will remain to allow us to
  // tweak the arguments.
  drupal_alter('slickgrid_view_arguments', $arguments, $view);
  $view->set_arguments($arguments);
  $view->render();
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  // Prevent null from being returned when there are no results.
  if(!$view->data){
    $view->data = array();
  }
  echo drupal_json_encode(array(
    'data' => $view->data
  ));
  ajax_footer();
}

/**
 * Similar function to taxonomy_get_term_by_name, but allows for matching!
 */
function taxonomy_get_terms_by_name_match($name, $vocabulary = NULL, $match = 'both'){
  switch($match){
    case 'both':
      $name = '%' . db_like(trim($name)) . '%';
      break;
    default:
      $name = db_like(trim($name)) . '%';
  }
  $query = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid'
  ))->condition('name', $name, 'LIKE');
  if(isset($vocabulary)){
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary);
    $query->condition('vid', $vocabulary->vid);
  }
  $tids = $query->execute()->fetchCol();
  if(count($tids)){
    return taxonomy_term_load_multiple($tids);
  }else{
    return array(
      PHP_INT_MAX => PHP_INT_MAX
    );
  }
}