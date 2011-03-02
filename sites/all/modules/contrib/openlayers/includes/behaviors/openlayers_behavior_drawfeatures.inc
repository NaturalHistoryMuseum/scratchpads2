<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Draw Features behavior
 */
class openlayers_behavior_drawfeatures extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'element_id' => '',
      'feature_types' => array(),
      'feature_limit' => 0,
    );
  }

  function options_form($defaults) {  
    $features = array(
      'point' => t('Point'),
      'path' => t('Path'),
      'polygon' => t('Polygon'),
    );
      
    return array(
      'feature_types' => array(
        '#title' => t('Available Features'),
        '#type' => 'checkboxes',
        '#options' => $features,
        '#description' => t('Select what features are available to draw.'),
        '#default_value' => isset($defaults['feature_types']) ? $defaults['feature_types'] : array(),
      ),
      'feature_limit' => array(
        '#title' => t('Number of features'),
        '#type' => 'textfield',
        '#description' => t('The number of features that are allowed to be 
          drawn. Leave blank or at 0 for unlimited.'),
        '#default_value' => isset($defaults['feature_limit']) ? 
          $defaults['feature_limit'] : 0,
      ),
      'element_id' => array(
        '#type' => 'textfield',
        '#default_value' => (isset($defaults['element_id'])) ?
          $defaults['element_id'] : '',
        '#title' => t('Element ID'),
        '#description' => t('The DOM element ID that will be passed the value of the features.  This will probably be a textfield or textarea.'),
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers') 
      . '/includes/behaviors/js/openlayers_behavior_drawfeatures.css');
    drupal_add_js(drupal_get_path('module', 'openlayers')
      . '/includes/behaviors/js/openlayers_behavior_drawfeatures.js');
    return $this->options;
  }
}
