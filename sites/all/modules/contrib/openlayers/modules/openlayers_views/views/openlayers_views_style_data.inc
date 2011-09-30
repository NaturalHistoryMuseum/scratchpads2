<?php

/**
 * @file
 * This file holds style plugin for OpenLayers Views
 *
 * @ingroup openlayers
 */

/**
 * @class
 * Extension of the Views Plugin Style for OpenLayers
 *
 * This class extended the default views plugin class to provide
 * a style plugin for the Open Layers module.
 */
class openlayers_views_style_data extends views_plugin_style {
 
   /**
   * Render the map features.
   *
   * Overrides views_plugin_style->render
   */
  function render($result) {
    $grouped_results = $this->render_grouping($result, $this->options['grouping']);
    $data = $this->map_features($grouped_results);
    
    // If we are not in preview, just return the data
    if (empty($this->view->live_preview)) {
      return $data;
    }
    else {
      // If we are in preview mode, dump out some useful information about this data layer
      $output = "You can use the following parameters in your styles as dynamic values";
      $output .= "\n------------\n";
      $keys = array_keys($data);
      foreach ($data[$keys[0]]['attributes'] as $key => $value) {
        $output .= '${'.$key."}\n";
      }
      $output .= "\n------------\n";
      
      $output .= t('The following is a dump of the data that is rendered from this display. It is used for debugging purposes only.') . '
        ' . var_export($data, TRUE);
      
      return $output;
    }
  }
   
  /**
   * Set default options
   *
   * Overrides views_plugin_style->option_definition
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => 'openlayers_wkt');
    return $options;
  }

  /**
   * Options form
   *
   * Overrides views_plugin_style->options_form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get list of fields in this view & flag available geodata fields
    $handlers = $this->display->handler->get_handlers('field');

    // Check for any fields, as the view needs them
    if (empty($handlers)) {
      $form['error_markup'] = array(
          '#value' => t('You need to enable at least one field before you can '
          . 'configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
      return;
    }

    $fields = array();
    foreach ($handlers as $field_id => $handler) {
      $fields[$field_id] = $handler->definition['title'];
    }

    $form['data_source'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Data Source'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['data_source']['value'] = array(
      '#type' => 'select',
      '#title' => t('Map Data Sources'),
      '#description' => t('Choose which sources of data that the map will '
      . 'provide features for.'),
      '#options' => array(
        'other_latlon' => t('Lat/Lon Pair'),
        'other_boundingbox' => t('Bounding Box'),
        'wkt' => t('WKT')
      ),
      '#default_value' => $this->options['data_source']['value'],
    );

    if (count($fields > 0)) {
      $form['data_source']['other_lat'] = array(
        '#type' => 'select',
        '#title' => t('Latitude Field'),
        '#description' => t('Choose a field for Latitude.  This should be a '
        . 'field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lat'],
        '#states' => $this->datasource_dependent('other_latlon')
      );
      $form['data_source']['other_lon'] = array(
        '#type' => 'select',
        '#title' => t('Longitude Field'),
        '#description' => t('Choose a field for Longitude.  This should be a '
        . 'field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lon'],
        '#states' => $this->datasource_dependent('other_latlon')
      );

      $form['data_source']['wkt'] = array(
        '#type' => 'select',
        '#title' => t('WKT Field'),
        '#description' => t('Choose the OpenLayers WKT field.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['wkt'],
        '#states' => $this->datasource_dependent('wkt')
      );

      $form['data_source']['other_top'] = array(
        '#type' => 'select',
        '#title' => t('Top Field'),
        '#description' => t('Choose a field for Top.  This should be a '
        . 'field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_top'],
        '#states' => $this->datasource_dependent('other_boundingbox')
      );
      $form['data_source']['other_right'] = array(
        '#type' => 'select',
        '#title' => t('Right Field'),
        '#description' => t('Choose a field for Right.  This should be a field '
        . 'that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_right'],
        '#states' => $this->datasource_dependent('other_boundingbox')
      );
      $form['data_source']['other_bottom'] = array(
        '#type' => 'select',
        '#title' => t('Bottom Field'),
        '#description' => t('Choose a field for Bottom.  This should be a '
        . 'field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_bottom'],
        '#states' => $this->datasource_dependent('other_boundingbox')
      );
      $form['data_source']['other_left'] = array(
        '#type' => 'select',
        '#title' => t('Left Field'),
        '#description' => t('Choose a field for Left.  This should be a field '
        . 'that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_left'],
        '#states' => $this->datasource_dependent('other_boundingbox')
      );
    }
    $form['data_source']['name_field'] = array(
        '#type' => 'select',
        '#title' => t('Title Field'),
        '#description' => t('Choose the field which will appear as a title on '
        . 'tooltips.'),
        '#options' => array_merge(array('' => ''), $fields),
        '#default_value' => $this->options['data_source']['name_field'],
    );
    $form['data_source']['description_field'] = array(
        '#type' => 'select',
        '#title' => t('Description Field'),
        '#description' => t('Choose the field which will appear as a '
        . 'description on tooltips.'),
        '#required' => FALSE,
        '#options' => array_merge(array('' => '', '#row' => '<' . t('entire row') . '>'), $fields),
        '#default_value' => $this->options['data_source']['description_field'],
    );

    $form['attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attributes and Styling'),
      '#description' => t('Attributes are field data attached to each '
      . 'feature.  This can be used with styling to create Variable styling.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $variable_fields = array();
    if (!empty($this->options['data_source']['name_field'])) {
      $variable_fields['name'] = '${name}';
    }
    if (!empty($this->options['data_source']['description_field'])) {
      $variable_fields['description'] = '${description}';
    }
    foreach ($this->view->display_handler->get_handlers('field') as
      $field => $handler) {
          if (($field != $this->options['data_source']['name_field']) &&
              ($field != $this->options['data_source']['description_field'])) {
        $variable_fields[$field] = '${' . $field . '}';
        $variable_fields[$field . '_rendered'] = '${' . $field . '_rendered}';
      }
    }
    $form['attributes']['styling'] = array(
      '#markup' => '
      <p>' . t('Any fields that you add to this view will be attached to '
      . 'their respective feature (point, line, polygon) as attributes.  '
      . 'These attributes can then be used to add variable styling to your '
      . 'themes.  This is accomplished by using the %syntax syntax in the '
      . 'values for a style.  You can see a list of available variables in '
      . 'the view preview; these can be placed right in the style interface. '
      . 'The %rendered one means that it has been processed '
      . 'by Views and may have a different value.',
      array(
        '%syntax' => '${field_name}',
        '%rendered' => '_rendered'))
      . '</p>'
      . theme('item_list', array('items' => $variable_fields))
      . '<p>'
      . t('Please note that this does not apply to Grouped Displays.')
      . '</p>',
    );
  }


  /**
   * @param $records ...
   *
   * openlayers_views_style_data specific
   */
  function map_features($sets = array()) {
    $features = $excluded_fields = array();
    $handlers = $this->display->handler->get_handlers('field');
    foreach ($sets as $title => $records) {
      foreach ($records as $id => $record) {
        $this->view->row_index = $id;
        $attributes = array();
        foreach ($handlers as $hid => $handler) {
          if (!empty($handler->options['exclude']) && !in_array($hid, $this->options['data_source'])) {
            $excluded_fields[] = $hid;
          }
          else {
            $attributes[$hid . '_rendered'] = $handler->allow_advanced_render() ? $handler->advanced_render($record) : $handler->render($record);
            $attributes[$hid] = isset($handler->original_value) ? $handler->original_value : NULL;
          }
        }
        
        if ($this->options['data_source']['description_field'] === '#row') {
          $attributes['#row_rendered'] = $this->row_plugin->render($record);
        }
        
        $feature = array(
          'projection' => '4326',
          'attributes' => $attributes,
          'wkt' => $this->get_wkt($attributes, $handlers, $this->options['data_source']),
        );

        $features = $this->reduce_features($this->options['grouping'], $features, $feature, $title);
      }
    }
    
    if ($this->options['grouping']) {
      $features = $this->coalesce_groups($features, $handlers, $this->options['data_source']);
    }
    else {
      $features = $this->add_title_desc($features, $handlers, $this->options['data_source']);
    }
    return $features;
  }

