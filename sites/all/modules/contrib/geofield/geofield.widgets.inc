<?php

/**
 * @file
 * Provides field widget hooks for geofield module.
 */

/**
 * Implements hook_field_widget_info().
 */
function geofield_field_widget_info() {
  $widgets = array();
  $widgets['geofield_wkt'] = array(
    'label' => t('Well Known Text (WKT)'),
    'field types' => array('geofield'),
  );
  $widgets['geofield_latlon'] = array(
    'label' => t('Latitude / Longitude'),
    'field types' => array('geofield'),
  );
  $widgets['geofield_bounds'] = array(
    'label' => t('Bounds'),
    'field types' => array('geofield'),
  );
  $widgets['geofield_textfields'] = array(
    'label' => t('All Textfields'),
    'field types' => array('geofield'),
  );
  
  // OpenLayers dependant widget
  if (module_exists('openlayers_ui')) {
    $widgets['geofield_openlayers'] = array(
      'label' => t('Openlayers Map'),
      'field types' => array('geofield'),
    );
  }
  
  return $widgets;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function geofield_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  
  $form = array();
  
  //TODO: Allow more fine-grained control
  if ($widget['type'] == 'geofield_openlayers') {
    
    // Get preset options, filtered to those which have the GeoField behavior and *don't* have the draw features behavior, which is incompatible
    $maps = openlayers_maps();
    $map_options = array();
    foreach ($maps as $map) {
      if (array_key_exists('openlayers_behavior_geofield', $map->data['behaviors']) && !array_key_exists('openlayers_behavior_drawfeatures', $map->data['behaviors'])) {
        $map_options[$map->name] = $map->title;
      }
    }
    
    if (empty($map_options)) {
      form_set_error('openlayers_map', "Error: You have no compatible openlayers maps. Make sure that at least one preset has the 'GeoField' behavior enabled and that it does not have the 'Draw Features' behavior enabled (which is incompatible).");
    }
    
    $form['openlayers_map'] = array(
      '#type' => 'select',
      '#title' => t('OpenLayers Map'),
      '#default_value' => isset($settings['openlayers_map']) ? $settings['openlayers_map'] : 'geofield_widget_map',
      '#options' => $map_options,
      '#description' => t('Select which OpenLayers map you would like to use. Only maps which have the GeoField behavior may be selected. If your preferred map is not here, add the GeoField behavior to it first. The "Draw Features" bahavior is incompatible - presets with this behavior are not shown.'),
    );
    
  }

  return $form;
}


/**
 * Implements hook_field_widget_form().
 */
function geofield_field_widget_form(&$form, &$form_state, $field, $instance,
  $langcode, $items, $delta, $base) {
  
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  $element = geofield_get_base_element($base, $items, $delta);

  if ($widget['type'] == 'geofield_wkt') {
    $element['wkt']['#title'] = 'Well Known Text';
    $element['wkt']['#type'] = 'textarea';
    
    $element['master_column']['#value'] = 'wkt';
  }

  if ($widget['type'] == 'geofield_latlon') {
    $element['lat']['#title'] = 'Latitude';
    $element['lat']['#type'] = 'textfield';
    
    $element['lon']['#title'] = 'Longitude';
    $element['lon']['#type'] = 'textfield';
    
    $element['master_column']['#value'] = 'latlon';
  }

  if ($widget['type'] == 'geofield_bounds') {
    $element['left']['#title'] = 'Left Longitude';
    $element['left']['#type'] = 'textfield';
    
    $element['top']['#title'] = 'Top Latitude';
    $element['top']['#type'] = 'textfield';
    
    $element['right']['#title'] = 'Right Longitude';
    $element['right']['#type'] = 'textfield';
    
    $element['bottom']['#title'] = 'Bottom Latitude';
    $element['bottom']['#type'] = 'textfield';
    
    $element['master_column']['#value'] = 'bounds';
  }
  
  if ($widget['type'] == 'geofield_textfields') {
    $element['wkt']['#title'] = 'Well Known Text';
    $element['wkt']['#type'] = 'textarea';

    $element['geo_type']['#title'] = 'Geometry Type';
    $element['geo_type']['#type'] = 'textfield';

    $element['lat']['#title'] = 'Latitude';
    $element['lat']['#type'] = 'textfield';
    
    $element['lon']['#title'] = 'Longitude';
    $element['lon']['#type'] = 'textfield';

    $element['left']['#title'] = 'Left Longitude';
    $element['left']['#type'] = 'textfield';
    
    $element['top']['#title'] = 'Top Latitude';
    $element['top']['#type'] = 'textfield';
    
    $element['right']['#title'] = 'Right Longitude';
    $element['right']['#type'] = 'textfield';
    
    $element['bottom']['#title'] = 'Bottom Latitude';
    $element['bottom']['#type'] = 'textfield';
  }
  
  if ($widget['type'] == 'geofield_openlayers') {
    $element['helpmap'] = array(
      '#markup' => '<div class="form-item geotaxonomy-latlon-helpmap" style="display:block">'
                      . geofield_form_latlon_map(array(), isset($instance['widget']['settings']['openlayers_map']) ? $instance['widget']['settings']['openlayers_map'] : 'geofield_widget_map')
                      . '</div>'
    );
    $element['helpmap_desc'] = array(
      '#markup' => t('Click the map to set a point for this location. Control-click to add lines. Alt-click to add polygons. Shift-click and drag to set bounds. Pan and zoom with arrows and the zoom bar.')
    );
  }
  
  return $element;
}

