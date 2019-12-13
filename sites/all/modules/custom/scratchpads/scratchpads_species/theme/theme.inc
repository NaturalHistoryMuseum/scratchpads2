<?php

function theme_scratchpads_species_tab_display_element($variables)
{
  $element = &$variables['element'];
  $table_id = $element['#id'] + '_table';
  $columns = array('Tab name', 'Enabled/disabled', 'weight');
  $rows = array();

  foreach (element_children($element) as $index => $row_id) {


    $row = array(
      'data' => array(),
      'class' => array('draggable'),
    );

    $row['data'][] = $element[$row_id]['#title'];
    $row['data'][] = drupal_render($element[$row_id]);
    $row['data'][] = drupal_render($element[$row_id][$row_id . '_weight']);

    $rows[] = $row;

  }

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-row-weight');

  return theme('table', array(
    'header' => $columns,
    'rows' => $rows,
    'attributes' => array(
      'id' => $table_id
    )
  ));
}