  /**
   * Basically a macro because
   * #state is rather verbose
   *
   * openlayers_views_style_data specific
   */
  function datasource_dependent($type) {
    return array('visible' => array('#edit-style-options-data-source-value' => array('value' => $type)));
  }
  
  /**
   * Find the data source of an element and pull it into a wkt field
   *
   * openlayers_views_style_data specific
   */
  function get_wkt($feature, $handlers, $datasource) {
    $feature['projection'] = '4326';
    switch ($datasource['value']) {
      case 'wkt':
        return strip_tags($feature[$datasource['wkt']]);
        break;

      case 'other_latlon':
        return 'POINT(' . strip_tags($feature[$datasource['other_lon']]) . ' ' . strip_tags($feature[$datasource['other_lat']]) . ')';
        break;
    }
  }

  /**
   * Coalesce features into single grouped feature when grouping is enabled.
   *
   * openlayers_views_style_data specific
   */
  function coalesce_groups($features, $handlers, $ds) {
    
    // Combine wkt into geometry collections if they are an array
    foreach ($features as &$feature) {
      if (is_array($feature['wkt'])) {
        if (count($feature['wkt']) > 1) {
          $feature['wkt'] = $this->get_group_wkt($feature['wkt']);
        }
        else {
          $feature['wkt'] = $feature['wkt'][0];
        }
      }
    }
    
    // Process title and description
    foreach ($features as $k => &$feature) {
      $feature['attributes']['name'] = $k;
      $feature['attributes'] = array_merge($feature['attributes'], $feature['features'][0]['attributes']);
      
      $formatted_features = array();
      foreach ($feature['features'] as $subfeature) {
        $formatted_features[] = theme('openlayers_views_group_display_item',
          array(
            'name' => isset($handlers[$ds['name_field']]) ? $subfeature['attributes'][$ds['name_field'] . '_rendered'] : false,
            'description' => ($ds['description_field'] === '#row' || isset($handlers[$ds['description_field']])) ? $subfeature['attributes'][$ds['description_field'] . '_rendered'] : false
          )
        );
        
        // Remove rendered row to keep data size down for JS.
        if ($ds['description_field'] === '#row') {
          unset($subfeature['attributes'][$ds['description_field'] . '_rendered']);
        }
      }

      $feature['attributes']['description'] = theme('item_list',
        array(
          'items' => $formatted_features
        )
      );
    }
    return $features;
  }
  
