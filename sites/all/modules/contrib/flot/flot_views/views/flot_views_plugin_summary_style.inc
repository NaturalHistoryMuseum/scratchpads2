<?php
// $Id: flot_views_plugin_summary_style.inc,v 1.1.2.3 2009/09/23 14:14:56 yhahn Exp $

/**
 * Summary style plugin for flot.
 */
class flot_views_plugin_summary_style extends flot_views_plugin_style {
  function preprocess(&$vars) {
    $view = $this->view;
    $argument = $view->argument[$view->build_info['summary_level']];
    $options = $this->options;

    // Parameters
    $type = !empty($options['type']) ? $options['type'] : 'line';
    $size = !empty($options['size']) ? explode('x', $options['size']) : array('200', '100');
    $reverse = !empty($options['reverse']) ? $options['reverse'] : 1;
    $pad_y = !empty($options['pad_y']) ? $options['pad_y'] : 1;

    // DOM element options
    $element = array();
    $element['style'] = "width:{$size[0]}px; height:{$size[1]}px;";
    $vars['element'] = $element;

    $series = array();
    $range = array('min' => NULL, 'max' => NULL);
    $ticks = array();

    $vars['rows'] = $reverse ? array_reverse($vars['rows']) : $vars['rows'];

    // Iterate over results to build data and ticks
    foreach ($vars['rows'] as $id => $row) {
      $vars['rows'][$id]->link = $argument->summary_name($row);
      $vars['rows'][$id]->count = intval($row->{$argument->count_alias});

      if (!empty($vars['rows'][$id]->link)) {
        $value = $vars['rows'][$id]->count;

        $series[] = array($id, $value);
        $ticks[] = array($id, $argument->summary_name($row));

        if (!isset($range['min']) || $value < $range['min']) {
          $range['min'] = $value;
        }
        if (!isset($range['max']) || $value > $range['max']) {
          $range['max'] = $value;
        }
      }
      else {
        unset($vars['rows'][$id]);
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
    if ($pad_y) {
      $range['min'] = 0;
      $range['max'] = floor($range['max'] + ($range['max'] * .1));
    }

    switch ($options['yaxis']) {
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
    if ($options['xaxis'] == 'endpoints' && count($ticks) > 1) {
      $simplified_ticks = array();
      $simplified_ticks[] = array_shift($ticks);
      $simplified_ticks[] = array_pop($ticks);
      $ticks = $simplified_ticks;
    }
    $style->axis_ticks('xaxis', $ticks);

    $vars['options'] = $style;

    // We don't actually render the flot graph in the preprocess so other
    // theme preprocessors can alter it.
  }
}
