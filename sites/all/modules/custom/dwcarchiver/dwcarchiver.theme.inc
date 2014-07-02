<?php

/**
 * Theme the add/edit form.
 */
function theme_dwcarchiver_archive_form($variables){
  return drupal_render_children($variables['form']);
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
                  drupal_render($variables['form'][$key][$key2]['rowtype'])
                )
              );
              $output .= theme('table', $table);
              break;
            default:
              $output .= drupal_render_children($variables['form'][$key][$key2]);
          }
        }
        break;
      default:
        $output .= drupal_render_children($variables['form'][$key]);
    }
  }
  return $output;
}