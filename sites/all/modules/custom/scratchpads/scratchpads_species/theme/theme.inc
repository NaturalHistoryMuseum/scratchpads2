<?php

function theme_scratchpads_species_tab_display_element($variables)
{
  $element = &$variables['element'];
  $table_id = $element['#id'];
  $columns = array('Tab name', 'Enabled/disabled');
  $rows = array();

  foreach (element_children($element) as $row_id) {

    $element[$row_id]['#attributes']['class'] = array(
      'item-row-weight'
    );

    $row = array(
      'data' => array(),
      'class' => array('draggable'),
    );

    $row['data'][] = drupal_render($element[$row_id]);
    $row['data'][] = '';

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
