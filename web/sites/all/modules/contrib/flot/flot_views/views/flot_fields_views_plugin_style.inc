<?php

/**
 * @file
 * Style plugin for views
 */
class flot_fields_views_plugin_style extends views_plugin_style {

  var $rebuild_zoom_flot = FALSE;

  function option_definition() {
    $options = parent::option_definition();
    $options['size'] = array('default' => '400x200');
    $options['showlegend'] = array('default' => 'yes');
    $options['zoomable'] = array('default' => 1);
    $options['hoverable'] = array('default' => 1);
    $options['shadowSize'] = array('default' => '');
    $options['legend'] = array(
      'default' => array(
        'noColumns' => 1, // number of colums in legend table
        'labelBoxBorderColor' => "#ccc", // border color for the little label boxes
        'position' => "bottom", // position of default legend container within plot
        'margin' => array( // distance from grid edge to default legend container within plot
          'x' => 5,
          'y' => 5,
        ),
        'backgroundColor' => "", // empty means auto-detect
        'backgroundOpacity' => 0.85 // set to 0 to avoid background
      ),
    );
    $options['showxaxis'] = array('default' => 1);
    $options['showyaxis'] = array('default' => 1);
    $options['xaxis'] = array(
      'default' => array(
        'position' => "bottom", // or "top"
        'color' => "", // base color, labels, ticks
        'tickColor' => "", // possibly different color of ticks, e.g. "rgba(0,0,0,0.15)"
        'min' => "", // min. value to show, "" means set automatically
        'max' => "", // max. value to show, "" means set automatically
        'autoscaleMargin' => "", // margin in % to add if auto-setting min/max
        'granularity' => "auto",
        'labelWidth' => "", // size of tick labels in pixels
        'labelHeight' => "",
      ),
    );
    $options['yaxis'] = array(
      'default' => array(
        'label' => 'default',
        'position' => "left", // or "right"
        'color' => "", // base color, labels, ticks
        'tickColor' => "", // possibly different color of ticks, e.g. "rgba(0,0,0,0.15)"
        'min' => "", // min. value to show, "" means set automatically
        'max' => "", // max. value to show, "" means set automatically
        'autoscaleMargin' => 0.02, // margin in % to add if auto-setting min/max
        'granularity' => "auto",
        'labelWidth' => "", // size of tick labels in pixels
        'labelHeight' => "",
      ),
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#description' => t("Enter the dimensions for the chart. Format: WIDTHxHEIGHT (e.g. 200x100)"),
      '#default_value' => $this->options['size'],
    );

    $form['shadowSize'] = array(
      '#type' => 'textfield',
      '#title' => t('Shadow size'),
      '#description' => t("Enter the shadow size. Leave blank for default behaviour."),
      '#default_value' => $this->options['shadowSize'],
    );

    $form['zoomable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Zoomable'),
      '#description' => t('Allow users to zoom in on the graph.'),
      '#default_value' => $this->options['zoomable'],
    );

