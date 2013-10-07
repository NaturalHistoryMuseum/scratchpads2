<?php
/**
 * @file
 * This file contains the class to override views_handler_field_date.
 */

/**
 * Allows to removes the date formatting from render() and put it in query().
 */
class views_date_format_sql_handler_date_field extends views_handler_field_field {

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

  function query($use_groupby = FALSE) {
    parent::query($use_groupby);
    if ($this->options['format_date_sql'] != TRUE) {
      return;
    }

    $format_string = _views_date_format_sql_get_date_format($this->options['settings']['format_type']);

    // If this is a date module filed we have to get the storage field type.
    $formula = views_date_sql_format($format_string, "$this->table_alias.$this->real_field", $this->field_info['type']);
    $this->query->fields[$this->aliases[$this->real_field]]['field'] = $formula;
    $this->query->fields[$this->aliases[$this->real_field]]['table'] = NULL;
  }

  /**
   * Return an array of items for the field.
   */
  function set_items($values, $row_id) {
    if ($this->options['format_date_sql'] == TRUE && $this->view->query->has_aggregate) {
      // If we get the formatted value from storage, then it should be output as
      // plain.
      $this->options['type'] = 'date_plain';
    }
    return parent::set_items($values, $row_id);
  }
}
