<?php

/**
 * function theme_citethispage_info
 * 
 * Provides the default theme implementation for the citethispage
 * information section that displays parsed data
 */
function theme_citethispage_info($variables){
  $results = $variables['results'];
  $out = '';
  foreach($results as $key => $value){
    $label = ucwords(preg_replace('/[-_]+/', ' ', $key));
    $out .= '<strong>' . t($label) . ': </strong>';
    if(!is_array($value)){
      $value = array(
        $value
      );
    }
    array_walk($value, 'check_plain');
    $out .= implode(', ', $value) . '<br/>';
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