/**
 * Create LatLon Helper Map.
 */
function geofield_form_latlon_map($defaults = array(), $map_name) {
  // Pass variables etc. to javascript
  // Set up our map to help set lat and lon
  // This map will always be projected as 4326 and use just the default map preset
  $map_data = openlayers_map_load($map_name);
  $map = $map_data->data;
  return openlayers_render_map($map);
}

function geofield_get_base_element($base, $items, $delta) {
  $element = $base;
  
  $element['wkt'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_wkt'),
    '#default_value' => isset($items[$delta]['wkt']) ? $items[$delta]['wkt'] : NULL,
  );
  $element['geo_type'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_geo_type'),
    '#default_value' => isset($items[$delta]['geo_type']) ? $items[$delta]['geo_type'] : NULL,
  );
  $element['lat'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_lat'),
    '#default_value' => isset($items[$delta]['lat']) ? $items[$delta]['lat'] : NULL,
  );
  $element['lon'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_lon'),
    '#default_value' => isset($items[$delta]['lon']) ? $items[$delta]['lon'] : NULL,
  );
  $element['left'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_left'),
    '#default_value' => isset($items[$delta]['left']) ? $items[$delta]['left'] : NULL,
  );
  $element['right'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_right'),
    '#default_value' => isset($items[$delta]['right']) ? $items[$delta]['right'] : NULL,
  );
  $element['bottom'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_bottom'),
    '#default_value' => isset($items[$delta]['bottom']) ? $items[$delta]['bottom'] : NULL,
  );
  $element['top'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'geofield_top'),
    '#default_value' => isset($items[$delta]['top']) ? $items[$delta]['top'] : NULL,
  );
  
  // Master column is used by element-validate to decide which set of columns it should use to compute all other values.
  // By default, wkt is the master-column, all we compute all other values from it. For other widget (such as lat/lon) this will be different
  $element['master_column'] = array(
    '#type' => 'hidden',
    '#value' => 'wkt',
  );
  
  // This validate function computes all other columns from the master field
  $element['#element_validate'] = array('geofield_element_validate');
  
  return $element;
}

/**
 * Geofield Element Validate
 *
 */
function geofield_element_validate($element, &$form_state) {
  
  $master_column = $element['master_column']['#value'];
  
  $values = array(
    'wkt' => $element['wkt']['#value'],
    'lat' => $element['lat']['#value'] ? geofield_latlon_DMStoDEC($element['lat']['#value']) : '',
    'lon' => $element['lon']['#value'] ? geofield_latlon_DMStoDEC($element['lon']['#value']) : '',
    'top' => $element['top']['#value'],
    'bottom' => $element['bottom']['#value'],
    'right' => $element['right']['#value'],
    'left' => $element['left']['#value'],
  );
  
  geofield_compute_values($values, $master_column);    
  
  // Set form values from the $values array
  if (isset($values['wkt']))       form_set_value($element['wkt'], $values['wkt'], $form_state);
  if (isset($values['geo_type']))  form_set_value($element['geo_type'], $values['geo_type'], $form_state);
  if (isset($values['lat']))       form_set_value($element['lat'], $values['lat'], $form_state);
  if (isset($values['lon']))       form_set_value($element['lon'], $values['lon'], $form_state);
  if (isset($values['top']))       form_set_value($element['top'], $values['top'], $form_state);
  if (isset($values['bottom']))    form_set_value($element['bottom'], $values['bottom'], $form_state);
  if (isset($values['right']))     form_set_value($element['right'], $values['right'], $form_state);
  if (isset($values['left']))      form_set_value($element['left'], $values['left'], $form_state);
}
