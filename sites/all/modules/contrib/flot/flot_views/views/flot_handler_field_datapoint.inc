<?php
// $Id: flot_handler_field_datapoint.inc,v 1.1.2.1 2009/09/21 17:00:41 yhahn Exp $

class flot_handler_field_datapoint extends views_handler_field  {
  protected $use_date;

  /**
   * Disable advanced render.
   */
  function allow_advanced_render() { return FALSE; }

  /**
   * Option defaults.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['series'] = array('default' => array('field' => '', 'grouping' => '', 'date_format' => 'small'));
    $options['value'] = array('default' => array('field' => '', 'format' => ''));
    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['series'] = array(
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Series (X axis)'),
    );
    $form['series']['field'] = array(
      '#title' => t('Field'),
      '#type' => 'select',
      '#options' => array(),
      '#default_value' => $this->options['series']['field'],
    );

    $form['value'] = array(
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Series (Y axis)'),
    );
    $form['value']['field'] = array(
      '#title' => t('Field'),
      '#type' => 'select',
      '#options' => array(),
      '#default_value' => $this->options['value']['field'],
    );

    // Get field options and generate subsequent options based on class
    foreach (views_fetch_fields($this->view->base_table, 'field') as $field_id => $field_info) {
      $form['series']['field']['#options'][$field_id] = !empty($field_info['title']) ? $field_info['title'] : $field_id;
      $form['value']['field']['#options'][$field_id] = !empty($field_info['title']) ? $field_info['title'] : $field_id;

      list($table, $field) = explode('.', $field_id);
      $handler = get_class(views_get_handler($table, $field, 'field'));
      if (strpos($handler, '_date') !== FALSE) {
        // If the series field is empty, use a date by default
        $form['series']['field']['#default_value'] = empty($form['series']['field']['#default_value']) ? $field_id : $form['series']['field']['#default_value'];

        if (!isset($form['series']['grouping'])) {
          $form['series']['grouping'] = array(
            '#title' => t('Group dates by'),
            '#type' => 'select',
            '#options' => array(
              'Y-m-d-H' => t('Hour'),
              'Y-m-d-3' => t('3 hours'),
              'Y-m-d-6' => t('6 hours'),
              'Y-m-d-A' => t('12 hours'),
              'Y-m-d' => t('Day'),
              'Y-m' => t('Month'),
              'Y' => t('Year'),
            ),
            '#dependency' => array(),
            '#default_value' => $this->options['series']['grouping'],
          );
          $form['series']['date_format'] = array(
            '#title' => t('Date format'),
            '#type' => 'select',
            '#options' => array(
              'short' => format_date(REQUEST_TIME, 'short'),
              'medium' => format_date(REQUEST_TIME, 'medium'),
              'long' => format_date(REQUEST_TIME, 'long'),
            ),
            '#dependency' => array(),
            '#default_value' => $this->options['series']['date_format'],
          );
        }

        // Add this field to the list of timestamp options which trigger the grouping field display.
        $form['series']['grouping']['#dependency']['edit-options-series-field'][] = $field_id;
        $form['series']['date_format']['#dependency']['edit-options-series-field'][] = $field_id;
      }
    }

    $form['value']['format'] = array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => array(
        'raw' => t('Raw value'),
        'count' => t('Count'),
      ),
      '#default_value' => $this->options['value']['format'],
    );
  }

  /**
   * Query method.
   */
  function query() {
    // Killswitch
    if (empty($this->options['series']['field']) || empty($this->options['value']['field'])) {
      return;
    }

    // Yank off the base field & any other groupby's / having's. They will screw up our results.
    unset($this->query->fields[$this->view->base_field]);
    $this->query->groupby = array();
    $this->query->having = array();

    // Add in series field
    list($series_table, $series_field) = explode('.', $this->options['series']['field']);
    $alias = $this->query->ensure_table($series_table);

    // Determine whether we are using a date field.
    $handler = get_class(views_get_handler($series_table, $series_field, 'field'));
    $this->use_date = strpos($handler, '_date') !== FALSE;

    $field_alias = $this->query->add_field($series_table, $series_field);
    $this->query->add_groupby($field_alias);
    $this->series_table = $alias;
    $this->series_field = $field_alias;

    // Add in value field
    list($value_table, $value_field) = explode('.', $this->options['value']['field']);
    $alias = $this->query->ensure_table($value_table);

    if ($this->use_date) {
      $field_alias = $this->query->add_field($value_table, $value_field);
    }
    else {
      switch ($this->options['value']['format']) {
        case 'count':
          $field_alias = $this->query->add_field(NULL, "COUNT({$alias}.{$value_field})", "{$value_table}_{$value_field}", array('aggregate' => TRUE));
          break;
        default:
          $field_alias = $this->query->add_field($value_table, $value_field);
          break;
      }
    }
    $this->value_table = $alias;
    $this->value_field = $field_alias;
    $this->query->set_count_field($alias, $value_field, $field_alias);
  }

