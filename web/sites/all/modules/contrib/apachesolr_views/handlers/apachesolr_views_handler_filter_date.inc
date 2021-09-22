<?php

/**
 * @file
 * Date filter handler for Apache Solr Views.
 */

// Date format used by Solr.
define('APACHESOLR_VIEWS_DATE_ISO8601', 'Y-m-d\TH:i:s\Z');

class apachesolr_views_handler_filter_date extends views_handler_filter_date {

  /**
   * Filter for a date within certain range.
   */
  public function op_between($field) {
    $min_value = $this->obtain_formated_date($this->value['min'], '00:00:00');
    $max_value = $this->obtain_formated_date($this->value['max'], '23:59:59.999');
    $value = "[$min_value TO $max_value]";
    $operator = strtoupper($this->operator);
    $this->query->add_where($this->options['group'], $this->real_field, $value, $operator);
  }

  /**
   * Add the date filter to the query parameters.
   */
  public function op_simple($field) {
    $value = intval(strtotime($this->value['value']));
    $value = new DateObject($value);

    // Probably there is a better way with dateobject, but no way to find how.
    $has_time = strpos($this->value['value'], ':');

    $value_format = $value->format(APACHESOLR_VIEWS_DATE_ISO8601);
    switch ($this->operator) {
      case '<':
      case '<=':
        if ($has_time) {
          $value = '[* TO ' . $value_format . ']';
        }
        else {
          $value = '[* TO ' . $value_format . '/DAY+1DAY]';
        }
        break;

      case '>':
      case '>=':
        if ($has_time) {
          $value = '[' . $value_format . ' TO *]';
        }
        else {
          $value = '[' . $value_format . '/DAY TO *]';
        }
        break;

      case '!=':
        if ($has_time) {
          $value = '[* TO ' . $value_format . '-1SECOND] OR [' . $value_format . '+1SECOND TO *]';
        }
        else {
          $value = '[* TO ' . $value_format . '/DAY-1DAY] OR [' . $value_format . '/DAY+1DAY TO *]';
        }
        break;

      case '=':
      default:
        if ($has_time) {
          $value = '[' . $value_format . ' TO ' . $value_format . ']';
        }
        else {
          $value = '[' . $value_format . '/DAY TO ' . $value_format . '/DAY+1DAY]';
        }
        break;
    }
    $this->query->add_where($this->options['group'], $this->real_field, $value, $this->operator);
  }

  /**
   * Add a type selector to the value form.
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    // Remove offset functionality.
    unset($form['value']['type']);

    // Add class to identify in jquery input fields and add jquery.ui.datepicker
    if (isset($form['value']['min'])) {
      $form['value']['max']['#attributes']['class'][] = 'views_input_date';
      $form['value']['min']['#attributes']['class'][] = 'views_input_date';
    }
    else {
      $form['value']['value']['#attributes']['class'][] = 'views_input_date';
    }
  }

  /**
   * Override to allow ranges without end-date, assume * if not present.
   */
  public function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // Store this because it will get overwritten.
    $type = $this->value['type'];
    $rc = parent::accept_exposed_input($input);

    $operators = $this->operators();
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id'])) {
      $operator = $input[$this->options['expose']['operator_id']];
    }
    else {
      $operator = $this->operator;
    }

    if ($operators[$operator]['values'] != 1) {
      // This is the line overridden.
      if ($this->value['min'] == '' && $this->value['max'] == '') {
        return FALSE;
      }
      else {
        return TRUE;
      }
    }

    // Restore what got overwritten by the parent.
    $this->value['type'] = $type;
    return $rc;
  }

  /**
   * Obtains a formated data for SOLR.
   *
   * @param string $date_str
   *   Date in string format.
   * @param string $round_string
   *   String to add if no time is present.
   *
   * @return string
   *   The date formated for SOLR.
   */
  public function obtain_formated_date($date_str, $round_string = '00:00:00') {
    // Guess if we time part to the date and if not, add the round string.
    if ($date_str != '' && !strpos($date_str, ':')) {
      $date_str .= ' ' . $round_string;
    }
    $time = intval(strtotime($date_str));
    if ($time != 0) {
      $date = new DateObject($time);
      $date_formatted = $date->format(APACHESOLR_VIEWS_DATE_ISO8601);
    }
    else {
      $date_formatted = '*';
    }
    return $date_formatted;
  }
}