  /**
   * Combine all group wkt into a single geometry collection
   * 
   * openlayers_views_style_data specific
   */
  function get_group_wkt($wkt_array) {
    //@@TODO: DO this in geoPHP - four lines of code...
    $output = 'GEOMETRYCOLLECTION(';
    $i = 0;
    foreach ($wkt_array as $wkt) {
      $wkt = strtoupper($wkt);
      if ($i != 0) {
        $output .= ',';
      }
      switch (substr($wkt,0,5)) {
        case 'POINT':
        case 'LINES':
        case 'POLYG':
          $output .= $wkt;
          break;
        case 'MULTI':
          $subwkt = str_replace(array('MULTIPOINT','MULTIPOLYGON','MULTILINESTRING'),'',$wkt);
          $subwkt = str_replace(', ',',',$subwkt);
          $subwkt = trim($subwkt);
          switch (substr($wkt,0,10)) {
            case 'MULTIPOINT':
              $subwkt = str_replace(array('(',')'),'',$subwkt);
              $parts = explode(',',$subwkt);
              $j = 0;
              foreach ($parts as $part) {
                if ($j != 0) $output .= ',';
                $output .= 'POINT('.$part.')';
                $j++;
              }
              break;
            case 'MULTILINES':
              $subwkt = str_replace(array('((','))'),'',$subwkt);
              $parts = explode('),(',$subwkt);
              $j = 0;
              foreach ($parts as $part) {
                if ($j != 0) $output .= ',';
                $output .= 'LINESTRING('.$part.')';
                $j++;
              }
              break;
            case 'MULTIPOLYG':
              //@@TODO: This does not support polygons with holes
              $subwkt = str_replace(array('(((',')))'),'',$subwkt);
              $parts = explode(')),((',$subwkt);
              $j = 0;
              foreach ($parts as $part) {
                if ($j != 0) $output .= ',';
                $output .= 'POLYGON(('.$part.'))';
                $j++;
              }
              break;
          }
          break;
        case 'GEOME':
          $output .= trim(substr($wkt,18,-1));
          break;  
      }
      $i++;
    }
    $output .= ')';
    return $output;
  }

  /**
   * Retrieve name and description for individual features when grouping is not enabled.
   * 
   * openlayers_views_style_data specific
   */
  function add_title_desc($features, $handlers, $ds) {
    foreach ($features as $k => &$f) {
      if (isset($handlers[$ds['name_field']])) {
        $f['attributes']['name'] = $f['attributes'][$ds['name_field'] . '_rendered'];
      }
      if ($ds['description_field'] === '#row' || isset($handlers[$ds['description_field']])) {
        $f['attributes']['description'] = $f['attributes'][$ds['description_field'] . '_rendered'];
        
        // Remove rendered row to keep data size down for JS.
        if ($ds['description_field'] === '#row') {
          unset($f['attributes'][$ds['description_field'] . '_rendered']);
        }
      }
    }
    return $features;
  }

  /**
   * Split string according to first match of passed regEx index of $regExes
   */
  function preg_explode($regEx, $str) {
    $matches = array();
    preg_match($this->regExes[$regEx], $str, $matches);
    return empty($matches)?array(trim($str)):explode($matches[0], trim($str));
  }

  /**
   * Basically an algebraic reduction; given whether to group,
   * a feature, a list of features, etc., return a full $features
   * array with the element added, either straight non-grouped, 
   * or within a new or existing group
   *
   * openlayers_views_style_data specific
   */
  function reduce_features($is_grouped, $features, $feature, $group) {
    if ($is_grouped) {
      if (isset($features[$group])) {
        $features[$group]['attributes']['count']++;
        $features[$group]['wkt'][] = $feature['wkt'];
        $features[$group]['features'][] = $feature;
        return $features;
      }
      else {
        $features[$group] = array(
          'attributes' => array('count' => 1),
          'wkt' => array($feature['wkt']),
          'projection' => $feature['projection'],
          'features' => array($feature)
        );
        return $features;
      }
    }
    else {
      array_push($features, $feature);
      return $features;
    }
  }
  
}
