<?php
// $Id: date_views_filter_handler.inc,v 1.7 2011/01/20 13:07:24 karens Exp $
/**
 * @file
 * A flexible, configurable date filter.
 * This filter combines multiple date filters into a single filter
 * where all fields are controlled by the same date and can be combined with either AND or OR.
 */

class date_views_filter_handler extends date_views_filter_handler_simple {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (empty($this->view->date_info)) {
      $this->view->date_info = new stdClass();
    }
    if (empty($this->view->date_info->date_fields)) {
      $this->view->date_info->date_fields = array();
    }
    $this->view->date_info->date_fields = array_merge($this->view->date_info->date_fields, $this->options['date_fields']);
  }

  // Set default values for the date filter.
  function option_definition() {
    $options = parent::option_definition();
    $options['date_fields'] = array('default' => array());
    $options['date_method'] = array('default' => 'OR');
    $options['date_group'] = array('default' => 'date');
    return $options;
  }

  function op_between($field) {
    $this->get_query_fields();
    if (!empty($this->query_fields)) {

      // If we don't add a dummy where clause and there is no other filter defined for this view,
      // Views will dump in an invalid WHERE () in addition to our custom filters, so give it a valid value.
      // @TODO This is probably the wrong way to solve this problem.
      if (empty($this->query->where[$this->options['group']]['conditions'])) {
        $this->query->add_where_expression(NULL, '1=1', array());
      }

      $this->query->set_where_group($this->options['date_method'], $this->options['date_group']);
      foreach ((array) $this->query_fields as $query_field) {
        $field = $query_field['field'];
        $this->date_handler = $query_field['date_handler'];
        if ($field['table_name'] != $this->table || !empty($this->relationship)) {
          $this->related_table_alias = $this->query->queue_table($field['table_name'], $this->relationship);
        }
        $table_alias = !empty($this->related_table_alias) ? $this->related_table_alias : $field['table_name'];
        $query_field['field']['fullname'] = $table_alias .'.'. $query_field['field']['field_name'];
        parent::op_between($query_field['field']['fullname']);
      }
    }
  }

  function op_simple($field) {

    $this->get_query_fields();
    if (!empty($this->query_fields)) {

      // If we don't add a dummy where clause and there is no other filter defined for this view,
      // Views will dump in an invalid WHERE () in addition to our custom filters, so give it a valid value.
      // @TODO This is probably the wrong way to solve this problem.
      if (empty($this->query->where[$this->options['group']]['conditions'])) {
        $this->query->add_where_expression(NULL, '1=1', array());
      }

      $this->query->set_where_group($this->options['date_method'], $this->options['date_group']);
      foreach ((array) $this->query_fields as $query_field) {
        $field = $query_field['field'];
        $this->date_handler = $query_field['date_handler'];
        if ($field['table_name'] != $this->table || !empty($this->relationship)) {
          $this->related_table_alias = $this->query->queue_table($field['table_name'], $this->relationship);
        }
        // $this->related_table_alias gets set when the first field is processed if otherwise empty.
        // For subsequent fields, we need to be sure it is emptied again.
        elseif (empty($this->relationship)) {
          $this->related_table_alias = NULL;
        }
        $table_alias = !empty($this->related_table_alias) ? $this->related_table_alias : $field['table_name'];
        $query_field['field']['fullname'] = $table_alias .'.'. $query_field['field']['field_name'];
        parent::op_simple($query_field['field']['fullname']);
      }
    }
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $fields = date_views_fields($this->view->base_table);
    $options = array();
    foreach ($fields['name'] as $name => $field) {
      $options[$name] = $field['label'];
    }

    $form['date_fields'] = array(
      '#title' => t('Date field(s)'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['date_fields'],
      '#multiple' => FALSE,
      '#description' => t('Select date field(s) to filter.'),
      '#required' => TRUE,
    );
    $form['date_method'] = array(
      '#title' => t('Method'),
      '#type' => 'radios',
      '#options' => array('OR' => t('OR'), 'AND' => t('AND')),
      '#default_value' => $this->options['date_method'],
      '#description' => t('Method of handling multiple date fields in the same query. Return items that have any matching date field (date = field_1 OR field_2), or only those with matches in all selected date fields (date = field_1 AND field_2).'),
      );
  }

  function extra_options_validate($form, &$form_state) {
    $check_fields = array_filter($form_state['values']['options']['date_fields']);
    if (empty($check_fields)) {
      form_error($form['date_fields'], t('You must select at least one date field for this filter.'));
    }
  }

  function extra_options_submit($form, &$form_state) {
    $form_state['values']['options']['date_fields'] = array_filter($form_state['values']['options']['date_fields']);
  }

  // Update the summary values to provide
  // meaningful information for each option.
  function admin_summary() {
    if (empty($this->options['date_fields'])) {
      return t('Missing date fields!');
    }
    $handler = $this->date_handler;

    $fields = date_views_fields($this->view->base_table);
    if (!empty($this->options['date_fields'])) {
      $output = array();
      foreach ($this->options['date_fields'] as $field) {
        if (array_key_exists($field, $fields['name'])) {
          $output[] = $fields['name'][$field]['label'];
        }
      }
    }
    $field = implode(' ' . $this->options['date_method'] . ' ', $output);
    $output = "$field " . check_plain($this->operator) . ' ';
    $parts = $handler->date_parts();
    $widget_options = $this->widget_options();
    // If the filter is exposed, display the granularity.
    if ($this->options['exposed']) {
      return t('(@field) <strong>Exposed</strong> @widget @format', array('@field' => $field, '@format' => $parts[$handler->granularity], '@widget' => $widget_options[$this->options['form_type']]));
    }
    // If not exposed, display the value.
    if (in_array($this->operator, $this->operator_values(2))) {
      $min = check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['min']);
      $max = check_plain(!empty($this->options['default_to_date']) ? $this->options['default_to_date'] : $this->options['value']['max']);
      $output .= t('@min and @max', array('@min' => $min, '@max' => $max));
    }
    else {
      $output .= check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['value']);
    }
    return $output;
  }

  function get_query_fields() {
    $fields = date_views_fields($this->view->base_table);
    $fields = $fields['name'];
    $this->query_fields = array();
    foreach ((array) $this->options['date_fields'] as $delta => $name) {
      if (array_key_exists($name, $fields) && $field = $fields[$name]) {
        $date_handler = new date_sql_handler($field['sql_type'], date_default_timezone());
        $date_handler->granularity = $this->options['granularity'];
        $date_handler->db_timezone = date_get_timezone_db($field['tz_handling']);
        $date_handler->local_timezone = date_get_timezone($field['tz_handling']);
        $this->query_fields[] = array('field' => $field, 'date_handler' => $date_handler);
      }
    }
  }
}