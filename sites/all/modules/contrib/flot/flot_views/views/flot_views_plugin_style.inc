<?php
// $Id: flot_views_plugin_style.inc,v 1.1.2.1 2009/09/21 17:00:41 yhahn Exp $

class flot_views_plugin_style extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'line');
    $options['size'] = array('default' => '400x200');
    $options['x'] = array('default' => array('granularity' => 'auto', 'label' => 'default'));
    $options['y'] = array('default' => array('granularity' => 'auto', 'label' => 'default', 'pad' => 1));
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Graph type'),
      '#options' => array('line' => t('Line'), 'bar' => t('Bar'), 'point' => t('Point'), 'pie' => t('Pie chart')),
      '#description' => t("Choose the type of chart you would like to display."),
      '#default_value' => $this->options['type'],
    );

    $form['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#description' => t("Enter the dimensions for the chart. Format: WIDTHxHEIGHT (e.g. 200x100)"),
      '#default_value' => $this->options['size'],
    );

    // Generate label fields
    $label_options = array(
      '' => '< ' . t('No labels') . ' >',
      'default' => t('Default (from data points)'),
    );

    // Generate granularity options
    $yaxis_granularity =
    $xaxis_granularity = array(
      'auto' => t('Auto generate'),
      'endpoints' => t('Endpoints only'),
    );
    for ($i = 3; $i < 10; $i++) {
      $yaxis_granularity[$i] = t('Granularity: !count ticks', array('!count' => $i));
    }

    $form['x'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('X Axis'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['x']['label'] = array(
      '#type' => 'select',
      '#options' => $label_options,
      '#title' => t('Labels'),
      '#default_value' => $this->options['x']['label'],
    );
    $form['x']['granularity'] = array(
      '#type' => 'select',
      '#options' => $xaxis_granularity,
      '#title' => t('Granularity'),
      '#default_value' => $this->options['x']['granularity'],
    );

    $form['y'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Y Axis'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['y']['label'] = array(
      '#type' => 'select',
      '#options' => $label_options,
      '#title' => t('Labels'),
      '#default_value' => $this->options['y']['label'],
    );
    $form['y']['granularity'] = array(
      '#type' => 'select',
      '#options' => $yaxis_granularity,
      '#title' => t('Granularity'),
      '#default_value' => $this->options['y']['granularity'],
    );
    $form['y']['pad'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add headroom above points'),
      '#default_value' => $this->options['y']['pad'],
    );
  }

  /**
   * Theme template preprocessor.
   */
  function preprocess(&$vars) {
    // Get flot field, and bail if not present.
    $flot_field = $this->get_flot_field();
    if (!$flot_field) {
      return;
    }

    $view = $this->view;
    $options = $this->options;

    // Parameters
    $type = !empty($options['type']) ? $options['type'] : 'line';
    $size = !empty($options['size']) ? explode('x', $options['size']) : array('200', '100');

    // DOM element options
    $element = array();
    $element['style'] = is_numeric($size[0]) ? "width:{$size[0]}px;" : "width:{$size[0]};";
    $element['style'] .= is_numeric($size[1]) ? "height:{$size[1]}px;" : "height:{$size[1]};";
    $vars['element'] = $element;

    $series = array();
    $range = array('min' => NULL, 'max' => NULL);
    $ticks = array();

    // Iterate over results to build data and ticks
    foreach ($vars['rows'] as $id => $row) {
      $datapoint = $view->field[$flot_field]->flot_render($row);
      $value = $datapoint['value'];
      $label = isset($datapoint['label']) ? $datapoint['label'] : $datapoint['value'];

      $series[] = array($value[0], $value[1]);
      $ticks[] = array($value[0], $label[0]);

      if (!isset($range['min']) || $value[1] < $range['min']) {
        $range['min'] = $value[1];
      }
      if (!isset($range['max']) || $value[1] > $range['max']) {
        $range['max'] = $value[1];
      }
    }
    $series = new flotData($series);
    $vars['data'] = array($series);

    // Set up the type class, set axes
    switch ($options['type']) {
      case 'point':
        $style = new flotStylePoint();
        break;
      case 'bar':
        $style = new flotStyleBar();
        break;
      case 'pie':
        $style = new flotStylePie();
        break;
      case 'line':
      default:
        $style = new flotStyleLine();
        break;
    }

    // Format Y Axis
    $granularity = 0;

    // If max is too small Flot barfs -- set a minimum value
    $range['max'] = ($range['max'] < 5) ? 5 : $range['max'];

    // Pad Y axis if necessary
    if ($options['y']['pad']) {
      $range['min'] = 0;
      $range['max'] = floor($range['max'] + ($range['max'] * .1));
    }

    switch ($options['y']['granularity']) {
      case 'endpoints':
        $yticks = array(array($range['min'], $range['min']), array($range['max'], $range['max']));
        $style->axis_ticks('yaxis', $yticks);
        break;
      case 'auto':
        $style->axis_range('yaxis', $range);
        break;
      default:
        $style->axis_range('yaxis', $range, $options['yaxis']);
        break;
    }

    // Format X Axis
    if ($options['x']['granularity'] == 'endpoints' && count($ticks) > 1) {
      $simplified_ticks = array();
      $simplified_ticks[] = array_shift($ticks);
      $simplified_ticks[] = array_pop($ticks);
      $ticks = $simplified_ticks;
    }
    $style->axis_ticks('xaxis', $ticks);

    $vars['options'] = $style;
  }

  /**
   * Validate function.
   */
  function validate() {
    parent::validate();
    $field = $this->get_flot_field();
    if (!$field) {
      return array(t('You must use a field that is compatible (e.g. <strong>Data point</strong>) with Flot to use the Flot style plugin.'));
    }
  }

  /**
   * Get the first usable flot field on this view.
   */
  function get_flot_field() {
    $fields = $this->display->handler->get_option('fields');
    foreach ($fields as $field => $info) {
      $handler = get_class(views_get_handler($info['table'], $info['field'], 'field'));
      if (method_exists($handler, 'flot_render')) {
        return $field;
      }
    }
    return FALSE;
  }
}
