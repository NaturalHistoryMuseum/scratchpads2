<?php
/**
 * @file
 * This file contains the class to override views_handler_field_date.
 */

/**
 * Allows to removes the date formatting from render() and put it in query().
 */
class views_date_format_sql_handler_field_date extends views_handler_field_date {
  function option_definition() {
    $options = parent::option_definition();

    $options['format_date_sql'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['format_date_sql'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use SQL to format date'),
      '#description' => t('Use the SQL databse to format the date. This enables date values to be used in grouping aggregation.'),
      '#default_value' => $this->options['format_date_sql'],
    );
  }

  function query() {
    if ($this->options['format_date_sql'] != TRUE) {
      return parent::query();
    }
    $this->ensure_my_table();
    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();

    $format = $this->options['date_format'];
    $custom_format = $this->options['custom_date_format'];
    $format_string = $this->get_date_format($format, $custom_format);
    $formula = views_date_sql_format($format_string, "$this->table_alias.$this->real_field");

    $this->field_alias = $this->query->add_field(NULL, $formula, "{$this->table_alias}_{$this->real_field}", $params);
    $this->query->add_groupby($this->field_alias);

    $this->add_additional_fields();
  }

  function render($values) {
    if ($this->options['format_date_sql'] != TRUE) {
      return parent::render($values);
    }
    $format = $this->options['date_format'];
    return $this->get_value($values);
  }

  /**
   * Helper to retrieve the format to a given date format name.
   * see includes/common.inc:function format_date()
   */
  function get_date_format($type = 'medium', $format = '') {
    switch ($type) {
      case 'short':
        $format = variable_get('date_format_short', 'm/d/Y - H:i');
        break;

      case 'long':
        $format = variable_get('date_format_long', 'l, F j, Y - H:i');
        break;

      case 'custom':
        // No change to format.
        break;

      case 'medium':
      default:
        // Retrieve the format of the custom $type passed.
        if ($type != 'medium') {
          $format = variable_get('date_format_' . $type, '');
        }
        // Fall back to 'medium'.
        if ($format === '') {
          $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
        }
        break;
    }
    return $format;
  }
}
