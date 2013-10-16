<?php

/**
 * function theme_citethispage_info
 * 
 * Provides the default theme implementation for the citethispage
 * information section that displays parsed data.
 * 
 * Expected formating of the data is:
 * 
 * <group> => array(
 *   <item-id> => array(
 *     'title' => <string>,
 *      'date' => <timestamp>,
 *      'authors' => array(
 *        'initial' => array(
 *          <uid or formatted name>, ...
 *        ),
 *        'current' => array(
 *          <uid or formatted name>, ...
 *        ),
 *        'others' => array(
 *          <uid or formatted name>, ...
 *        )
 *      )
 *    )
 *  )
 * 
 * Note that we allow both uid and formatted names, as the author may not be a user on this site.
 * 
 */
function theme_citethispage_info($variables){
  $results = $variables['results'];
  $out = '';
  $title = array();
  $date = array();
  $initial_authors = array();
  $other_authors = array();
  foreach($results as $group => $group_results){
    foreach($group_results as $item_id => $item_results){
      if(!empty($item_results['title'])){
        $title[] = $item_results['title'];
      }
      if(!empty($item_results['date'])){
        $date[] = $item_results['date'];
      }
      if(!empty($item_results['authors']['initial'])){
        if(!is_array($item_results['authors']['initial'])){
          $list = array(
            $item_results['authors']['initial']
          );
        }else{
          $list = $item_results['authors']['initial'];
        }
        foreach($list as $user){
          if(is_numeric($user)){
            $user = check_plain(format_username(user_load($user)));
          }
          $initial_authors[] = $user;
        }
      }
      if(!empty($item_results['authors']['others'])){
        foreach($item_results['authors']['others'] as $user){
          if(is_numeric($user)){
            $user = check_plain(format_username(user_load($uid)));
          }
          $other_authors[] = $user;
        }
      }
    }
  }
  if(!empty($title)){
    $out .= '<strong>' . t('Title') . ':</strong> ' . implode(', ', $title) . '<br/>';
  }
  $initial_authors = array_unique(array_filter($initial_authors));
  sort($initial_authors);
  $other_authors = array_diff(array_unique(array_filter($other_authors)), $initial_authors);
  sort($other_authors);
  $authors = array_merge($initial_authors, $other_authors);
  if(!empty($authors)){
    $out .= '<strong>' . t('Authors') . ':</strong> ';
    $out .= implode(', ', array_slice($authors, 0, 3));
    if(count($authors) > 3){
      $out .= t(' et al.');
    }
    $out .= '<br/>';
  }
  if(!empty($date)){
    sort($date);
    foreach($date as $date_index => $date_value){
      $date[$date_index] = format_date($date_value);
    }
    $out .= '<strong>' . t('Date') . ':</strong> ' . implode(', ', $date);
  }
  return $out;
}

/**
 * function theme_citethispage_table_drag
 */
function theme_citethispage_table_drag($variables){
  $element = $variables['element'];
  $columns = $element['#columns'];
  $weight_column = $element['#weight_column'];
  $table_id = $element['#table_id'];
  $rows = array();
  foreach(element_children($element) as $row_id){
    $row = array(
      'data' => array()
    );
    $element[$row_id][$weight_column . $row_id]['#attributes']['class'] = array(
      'item-row-weight'
    );
    foreach(array_keys($columns) as $column_name){
      $row['data'][] = drupal_render($element[$row_id][$column_name . $row_id]);
      $row['class'] = array(
        'draggable'
      );
    }
    $rows[] = $row;
  }
  drupal_add_tabledrag($table_id, 'order', 'self', 'item-row-weight');
  return theme('table', array(
    'header' => $columns,
    'rows' => $rows,
    'attributes' => array(
      'id' => $table_id
    )
  ));
}