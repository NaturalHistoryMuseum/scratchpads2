<?php

/**
 * Return the data for a dynamically loaded SlickGrid.
 * 
 * Note, this function may need to be tweaked if we wish to accept arguments
 * to a view.
 */
function slickgrid_get_data($view_name, $offset, $rows, $sortcol = FALSE, $sortdir = 'asc'){
  // Ensure this page is not cached
  drupal_add_http_header('Cache-Control','private, no-cache, no-store, must-revalidate, max-age=0');
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
      $operator = 'contains';
      $op_val = $value;
      // Add regexp support for MySQL.
      if(Database::getConnection()->databaseType() == 'mysql'){
        if(_is_regex($value)){
          // Remove delimiters, as views regex works without them
          $value = substr($value, 1, -1);
          $op_val = $value;
          $operator = 'regular_expression';
        }
      }
      $filter = array(
        'id' => $field,
        'table' => 'node',
        'field' => $field,
        'relationship' => 'none',
        'group_type' => 'group',
        'ui_name' => '',
        'operator' => $operator,
        'value' => $op_val,
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
                if(($operator == 'regular_expression') && ($op_val != '')){
                  $val = $op_val;
                  $op = 'RLIKE';
                }else{
                  $val = db_like($value) . '%';
                  $op = 'LIKE';
                }
                $filter['value'] = drupal_map_assoc(db_select('node', 'n')->condition('title', $val, $op)->fields('n', array(
                  'nid'
                ))->execute()->fetchCol());
                // Ensure that empty sets don't display items
                if(empty($filter['value'])){
                  $filter['value'] = $val;
                }
                unset($filter['operator']);
                break;
              case 'taxonomy_term_reference':
                if(($operator == 'regular_expression') && ($op_val != '')){
                  $t_val = array_keys(taxonomy_get_terms_by_name_match_regex($op_val));
                }else{
                  $t_val = array_keys(taxonomy_get_terms_by_name_match($value));
                }
                foreach($real_field['storage']['details']['sql'][FIELD_LOAD_CURRENT] as $table => $more_info){
                  foreach($more_info as $key => $name){
                    if($key != 'format'){
                      $filter['operator'] = 'or';
                      $filter['value'] = $t_val;
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
 * Helper function
 * Returns TRUE if $str is a regular expression,
 * according to the following rules:
 * - Does the string start and end with the same character? 
 *   (non alphanumeric, excluding whitespaces and backslashes)
 * - Does the string contain "*" or or '?' or '$' or '^' or "[" and "]"?
 */
function _is_regex($str){
  $first_char = substr($str, 0, 1);
  $last_char = substr($str, -1);
  // You can use any non alphanumeric delimiter (excluding whitespaces and backslashes)
  if((strlen($str) < 3) || ($first_char != $last_char) || ctype_alnum($first_char) || ($first_char == " ") || ($first_char == "\\")){return false;}
  // Look for 'special' characters and return true if found
  $str = substr($str, 1, -1);
  if((strpos($str, '[') !== false) && (strpos($str, ']') !== false)){
    return true;
  }elseif(strpos($str, '*') !== false){
    return true;
  }elseif(strpos($str, '?') !== false){
    return true;
  }elseif(strpos($str, '$') !== false){
    return true;
  }elseif(strpos($str, '^') !== false){
    return true;
  }else{
    return false;
  }
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

/**
 * Similar to taxonomy_get_terms_by_name_match(), but with regex search
 */
function taxonomy_get_terms_by_name_match_regex($name){
  $name = db_like(trim($name));
  $query = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid'
  ))->condition('name', $name, 'RLIKE');
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