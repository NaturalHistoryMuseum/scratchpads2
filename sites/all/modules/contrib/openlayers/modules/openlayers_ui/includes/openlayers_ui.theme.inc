<?php

/**
 * @file
 * This file holds the theme functions for openlayers ui module
 *
 * @ingroup openlayers
 */

/**
 * Theme function for form description of layers
 */
function theme_openlayers_ui_form_layer_description($args) {
  return $args['layer_title'] . ($args['layer_description'] ?
    ': <span class="openlayers-form-layer-description">' .
    $args['layer_description'] . '</span>' :
    '');
}

/**
 * Theme function for form description of projection
 */
function theme_openlayers_ui_form_projection_description($args) {
  $output = $args['projection_title'];
  if (count($args['available_layers'])) {
    $names = implode(', ', $args['available_layers']);
    $output .= " - <span class='openlayers-form-projection-description'>$names</span>";
  }
  return $output;
}

function theme_openlayers_ui_maps_form_layers($args) {
  $form = $args['elements'];

  $output = drupal_render($form['ahah-start']) .
    drupal_render($form['projections']);

  // Base layers
  $rows = array();
  foreach (element_children($form['layers']['baselabels']) as $key) {
    unset($form['layers']['default_layer'][$key]['#title']);
    unset($form['layers']['baselayers'][$key]['#title']);
    
    $rows[] = array(
      drupal_render($form['layers']['baselabels'][$key]),
      drupal_render($form['layers']['baselayers'][$key]),
      drupal_render($form['layers']['default_layer'][$key]));
  }
  $output .= theme('table', array(
    'header' => array(t('Base layers'), t('Enabled'), t('Default')), 
    'rows' => $rows)
  );

  // Overlay layers
  $rows = array();
  foreach (element_children($form['layers']['overlaylabels']) as $key) {
    unset($form['layers']['overlays'][$key]['#title']);
    unset($form['layer_activated'][$key]['#title']);
    unset($form['layer_switcher'][$key]['#title']);

    $rows[] = array(
      'data' => array(
        drupal_render($form['layers']['overlaylabels'][$key]),
        drupal_render($form['layer_weight'][$key]),
        drupal_render($form['layers']['overlays'][$key]),
        drupal_render($form['layer_activated'][$key]),
        drupal_render($form['layer_switcher'][$key]),
        drupal_render($form['layer_styles'][$key])),
      'class' => array('draggable')
    );
  }
  $output .= theme('table', 
    array(
      'header' => array(t('Overlay layers'),
        t('Weight'), t('Enabled'), t('Activated'),
        t('In Switcher'), t('Style')), 
      'rows' => $rows, 
      array('id' => 'overlay-layer-table')
    )
  );

  drupal_add_tabledrag('overlay-layer-table', 'order', 'sibling',
    'layer-weight');
  return $output . drupal_render_children($form);
}

/**
 * Drupal theme implementation for Behaviors in the map form
 */
function theme_openlayers_ui_maps_form_behaviors($args) {
  $form = $args['elements'];

  $rows = array();
  foreach (element_children($form) as $key) {
    $rows[] = array(drupal_render($form[$key]['enabled']) . 
      drupal_render($form[$key]['options_set']) . 
      drupal_render($form[$key]['dependencies']));
  }

  return theme('table',
    array(
      'header' => array(t('Behaviors')),
      'rows' => $rows));
}
