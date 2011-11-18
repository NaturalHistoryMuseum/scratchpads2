<?php

/**
 * @file
 * This file holds style plugin for Geofield Maps
 *
 * @ingroup geofield_map
 */

/**
 * @class
 * Extension of the Views Plugin Syle for Geofield Map
 */
class geofield_map_plugin_style_map extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => '');
    $options['popup_source'] = array('default' => '');
    $options['icon'] = array('default' => '');
    return $options;
  }

  /**
   * Options form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $handlers = $this->display->handler->get_handlers('field');
    
    $data_source_options = $popup_source_options = array(
      '' => '<none>',
    );
    
    foreach ($handlers as $handle) {
      $popup_source_options[$handle->options['id']] = (!empty($handle->options['label'])) ? $handle->options['label'] : $handle->options['id'];
      
      if (!empty($handle->field_info['type']) && $handle->field_info['type'] == 'geofield') {
        $data_source_options[$handle->options['id']] = $handle->options['label'];
      }
    }
    
    if (count($data_source_options) == 1) {
      $form['error'] = array(
        '#markup' => 'Please add at least 1 geofield to the view',
      );
    }
    else {
    
      // Map Preset
      $form['data_source'] = array(
        '#type' => 'select',
        '#title' => t('Data Source'),
        '#description' => t('Which field contains geodata?'),
        '#options' => $data_source_options,
        '#default_value' => $this->options['data_source'] ? $this->options['data_source'] : '',
      );
      
      $form['popup_source'] = array(
        '#type' => 'select',
        '#title' => t('Popup Text'),
        '#options' => $popup_source_options,
        '#default_value' => $this->options['popup_source'] ? $this->options['popup_source'] : '',
      );    
    }
  }

  /**
   * Renders views (map)
   */
  function render() {
    $style_options = $this->view->style_options;
    
    $geo_data = (!empty($style_options['data_source'])) ? 'field_' . $style_options['data_source']: NULL;
    $popup_data = (!empty($style_options['popup_source'])) ? $style_options['popup_source'] : NULL;
    
    if ($geo_data && $popup_data) {
      $this->render_fields($this->view->result);    
      $raw_data = array();
      
      foreach ($this->view->result as $id => $result) {
        $geofield = (!empty($result->$geo_data)) ? $result->$geo_data : NULL;
        
        if (!empty($geofield)) {
          $description = $this->rendered_fields[$id][$popup_data];
          
          foreach ($geofield as $item) {
            $raw_data[] = array(
              'geo' => $item['raw'],
              'options' => array(
                'description' => $description,
              ),
            );
          }
        }
      }
  
      $js_settings = array(
        $this->view->name . '_' . $this->view->current_display => array(
          'map_id' => 'geofield_map_' . $this->view->name . '_' . $this->view->current_display,
          'data' => geofield_map_process_points($raw_data),
        ),
      );
  
      drupal_add_js(array('geofieldMap' => $js_settings), 'setting');
    }
    
    drupal_add_js('http://maps.googleapis.com/maps/api/js?sensor=false', 'external');
    drupal_add_js(drupal_get_path('module', 'geofield_map') . '/js/geofield_map.js');

    return '<div style="width: 100%; height: 300px" id="geofield_map_' . $this->view->name . '_' . $this->view->current_display . '" class="geofieldMap">Test</div>';
  }
}