  /**
   * Return an array usable as a plot point by flot.
   */
  function flot_render($values) {
    if ($this->use_date) {
      $series = $values->{$this->series_field};
      $series_label = format_date($values->{$this->series_field}, $this->options['series']['date_format']);
    }
    else {
      $series_label =
      $series = check_plain($values->{$this->series_field});
    }
    $value = check_plain($values->{$this->value_field});
    return array(
      'value' => array($series, $value),
      'label' => array($series_label, $value),
    );
  }

  /**
   * Optional method that allows the data source to determine axis bounds.
   */
  function pre_render($result) {
    if ($this->use_date) {
      // First grab endpoints from any date filters
      $filters = $this->view->display_handler->get_handlers('filter');
      foreach ($filters as $filter => $handler) {
        if (strpos(get_class($handler), '_date') !== FALSE) {
          $min = REQUEST_TIME + intval(strtotime($handler->value['min'], 0));
          $max = REQUEST_TIME + intval(strtotime($handler->value['max'], 0));
          $value = intval(strtotime($handler->value['value'], 0));
          switch ($handler->operator) {
            case 'between':
              $start = $min;
              $end = $max;
              break;
            case '>=':
            case '>':
              $start = $min;
              $end = strtotime(end($result)->{$this->series_field});
              break;
            case '<=':
            case '<':
              $start = strtotime($result[0]->{$this->series_field});
              $end = $max;
              break;
          }
          break;
        }
      }

      $units = array(
        'Y-m-d-H' => '+1 hour',
        'Y-m-d-3' => '+3 hours',
        'Y-m-d-6' => '+6 hours',
        'Y-m-d-A' => '+12 hours',
        'Y-m-d' => '+1 day',
        'Y-m' => '+1 month',
        'Y' => '+1 year',
      );
      $grouping = $this->options['series']['grouping'];
      $grouping = isset($units[$grouping]) ? $grouping : 'Y-m-d';

      // Fill in default values in processed array.
      $processed = array();
      while ($start < $end) {
        $start = strtotime($units[$grouping], $start);
        $start_formatted = $this->format_date($start, $grouping);
        $blank = new stdClass();
        $blank->{$this->series_field} = $start;
        $blank->{$this->value_field} = 0;
        $processed[$start_formatted] = $blank;
      }

      foreach ($result as $row) {
        $timestamp_formatted = $this->format_date($row->{$this->series_field}, $grouping);
        if (isset($processed[$timestamp_formatted])) {
          if ($this->options['value']['format'] == "raw") {
             $processed[$timestamp_formatted]->values[] = $row->{$this->value_field};
          }
          elseif ($this->options['value']['format'] == "count") {
            $processed[$timestamp_formatted]->{$this->value_field}++;
          }
        }
      }
      // Average out the values.
      if ($this->options['value']['format'] == "raw") {
        foreach ($processed as &$item) {
          $values = isset($item->values) ? $item->values : array();
          if (count($values)) {
            $item->{$this->value_field} = array_sum($values) / count($values);
          }
        }
      }
      $this->view->result = array_values($processed);
    }
  }

  /**
   * Custom date formatter for achieving date granularity between 1 hour and 12 hours (AM/PM).
   */
  function format_date($timestamp, $format) {
    switch ($format) {
      case 'Y-m-d-3':
      case 'Y-m-d-6':
        $base = strtotime(format_date($timestamp, 'custom', 'Y-m-d'));
        $hour = format_date($timestamp, 'custom', 'H');
        $hours = array(
          'Y-m-d-3' => array(3, 6, 9, 12, 15, 18, 21, 24),
          'Y-m-d-6' => array(6, 12, 18, 24),
        );
        foreach ($hours[$format] as $slot) {
          if ($hour < $slot) {
            $formatted = strtotime("+{$slot} hours", $base);
            break;
          }
        }
        return format_date($formatted, 'custom', 'Y-m-d-H');
      default:
        return format_date($timestamp, 'custom', $format);
    }
  }

  /**
   * The default render function.
   */
  function render($values) {
    $series = check_plain($values->{$this->series_field});
    $value = check_plain($values->{$this->value_field});
    return "$series, $value";
  }
}