    $form['hoverable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hoverable'),
      '#description' => t('Allow users to hover over datapoints (developers can bind a javascript function to the plothover event).'),
      '#default_value' => $this->options['hoverable'],
    );

    $form += $this->legendForm();
    $form += $this->axisForm('x');
    $form += $this->axisForm('y');

  }

  function axisForm($axis) {
    $form = array();
    $axisname = $axis . 'axis';
    $form['show' . $axisname] = array(
      '#type' => 'checkbox',
      '#title' => t('Show ' . $axis . ' axis'),
      '#default_value' => $this->options['show' . $axisname],
    );

    $form[$axisname] = array(
      '#type' => 'fieldset',
      '#title' => t(drupal_strtoupper($axis) . ' axis settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[show' . $axisname . ']"]' => array('checked' => TRUE),
        ),
      ),
    );
    if ($axis == 'y') {
      $label_options = array(
        '' => '< ' . t('No labels') . ' >',
        'default' => t('Default (from fields)'),
      );
      $form[$axisname]['label'] = array(
        '#type' => 'select',
        '#options' => $label_options,
        '#title' => t('Labels'),
        '#default_value' => $this->options[$axisname]['label'],
      );
    }

    $form[$axisname]['position'] = array(
      '#type' => 'select',
      '#title' => t('Position'),
      '#options' => ($axis == 'x') ? array('bottom' => t('Bottom'), 'top' => t('Top')) : array('left' => t('Left'), 'right' => t('Right')),
      '#default_value' => $this->options[$axisname]['position'],
    );

    $form[$axisname]['color'] = array(
      '#type' => 'textfield',
      '#title' => t('Color'),
      '#description' => t('Determines the color of the labels and ticks for
        the axis (default is the grid color). For more fine-grained control
        you can also set the color of the ticks separately with "tickColor"
        (otherwise it\'s autogenerated as the base color with some
        transparency).'),
      '#default_value' => $this->options[$axisname]['color'],
    );

    $form[$axisname]['tickColor'] = array(
      '#type' => 'textfield',
      '#title' => t('Tick color'),
      '#description' => t('Determines the color of the ticks for
        the axis (default is the grid color. If left emtpy the color is
        autogenerated as the base color with some transparency).'),
      '#default_value' => $this->options[$axisname]['tickColor'],
    );

    $form[$axisname]['min'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum'),
      '#description' => t('The precise minimum value on the
        scale. If you don\'t specify this, a value will automatically
        be chosen based on the minimum data value. '),
      '#default_value' => $this->options[$axisname]['min'],
    );

    $form[$axisname]['max'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum'),
      '#description' => t('The precise maximum value on the
        scale. If you don\'t specify this, a value will automatically
        be chosen based on the maximum data value. '),
      '#default_value' => $this->options[$axisname]['max'],
    );

    $form[$axisname]['autoscaleMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('Autoscale margin'),
      '#description' => t('The "autoscaleMargin" is a bit esoteric: it\'s the fraction of margin
        that the scaling algorithm will add to avoid that the outermost points
        ends up on the grid border. Note that this margin is only applied when
        a min or max value is not explicitly set. If a margin is specified,
        the plot will furthermore extend the axis end-point to the nearest
        whole tick. The default value is 0 for the x axes and 0.02 for y
        axes which seems appropriate for most cases.'),
      '#default_value' => $this->options[$axisname]['autoscaleMargin'],
    );

    //ticks
    $axis_granularity = array(
      'auto' => t('Auto generate'),
      'endpoints' => t('Endpoints only'),
    );
    if ($axis == 'y') {
      for ($i = 3; $i < 10; $i++) {
        $axis_granularity[$i] = t('Granularity: !count ticks', array('!count' => $i));
      }
    }
    $form[$axisname]['granularity'] = array(
      '#type' => 'select',
      '#options' => $axis_granularity,
      '#title' => t('Granularity'),
      '#default_value' => $this->options[$axisname]['granularity'],
    );

    if ($axis == 'y') {
      $form[$axisname]['granularity'] += array(
        '#description' => t('The algorithm always tries to generate reasonably
          round tick values so even if you ask for three ticks, you might get
          five if that fits better with the rounding.')
      );
    }

    $form[$axisname]['labelWidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Label width'),
      '#description' => t('Specifies a fixed width of the tick labels in pixels.'),
      '#default_value' => $this->options[$axisname]['labelWidth'],
    );

    $form[$axisname]['labelHeight'] = array(
      '#type' => 'textfield',
      '#title' => t('Label height'),
      '#description' => t('Specifies a fixed height of the tick labels in pixels.'),
      '#default_value' => $this->options[$axisname]['labelHeight'],
    );
    return $form;
  }

  function legendForm() {
    $form = array();
    $form['showlegend'] = array(
      '#type' => 'select',
      '#title' => t('Show legend'),
      '#options' => array('yes' => t('Yes'), 'no' => t('No')),
      '#description' => t("Do you want to show the legend."),
      '#default_value' => $this->options['showlegend'],
    );

    $form['legend'] = array(
      '#type' => 'fieldset',
      '#title' => t('Legend'),
      '#description' => t('Customize the legend'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[showlegend]"]' => array('value' => 'yes'),
        ),
      ),
    );

    $form['legend']['labelBoxBorderColor'] = array(
      '#type' => 'textfield',
      '#title' => t('Label box border color'),
      '#description' => t('Define the border color of the label boxes of the legend.'),
      '#default_value' => $this->options['legend']['labelBoxBorderColor'],
    );

    $form['legend']['noColumns'] = array(
      '#type' => 'textfield',
      '#title' => t('Columns'),
      '#description' => t('The number of columns to display the legend in.'),
      '#default_value' => $this->options['legend']['noColumns'],
    );

    $form['legend']['position'] = array(
      '#type' => 'select',
      '#title' => t('Position'),
      '#description' => t('Specifies the overall placement of the legend within the plot, select "bottom" to put the legend below the graph.'),
      '#options' => array(
        'ne' => t('North-East'),
        'nw' => t('North-West'),
        'se' => t('South-East'),
        'sw' => t('South-West'),
        'bottom' => t('Bottom (below the graph)'),
      ),
      '#default_value' => $this->options['legend']['position'],
    );

    $form['legend']['margin'] = array(
      '#type' => 'fieldset',
      '#title' => t('Margin'),
      '#description' => t('Customize the distance of the legend to the plot edge'),
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[legend][position]"]' => array('value' => 'bottom'),
        ),
      ),
    );

    $form['legend']['margin']['x'] = array(
      '#type' => 'textfield',
      '#title' => t('X margin'),
      '#default_value' => $this->options['legend']['margin']['x'],
    );

    $form['legend']['margin']['y'] = array(
      '#type' => 'textfield',
      '#title' => t('Y margin'),
      '#default_value' => $this->options['legend']['margin']['y'],
    );

    $form['legend']['backgroundColor'] = array(
      '#type' => 'textfield',
      '#title' => t('Background color'),
      '#description' => t('Specify the background color for the legend, leave empty for auto-detect.'),
      '#default_value' => $this->options['legend']['backgroundColor'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[legend][position]"]' => array('value' => 'bottom'),
        ),
      ),
    );

    $form['legend']['backgroundOpacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Background opacity'),
      '#description' => t('The background opacity of the legend. A number between 1 and 0, use decimal point.'),
      '#default_value' => $this->options['legend']['backgroundOpacity'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[legend][position]"]' => array('value' => 'bottom'),
        ),
      ),
    );

    return $form;
  }

  function query() {
    switch (get_class($this->view->query)) {
      case 'views_plugin_query_default':
        $this->query_views_plugin_query_default();
        break;
      case 'SearchApiViewsQuery':
        // Nothing to do here, search api returns entities
        break;
      default:
        drupal_set_message('Unsupported query type: ' . get_class($this->view->query));
        break;
    }
  }

  function query_views_plugin_query_default() {
    // get all fields
    $fields = $this->display->handler->handlers['field'];
    // Get flot fields, and bail if non present.
    $fields = $this->display->handler->handlers['field'];
    foreach ($fields as $fieldname => $field) {
      if ($field->options['flot']['axis'] == 'x') {
        $fieldx = $field;
        unset($fields[$fieldname]);
      }
    }

    $alias = $fieldx->field_alias;
    //if there are no y fields
    if (empty($fields)) {
      $this->view->query->add_field(
          NULL,
          $this->view->query->fields[$alias]['table'] . '.' . $this->view->query->fields[$alias]['field'],
          "count",
          array('function' => 'count')
        );
    }

    // is first field a data field?
    if ($fieldx->options['flot']['x']['mode'] == 'time') {
      // check filters for timestamps
      // needs love, don't assume name of the field is timestamp
      // check for class viewss_handler_filter_date?
      // check cache as well, if not in cache we need to execute an extra query to get the full interval
      $this->rebuild_zoom_flot = FALSE;
      $filters = $this->display->handler->handlers['filter'];
      foreach ($filters as $filter) {
        if ($filter->options['field'] == $fieldx->options['field'] && $filter->options['table'] == $fieldx->options['table']) {
          if ($filter->options['exposed'] && $filter->options['operator'] == 'between') {
            if (isset($filter->value['min']) && isset($filter->value['max']) && is_numeric($filter->value['min']) && is_numeric($filter->value['max'])) {
              // How to find the right condition to alter
              $filter_field = $filter->table . '.' . $filter->field;
              // find right condition
              $wherekey = min(array_keys($this->view->query->where));
              foreach ($this->view->query->where[$wherekey]['conditions'] as $key => $condition) {
                if (strpos($condition['field'], 'BETWEEN') && strpos($condition['field'], $filter_field) === 0) {
                  $this->view->query->where[$wherekey]['conditions'][$key]['field'] = $filter_field . ' BETWEEN ' . $filter->value['min'] . ' AND ' . $filter->value['max'];
                  break;
                }
              }
            }
            else {
              // Only build cache if there is no range given
              $this->rebuild_zoom_flot = TRUE;
            }
            break;
          }
        }
      }

      // calculate a sane interval for datetime fields
      $minmaxview = new view;
      foreach ($this->view as $key => $val) {
        if (is_object($val) && $key != 'query') {
            $minmaxview->{$key} = clone $val;
        }
        elseif (is_array($val)) {
            $minmaxview->{$key} = $val;
        }
        else {
            $minmaxview->{$key} = $val;
        }
      }
      $minmaxview->query = new views_plugin_query_default;
      foreach ($this->view->query as $key => $val) {
        if (is_object($val)) {
            $minmaxview->query->{$key} = clone $val;
        }
        elseif (is_array($val)) {
            $minmaxview->query->{$key} = $val;
        }
        else {
            $minmaxview->query->{$key} = $val;
        }
      }
      $minmaxview->query->orderby = array();
      $minmaxview->query->fields = array();
      $minmaxview->query->fields[$alias . '_min'] = array(
        'field' => 'MIN(' . $this->view->query->fields[$alias]['table'] . '.' . $this->view->query->fields[$alias]['field'] . ')',
        'table' => '',
        'alias' => $alias . '_min',
      );
      $minmaxview->query->fields[$alias . '_max'] = array(
        'field' => 'MAX(' . $this->view->query->fields[$alias]['table'] . '.' . $this->view->query->fields[$alias]['field'] . ')',
        'table' => '',
        'alias' => $alias . '_max',
      );
      $minmaxview->query->view = $minmaxview;
      $minmaxview->query->query(FALSE);
      $minmaxview->query->build($minmaxview);
      $minmaxview->query->execute($minmaxview);
      $timeinterval = 86400;
      if (isset($minmaxview->result) && isset($minmaxview->result[0])) {
        $minmax = $minmaxview->result[0];
        $min = $minmax->{$alias . '_min'};
        $max = $minmax->{$alias . '_max'};
        if ($max - $min > 30 * 86400) {
          $timeinterval = 86400;
        }
        elseif ($max - $min > 7 * 86400) {
          $timeinterval = 21600;
        }
        elseif ($max - $min > 3 * 86400) {
          $timeinterval = 10800;
        }
        elseif ($max - $min > 86400) {
          $timeinterval = 3600;
        }
        else {
          $timeinterval = 60;
        }
      }

      $this->view->query->fields[$alias]['field'] = '(ROUND(' . $this->view->query->fields[$alias]['table'] . '.' . $this->view->query->fields[$alias]['field'] . " / $timeinterval) * $timeinterval)";
      $this->view->query->fields[$alias]['table'] = '';
    }
    if (!empty($fields)) {
      foreach ($fields as $field) {
        if ($field->options['flot']['y']['function'] != 'label') {
          // get the alias
          //$y_alias = $this->view->query->add_field($field->table, $field->real_field);
          
          $y_alias = $field->field_alias;
          $this->view->query->add_field(
            $field->table,
            $field->real_field,
            $y_alias . '_' . $field->options['flot']['y']['function'],
            array('function' => $field->options['flot']['y']['function'])
          );
        }
      }
    }
    // group by first field
    $groupby = isset($this->view->query->groupby) ? $this->view->query->groupby : array();
    array_unshift($groupby, $this->view->query->fields[$alias]['alias']);
    $this->view->query->groupby =  $groupby;
    $this->view->query->has_aggregate = TRUE;
  }

  /**
   * Theme template preprocessor.
   */
  function preprocess(&$vars) {
    if (!isset($vars['addselectionfilter'])) {
      $vars['addselectionfilter'] = FALSE;
    }

    // Get flot fields, and bail if non present.
    $fields = $this->display->handler->handlers['field'];
    $fieldx = NULL;
    $group_fields = array();

    switch (get_class($this->view->query)) {
      case 'views_plugin_query_default':
        foreach ($fields as $fieldname => $field) {
          if ($field->options['flot']['axis'] == 'x') {
            $fieldx = $field;
          }
          elseif (isset($field->options['group_type']) && $field->options['group_type'] == 'group') {
            $group_fields[] = $field;
          }
        }
        break;
      case 'SearchApiViewsQuery':
        foreach ($fields as $fieldname => $field) {
          if ($field->options['flot']['axis'] == 'x') {
            $fieldx = $field;
          }
          // Fields on Y axis are used as labels by default since we cannot used duplicate labels
          elseif ($field->options['flot']['axis'] == 'y' && isset($field->options['flot']['y']['function']) && $field->options['flot']['y']['function'] == 'label') {
            $group_fields[] = $field;
          }
        }
        break;
      default:
        drupal_set_message('Unsupported query type: ' . get_class($this->view->query));
        break;
    }

    if (is_null($fieldx)) {
      return;
    }
    $yfields = array();
    foreach ($fields as $fieldname => $field) {
      if ($field->options['flot']['axis'] == 'y') {
        $yfields[] = $field;
      }
    }

    // min / max values
    $view = $this->view;
    $options = $this->options;
    $filters = $this->display->handler->handlers['filter'];

    // Parameters
    $size = !empty($options['size']) ? explode('x', $options['size']) : array('200', '100');
    if (!$vars['addselectionfilter'] && $options['zoomable']) {
      $vars['addselectionfilter'] = TRUE;
    }
    // DOM element options
    // By default no zoom selection
    $element = array();
    $element['style'] = is_numeric($size[0]) ? "width:{$size[0]}px;" : "width:{$size[0]};";
    $vars['flotwidth'] = $size[0];
    $element['style'] .= is_numeric($size[1]) ? "height:{$size[1]}px;" : "height:{$size[1]};";

    $selectionfilter_added = FALSE;
    foreach ($filters as $filter) {
      if ($filter->options['field'] == $fieldx->options['field'] && $filter->options['table'] == $fieldx->options['table']) {
        if ($filter->options['exposed'] && $filter->options['operator'] == 'between') {
          $vars['addselectionfilter'] = TRUE;
          $element['class'] = 'has-inbetween-exposed-filter';
          drupal_add_js(array('flot' => array('field_name' => $filter->options['expose']['identifier'])), 'setting');
          drupal_add_js(drupal_get_path('module', 'flot_views') . '/views/flot_hide_exposed_form.js',
            array('scope' => 'footer', 'weight' => 4)
          );
          drupal_add_css(drupal_get_path('module', 'flot') . '/flot.legend.css');
          $selectionfilter_added = TRUE;
          break;
        }
      }
    }
    $vars['element'] = $element;

    // input
    $input = array(
      'fieldx' => $fieldx,
      'yfields' => $yfields,
      'group_fields' => $group_fields,
    );

    // output
    $output = array();

    switch (get_class($this->view->query)) {
      case 'views_plugin_query_default':
        $this->preprocess_views_plugin_query_default($vars, $input, $output);
        break;
      case 'SearchApiViewsQuery':
        $this->preprocess_SearchApiViewsQuery($vars, $input, $output);
        break;
      default:
        drupal_set_message('Unsupported query type: ' . get_class($this->view->query));
        break;
    }

    $series = $output['series'];
    $series_options = $output['series_options'];
    $range = $output['range'];
    $ticks = $output['ticks'];

    $flotseries = array();
    $style = new flotStyle();

    // Convert to flot compatible format
    foreach ($series as $s => $serie) {
      $flotdata = new flotData($serie);
      $settings = array('color' => $series_options[$s]['color']);
      switch ($series_options[$s]['type']) {
        case 'point':
          $flotdata->points = new flotPoint($settings);
          break;
        case 'bar':
          $flotdata->bars = new flotBar($settings);
          break;
        case 'pie':
          $style->createPie();
          break;
        case 'line':
        default:
          $flotdata->lines = new flotLine($settings);
          $flotdata->points = new flotPoint();
          break;
      }
      if (isset($options['yaxis']['label']) && $options['yaxis']['label'] != '') {
        $flotdata->label = $s;
      }
      $flotseries[] = $flotdata;
    }
    $vars['data'] = $flotseries;

    // cache the zoom series to always include the full range
    // not stable, need better solution
    if ($vars['addselectionfilter']) {
      $cid = 'views_flot:' . $this->view->name . ':' . $this->view->current_display;
      $cache = cache_get($cid);

      if ($cache && isset($cache->data) && !$this->rebuild_zoom_flot) {
        $zoomflotseries = $cache->data;
      }
      else {
        $zoomflotseries = $flotseries;
        cache_set($cid, $zoomflotseries, 'cache', CACHE_PERMANENT);
      }
      $vars['zoomdata'] = $zoomflotseries;
    }

    foreach (array('xaxis', 'yaxis') as $axis) {
      $style->{$axis}->show = ($options['show' . $axis]) ? TRUE : FALSE;
      if ($style->{$axis}->show) {
        switch ($options[$axis]['granularity']) {
          case 'endpoints':
            $ticks = array($range[$axis]['min'], $range[$axis]['max']);
            $style->axis_ticks($axis, $ticks);
            break;
          case 'auto':
            $style->axis_range($axis, $range[$axis]);
            break;
          default:
            $style->axis_range($axis, $range[$axis], $options[$axis]['granularity']);
            break;
        }
      }
      foreach ($options[$axis] as $key => $val) {
        if (!empty($val) && $val !== 0) {
          $style->{$axis}->{$key} = $val;
        }
      }
    }
    if ($fieldx->options['flot']['x']['mode'] == 'time') {
      $style->xaxis->mode = 'time';
    }
    $style->legend->show = (!isset($options['showlegend']) || $options['showlegend'] == 'yes') ? TRUE : FALSE;
    if ($style->legend->show) {
      foreach ($options['legend'] as $key => $val) {
        if (!empty($val) && $val !== 0) {
          $style->legend->{$key} = $val;
        }
      }
      if (isset($style->legend->margin)) {
        $style->legend->margin = array($style->legend->margin['x'], $style->legend->margin['y']);
      }
    }
    $style->grid->hoverable = $options['hoverable'] ? TRUE : FALSE;
    $style->shadowSize = $options['shadowSize'];
    $vars['options'] = $style;
  }

  function preprocess_views_plugin_query_default($vars, $input, &$output) {
    // input
    $yfields = $input['yfields'];
    $fieldx = $input['fieldx'];
    $group_fields = $input['group_fields'];

    // output
    $series = array();
    $series_options = array();
    $range = array();
    $range['xaxis'] = $range['yaxis'] = array('min' => NULL, 'max' => NULL);
    $ticks = array();

    // Iterate over results to build data and ticks
    foreach ($vars['rows'] as $id => $row) {
      if (isset($row)) {
        if (empty($yfields)) {
          if (isset($fieldx->options['date_format'])) {
            $datapoint = array(
              $row->{$fieldx->field_alias} * 1000,
              $row->{"count"},
            );
          }
          else {
            if (!in_array($row->{$fieldx->field_alias}, $ticks)) {
              $ticks[] = $row->{$fieldx->field_alias};
            }
            $datapoint = array(
              array_shift(array_keys($ticks, $row->{$fieldx->field_alias})),
              $row->{"count"},
            );
          }
          $range['xaxis']['min'] = is_null($range['xaxis']['min']) ? $row->{"count"} : min($row->{"count"}, $range['xaxis']['min']);
          $range['xaxis']['max'] = is_null($range['xaxis']['max']) ? $row->{"count"} : max($row->{"count"}, $range['xaxis']['max']);
          $series[$fieldx->field_alias][] = $datapoint;
        }
        else {
          $labelfields = array();
          foreach($yfields as $fieldy) {
            if ($fieldy->options['flot']['y']['function'] == 'label') {
              $labelfields[] = $fieldy;
            }
          }
          foreach ($yfields as $fieldy) {
            //filter out non-numeric.
            if ($fieldy->options['flot']['y']['function'] != 'label') {
              $y_alias = $fieldy->field_alias . '_' . $fieldy->options['flot']['y']['function'];
              // datetime fields need special care
              // TODO: Also handle datetime for the Y axis, not just the X axis
              if (isset($fieldx->options['date_format'])) {
                $datapoint = array(
                  $row->{$fieldx->field_alias} * 1000,
                  $row->{$y_alias},
                );
              }
              else {
                if (!in_array($row->{$fieldx->field_alias}, $ticks)) {
                  $ticks[] = $row->{$fieldx->field_alias};
                }
                $datapoint = array(
                  array_shift(array_keys($ticks, $row->{$fieldx->field_alias})),
                    $row->{$y_alias},
                );
              }

              $range['yaxis']['min'] = is_null($range['yaxis']['min']) ? $datapoint[1] : min($datapoint[1] - 1, $range['yaxis']['min']); //-1 for padding
              $range['yaxis']['max'] = is_null($range['yaxis']['max']) ? $datapoint[1] : max($datapoint[1] + 1, $range['yaxis']['max']); //+1 for padding
              $range['xaxis']['min'] = is_null($range['xaxis']['min']) ? $datapoint[0] : min($datapoint[0], $range['xaxis']['min']);
              $range['xaxis']['max'] = is_null($range['xaxis']['max']) ? $datapoint[0] : max($datapoint[0], $range['xaxis']['max']);
              if (empty($labelfields)) {
                $serieskey = $fieldy->definition['title'] . ': ' . $row->{$fieldy->field_alias};
                if (!empty($group_fields)) {
                  $serieskey = array();
                  foreach ($group_fields as $group_field) {
                    $serieskey[] = $group_field->definition['title'] . ': ' . $row->{$group_field->field_alias};
                  }
                  $serieskey = implode(', ', $serieskey);
                }
              }
              else {
                $serieskey = array();
                foreach ($labelfields as $label_field) {
                  $serieskey[] = $label_field->definition['title'] . ': ' . $row->{$label_field->field_alias};
                }
                $serieskey = implode(', ', $serieskey);
              }
              $series[$serieskey][] = $datapoint;
              $series_options[$serieskey] = $fieldy->options['flot']['y'];
            }
          }
        }
      }
    }
    $output = array(
      'series' => $series,
      'series_options' => $series_options,
      'range' => $range,
      'ticks' => $ticks,
    );
  }

  function preprocess_SearchApiViewsQuery($vars, $input, &$output) {
    // input
    $yfields = $input['yfields'];
    $fieldx = $input['fieldx'];
    $group_fields = $input['group_fields'];

    // output
    $data = array();
    $series = array();
    $series_options = array();
    $range = array();
    $range['xaxis'] = $range['yaxis'] = array('min' => NULL, 'max' => NULL);
    $ticks = array();

    // convert to usable format
    if (!isset($vars['rows'])){
      $output = array('series' => $series, 'series_options' => $series_options, 'range' => $range, 'ticks' => $ticks);
      return;
    }
    foreach ($vars['rows'] as $id => $row) {
      if (isset($row) && isset($row->_entity_properties)) {
        $datarow = array();
        foreach ($row->_entity_properties as $key => $value) {
          if ($key != 'search_api_relevance' && $key != 'search_api_excerpt') {
            $datarow[$key] = $value;
          }
        }
        if (!empty($datarow)) {
          $data[] = (object)$datarow;
        }
      }
    }

    // count doen - min/max bepalen
    $min = $data[0]->{$fieldx->field_alias};
    $max = $data[0]->{$fieldx->field_alias};
    foreach ($data as $id => $row) {
      if ($row->{$fieldx->field_alias} < $min) {
        $min = $row->{$fieldx->field_alias};
      }
      elseif ($row->{$fieldx->field_alias} > $max) {
        $max = $row->{$fieldx->field_alias};
      }
    }
    if ($max - $min > 30 * 86400) {
      $timeinterval = 86400;
    }
    elseif ($max - $min > 7 * 86400) {
      $timeinterval = 21600;
    }
    elseif ($max - $min > 3 * 86400) {
      $timeinterval = 10800;
    }
    elseif ($max - $min > 86400) {
      $timeinterval = 3600;
    }
    else {
      $timeinterval = 60;
    }

    $countdata = array();
    foreach ($data as $id => $row) {
      $x = round($row->{$fieldx->field_alias} / $timeinterval) * $timeinterval;
      if (empty($yfields)) {
        if (!isset($countdata[$x])) {
          $countdata[$x]->{$fieldx->field_alias} = $x;
          $countdata[$x]->count = 1;
        }
        else {
          $countdata[$x]->count++;
        }
      }
      else {
        $labelfields = array();
        foreach($yfields as $fieldy) {
          if ($fieldy->options['flot']['y']['function'] == 'label') {
            $labelfields[] = $fieldy;
          }
        }
        foreach ($yfields as $fieldy) {
          $rowval = 0;
          $label = $fieldy->field;
          if (isset($row->{"entity object"})) {
            $wrapper = entity_metadata_wrapper($fieldy->entity_type, $row->{"entity object"});
            $label = field_info_instance($wrapper->type(), $fieldy->field, $wrapper->getBundle());
            if (empty($label)) {
              //this is an entity that does not have this field (e.g. nodes of different types do not have the same fields)
              $label = t('No data');
            }
            else {
              $label = $label['label'];
            }
          }
          if (isset($row->{$fieldy->field_alias})) {
            $rowval = $row->{$fieldy->field_alias};
          }
          else {
            if (isset($wrapper->{$fieldy->field})) {;
              $rowval = $wrapper->{$fieldy->field}->value();
            }
          }
          $rowval = is_null($rowval) ? 0 : $rowval;
          $datakey = $x;
          if ($fieldy->options['flot']['y']['function'] == 'count') {
            $datakey = $x . $rowval;
          }
          if (!isset($countdata[$datakey]) && !in_array($fieldy, $labelfields)) {
            $countdata[$datakey]->x = $x;
            $labelval = array();
            if (!empty($labelfields)) {
              foreach($labelfields as $labelfield) {
                if (isset($row->{$labelfield->field_alias})) {
                  $labelval[] = $labelfield->definition['title'] . ': ' . $row->{$labelfield->field_alias};
                }
                else {
                  if (isset($wrapper->{$labelfield->field})) {;
                    $labelval[] = $labelfield->definition['title'] . ': ' . $wrapper->{$labelfield->field}->value();
                  }
                }
              }
            }
            switch ($fieldy->options['flot']['y']['function']) {
              case 'count':
                $countdata[$datakey]->count = 1;
                if (empty($labelfields) && isset($row->{$fieldy->field_alias})) {
                  $labelval[] = $fieldy->definition['title'] . ': ' . $row->{$fieldy->field_alias};
                }
                else {
                  if (isset($wrapper->{$fieldy->field})) {;
                    $labelval[] = $fieldy->definition['title'] . ': ' . $wrapper->{$fieldy->field}->value();
                  }
                }
                break;
              default :
                //min, max, sum
                $countdata[$datakey]->count = $rowval;
                break;
            }
            $countdata[$datakey]->label = empty($labelval) ? $label : implode(', ', $labelval);
          }
          else {
            switch ($fieldy->options['flot']['y']['function']) {
              case 'count':
                $countdata[$datakey]->count++;
                break;
              case 'sum':
                $countdata[$datakey]->count += $rowval;
                break;
              case 'min':
                $countdata[$datakey]->count = min($rowval, $countdata[$x]->count);
                break;
              case 'max':
                $countdata[$datakey]->count = max($rowval, $countdata[$x]->count);
                break;
            }
          }
        }
      }
    }
    // sort data by timestamp
    function cmp_dp($a, $b) {
      if ($a->x == $b->x) {
        return 0;
      }
      return ($a->x < $b->x) ? -1 : 1;
    }
    usort($countdata, 'cmp_dp');

    // Iterate over results to build data and ticks
    foreach ($countdata as $id => $row) {
      if (isset($row)) {
        if (empty($yfields)) {
          if (isset($fieldx->options['date_format'])) {
            $datapoint = array(
              $row->x * 1000,
              $row->{"count"},
            );
          }
          else {
            if (!in_array($row->x, $ticks)) {
              $ticks[] = $row->x;
            }
            $datapoint = array(
              array_shift(array_keys($ticks, $row->x)),
              $row->{"count"},
            );
          }
          $range['xaxis']['min'] = is_null($range['xaxis']['min']) ? $row->{"count"} : min($row->{"count"}, $range['xaxis']['min']);
          $range['xaxis']['max'] = is_null($range['xaxis']['max']) ? $row->{"count"} : max($row->{"count"}, $range['xaxis']['max']);
          
          $series[$fieldx->field_alias][] = $datapoint;
        }
        else {
          // datetime fields need special care
          // TODO: Also handle datetime for the Y axis, not just the X axis
          if (isset($fieldx->options['date_format'])) {
            $datapoint = array(
              $row->x * 1000,
              $row->count,
            );
          }
          else {
            if (!in_array($row->{$fieldx->field_alias}, $ticks)) {
              $ticks[] = $row->x;
            }
            $datapoint = array(
              array_shift(array_keys($ticks, $row->x)),
                $row->count,
            );
          }

          $range['yaxis']['min'] = is_null($range['yaxis']['min']) ? $datapoint[1] : min($datapoint[1] - 1, $range['yaxis']['min']); //-1 for padding
          $range['yaxis']['max'] = is_null($range['yaxis']['max']) ? $datapoint[1] : max($datapoint[1] + 1, $range['yaxis']['max']); //+1 for padding
          $range['xaxis']['min'] = is_null($range['xaxis']['min']) ? $datapoint[0] : min($datapoint[0], $range['xaxis']['min']);
          $range['xaxis']['max'] = is_null($range['xaxis']['max']) ? $datapoint[0] : max($datapoint[0], $range['xaxis']['max']);
          $serieskey = $row->label;
          $series[$serieskey][] = $datapoint;
          $series_options[$serieskey] = $fieldy->options['flot']['y'];
        }
      }
    }
    $output = array(
      'series' => $series,
      'series_options' => $series_options,
      'range' => $range,
      'ticks' => $ticks,
    );
  }

  function validate() {
    $errors = parent::validate();
    $fields = $this->display->handler->handlers['field'];
    $xfields = array();
    foreach ($fields as $field) {
      if ($field->options['flot']['axis'] == 'x') {
        $xfields[] = $field->definition['group'] . ': ' . $field->definition['title'];
      }
    }

    if (count($xfields) == 0) {
      $errors[] = t('You need to specify a field for the x-axis');
    }
    elseif (count($xfields) > 1) {
      $errors[] = t('You have specified more than one field for the x-axis (!fields)', array('!fields' => implode(', ', $xfields)));
    }
    return $errors;
  }
}
