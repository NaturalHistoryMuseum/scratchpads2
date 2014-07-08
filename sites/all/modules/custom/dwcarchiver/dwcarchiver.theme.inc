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
                  array(
                    'data' => drupal_render($variables['form'][$key][$key2]['rowtype']),
                    'class' => array(
                      'dwcarchiver-full-width'
                    )
                  ),
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
  drupal_add_css(drupal_get_path('module', 'dwcarchiver') . '/css/dwcarchiver.css');
  return $output;
}

/**
 * Theme the fields table.
 */
function theme_dwcarchiver_archiver_form_fields_table($variables){
  $table = $variables['element']['#table'];
  $table['rows'] = array();
  foreach(element_children($variables['element']) as $key3){
    if(!empty($variables['element'][$key3]['#type']) && $variables['element'][$key3]['#type'] == 'fieldset'){
      $table['rows'][] = array(
        $variables['element'][$key3]['#title'],
        array(
          'data' => drupal_render_children($variables['element'][$key3]),
          'colspan' => 2
        )
      );
    }else{
      $table['rows'][] = array(
        drupal_render($variables['element'][$key3]['label']),
        array(
          'data' => drupal_render($variables['element'][$key3]['term']),
          'class' => array(
            'dwcarchiver-full-width'
          )
        ),
        array(
          'data' => drupal_render($variables['element'][$key3]['processor']),
          'class' => array(
            'dwcarchiver-full-width'
          )
        )
      );
    }
  }
  return theme('table', $table);
}