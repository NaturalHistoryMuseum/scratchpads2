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
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => 'openlayers_wkt');
    return $options;
  }

  /**
   * Basically a macro because
   * #state is rather verbose
   */
  function datasource_dependent($type) {
    return array('visible' => array(
        '#edit-style-options-data-source-value' => 
            array('value' => $type)));
  }

  /**
   * Options form
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
        if ($handler->content_field['type'] == 'openlayers_wkt' &&
          $handler->options['format'] !== 'openlayers_wkt'
        ) {
          form_set_error('', t('WKT Field %field not available 
            because it is formatted as a map. Set its format to WKT Value to 
            make it available.', array(
                '%field' => $handler->definition['title'])));
        }
        else {
          $fields[$field_id] = $handler->definition['title'];
        }
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
        'other_latlon' => t('Other: Lat/Lon Point'),
        'other_boundingbox' => t('Other: Bounding Box'),
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
        '#options' => array_merge(array('' => ''), $fields),
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
      . 'values for a style.  The following is a list of formatted variables '
      . 'that are currently available; these can be placed right in the style '
      . 'interface.  The %rendered one means that it has been processed '
      . 'by Views and may have a different value.',
      array(
        '%syntax' => '${field_name}',
        '%rendered' => '_rendered'))
      . '</p>'
      . theme('item_list', $variable_fields)
      . '<p>'
      . t('Please note that this does not apply to Grouped Displays.')
      . '</p>',
    );
  }

  /**
   * Find the data source of an element and pull it into a
   * wkt field
   */
  function coalesce_wkt($feature, $handlers, $ds) {
    $feature['projection'] = '4326';
    switch ($ds['value']) {
      case 'wkt':
        $feature['wkt'] = $feature[$ds['wkt']];
        break;
      case 'other_latlon':
        $feature['wkt'] = 'POINT(' . $feature[$ds['other_lon']] . ' ' . $feature[$ds['other_lat']] . ')';
        break;
    }
    return $feature;
  }

  /**
   * Coalesce features into single grouped feature when
   * grouping is enabled.
   */
  function coalesce_groups($features, $handlers, $ds) {
    foreach ($features as $k => &$f) {
      $f['attributes']['name'] = $k;

      $formatted_features = array();
      foreach ($f['features'] as $feature) {
        $formatted_features[] = theme('openlayers_views_group_display_item',
          array(
            'name' => isset($handlers[$ds['name_field']]) ?
              $feature[$ds['name_field'] . '_rendered'] : false,
            'description' => isset($handlers[$ds['description_field']]) ?
              $feature[$ds['description_field'] . '_rendered'] : false
          )
        );
      }
      $f['attributes']['description'] = theme('item_list',
        array(
          'items' => $formatted_features
        )
      );
    }
    return $features;
  }

  /**
   * Retrieve name and description for individual features when
   * grouping is not enabled.
   */
  function coalesce_features($features, $handlers, $ds) {
    foreach ($features as $k => &$f) {
      $f['attributes']['name'] = isset($handlers[$ds['name_field']])
        ? $f['attributes'][$ds['name_field'] . '_rendered']
        : false;
      $f['attributes']['description'] = isset($handlers[$ds['description_field']])
        ? $f['attributes'][$ds['description_field'] . '_rendered']
        : false;
    }
    return $features;
  }

  /**
   * Basically an algebraic reduction; given whether to group,
   * a feature, a list of features, etc., return a full $features
   * array with the element added, either straight non-grouped, 
   * or within a new or existing group
   */
  function reduce_features($is_grouped, $features, $feature, $group) {
    if ($is_grouped) {
      if (isset($features[$group])) {
        $features[$group]['attributes']['count']++;
        $features[$group]['features'][] = $feature;
        return $features;
      }
      else {
        $features[$group] = array(
          'attributes' => array('count' => 1),
          'wkt' => $feature['wkt'],
          'projection' => $feature['projection'],
          'features' => array($feature)
        );
        return $features;
      }
    }
    else {
      array_push($features, array(
        'attributes' => $feature,
        'wkt' => $feature['wkt'],
        'projection' => $feature['projection'],
        'features' => array($feature)
      ));;
      return $features;
    }
  }

  /**
   * @param $records ...
   */
  function map_features($sets = array()) {
    $features = $excluded_fields = array();
    $handlers = $this->display->handler->get_handlers('field');

    foreach ($sets as $title => $records) {
      foreach ($records as $id => $record) {
        foreach ($handlers as $hid => $handler) { 
          if (!empty($handler->options['exclude'])) {
            $excluded_fields[] = $hid;
          }
          else {
            if ($handler->real_field == 'entity_id') {
              $type = $handler->definition['field_info']['type'];
              $entity = $record->_field_data[$handler->field_alias]['entity'];
              $language = $handler->field_language();
              if (in_array($type, array('text', 'text_long', 'text_with_summary', 'number_integer', 'number_decimal','number_float'))) {
                  $feature[$hid] = $entity->{$handler->definition['field_name']}[$language][0]['value'];
              }
              else {
                  $feature[$hid] = isset($handler->original_value) ? $handler->original_value : NULL;
              }
            }
            else {
              $feature[$hid] = isset($handler->original_value) ? $handler->original_value : NULL;
            }
            $feature[$hid . '_rendered'] = $handler->allow_advanced_render() ? $handler->advanced_render($record) : $handler->render($record);
          }
        }
        
        $features = $this->reduce_features(
          $this->options['grouping'],
          $features,
          $this->coalesce_wkt($feature,
            $handlers,
            $this->options['data_source']),
          $title);
      }
    }
    if ($this->options['grouping']) {
      return $this->coalesce_groups($features,
        $handlers,
        $this->options['data_source']);
    }
    else {
      return $this->coalesce_features($features,
        $handlers,
        $this->options['data_source']);
    }
  }

  /**
   * Render the map features.
   */
  function render($result) {
    return empty($this->view->live_preview) ?
      $this->map_features(
        $this->render_grouping($result, $this->options['grouping'])
      ) :
      t('OpenLayers views are not compatible with live preview.');
  }
}
