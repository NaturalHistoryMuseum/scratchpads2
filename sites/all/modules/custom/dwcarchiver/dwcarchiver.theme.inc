<?php

/**
 * Theme the add/edit form.
 */
function theme_dwcarchiver_archive_form($variables){
  $output = '';
  foreach(element_children($variables['form']) as $key){
    switch($key){
      case 'extensions':
        foreach(element_children($variables['form'][$key]) as $key2){
          switch($key2){
            case 'add':
              $table = $variables['form'][$key][$key2]['#table'];
              $table['rows'] = array(
                array(
                  drupal_render($variables['form'][$key][$key2]['name']) . drupal_render($variables['form'][$key][$key2]['machine_name']),
                  drupal_render($variables['form'][$key][$key2]['rowtype']),
                  drupal_render($variables['form'][$key][$key2]['entity_bundle'])
                )
              );
              $output .= theme('table', $table);
              break;
            default:
              $output .= drupal_render($variables['form'][$key][$key2]);
              break;
          }
        }
        break;
      default:
        $output .= drupal_render($variables['form'][$key]);
        break;
    }
  }
  return $output;
}

/**
 * Theme the fields table.
 */
function theme_dwcarchiver_archiver_form_fields_table($variables){
  $table = $variables['element']['#table'];
  $table['rows'] = array();
  foreach(element_children($variables['element']) as $key3){
    $table['rows'][] = array(
      drupal_render($variables['element'][$key3]['label']),
      drupal_render($variables['element'][$key3]['term']),
      drupal_render($variables['element'][$key3]['processor'])
    );
  }
  return theme('table', $table);
}